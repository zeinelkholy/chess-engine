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

    return 0;
}
