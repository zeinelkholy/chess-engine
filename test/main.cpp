#include "Board.h"
#include "Perft.h"

#include <iostream>
#include <string>

int main() {
    Board board;
    board.reset();

    std::string move;
    while (true) {
        board.print();
        std::cout << "\nEnter move: ";
        std::cin >> move;

        if (move == "x")
            break;

        std::string from = move.substr(0, 2);
        std::string to = move.substr(2, 2);
        board.makeMove(Move(algebraToSquare(from), algebraToSquare(to)));
    }

    return 0;
}