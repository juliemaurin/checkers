#ifndef CHECKERBOARD_H
#define CHECKERBOARD_H

#include <iostream>
#include <iomanip>
#include <string>
#include <utility>
#include <vector>

#include <bitset>

typedef unsigned long Word;
typedef unsigned char PieceType;

class CheckerBoard {
public:
    CheckerBoard();

//private:
    static const PieceType BLACK;
    static const PieceType WHITE;

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

    Word forward[2];
    Word backward[2];
    Word pieces[2];
    Word empty;

    PieceType active;
    PieceType passive;

    bool jumping;

    std::vector<Word> mandatory_jumps;

    void new_game();
    void make_move(Word move);
    void add_move(std::vector<Word>& arr, Word move, Word movers, int offset);

    CheckerBoard *peek_move(Word move);
    std::vector<Word> get_moves();
    std::vector<Word> get_jumps();

    bool is_draw();
    bool is_over();

    Word forward_movers3();
    Word forward_movers4();
    Word forward_movers5();
    Word backward_movers3();
    Word backward_movers4();
    Word backward_movers5();

    Word forward_jumpers7();
    Word forward_jumpers9();
    Word backward_jumpers7();
    Word backward_jumpers9();

    std::string to_string(Word move);

    friend std::ostream& operator<<(std::ostream& os, const CheckerBoard& b);

};

#endif // CHECKERBOARD_H
