#include "Notation.h"

int fileRankToSquare(int file, int rank) {
    return (rank * 8 + file);
}

std::pair<int, int> squareToFileRank(int square) {
    return {square % 8, square / 8};
}

int algebraToSquare(const std::string& str) {
    int file = str[0] - 'a';
    int rank = str[1] - '0' - 1;
    return fileRankToSquare(file, rank); 
}

std::string squareToAlgebra(int square) {
    auto [file, rank] = squareToFileRank(square);
    std::string res = "";
    res += static_cast<char>(file + 'a');
    res += static_cast<char>(rank + 1 + '0');
    return res;
}
