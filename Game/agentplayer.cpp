#include "agentplayer.h"

AgentPlayer::AgentPlayer()
{

}

AgentPlayer::~AgentPlayer() {

}

Word AgentPlayer::make_move(const CheckerBoard &board) {
  // Kevin always choose a move randomly.
  // Kevin is dumb.
  // Don't be like Kevin.
  std::vector<Word> moves = board.get_moves();
  std::vector<Word>::size_type n = static_cast<std::vector<Word>::size_type>(std::rand() % static_cast<int>(moves.size()));
  return moves.at(n);
}
