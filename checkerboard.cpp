#include "checkerboard.h"

const std::string CheckerBoard::BLACK_COLOR = "\033[41m\033[97m";
const std::string CheckerBoard::WHITE_COLOR = "\033[107m\033[30m";
const std::string CheckerBoard::RESET_COLOR = "\033[0m";

const PieceType CheckerBoard::BLACK = 0;
const PieceType CheckerBoard::WHITE = 1;

const Word CheckerBoard::MASK_R3 = 0x70707070;
const Word CheckerBoard::MASK_L5 = 0x00707070;
const Word CheckerBoard::MASK_R5 = 0x0E0E0E00;
const Word CheckerBoard::MASK_L3 = 0x0E0E0E0E;

const Word CheckerBoard::MASK_L9 = 0xEEEEEE00;
const Word CheckerBoard::MASK_L7 = 0x77777700;
const Word CheckerBoard::MASK_R9 = 0x00777777;
const Word CheckerBoard::MASK_R7 = 0x00EEEEEE;

const Word CheckerBoard::MASK_ALL = 0xFFFFFFFF;
const Word CheckerBoard::MASK_TOP = 0xF0000000;
const Word CheckerBoard::MASK_BOT = 0x0000000F;


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
  // Throw exceptionon invalid move
  if (__builtin_popcountl(move) != 2) throw std::invalid_argument("Wrong popcount");

  // Get low move index
  int index_low = __builtin_ctzl(move);
  // Remove trailing zeroes and least set bit
  move >>= index_low + 1;
  // Get high move index
  int index_high = __builtin_ctzl(move) + index_low + 1;

  // Jumps are 7 or 9 bits apart, moves are 3, 4 or 5
  if (index_high - index_low > 6) {
      // Computing taken_piece position
      Word taken_piece = static_cast<Word>(1) << ((index_high + index_low) / 2);
      std::cout << taken_piece << std::endl;

      // Removing taken_piece from board
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
      // Do stuff about next jumps
      /*
      mandatory_jumps = jumps_from(destination)
      if self.mandatory_jumps:*/
      return;
  }

  // Promote a new black king
  if (active == BLACK && (destination & MASK_TOP)) backward[BLACK] |= destination;
  // Promote a new white king
  else if (active == WHITE && (destination & MASK_BOT)) forward[WHITE] |= destination;

  // Stop jumping sequence on move or promotion
  jumping = false;

  // Change turns
  std::swap(active, passive);
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

  int i3 = 0, i4 = 0, i5 = 0;
  int p3 = __builtin_popcountl(fm3);
  int p4 = __builtin_popcountl(fm4);
  int p5 = __builtin_popcountl(fm5);

  std::cout << p3 << std::endl;
  do {
      if (fm3 & 1) moves.push_back(static_cast<Word>(0x09) << i3);
      i3++;
  } while(fm3 >>= 1);

  return moves;
}

std::vector<Word> CheckerBoard::get_jumps() {
  std::vector<Word> a;
  return a;
}

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

Word CheckerBoard::forward_jumpers() {
  Word victims = (pieces[passive] >> 4) | ((pieces[passive] & MASK_R3) >> 3) | ((pieces[passive] & MASK_R5) >> 5);
  Word landings = ((empty & MASK_R7) >> 7) | ((empty & MASK_R9) >> 9);
  return victims & landings & forward[active];
}

Word CheckerBoard::backward_jumpers() {
  Word victims = (pieces[passive] << 4) | ((pieces[passive] & MASK_L3) << 3) | ((pieces[passive] & MASK_L5) << 5);
  Word landings = ((empty & MASK_L7) << 7) | ((empty & MASK_L9) << 9);
  return victims & landings & backward[active];
}


