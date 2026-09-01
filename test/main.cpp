#include "Board.h"
#include "Perft.h"
#include "MoveGenerator.h"
#include "Move.h"

#include <iostream>
#include <string>
#include <vector>

int main() {
    Board board;
    board.reset();

    board.setSquare("c1", Piece());
    board.setSquare("b1", Piece());
    board.setSquare("d1", Piece());
    board.setSquare("g1", Piece());
    board.setSquare("f1", Piece());

    board.makeMove(Move("d2", "d4"));
    board.makeMove(Move("d8", "b4"));

    board.print();

    for (Move move : generateAllLegalMoves(board)) {
        std::cout << move.algebraicNotation() << '\n';
    }
    

    return 0;
}