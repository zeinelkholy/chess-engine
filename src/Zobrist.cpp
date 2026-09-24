#include "Zobrist.h"
#include <random>

namespace Zobrist {
    uint64_t pieceKeys[7][2][64];
    uint64_t blackToMoveKey;
    uint64_t castlingKeys[4];
    uint64_t enPassantFileKeys[8];    

    void init() {
        std::random_device seedSource;
        std::mt19937 generator(seedSource());
        std::uniform_int_distribution<uint64_t> distribution;

        for (int type = 0; type < 7; ++type) {
            for (int side = 0; side < 2; ++side) {
                for (int square = 0; square < 64; ++square) {
                    pieceKeys[type][side][square] = distribution(generator);
                }
            }
        }

        blackToMoveKey = distribution(generator);

        for (int i = 0; i < 4; ++i) {
            castlingKeys[i] = distribution(generator);
        }

        for (int i = 0; i < 8; ++i) {
            enPassantFileKeys[i] = distribution(generator);
        }
    }
}
