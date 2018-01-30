#include "agentvision.h"

AgentVision::AgentVision(PieceType self) : self(self) {
    // Read vision
    vision = TCPHelper();

    std::cout << "Connecting to vision" << std::endl;
    vision.openSocket("127.0.0.1", "20000");
}

AgentVision::~AgentVision() {
    vision.closeSocket();
}

Word AgentVision::make_move(const CheckerBoard &board) {
  std::cout << "Wait what ?" << std::endl;
  return 0;
}


void AgentVision::replace_board(CheckerBoard &board) {
  std::vector<Word> moves = board.get_moves();

  int index = 0;
  for (const Word& move : moves) std::cout << "Move " << index++ << ": " << board.to_string(move) << std::endl;

  std::cout << "Please press a key then enter when you have finished moving pieces" << std::endl;

  std::string c;
  std::cin >> c;
  std::cin.clear();

  std::cout << "Sending request" << std::endl;
  vision.send("PLEASE");

  std::cout << "Receiving data response" << std::endl;
  std::string s = vision.receive();

  std::cout << "Response :" << s << std::endl;

  // Find move made

  // Find delimiter
  std::string delimiter = "+";
  size_t split = s.find(delimiter);

  // Split the data string
  std::string black_string = s.substr(0, split);
  std::string white_string = s.substr(split + delimiter.length());

  Word black = static_cast<Word>(stoull(black_string));
  Word white = static_cast<Word>(stoull(white_string));

  std::cout << s << " -> " << black << " " << white << std::endl;

  // TODO : Guess move made from picture
  // ON REMPLACE LE BOARD ICI
  board.new_game(black, white, self);
}
