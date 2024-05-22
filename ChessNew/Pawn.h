#pragma once
#include "Piece.h"

class Pawn : public Piece {
public:
    // Constructor
    Pawn(Team team, std::pair<int, int> pos, SDL_Handler* handler);

    // Hiển thị tên của quân cờ
    void sayMyName();

    // Tính toán các nước đi có thể
    void calcPossibleMoves(Piece* field[8][8], bool checkCheck);

    // Getter cho m_enPassant
    std::pair<bool, int> getEnPassant() { return m_enPassant; };

    // Setter cho m_enPassant
    void setEnPassant(std::pair<bool, int> modifi) { m_enPassant = modifi; };

private:
    // Xác định xem en passant có thể thực hiện được trong hướng cụ thể không
    std::pair<bool, int> m_enPassant;
};
