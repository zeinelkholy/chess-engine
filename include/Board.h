#pragma once

#include "Piece.h"
#include "Move.h"
#include "Zobrist.h"

#include <array>
#include <string>

class Board {
private:
    std::array<Piece, 64> squares_;
    Color turn_ = Color::None;
    bool whiteCanShortCastle_ = true;
    bool whiteCanLongCastle_ = true;
    bool blackCanShortCastle_ = true;
    bool blackCanLongCastle_ = true;
    int enPassantTarget_ = -1;

    int whiteKingSquare_ = 4;
    int blackKingSquare_ = 60;

    uint64_t hash_;

public:

    void reset();
    void print() const;

    Piece getSquare(int square) const;
    Piece getSquare(const std::string& square) const;
    void setSquare(int square, const Piece& piece);
    void setSquare(const std::string& square, const Piece& piece);

    uint64_t computeHash() const;

    void makeMove(const Move& move);
    void flipTurn();

    Color turn() const { return turn_; }
    bool canShortCastle(Color color) const;
    bool canLongCastle(Color color) const;
    int enPassantTarget() const { return enPassantTarget_; }

    int whiteKingSquare() const { return whiteKingSquare_; };
    int blackKingSquare() const { return blackKingSquare_; };

    int kingSquare(Color color) const;
};