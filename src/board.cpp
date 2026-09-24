#include "Board.h"
#include "Piece.h"
#include "Move.h"
#include "Notation.h"
#include "Zobrist.h"

#include <iostream>
#include <string>

void Board::setSquare(int square, const Piece& piece) {
    squares_[square] = piece;
}

Piece Board::getSquare(int square) const {
    return squares_[square];
}

void Board::setSquare(const std::string& square, const Piece& piece) {
    squares_[algebraToSquare(square)] = piece;
}

Piece Board::getSquare(const std::string& square) const {
    return squares_[algebraToSquare(square)];
}

void Board::reset() {
    std::array<PieceType, 8> types = {
        PieceType::Rook,
        PieceType::Knight,
        PieceType::Bishop,
        PieceType::Queen,
        PieceType::King,
        PieceType::Bishop,
        PieceType::Knight,
        PieceType::Rook
    };

    turn_ = Color::White;
    whiteCanShortCastle_ = true;
    whiteCanLongCastle_ = true;
    blackCanShortCastle_ = true;
    blackCanLongCastle_ = true;
    enPassantTarget_ = -1;

    whiteKingSquare_ = 4;
    blackKingSquare_ = 60;

    for (int square = 0; square < 64; ++square) {
        setSquare(square, Piece());
    }

    for (int file = 0; file < 8; ++file) {
        setSquare(56 + file, Piece(types[file], Color::Black));
        setSquare(48 + file, Piece(PieceType::Pawn, Color::Black));

        setSquare(8 + file, Piece(PieceType::Pawn, Color::White));
        setSquare(file, Piece(types[file], Color::White));
    }

    hash_ = computeHash();
}

void Board::print() const {
    std::cout << "\n\n";
    for (int rank = 7; rank >= 0; --rank) {
        std::cout << (rank + 1) << ' ';
        for (int file = 0; file < 8; ++file) {
            std::cout << getSquare(rank * 8 + file).toSymbol() << ' ';
        }
        std::cout << '\n';
    }
    std::cout << "  a b c d e f g h\n";
}

bool Board::canShortCastle(Color color) const {
    return (color == Color::White) ? whiteCanShortCastle_ : blackCanShortCastle_;
}

bool Board::canLongCastle(Color color) const {
    return (color == Color::White) ? whiteCanLongCastle_ : blackCanLongCastle_;
}

void Board::flipTurn() {
    hash_ ^= Zobrist::blackToMoveKey;
    turn_ = (turn() == Color::White) ? Color::Black : Color::White;
}

int Board::kingSquare(Color color) const {
    if (color == Color::White)
        return whiteKingSquare();
    return blackKingSquare();
}

