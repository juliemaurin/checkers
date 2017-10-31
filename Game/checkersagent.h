#ifndef CHECKERSAGENT_H
#define CHECKERSAGENT_H

#include "checkerboard.h"

class CheckersAgent {
public:
  virtual ~CheckersAgent() {}
  virtual Word make_move(CheckerBoard &b) = 0;
};

#endif // CHECKERSAGENT_H
