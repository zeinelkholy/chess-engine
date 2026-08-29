#include "MoveGenerator.h"
#include "Notation.h"
#include <vector>
#include <array>

std::vector<Move> generateKnightMoves(const Board& board, int square) {
    std::vector<Move> moves;
    std::array<std::pair<int, int>, 8> offsets = {{
        {2, 1}, {2, -1}, {-2, 1}, {-2, -1},
        {1, 2}, {1, -2}, {-1, 2}, {-1, -2},
    }};

    auto [file, rank] = squareToFileRank(square);

    for (auto [fileOffset, rankOffset] : offsets) {
        int targetFile = file + fileOffset;
        int targetRank = rank + rankOffset;

        if (targetFile < 0 || targetFile > 7 || targetRank < 0 || targetRank > 7)
            continue;

        int target = fileRankToSquare(targetFile, targetRank);
        if (board.getSquare(target).color != board.getSquare(square).color)
            moves.push_back(Move(square, target));
    }

    return moves;
}

std::vector<Move> generateKingMoves(const Board& board, int square) {
    std::vector<Move> moves;
    std::array<std::pair<int, int>, 8> offsets = {{
        {1, 0}, {0, 1}, {1, 1}, {-1, 1},
        {-1, 0}, {0, -1}, {-1, -1}, {1, -1}
    }};
    auto [file, rank] = squareToFileRank(square);

    for (auto [x, y] : offsets) {
        int targetFile = file + x;
        int targetRank = rank + y;

        if (targetFile < 0 || targetFile > 7 || targetRank < 0 || targetRank > 7)
            continue;

        int target = fileRankToSquare(targetFile, targetRank);
        if (board.getSquare(target).color != board.getSquare(square).color)
            moves.push_back(Move(square, target));
    }

    return moves;
}
