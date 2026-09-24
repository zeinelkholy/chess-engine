#pragma once
#include <cstdint>

namespace Zobrist {
    extern uint64_t pieceKeys[7][2][64];
    extern uint64_t blackToMoveKey;
    extern uint64_t castlingKeys[4];
    extern uint64_t enPassantFileKeys[8];

    void init();
}
