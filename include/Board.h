#pragma once

#include "Piece.h"
#include <array>

class Board {
private:
    std::array<Piece, 64> squares_;
    Color turn_ = Color::None;
    bool whiteCanShortCastle_ = true;
    bool whiteCanLongCastle_ = true;
    bool blackCanShortCastle_ = true;
    bool blackCanLongCastle_ = true;
    int enPassantTarget_ = -1;

public:
    void reset();
    void print() const;

    Piece getSquare(int square) const;
    void setSquare(int square, Piece piece);

    Color turn() const { return turn_; }
    bool canShortCastle(Color color) const;
    bool canLongCastle(Color color) const;
    int enPassantTarget() const { return enPassantTarget_; }
};