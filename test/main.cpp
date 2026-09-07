#include "Board.h"
#include "Perft.h"
#include "MoveGenerator.h"
#include "Move.h"
#include "Search.h"
#include "Notation.h"
#include "Evaluation.h"

#include <iostream>
#include <string>
#include <vector>

int main() {
    Board board;
    board.reset();

    while (true) {
        board.print();
        Move move = findBestMove(board, 4);
        if (move.isNull()) {
            std::cout << "Game Over" << '\n';
            break;
        }
        board.makeMove(move);
    }

    return 0;
}