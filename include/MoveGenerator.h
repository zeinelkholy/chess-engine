#pragma once

#include "Move.h"
#include "Board.h"

#include <vector>

std::vector<Move> generatePawnMoves(const Board& board, int square);
std::vector<Move> generateKnightMoves(const Board& board, int square);
std::vector<Move> generateBishopMoves(const Board& board, int square);
std::vector<Move> generateRookMoves(const Board& board, int square);
std::vector<Move> generateQueenMoves(const Board& board, int square);
std::vector<Move> generateKingMoves(const Board& board, int square);