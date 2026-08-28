#include <iostream>
#include <string>

#include "Board.h"
#include "Piece.h"
#include "Move.h"

int Board::algebraToSquare(const std::string& str) {
    int file = str[0] - 'a';
    int rank = str[1] - '0' - 1;
    return rank * 8 + file; 
}

std::string Board::squareToAlgebra(int square) {
    std::string res = "";
    char file = (square % 8) + 'a';
    char rank = (rank / 8) + '0';
    res += file;
    res += rank;
    return res;
}

void Board::setSquare(int square, const Piece& piece) {
    squares_[square] = piece;
}

Piece Board::getSquare(int square) const {
    return squares_[square];
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

    for (int square = 0; square < 64; ++square) {
        setSquare(square, Piece());
    }

    for (int file = 0; file < 8; ++file) {
        setSquare(56 + file, Piece(types[file], Color::Black));
        setSquare(48 + file, Piece(PieceType::Pawn, Color::Black));

        setSquare(8 + file, Piece(PieceType::Pawn, Color::White));
        setSquare(file, Piece(types[file], Color::White));
    }
}

void Board::print() const {
    std::cout << "\n\n";
    for (int rank = 7; rank >= 0; --rank) {
        std::cout << (rank + 1) << "  ";
        for (int file = 0; file < 8; ++file) {
            std::cout << getSquare(rank * 8 + file).toChar() << ' ';
        }
        std::cout << '\n';
    }
    std::cout << "---------------------\n";
    std::cout << "   a b c d e f g h\n";
}

void Board::flipTurn() {
    turn_ = (turn() == Color::White) ? Color::Black : Color::White;
}

void Board::makeMove(const Move& move) {
    Piece piece = getSquare(move.from);

    setSquare(move.to, piece);
    setSquare(move.from, Piece());
    
    if (move.promotion != PieceType::None) {
        setSquare(move.to, Piece(move.promotion, piece.color));
    }

    if (move.isEnPassant) {
        int target = (piece.color == Color::White) ? move.to - 8 : move.to + 8;
        setSquare(target, Piece());
    }

    if (move.isCastling) {
        int rookAt, rookTo;
        
        if (move.to < move.from) {
            // Long Castle
            rookAt = move.from - 4;
            rookTo = move.from - 1;
            
        }
        else {
            // Short castle
            rookAt = move.from + 3;
            rookTo = move.from + 1;
        }

        Piece rook = getSquare(rookAt);
        setSquare(rookTo, rook);
        setSquare(rookAt, Piece());
    }

    if (move.isDoublePawnPush)
        enPassantTarget_ = (move.to + move.from) / 2;
    else
        enPassantTarget_ = -1;

    if (piece.type == PieceType::King) {
        if (piece.color == Color::White) {
            whiteCanLongCastle_ = false;
            whiteCanShortCastle_ = false;
        } else {
            blackCanLongCastle_ = false;
            blackCanShortCastle_ = false;
        }
    }

    if (move.to == 0 || move.from == 0)
        whiteCanLongCastle_ = false;
    if (move.to == 7 || move.from == 7)
        whiteCanShortCastle_ = false;
    if (move.to == 56 || move.from == 56)
        blackCanLongCastle_ = false;
    if (move.to == 63 || move.from == 63)
        blackCanShortCastle_ = false;

    flipTurn();

}
