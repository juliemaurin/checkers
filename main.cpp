#include <iostream>
#include "checkerboard.h"
#include <bitset>
#include <ctime>

int main() {
  std::cout << 23/4 << std::endl;
  CheckerBoard B;
  PieceType current_player = B.active;
  Word move;
  int turn = 1;
  std::cout << B;
  while(!B.is_over()) {
      move = B.get_moves().at(0);
      std::cout << "M : " << std::bitset<32>(move) << std::endl;
      std::cout << B.to_string(move) << std::endl;
      B.make_move(move);
      std::cout << B;
      if (B.active == current_player) continue;
      current_player = B.active;

      turn++;
      //return 0;
      std::cout << "TURN : " << turn << std::endl;
      while(!B.is_over() && B.active == current_player) {
          move = B.get_moves().at(0);
          std::cout << "M : " << std::bitset<32>(move) << std::endl;
          std::cout << B.to_string(move) << std::endl;
          B.make_move(move);
          std::cout << B;
      }

      current_player = B.active;
  }

  std::cout << B;

  if (B.is_draw()) std::cout << "It's a draw !" << std::endl;
  else if(B.active == CheckerBoard::WHITE) std::cout << "Congrats Black, you win!" << std::endl;
  else std::cout << "Congrats White, you win!" << std::endl;

  return 0;
}
