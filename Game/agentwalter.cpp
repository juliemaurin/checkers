#include "agentwalter.h"

AgentWalter::AgentWalter() {

}

AgentWalter::~AgentWalter() {

}

// TODO : Walter (Walter Pitts) will be our main AI
Word AgentWalter::make_move(CheckerBoard &b) {
  return 0;
}

int AgentWalter::score(CheckerBoard &old_board, CheckerBoard &new_board, PieceType player) {
  return 0;
}

int AgentWalter::negamax() {
  return 0;
}

int AgentWalter::minimax(CheckerBoard &old_board, CheckerBoard &new_board, int depth, int alpha, int beta, PieceType maximizingPlayer) {
  if (!depth || new_board.is_over()) return score(old_board, new_board, maximizingPlayer);

  if (maximizingPlayer == CheckerBoard::BLACK) {
      int v = std::numeric_limits<int>::min();
      for(const Word &move : new_board.get_moves()) {
          CheckerBoard B = CheckerBoard(new_board);
          B.make_move(move);
          int max = minimax(new_board, B, depth - 1, alpha, beta, CheckerBoard::WHITE);
          if (max > v) v = max;
          if (max > alpha) alpha = max;
          if (beta <= alpha) break;
      }
      return v;
    } else {
      int v = std::numeric_limits<int>::max();
      for(const Word &move : new_board.get_moves()) {
          CheckerBoard B = CheckerBoard(new_board);
          B.make_move(move);
          int min = minimax(new_board, B, depth - 1, alpha, beta, CheckerBoard::BLACK);
          if (min < v) v = min;
          if (min > beta) beta = min;
          if (beta <= alpha) break;
      }
      return v;
    }
}
