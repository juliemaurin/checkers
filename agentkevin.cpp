#include "agentkevin.h"

AgentKevin::AgentKevin() {

}

AgentKevin::~AgentKevin() {

}

Word AgentKevin::make_move(CheckerBoard &b) {
  std::vector<Word> moves = b.get_moves();
  int n = std::rand() % static_cast<int>(moves.size());
  return moves.at(static_cast<std::vector<Word>::size_type>(n));
}
