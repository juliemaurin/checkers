#include "checkerboard.h"

const PieceType CheckerBoard::BLACK = 0;
const PieceType CheckerBoard::WHITE = 1;

// Forward Moves
const Word CheckerBoard::MASK_R3 = 0x70707070;
const Word CheckerBoard::MASK_R5 = 0x0E0E0E00;

// Backward Moves
const Word CheckerBoard::MASK_L5 = 0x00707070;
const Word CheckerBoard::MASK_L3 = 0x0E0E0E0E;

// Forward Jumps
const Word CheckerBoard::MASK_R9 = 0xEEEEEE00;
const Word CheckerBoard::MASK_R7 = 0x77777700;

// Backward Jumps
const Word CheckerBoard::MASK_L9 = 0x00EEEEEE;
const Word CheckerBoard::MASK_L7 = 0x00777777;

const Word CheckerBoard::MASK_ALL = 0xFFFFFFFF;
const Word CheckerBoard::MASK_TOP = 0xF0000000;
const Word CheckerBoard::MASK_BOT = 0x0000000F;

const Word CheckerBoard::MASK_MOVE3 = 0x00000009;
const Word CheckerBoard::MASK_MOVE4 = 0x00000011;
const Word CheckerBoard::MASK_MOVE5 = 0x00000021;
const Word CheckerBoard::MASK_JUMP7 = 0x00000081;
const Word CheckerBoard::MASK_JUMP9 = 0x00000201;

// Constructor
CheckerBoard::CheckerBoard() :
  forward{0,0},
  backward{0,0},
  pieces{0,0}
{
  new_game();
}

// Initialize the board with the default layout (blacks at the bottom, whites on top)
void CheckerBoard::new_game() {
  // Blacks starts first
  active = BLACK;
  passive = WHITE;

  // Setup black pieces
  forward[BLACK]  = 0x00000FFF;
  backward[BLACK] = 0x00000000;
  pieces[BLACK] = forward[BLACK] | backward[BLACK];

  // Setup white pieces
  forward[WHITE]  = 0x00000000;
  backward[WHITE] = 0xFFF00000;
  pieces[WHITE] = forward[WHITE] | backward[WHITE];

  // Setup "empty" spaces
  empty = MASK_ALL ^ (pieces[BLACK] | pieces[WHITE]);

  jumping = false;
}

// Apply a move to the board
void CheckerBoard::make_move(Word move) {
  // Get low move index
  int index_low = __builtin_ctzl(move);
  // Get high move index
  int index_high = __builtin_ctzl(move >> (index_low + 1)) + index_low + 1;

  // Jumps are 7 or 9 bits apart, moves are 3, 4 or 5
  if (index_high - index_low > 6) {
      // Compute the position of the "taken piece" while jumping
      Word taken_piece = static_cast<Word>(1) << ((index_high + index_low) / 2);
      if (((index_low / 4) % 2) != 0) taken_piece <<= 1;

      // Removing the taken piece from the board
      pieces[passive] ^= taken_piece;
      if (forward[passive] & taken_piece) forward[passive] ^= taken_piece;
      if (backward[passive] & taken_piece) backward[passive] ^= taken_piece;

      jumping = true;
  }

  // Moving piece on board
  pieces[active] ^= move;
  if (forward[active] & move) forward[active] ^= move;
  if (backward[active] & move) backward[active] ^= move;
  empty = MASK_ALL ^ (pieces[BLACK] | pieces[WHITE]);

  if (jumping) {
      // If we are jumping
      // And the next move is another jump, we return now. Two things don't happen :
      // Promotion : because if we can still jump : a) We're already a king
      //                                            b) We're not on the top/bottom row
      // Swapping : Because the active player keeps playing.
      mandatory_jumps = get_jumps();

      // If there are jumps available, we have to jump again (without changing active player)
      if (mandatory_jumps.size()) return;
  }

  // Get destination cell to check for promotion
  Word destination = move & pieces[active];
  // Promote a new black king (if available)
  if (active == BLACK && (destination & MASK_TOP) != 0) backward[BLACK] |= destination;
  // Promote a new white king (if available)
  else if (active == WHITE && (destination & MASK_BOT) != 0) forward[WHITE] |= destination;

  // Stop jumping sequence on move or promotion
  jumping = false;

  // Change active player
  std::swap(active, passive);
}

// Utility function for move generation
void CheckerBoard::add_move(std::vector<Word>& arr, Word move, Word movers, int offset=0) {
  int i = 0;
  while(movers) {
      int t = __builtin_ctzl(movers)+1;
      movers >>= t;
      i += t-1;
      arr.push_back(static_cast<Word>(move) << (i++ - offset));
  }
}

// List all available moves
std::vector<Word> CheckerBoard::get_moves() {
  // If we are in a "jump chain" we have to jump directly to the next destination
  if (jumping) return mandatory_jumps;

  // Otherwise, if there are jumps available, jump.
  std::vector<Word> jumps = get_jumps();
  if (jumps.size()) return jumps;

  // Otherwise, check for simple moves
  // Movers are pieces that CAN move
  Word fm3 = forward_movers3();
  Word fm4 = forward_movers4();
  Word fm5 = forward_movers5();
  Word bm3 = backward_movers3();
  Word bm4 = backward_movers4();
  Word bm5 = backward_movers5();

  // Generate moves from the list of movers
  std::vector<Word> moves;
  // Forward
  add_move(moves, MASK_MOVE3, fm3);
  add_move(moves, MASK_MOVE4, fm4);
  add_move(moves, MASK_MOVE5, fm5);

  // Backward
  add_move(moves, MASK_MOVE3, bm3, 3);
  add_move(moves, MASK_MOVE4, bm4, 4);
  add_move(moves, MASK_MOVE5, bm5, 5);

  return moves;
}