void Board::makeMove(const Move& move) {
    Piece piece = getSquare(move.from);
    int colorIndex = (piece.color != Color::White);

    // Snapshot "before" state needed for hash bookkeeping later.
    bool prevWhiteShort = whiteCanShortCastle_;
    bool prevWhiteLong  = whiteCanLongCastle_;
    bool prevBlackShort = blackCanShortCastle_;
    bool prevBlackLong  = blackCanLongCastle_;
    int prevEnPassantTarget = enPassantTarget_;

    // Remove the moving piece from its origin square.
    hash_ ^= Zobrist::pieceKeys[static_cast<int>(piece.type)][colorIndex][move.from];

    // Remove a captured piece, if any -- using ITS OWN color, not the mover's.
    Piece captured = getSquare(move.to);
    if (captured.type != PieceType::None) {
        int capturedColorIndex = (captured.color != Color::White);
        hash_ ^= Zobrist::pieceKeys[static_cast<int>(captured.type)][capturedColorIndex][move.to];
    }

    setSquare(move.to, piece);
    setSquare(move.from, Piece());

    if (move.promotion != PieceType::None) {
        setSquare(move.to, Piece(move.promotion, piece.color));
        hash_ ^= Zobrist::pieceKeys[static_cast<int>(move.promotion)][colorIndex][move.to];
        // No early return -- en passant reset, castling-rights checks,
        // and the turn flip below still need to run.
    }

    if (move.isEnPassant) {
        int target = (piece.color == Color::White) ? move.to - 8 : move.to + 8;
        Piece capturedPawn = getSquare(target);
        int capturedColorIndex = (capturedPawn.color != Color::White);
        hash_ ^= Zobrist::pieceKeys[static_cast<int>(PieceType::Pawn)][capturedColorIndex][target];
        setSquare(target, Piece());
    }

    if (move.isCastling) {
        int rookAt, rookTo;

        if (move.to < move.from) {
            // Long castle
            rookAt = move.from - 4;
            rookTo = move.from - 1;
        } else {
            // Short castle
            rookAt = move.from + 3;
            rookTo = move.from + 1;
        }

        Piece rook = getSquare(rookAt);
        setSquare(rookTo, rook);
        setSquare(rookAt, Piece());
        hash_ ^= Zobrist::pieceKeys[static_cast<int>(PieceType::Rook)][colorIndex][rookAt];
        hash_ ^= Zobrist::pieceKeys[static_cast<int>(PieceType::Rook)][colorIndex][rookTo];
    }

    if (move.isDoublePawnPush)
        enPassantTarget_ = (move.to + move.from) / 2;
    else
        enPassantTarget_ = -1;

    // King move: revoke castling rights & update tracked king square.
    if (piece.type == PieceType::King) {
        if (piece.color == Color::White) {
            whiteCanLongCastle_ = false;
            whiteCanShortCastle_ = false;
            whiteKingSquare_ = move.to;
        } else {
            blackCanLongCastle_ = false;
            blackCanShortCastle_ = false;
            blackKingSquare_ = move.to;
        }
    }

    // Rook moved or captured: revoke the corresponding right.
    if (move.to == 0 || move.from == 0)
        whiteCanLongCastle_ = false;
    if (move.to == 7 || move.from == 7)
        whiteCanShortCastle_ = false;
    if (move.to == 56 || move.from == 56)
        blackCanLongCastle_ = false;
    if (move.to == 63 || move.from == 63)
        blackCanShortCastle_ = false;

    // --- Hash bookkeeping for everything that just changed ---

    if (prevWhiteShort && !whiteCanShortCastle_) hash_ ^= Zobrist::castlingKeys[0];
    if (prevWhiteLong  && !whiteCanLongCastle_)  hash_ ^= Zobrist::castlingKeys[1];
    if (prevBlackShort && !blackCanShortCastle_) hash_ ^= Zobrist::castlingKeys[2];
    if (prevBlackLong  && !blackCanLongCastle_)  hash_ ^= Zobrist::castlingKeys[3];

    if (prevEnPassantTarget != -1)
        hash_ ^= Zobrist::enPassantFileKeys[prevEnPassantTarget % 8];
    if (enPassantTarget_ != -1)
        hash_ ^= Zobrist::enPassantFileKeys[enPassantTarget_ % 8];

    flipTurn();
}

uint64_t Board::computeHash() const {
    uint64_t hash = 0;

    for (int square = 0; square < 64; ++square) {
        Piece piece = getSquare(square);
        if (piece.type == PieceType::None)
            continue;

        int colorIndex = (piece.color != Color::White);
        hash ^= Zobrist::pieceKeys[static_cast<int>(piece.type)][colorIndex][square];
    }

    if (turn_ == Color::Black)
        hash ^= Zobrist::blackToMoveKey;

    if (whiteCanShortCastle_) hash ^= Zobrist::castlingKeys[0];
    if (whiteCanLongCastle_)  hash ^= Zobrist::castlingKeys[1];
    if (blackCanShortCastle_) hash ^= Zobrist::castlingKeys[2];
    if (blackCanLongCastle_)  hash ^= Zobrist::castlingKeys[3];

    if (enPassantTarget_ != -1)
        hash ^= Zobrist::enPassantFileKeys[enPassantTarget_ % 8];

    return hash;
}