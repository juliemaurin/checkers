#ifndef AGENTWALTER_H
#define AGENTWALTER_H

#include "checkersagent.h"
#include <limits>

class AgentWalter : public CheckersAgent {
public:
  AgentWalter();
  virtual ~AgentWalter();
  virtual Word make_move(CheckerBoard &b);

  int score(CheckerBoard &old_board, CheckerBoard &new_board, PieceType player);
  int negamax();
  int minimax(CheckerBoard &old_board, CheckerBoard &new_board, int depth, int alpha, int beta, PieceType maximizingPlayer);

};

#endif // AGENTWALTER_H
