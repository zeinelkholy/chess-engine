#pragma once

#include <string>

int algebraToSquare(const std::string& str);
std::string squareToAlgebra(int square);
int fileRankToSquare(int rank, int file);
std::pair<int, int> squareToFileRank(int square);