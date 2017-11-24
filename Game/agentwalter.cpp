#include "agentwalter.h"

AgentWalter::AgentWalter(PieceType self, int maxDepth) : self(self), maxDepth(maxDepth) {
  // TODO : Do something else, this is ugly
  if (self == CheckerBoard::WHITE) enemy = CheckerBoard::BLACK;
  else enemy = CheckerBoard::WHITE;
}

AgentWalter::~AgentWalter() {

}

// TODO : Walter (Walter Pitts) will be our main AI
Word AgentWalter::make_move(const CheckerBoard &board) {
  std::vector<Word> moves = board.get_moves();
  std::vector<Word>::size_type best_move = 0;
  std::vector<Word>::size_type index = 0;

  int value = std::numeric_limits<int>::min();
  int depth = maxDepth;

  for(const Word &move : board.get_moves()) {
      CheckerBoard new_board = CheckerBoard(board);
      new_board.make_move(move);
      int new_value;

      if (new_board.active != board.active) {
          new_value = maxi(board, new_board, depth, std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
      } else {
          new_value = mini(board, new_board, depth, std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
      }

      if (new_value > value) {
          best_move = index;
          value = new_value;
      }

      index++;
  }
  std::cout << "Move Value : " << value << std::endl;
  return moves.at(best_move);
}

int AgentWalter::score(const CheckerBoard &old_board, const CheckerBoard &board) {
  // Score should return the odds of wining for player taking the move old_board->board
  // old_board.active is the "player"
  // On draw
  if (old_board.is_draw() || board.is_draw()) return 0;
  // On loss
  if (old_board.is_over()) return std::numeric_limits<int>::min();
  // On win
  if (board.is_over()) return std::numeric_limits<int>::max();

  // Calculating piece difference only
  //int piece_diff = __builtin_popcountl(board.pieces[old_board.active]) - __builtin_popcountl(board.pieces[old_board.passive]);

  // Calculating piece value
  Word player_kings = board.backward[CheckerBoard::BLACK];
  Word player_men = board.forward[CheckerBoard::BLACK] ^ player_kings;
  Word enemy_kings = board.forward[CheckerBoard::WHITE];
  Word enemy_men = board.backward[CheckerBoard::WHITE] ^ enemy_kings;

  int kings_mult = 10;
  int piece_score = kings_mult * (__builtin_popcountl(player_kings) - __builtin_popcountl(enemy_kings)) + __builtin_popcountl(player_men) - __builtin_popcountl(enemy_men);

  // TODO : NN goes here
  return piece_score;
}

int AgentWalter::negamax(const CheckerBoard &old_board, const CheckerBoard &board, int depth, int alpha, int beta, int color) {
  // TODO : Figure out why it does not give the same results as Minimax
  if (!depth || board.is_over()) return color * score(old_board, board);

  int v = std::numeric_limits<int>::min();

  for(const Word &move : board.get_moves()) {
      CheckerBoard new_board = CheckerBoard(board);
      new_board.make_move(move);

      if (new_board.active != board.active) {
          v = std::max(v, -negamax(board, new_board, depth - 1, -beta, -alpha, -color));
      } else {
          v = std::max(v, negamax(board, new_board, depth, alpha, beta, color));
      }

      alpha = std::max(alpha, v);
      if (beta <= alpha) break;
  }
  return v;
}

int AgentWalter::maxi(const CheckerBoard &old_board, const CheckerBoard &board, int depth, int alpha, int beta) {
  if (!depth || board.is_over()) return score(old_board, board);
  int max = std::numeric_limits<int>::min();
  for (const Word &move : board.get_moves()) {
      CheckerBoard new_board = CheckerBoard(board);
      new_board.make_move(move);
      int score;
      if (new_board.active != board.active) {
          score = mini(board, new_board, depth - 1, alpha, beta);
      } else {
          score = maxi(board, new_board, depth, alpha, beta);
      }
      max = std::max(max, score);
      alpha = std::max(alpha, max);
      if (beta <= alpha) break;
  }
  return max;
}

int AgentWalter::mini(const CheckerBoard &old_board, const CheckerBoard &board, int depth, int alpha, int beta) {
  if (!depth || board.is_over()) return -score(old_board, board);
  int min = std::numeric_limits<int>::max();
  for (const Word &move : board.get_moves()) {
      CheckerBoard new_board = CheckerBoard(board);
      new_board.make_move(move);
      int score;
      if (new_board.active != board.active) {
          score = maxi(board, new_board, depth - 1, alpha, beta);
      } else {
          score = mini(board, new_board, depth, alpha, beta);
      }
      min = std::min(min, score);
      beta = std::min(beta, min);
      if (beta <= alpha) break;
  }
  return min;
}
