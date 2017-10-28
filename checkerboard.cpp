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

CheckerBoard::CheckerBoard() :
  forward{0,0},
  backward{0,0},
  pieces{0,0}
{
  new_game();
}

void CheckerBoard::new_game() {
  active = BLACK;
  passive = WHITE;

  forward[BLACK]  = 0x00000FFF;
  backward[BLACK] = 0x00000000;
  pieces[BLACK] = forward[BLACK] | backward[BLACK];

  forward[WHITE]  = 0x00000000;
  backward[WHITE] = 0xFFF00000;
  pieces[WHITE] = forward[WHITE] | backward[WHITE];

  empty = MASK_ALL ^ (pieces[BLACK] | pieces[WHITE]);

  jumping = false;
}

void CheckerBoard::make_move(Word move) {
  if (move == 0x4020000) {
      std::cout << "FIRST" << std::endl;
    }

  // Get low move index
  int index_low = __builtin_ctzl(move);
  // Get high move index
  int index_high = __builtin_ctzl(move >> (index_low + 1)) + index_low + 1;

  // Jumps are 7 or 9 bits apart, moves are 3, 4 or 5
  if (index_high - index_low > 6) {
      // Computing taken_piece position
      Word taken_piece = static_cast<Word>(1) << ((index_high + index_low) / 2);
      if (((index_low / 4) % 2) != 0) taken_piece <<= 1;

      // Removing taken_piece from board
      if ((pieces[passive] & taken_piece) == 0) throw std::invalid_argument("WRONG JUMP");
      pieces[passive] ^= taken_piece;
      // Removing taken_piece from enemy pieces
      if (forward[passive] & taken_piece) forward[passive] ^= taken_piece;
      if (backward[passive] & taken_piece) backward[passive] ^= taken_piece;

      jumping = true;
  }

  // Moving piece on board
  pieces[active] ^= move;
  // Moving piece on own pieces
  if (forward[active] & move) forward[active] ^= move;
  if (backward[active] & move) backward[active] ^= move;

  Word destination = move & pieces[active];
  empty = MASK_ALL ^ (pieces[BLACK] | pieces[WHITE]);

  if (jumping) {
      // If we are jumping
      // And the next move is another jump, we return now. Two things don't happen :
      // Promotion : because if we can still jump : a) We're already a king
      //                                            b) We're not on the top/bottom row
      // Swapping : Because the active player keeps playing.
      std::cout << "MANDATORY" << std::endl;
      mandatory_jumps = get_jumps();

      if (mandatory_jumps.size()) return;
  }

  // Promote a new black king
  if (active == BLACK && (destination & MASK_TOP) != 0) backward[BLACK] |= destination;
  // Promote a new white king
  else if (active == WHITE && (destination & MASK_BOT) != 0) forward[WHITE] |= destination;

  // Stop jumping sequence on move or promotion
  jumping = false;

  // Change turns
  std::swap(active, passive);
}

void CheckerBoard::add_move(std::vector<Word>& arr, Word move, Word movers, int offset=0) {
  int i = 0;
  while(movers) {
      int t = __builtin_ctzl(movers)+1;
      movers >>= t;
      i += t-1;
      arr.push_back(static_cast<Word>(move) << (i++ - offset));
  }
}

std::vector<Word> CheckerBoard::get_moves() {
  if (jumping) return mandatory_jumps;

  std::vector<Word> jumps = get_jumps();
  if (jumps.size()) return jumps;

  Word fm3 = forward_movers3();
  Word fm4 = forward_movers4();
  Word fm5 = forward_movers5();
  Word bm3 = backward_movers3();
  Word bm4 = backward_movers4();
  Word bm5 = backward_movers5();

  std::vector<Word> moves;
  add_move(moves, MASK_MOVE3, fm3);
  add_move(moves, MASK_MOVE4, fm4);
  add_move(moves, MASK_MOVE5, fm5);

  add_move(moves, MASK_MOVE3, bm3, 3);
  add_move(moves, MASK_MOVE4, bm4, 4);
  add_move(moves, MASK_MOVE5, bm5, 5);

  return moves;
}

std::vector<Word> CheckerBoard::get_jumps() {
  Word fj7 = forward_jumpers7();
  Word fj9 = forward_jumpers9();
  Word bj7 = backward_jumpers7();
  Word bj9 = backward_jumpers9();

  std::vector<Word> jumps;
  add_move(jumps, MASK_JUMP7, fj7);
  add_move(jumps, MASK_JUMP9, fj9);

  add_move(jumps, MASK_JUMP7, bj7, 7);
  add_move(jumps, MASK_JUMP9, bj9, 9);

  return jumps;
}

bool CheckerBoard::is_draw() {
  return false;
}

bool CheckerBoard::is_over() {
  return !(get_moves().size()) || is_draw();
}

// ===== MOVERS AND JUMPERS GENERATION =====
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

std::string CheckerBoard::to_string(Word move) {
  int origin = __builtin_ctzl(move & pieces[active]);
  int destination = __builtin_ctzl(move & empty);
  return "Move : " + std::to_string(origin) + " to " + std::to_string(destination);
}

std::ostream &operator<<(std::ostream &os, const CheckerBoard &b) {
  const char EMPTY = ' ';
  const char BLACK_KING = 'B';
  const char WHITE_KING = 'W';
  const char WHITE_MAN = 'w';
  const char BLACK_MAN = 'b';
  const std::string BLACK_COLOR = "\033[41m\033[97m";
  const std::string WHITE_COLOR = "\033[107m\033[30m";
  const std::string RESET_COLOR = "\033[0m";

  Word black_kings = b.backward[CheckerBoard::BLACK];
  Word black_men = b.forward[CheckerBoard::BLACK] ^ black_kings;
  Word white_kings = b.forward[CheckerBoard::WHITE];
  Word white_men = b.backward[CheckerBoard::WHITE] ^ white_kings;

  Word cell;
  int num;
  // We browse the board from top to bottom and left to right, so :
  //    28  29  30  31
  //  24  25  26  27
  //    ...
  //     4   5   6   7
  //   0   1   2   3
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

          os << std::setfill('0') << std::setw(2) << num << RESET_COLOR << "   ";
      }
      os << std::endl;
  }
  return os;
}
