#include <iostream>
#include "Board.h"
#include "Move.h"

int main() {

    Board board;
    board.reset();
    board.print();

    Move e2e4(12, 28);

    board.makeMove(e2e4);
    board.print();

    Move d7d4(51, 27);
    board.makeMove(d7d4);
    board.print();

    Move enPassant(27, 20, PieceType::None, false, true, false);
    board.makeMove(enPassant);
    board.print();

    return 0;
}
