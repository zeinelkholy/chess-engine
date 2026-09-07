#pragma once
#include <string>

enum class PieceType {
    None,
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King
};

enum class Color {
    None,
    White,
    Black
};

struct Piece {
    PieceType type = PieceType::None;
    Color color = Color::None;

    Piece() = default;
    Piece(PieceType t, Color c) : type(t), color(c) {}

    bool isEmpty() const { return (type == PieceType::None); }

    std::string toSymbol() const {
        if (isEmpty()) return ".";
        
        switch (type) {
            case PieceType::Pawn:
                return (color == Color::Black) ? "\u2659" : "\u265F";
            case PieceType::Knight:
                return (color == Color::Black) ? "\u2658" : "\u265E";
            case PieceType::Bishop:
                return (color == Color::Black) ? "\u2657" : "\u265D";
            case PieceType::Rook:
                return (color == Color::Black) ? "\u2656" : "\u265C";
            case PieceType::Queen:
                return (color == Color::Black) ? "\u2655" : "\u265B";
            case PieceType::King:
                return (color == Color::Black) ? "\u2654" : "\u265A";
            default: return ".";
        }
    }

};