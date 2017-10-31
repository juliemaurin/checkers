#ifndef AGENTKEVIN_H
#define AGENTKEVIN_H

#include <cstdlib>
#include <vector>
#include "checkersagent.h"

class AgentKevin : public CheckersAgent {
public:
  AgentKevin();
  virtual ~AgentKevin();
  virtual Word make_move(CheckerBoard &b);
};

#endif // AGENTKEVIN_H
