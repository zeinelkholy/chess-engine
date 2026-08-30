#pragma once

#include "Move.h"
#include "Board.h"

#include <vector>

bool isSquareAttacked(const Board& board, int square, Color opponent);
std::vector<Move> generatePawnMoves(const Board& board, int square);
std::vector<Move> generateKnightMoves(const Board& board, int square);
std::vector<Move> generateBishopMoves(const Board& board, int square);
std::vector<Move> generateRookMoves(const Board& board, int square);
std::vector<Move> generateQueenMoves(const Board& board, int square);
std::vector<Move> generateKingMoves(const Board& board, int square);
std::vector<Move> generateSlidingMoves(const Board& board, int square, const std::vector<std::pair<int, int>>& directions);
std::vector<Move> generateAllPseudoLegalMoves(const Board& board);
std::vector<Move> generateAllLegalMoves(const Board& board);