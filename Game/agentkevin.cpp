#include "agentkevin.h"

AgentKevin::AgentKevin() {
    control = TCPHelper();
    std::cout << "Connecting AgentKevin to control" << std::endl;
    control.openSocket("127.0.0.1", "10000");
}

AgentKevin::~AgentKevin() {
    control.closeSocket();
}

Word AgentKevin::make_move(const CheckerBoard &board) {
  // Kevin always choose a move randomly.
  // Kevin is dumb.
  // Don't be like Kevin.
  std::cout << "Kevin thinking" << std::endl;
  std::vector<Word> moves = board.get_moves();
  size_t n = static_cast<size_t>(std::rand() % static_cast<int>(moves.size()));

  control.send(board.to_string(moves.at(n)));
  std::string s = control.receive();
  std::cout << "AgentKevin Response : " << s << std::endl;
  return moves.at(n);
}
