#ifndef CHECKERBOARD_H
#define CHECKERBOARD_H

#include <iostream>
#include <string>
#include <utility>
#include <vector>

typedef unsigned long Word;
typedef unsigned char PieceType;

class CheckerBoard {
public:
    CheckerBoard();

//private:
    static const std::string BLACK_COLOR;
    static const std::string WHITE_COLOR;
    static const std::string RESET_COLOR;

    static const PieceType BLACK;
    static const PieceType WHITE;

    static const Word MASK_R3;
    static const Word MASK_R5;
    static const Word MASK_L3;
    static const Word MASK_L5;

    static const Word MASK_L9;
    static const Word MASK_R9;
    static const Word MASK_L7;
    static const Word MASK_R7;

    static const Word MASK_ALL;
    static const Word MASK_TOP;
    static const Word MASK_BOT;

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
    CheckerBoard *peek_move(Word move);
    std::vector<Word> get_moves();
    std::vector<Word> get_jumps();
    std::vector<Word> jumps_from(Word piece);

    Word forward_movers3();
    Word forward_movers4();
    Word forward_movers5();
    Word backward_movers3();
    Word backward_movers4();
    Word backward_movers5();

    Word forward_jumpers();
    Word backward_jumpers();

};

#endif // CHECKERBOARD_H
