#ifndef AGENTARTHUR_H
#define AGENTARTHUR_H

#include "checkersagent.h"

class AgentArthur : public CheckersAgent {
public:
  AgentArthur();
  virtual ~AgentArthur();
  virtual Word make_move(CheckerBoard &b);
};

#endif // AGENTARTHUR_H
