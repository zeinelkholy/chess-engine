#include "MoveGenerator.h"
#include "Notation.h"
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

std::vector<Move> generateSlidingMoves(const Board& board, int square, const std::vector<std::pair<int, int>>& directions) {
    std::vector<Move> moves;

    auto [file, rank] = squareToFileRank(square);

    for (auto [fileStep, rankStep] : directions) {
        for (int i = 1; i < 8; ++i) {
            int targetFile = file + fileStep * i;
            int targetRank = rank + rankStep * i;

            if (targetFile < 0 || targetFile > 7 || targetRank < 0 || targetRank > 7)
                break;

            int target = fileRankToSquare(targetFile, targetRank);
            if (board.getSquare(target).color == board.getSquare(square).color)
                break;
            
            moves.push_back(Move(square, target));

            if (board.getSquare(target).color != Color::None)
                break;
        }
    }

    return moves;
}

std::vector<Move> generateBishopMoves(const Board& board, int square) {
    std::vector<std::pair<int, int>> directions = {
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
    };
    return generateSlidingMoves(board, square, directions);
}

std::vector<Move> generateRookMoves(const Board& board, int square) {
    std::vector<std::pair<int, int>> directions = {
        {1, 0}, {0, 1}, {-1, 0}, {0, -1}
    };
    return generateSlidingMoves(board, square, directions);
}

std::vector<Move> generateQueenMoves(const Board& board, int square) {
    std::vector<std::pair<int, int>> directions = {
        {1, 0}, {0, 1}, {-1, 0}, {0, -1}, 
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
    };
    return generateSlidingMoves(board, square, directions);
}

std::vector<Move> generatePawnMoves(const Board& board, int square) {
    static const std::array<PieceType, 4> promotionTypes = {
        PieceType::Knight, PieceType::Bishop, PieceType::Rook, PieceType::Queen
    };
    std::vector<Move> moves;
    Color color = board.getSquare(square).color;
    auto [file, rank] = squareToFileRank(square);
    int forward = (color == Color::White) ? 1 : -1;

    auto isPromotionRank = [](int r) { return r == 0 || r == 7; };

    // Single push
    int targetRank = rank + forward;
    if (targetRank >= 0 && targetRank <= 7) {
        int singleTarget = fileRankToSquare(file, targetRank);
        if (board.getSquare(singleTarget).type == PieceType::None) {
            if (isPromotionRank(targetRank)) {
                for (PieceType type : promotionTypes)
                    moves.push_back(Move(square, singleTarget, type));
            } else {
                moves.push_back(Move(square, singleTarget));
            }
        }

        // Double push
        bool onStartRank = (color == Color::White && rank == 1) || (color == Color::Black && rank == 6);
        if (onStartRank && board.getSquare(singleTarget).type == PieceType::None) {
            int doubleTarget = fileRankToSquare(file, rank + forward * 2);
            if (board.getSquare(doubleTarget).type == PieceType::None) {
                moves.push_back(Move(square, doubleTarget, PieceType::None, false, false, true));
            }
        }
    }

    // Diagonal capture + En passant
    for (int fileOffset : {-1, 1}) {
        int targetFile = file + fileOffset;
        if (targetFile < 0 || targetFile > 7 || targetRank < 0 || targetRank > 7)
            continue;

        int diagonalTarget = fileRankToSquare(targetFile, targetRank);
        Piece diagonal = board.getSquare(diagonalTarget);

        bool isEnemyCapture = diagonal.type != PieceType::None && diagonal.color != color;
        bool isEnPassantCapture = (board.enPassantTarget() != -1 && diagonalTarget == board.enPassantTarget());

        if (isEnemyCapture) {
            if (isPromotionRank(targetRank)) {
                for (PieceType type : promotionTypes)
                    moves.push_back(Move(square, diagonalTarget, type));
            } else {
                moves.push_back(Move(square, diagonalTarget));
            }
        } else if (isEnPassantCapture) {
            moves.push_back(Move(square, diagonalTarget, PieceType::None, false, true));
        }
    }

    return moves;
}

