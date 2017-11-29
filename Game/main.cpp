#include <iostream>
#include <ctime>
#include <cstdlib>
#include "checkerboard.h"
#include "agentkevin.h"
#include "agentwalter.h"
#include "agentplayer.h"

void play(const std::string black_agent, const std::string white_agent) {
  std::srand(std::time(0));

  CheckerBoard B;
  CheckersAgent *black;
  CheckersAgent *white;

  int max_depth = 10;

  if (black_agent == "walter") {
      black = new AgentWalter(CheckerBoard::BLACK, max_depth);
  } else if (black_agent == "player") {
      black = new AgentPlayer(CheckerBoard::BLACK);
  } else {
      black = new AgentKevin();
  }
  if (white_agent == "walter") {
      white = new AgentWalter(CheckerBoard::WHITE, max_depth);
  } else if (white_agent == "player") {
      white = new AgentPlayer(CheckerBoard::WHITE);
  } else {
      white = new AgentKevin();
  }

  PieceType current_player = B.active;
  Word move;
  int turn = 1;
  std::cout << B;
  while(!B.is_over()) {
      move = black->make_move(B);
      std::cout << "Move : " << B.to_string(move) << std::endl;
      B.make_move(move);
      std::cout << B;
      if (B.active == current_player) continue;
      current_player = B.active;

      turn++;
      std::cout << "Tour : " << turn << std::endl;
      while(!B.is_over() && B.active == current_player) {
          move = white->make_move(B);
          std::cout << "Move : " << B.to_string(move) << std::endl;
          B.make_move(move);
          std::cout << B;
      }

      current_player = B.active;
  }

  if (B.is_draw()) std::cout << "It's a draw !" << std::endl;
  else if(B.active == CheckerBoard::WHITE) std::cout << "Congrats Black, you win!" << std::endl;
  else std::cout << "Congrats White, you win!" << std::endl;

  delete black;
  delete white;
}

void usage(std::string argv0) {
  std::cout << "Usage : " << argv0 << " <black agent> <white agent>" << std::endl << std::endl;
  std::cout << "Available 'agents' are : - 'kevin' : Random player" << std::endl;
  std::cout << "                         - 'walter' : NN-based AlphaBeta AI" << std::endl;
  std::cout << "                         - 'player' : Human player" << std::endl;
  std::cout << "                         - 'arthur' : (TBI) Arthur Lee Samuel's scoring function (on top of AlphaBeta)" << std::endl;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
      usage(argv[0]);
      return EXIT_FAILURE;
  }

  play(argv[1], argv[2]);

  return EXIT_SUCCESS;
}
