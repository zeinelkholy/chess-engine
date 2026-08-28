#pragma once

#include "Piece.h"

struct Move {
    int from;
    int to;
    PieceType promotion = PieceType::None;
    bool isCastling = false;
    bool isEnPassant = false;
    bool isDoublePawnPush = false;
    
    Move(int f, int t, PieceType prom = PieceType::None,
     bool cas = false, bool en = false, bool dpp = false)
    : from(f), to(t), promotion(prom), isCastling(cas), isEnPassant(en), isDoublePawnPush(dpp) {}
    
};