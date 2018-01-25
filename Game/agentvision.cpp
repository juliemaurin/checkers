#include "agentvision.h"

AgentVision::AgentVision(PieceType self) : self(self) {

}

AgentVision::~AgentVision() {

}

Word AgentVision::make_move(const CheckerBoard &board) {
  std::vector<Word> moves = board.get_moves();
  std::vector<Word>::size_type choice = moves.size() + 1;

  int index = 0;
  for (const Word& move : moves) std::cout << "Move " << index++ << ": " << board.to_string(move) << std::endl;

  std::cout << "Please press a key then enter when you have finished moving pieces" << std::endl;

  string c;
  std::cin >> c;

  // Read vision
  TCPHelper tcpH = TCPHelper();
  tcpH.openSocket("127.0.0.1", "20000");
  tcpH.send("TEST");
  string s = tcpH.receive();
  std::cout << s << std::endl;

  // Find move made
  // well well well

  // Check

  // Return move made
  return moves.at(0);
}
