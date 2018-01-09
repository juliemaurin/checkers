#ifndef AGENTPLAYER_H
#define AGENTPLAYER_H

#include <limits>
#include "checkersagent.h"

class AgentPlayer : public CheckersAgent {
public:
  AgentPlayer(PieceType self);
  virtual ~AgentPlayer();
  virtual Word make_move(const CheckerBoard &board);

private:
  PieceType self;
};

#endif // AGENTPLAYER_H