// List all available jumps
std::vector<Word> CheckerBoard::get_jumps() {
  // Jumpers are pieces that CAN jump
  Word fj7 = forward_jumpers7();
  Word fj9 = forward_jumpers9();
  Word bj7 = backward_jumpers7();
  Word bj9 = backward_jumpers9();

  // Generate jumps from the list of movers
  std::vector<Word> jumps;
  // Forward
  add_move(jumps, MASK_JUMP7, fj7);
  add_move(jumps, MASK_JUMP9, fj9);

  // Backward
  add_move(jumps, MASK_JUMP7, bj7, 7);
  add_move(jumps, MASK_JUMP9, bj9, 9);

  return jumps;
}

// Check if the game is a draw
bool CheckerBoard::is_draw() {
  // TODO : Implementation not present yet
  return false;
}

// Check if the game is over
bool CheckerBoard::is_over() {
  // The game is over if the active player has no move left or if it's a draw
  return !(get_moves().size()) || is_draw();
}

// Theses fucntions are utility functions that determine "movers" and "jumpers" (ie. pieces that can move or jump)
// Forward/Backward indicates the direction, 3/4/5 and 7/9 the distance (in bits of the bitboard) from origin to destination
// The main principle is to bitshift empty space and OR with present pieces to check for empty cells
// Masks "MASK_*" defines which cells can actually be moved in a certain direction. (ex. MASK_R3 means all the cells that can shift right 3 bits)
Word CheckerBoard::forward_movers3() {
  return ((empty & MASK_R3) >> 3) & forward[active];
}

Word CheckerBoard::forward_movers4() {
  return (empty >> 4) & forward[active];
}

Word CheckerBoard::forward_movers5() {
  return ((empty & MASK_R5) >> 5) & forward[active];
}

Word CheckerBoard::backward_movers3() {
  return ((empty & MASK_L3) << 3) & backward[active];
}

Word CheckerBoard::backward_movers4() {
  return (empty << 4) & backward[active];
}

Word CheckerBoard::backward_movers5() {
  return ((empty & MASK_L5) << 5) & backward[active];
}

Word CheckerBoard::forward_jumpers7() {
  Word victims4 = (empty >> 4) & pieces[passive];
  Word victims3 = ((empty & MASK_R3) >> 3) & pieces[passive];
  return (((victims4 & MASK_R3) >> 3) | (victims3 >> 4)) & forward[active];
}

Word CheckerBoard::forward_jumpers9() {
  Word victims4 = (empty >> 4) & pieces[passive];
  Word victims5 = ((empty & MASK_R5) >> 5) & pieces[passive];
  return (((victims4 & MASK_R5) >> 5) | (victims5 >> 4)) & forward[active];
}

Word CheckerBoard::backward_jumpers7() {
  Word victims4 = (empty << 4) & pieces[passive];
  Word victims3 = ((empty & MASK_L3) << 3) & pieces[passive];
  return (((victims4 & MASK_L3) << 3) | (victims3 << 4)) & backward[active];
}

Word CheckerBoard::backward_jumpers9() {
  Word victims4 = (empty << 4) & pieces[passive];
  Word victims5 = ((empty & MASK_L5) << 5) & pieces[passive];
  return (((victims4 & MASK_L5) << 5) | (victims5 << 4)) & backward[active];
}

// Utility function that translate a move to "Move XX to YY"
std::string CheckerBoard::to_string(Word move) {
  int origin = __builtin_ctzl(move & pieces[active]);
  int destination = __builtin_ctzl(move & empty);
  return "Move : " + std::to_string(origin) + " to " + std::to_string(destination);
}

// Overload of operator<< to display ASCII art of a bitboard
std::ostream &operator<<(std::ostream &os, const CheckerBoard &b) {
  // Character representation of each pieces
  const char EMPTY = ' ';
  const char BLACK_KING = 'B';
  const char WHITE_KING = 'W';
  const char WHITE_MAN = 'w';
  const char BLACK_MAN = 'b';
  
  // Special terminal strings that add text coloring
  const std::string BLACK_COLOR = "\033[41m\033[97m";
  const std::string WHITE_COLOR = "\033[107m\033[30m";
  const std::string RESET_COLOR = "\033[0m";

  // Create list of kings and men
  Word black_kings = b.backward[CheckerBoard::BLACK];
  Word black_men = b.forward[CheckerBoard::BLACK] ^ black_kings;
  Word white_kings = b.forward[CheckerBoard::WHITE];
  Word white_men = b.backward[CheckerBoard::WHITE] ^ white_kings;

  // Printing the board
  Word cell;
  int num;
  // The board is represented as such :
  //    28  29  30  31
  //  24  25  26  27
  //    ...
  //     4   5   6   7
  //   0   1   2   3
  // So we have to print characters in this order : 28 29 30 31, 24 25 26 27, ..., 0 1 2 3
  for(int j = 7; j >= 0; --j) {
      if (j % 2) os << "   ";
      for (int i = 0; i < 4; ++i) {
          num = 4*j + i;
          cell = 1 << num;

          if (cell & black_men) os << BLACK_COLOR << BLACK_MAN;
          else if (cell & white_men) os << WHITE_COLOR << WHITE_MAN;
          else if (cell & black_kings) os << BLACK_COLOR << BLACK_KING;
          else if (cell & white_kings) os << WHITE_COLOR << WHITE_KING;
          else os << EMPTY;

          // setfill and setw are used to ensure that every number is printed as 00 01 02 03 etc...
          os << std::setfill('0') << std::setw(2) << num << RESET_COLOR << "   ";
      }
      os << std::endl;
  }
  return os;
}
