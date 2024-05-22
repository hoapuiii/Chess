#pragma once
#include "Piece.h"

class Queen :
    public Piece
{
public:
    // Constructor
    Queen(Team team, std::pair<int, int> pos, SDL_Handler* handler);

    // In ra tên của quân cờ
    void sayMyName();

    // Tính toán tất cả các nước đi có thể
    void calcPossibleMoves(Piece* field[8][8], bool checkCheck);
};


