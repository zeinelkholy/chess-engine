#include "Search.h"
#include "Evaluation.h"
#include "MoveGenerator.h"

#include <vector>
#include <climits>
#include <algorithm>
#include <iostream>

bool isBetter(int score, int best, Color turn) {
    return (turn == Color::White) ? score > best : score < best;
}

int minimax(const Board& board, int depth) {
    if (depth == 0)
        return evaluate(board);
    
    std::vector<Move> legal = generateAllLegalMoves(board);
    Color side = board.turn();
    Color opponent = (side == Color::White) ? Color::Black : Color::White;

    if (legal.empty()) {
        bool checkmate = isSquareAttacked(board, board.kingSquare(side), opponent);

        if (checkmate)
            return (side == Color::White) ? INT_MIN : INT_MAX;
        else
            return 0;
    }

    int best = (side == Color::White) ? INT_MIN : INT_MAX;
    for (const Move& move : legal) {
        Board copy = board;
        copy.makeMove(move);

        int score = minimax(copy, depth - 1);
        if (isBetter(score, best, side))
            best = score;
    }

    return best;
}

Move findBestMove(const Board& board, int depth) {
    std::vector<Move> legal = generateAllLegalMoves(board);
    if (legal.empty())
        return Move(-1, -1);

    Color side = board.turn();
    Move best = legal[0];
    
    int bestScore = (side == Color::White) ? INT_MIN : INT_MAX;
    for (const Move& move : legal) {
        Board copy = board;
        copy.makeMove(move);

        int score = minimax(copy, depth - 1);
        if (isBetter(score, bestScore, side)) {
                bestScore = score;
                best = move;
        }
    }

    return best;
}
