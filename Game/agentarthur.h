#ifndef AGENTARTHUR_H
#define AGENTARTHUR_H

#include "checkersagent.h"
#include <limits>

class AgentArthur : public CheckersAgent {
public:
  AgentArthur();
  virtual ~AgentArthur();
  virtual Word make_move(const CheckerBoard &board);
};

#endif // AGENTARTHUR_H
