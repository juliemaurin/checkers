#ifndef AGENTPLAYER_H
#define AGENTPLAYER_H

#include "checkersagent.h"

class AgentPlayer : public CheckersAgent {
public:
  AgentPlayer();
  virtual ~AgentPlayer();
  virtual Word make_move(const CheckerBoard &board);
};

#endif // AGENTPLAYER_H
