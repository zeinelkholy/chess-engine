#pragma once
#include "Board.h"
#include "Move.h"

int minimax(const Board& board, int depth);
Move findBestMove(const Board& board, int depth);