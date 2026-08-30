#include <iostream>
#include "Board.h"
#include "Perft.h"

int main() {
    Board board;
    board.reset();

    for (int depth = 1; depth <= 4; ++depth) {
        std::cout << "Perft(" << depth << ") = " << perft(board, depth) << "\n";
    }

    return 0;
}