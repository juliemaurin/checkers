#ifndef CHECKERBOARD_H
#define CHECKERBOARD_H

#include <iostream>
#include <iomanip>
#include <string>
#include <utility>
#include <vector>

typedef unsigned long Word;
typedef unsigned char PieceType;

class CheckerBoard {
public:
    CheckerBoard();
    ~CheckerBoard();
    CheckerBoard(const CheckerBoard &b);

    static const PieceType BLACK;
    static const PieceType WHITE;

    Word forward[2];
    Word backward[2];
    Word pieces[2];
    Word empty;

    PieceType active;
    PieceType passive;

    void make_move(Word move);

    const std::vector<Word> &get_moves() const;

    bool is_draw() const;
    bool is_over() const;

    std::string to_string(Word move) const;

    friend std::ostream& operator<<(std::ostream& os, const CheckerBoard& b);

private:
    static const Word MASK_R3;
    static const Word MASK_R5;
    static const Word MASK_L3;
    static const Word MASK_L5;

    static const Word MASK_R7;
    static const Word MASK_R9;
    static const Word MASK_L7;
    static const Word MASK_L9;

    static const Word MASK_ALL;
    static const Word MASK_TOP;
    static const Word MASK_BOT;

    static const Word MASK_MOVE3;
    static const Word MASK_MOVE4;
    static const Word MASK_MOVE5;
    static const Word MASK_JUMP7;
    static const Word MASK_JUMP9;

    bool jumping;

    void new_game();

    static void add_move(std::vector<Word> &arr, Word move, Word movers, int offset);

    std::vector<Word> current_moves;
    std::vector<Word> current_jumps;

    void gen_moves();
    void gen_jumps();

    Word forward_movers3() const;
    Word forward_movers4() const;
    Word forward_movers5() const;
    Word backward_movers3() const;
    Word backward_movers4() const;
    Word backward_movers5() const;

    Word forward_jumpers7() const;
    Word forward_jumpers9() const;
    Word backward_jumpers7() const;
    Word backward_jumpers9() const;

};

#endif // CHECKERBOARD_H
