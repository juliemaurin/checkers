# Checkers++/Game
C++ Port of [Checkers](https://github.com/Ziink4/checkers), a simple game engine for playing [English Draughts](https://en.wikipedia.org/wiki/English_draughts)

*This file is a work in progress, and its subsections are not yet ordered in any particular order.*

## Board Representation
The board is represented as a 32-bit unsigned integer, with each bit corresponding to a cell in the board.
The ordering we used is the following : 

      28  29  30  31
    24  25  26  27
      20  21  22  23
    16  17  18  19  
      12  13  14  15
    08  09  10  11
      04  05  06  07
    00  01  02  03

This type of encoding is known as a [Bitboard](https://en.wikipedia.org/wiki/Bitboard). this method of representing a board benefits from the performance of fully-pipelined bitwise operators of modern CPUs

Every [move or jump](https://en.wikipedia.org/wiki/English_draughts#Move_rules) during the game is represented as an integer with only 2 bits sets, the origin and the destination of the move.

## Rules
This engine follows [all the basic rules](https://en.wikipedia.org/wiki/English_draughts#Rules) for English draughts, with only one addition :

To avoid endless games (in AI vs. AI matches), draws are enforced by the [treefold repetition](https://en.wikipedia.org/wiki/Threefold_repetition) rule. Any board arrangement that shows up for the third time will automatically force a draw.

##

> Written with [StackEdit](https://stackedit.io/).
