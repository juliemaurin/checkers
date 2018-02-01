#ifndef AGENTKEVIN_H
#define AGENTKEVIN_H

#include "checkersagent.h"
#include "tcphelper.h"

class AgentKevin : public CheckersAgent {
public:
  AgentKevin();
  virtual ~AgentKevin();
  virtual Word make_move(const CheckerBoard &board);

private:
  TCPHelper control;
};

#endif // AGENTKEVIN_H
