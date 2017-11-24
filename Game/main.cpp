#include <iostream>
#include <ctime>
#include <cstdlib>
#include "checkerboard.h"
#include "agentkevin.h"
#include "agentwalter.h"

int main() {
  // Test script for AI vs. AI
  std::srand(std::time(0));

  CheckerBoard B;
  CheckersAgent *Black = new AgentKevin();
  CheckersAgent *White = new AgentWalter(CheckerBoard::WHITE, 6);

  PieceType current_player = B.active;
  Word move;
  int turn = 1;
  std::cout << B;
  while(!B.is_over()) {
      move = Black->make_move(B);
      std::cout << B.to_string(move) << std::endl;
      B.make_move(move);
      std::cout << B;
      if (B.active == current_player) continue;
      current_player = B.active;

      turn++;
      std::cout << "Tour : " << turn << std::endl;
      while(!B.is_over() && B.active == current_player) {
          move = White->make_move(B);
          std::cout << B.to_string(move) << std::endl;
          B.make_move(move);
          std::cout << B;
      }

      current_player = B.active;
  }

  if (B.is_draw()) std::cout << "It's a draw !" << std::endl;
  else if(B.active == CheckerBoard::WHITE) std::cout << "Congrats Black, you win!" << std::endl;
  else std::cout << "Congrats White, you win!" << std::endl;
  return 0;
}
