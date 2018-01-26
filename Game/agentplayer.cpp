#include "agentplayer.h"

AgentPlayer::AgentPlayer(PieceType self) : self(self) {

}

AgentPlayer::~AgentPlayer() {

}

Word AgentPlayer::make_move(const CheckerBoard &board) {
  std::vector<Word> moves = board.get_moves();
  size_t choice = moves.size() + 1;

  int index = 0;
  for (const Word& move : moves) std::cout << "Move " << index++ << ": " << board.to_string(move) << std::endl;

  do {
      std::cout << "Please input a number between 0 and " << moves.size() - 1 << " : ";
      if (std::cin.fail()) {
          std::cin.clear();
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
      std::cin >> choice;
  } while(choice >= moves.size() || std::cin.fail());

  return moves.at(choice);
}