bool isSquareAttacked(const Board& board, int square, Color opponent) {
    auto [file, rank] = squareToFileRank(square);

    std::array<std::pair<int, int>, 8> knightOffsets = {{
        {1, 2}, {2, 1}, {2, -1}, {1, -2},
        {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}
    }};
    for (auto [fileOffset, rankOffset] : knightOffsets) {
        int targetFile = file + fileOffset;
        int targetRank = rank + rankOffset;
        if (targetFile < 0 || targetFile > 7 || targetRank < 0 || targetRank > 7)
            continue;
        Piece piece = board.getSquare(fileRankToSquare(targetFile, targetRank));
        if (piece.type == PieceType::Knight && piece.color == opponent)
            return true;
    }

    std::array<std::pair<int,int>, 8> kingOffsets = {{
        {1, 0}, {1, 1}, {0, 1}, {-1, 1},
        {-1, 0}, {-1, -1}, {0, -1}, {1, -1}
    }};
    for (auto [fileOffset, rankOffset] : kingOffsets) {
        int targetFile = file + fileOffset;
        int targetRank = rank + rankOffset;
        if (targetFile < 0 || targetFile > 7 || targetRank < 0 || targetRank > 7)
            continue;
        Piece piece = board.getSquare(fileRankToSquare(targetFile, targetRank));
        if (piece.type == PieceType::King && piece.color == opponent)
            return true;
    }

    std::array<std::pair<int,int>, 8> slidingDirections = {{
        {1, 0}, {-1, 0}, {0, 1}, {0, -1},   // straight (rook)
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}  // diagonal (bishop)
    }};

    for (int d = 0; d < 8; ++d) {
        auto [fileStep, rankStep] = slidingDirections[d];
        bool isDiagonal = (d >= 4);

        for (int i = 1; i < 8; ++i) {
            int targetFile = file + fileStep * i;
            int targetRank = rank + rankStep * i;
            if (targetFile < 0 || targetFile > 7 || targetRank < 0 || targetRank > 7)
                break;

            Piece piece = board.getSquare(fileRankToSquare(targetFile, targetRank));
            if (piece.type == PieceType::None)
                continue;

            if (piece.color == opponent) {
                if (piece.type == PieceType::Queen) return true;
                if (!isDiagonal && piece.type == PieceType::Rook) return true;
                if (isDiagonal && piece.type == PieceType::Bishop) return true;
            }
            break;
        }
    }

    // Pawn attacks
    int pawnAttackFromRank = (opponent == Color::White) ? rank - 1 : rank + 1;

    if (pawnAttackFromRank >= 0 && pawnAttackFromRank <= 7) {
        for (int fileOffset : {-1, 1}) {
            int targetFile = file + fileOffset;
            if (targetFile < 0 || targetFile > 7)
                continue;

            Piece piece = board.getSquare(fileRankToSquare(targetFile, pawnAttackFromRank));
            if (piece.type == PieceType::Pawn && piece.color == opponent)
                return true;
        }
    }


    return false;
}

std::vector<Move> generateAllPseudoLegalMoves(const Board& board) {
    std::vector<Move> moves;
    Color turn = board.turn();
    for (int square = 0; square < 64; ++square) {
        Piece piece = board.getSquare(square);
        
        if (piece.color != turn)
        continue;
        
        std::vector<Move> current;
        switch (piece.type) {
            case PieceType::Pawn:
                current = generatePawnMoves(board, square);
                break;
            case PieceType::Knight:
                current = generateKnightMoves(board, square);
                break;
            case PieceType::Bishop:
                current = generateBishopMoves(board, square);
                break;
            case PieceType::Rook:
                current = generateRookMoves(board, square);
                break;
            case PieceType::Queen:
                current = generateQueenMoves(board, square);
                break;
            case PieceType::King:
                current = generateKingMoves(board, square);
                break;
            default: break;
        }

        moves.insert(moves.end(), current.begin(), current.end());
    }

    return moves;
}

std::vector<Move> generateAllLegalMoves(const Board& board) {
    std::vector<Move> pseudoLegal = generateAllPseudoLegalMoves(board);
    std::vector<Move> legal;
    Color side = board.turn();
    Color opponent = (side == Color::White) ? Color::Black : Color::White;

    for (const Move& move : pseudoLegal) {
        Board copy = board;
        copy.makeMove(move);

        if (!isSquareAttacked(copy, copy.kingSquare(side), opponent)) {
            legal.push_back(move);
        }
    }

    return legal;
}