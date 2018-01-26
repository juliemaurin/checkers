#ifndef AGENTVISION_H
#define AGENTVISION_H

#include <limits>
#include "tcphelper.h"
#include "checkersagent.h"

class AgentVision : public CheckersAgent {
public:
  AgentVision(PieceType self);
  virtual ~AgentVision();
  virtual Word make_move(const CheckerBoard &board);

private:
  PieceType self;
  TCPHelper vision;
};

#endif // AGENTVISION_H
