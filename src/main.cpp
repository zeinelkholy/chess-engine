#include "Board.h"
#include "Move.h"
#include "MoveGenerator.h"
#include "Notation.h"

#include <iostream>
#include <vector>

int main() {

    Board board;
    board.reset();
    board.print();

    Move e2e4("e2", "e4", PieceType::None, false, false, true);
    Move d7d5("d7", "d5", PieceType::None, false, false, true);
    Move nc3 = generateKnightMoves(board, 1).at(0);

    board.makeMove(e2e4);
    board.print();

    board.makeMove(d7d5);
    board.print();

    board.makeMove(nc3);
    board.print();

    return 0;
}
