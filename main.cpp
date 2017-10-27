#include <iostream>
#include "checkerboard.h"
#include <bitset>
#include <chrono>

int main() {
//  CheckerBoard B;
//  Word move = 0x00000011;

//  Word jump = 0x00000408;

//  B.make_move(move);
//  B.make_move(jump);
//  std::cout << std::bitset<32>(B.forward_movers3()) << std::endl;
//  std::cout << std::bitset<32>(B.forward_movers4()) << std::endl;
//  std::cout << std::bitset<32>(B.forward_movers5()) << std::endl;

//  std::vector<Word> M = B.get_moves();
//  for (auto m : M) std::cout << std::bitset<32>(m) << std::endl;

  std::vector<Word> moves;
  Word fm3 = 0x10000011;
  int i3 = 0;

  std::chrono::time_point<std::chrono::system_clock> start, end;
  start = std::chrono::system_clock::now();
  while(fm3) {
      int t = __builtin_ctzl(fm3);
      moves.push_back(static_cast<Word>(0x09) << i3);
      i3 += t;
      fm3 >>= t;
  }
  /*do {
      if (fm3 & 1) moves.push_back(static_cast<Word>(0x09) << i3);
      i3++;
  } while(fm3 >>= 1);*/
  end = std::chrono::system_clock::now();

  for (auto m : moves) std::cout << std::bitset<32>(m) << std::endl;

  std::cout << (end - start).count() << std::endl;
  return 0;
}
