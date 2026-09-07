#include "Evaluation.h"
#include <unordered_map>

int evaluate(const Board& board) {
    std::unordered_map<PieceType, int> values = {
        {PieceType::Pawn, 100},
        {PieceType::Knight, 300},
        {PieceType::Bishop, 300},
        {PieceType::Rook, 500},
        {PieceType::Queen, 900},
        {PieceType::King, 0},
    };

    int score = 0;
    for (int square = 0; square < 64; ++square) {
        Piece piece = board.getSquare(square);

        if (piece.type == PieceType::None)
            continue;
        
        int value = values[piece.type];
        score += (piece.color == Color::White) ? value : -value;
    }

    return score;
}