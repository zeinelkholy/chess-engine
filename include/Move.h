#pragma once

#include "Piece.h"
#include "Notation.h"
#include <string>

struct Move {
    int from;
    int to;
    PieceType promotion = PieceType::None;
    bool isCastling = false;
    bool isEnPassant = false;
    bool isDoublePawnPush = false;

    bool isNull() const { return from == -1; }
    
    Move(int f, int t, PieceType prom = PieceType::None,
     bool cas = false, bool en = false, bool dpp = false)
    : from(f), to(t), promotion(prom), isCastling(cas), isEnPassant(en), isDoublePawnPush(dpp) {}

    Move(std::string f, std::string t, PieceType prom = PieceType::None,
     bool cas = false, bool en = false, bool dpp = false)
    : from(algebraToSquare(f)), to(algebraToSquare(t)), promotion(prom), isCastling(cas), isEnPassant(en), isDoublePawnPush(dpp) {}

    std::string algebraicNotation() const {
        std::string alg;
        alg += squareToAlgebra(from);
        alg += squareToAlgebra(to);
        return alg;
    }
    
};