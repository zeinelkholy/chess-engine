#pragma once

enum class PieceType : char {
    None = '.',
    Pawn = 'P',
    Knight = 'N',
    Bishop = 'B',
    Rook = 'R',
    Queen = 'Q',
    King = 'K'
};

enum class Color {
    None,
    White,
    Black
};

struct Piece {
    PieceType type;
    Color color;

    Piece() : type(PieceType::None), color(Color::None) {}
    Piece(PieceType t, Color c) : type(t), color(c) {}

    bool isEmpty() const {
        return (type == PieceType::None);
    }

    char toChar() const {
        if (isEmpty()) return '.';
        if (color == Color::Black)
            return static_cast<char>(type) + 32;
        return static_cast<char>(type);
    }

};