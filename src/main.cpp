#include "Board.h"
#include "Move.h"
#include "MoveGenerator.h"
#include "Notation.h"

#include <iostream>
#include <vector>
#include <string>

int main() {

    Board board;
    board.reset();

    std::string move;
    while (move != "x") {
        board.print();
        std::cout << "\nEnter move: ";
        std::cin >> move;

        std::string from = move.substr(0, 2);
        std::string to = move.substr(2, 2);
        board.makeMove(Move(algebraToSquare(from), algebraToSquare(to)));
    }

    return 0;
}
