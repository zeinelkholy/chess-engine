#include "Perft.h"
#include "MoveGenerator.h"

int perft(const Board& board, int depth) {
    if (depth == 0) return 1;
    int nodes = 0;
    for (const Move& move : generateAllLegalMoves(board)) {
        Board copy = board;
        copy.makeMove(move);
        nodes += perft(copy, depth - 1);
    }
    return nodes;
}