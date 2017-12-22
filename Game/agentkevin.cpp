#include "agentkevin.h"

AgentKevin::AgentKevin() {

}

AgentKevin::~AgentKevin() {

}

Word AgentKevin::make_move(const CheckerBoard &board) {
  // Kevin always choose a move randomly.
  // Kevin is dumb.
  // Don't be like Kevin.
  std::vector<Word> moves = board.get_moves();
  std::vector<Word>::size_type n = static_cast<std::vector<Word>::size_type>(std::rand() % static_cast<int>(moves.size()));
  TCPHelper("127.0.0.1", board.to_string(moves.at(n)));
  std::cout << board.to_string(moves.at(n)) << std::endl;
  return moves.at(n);
}
