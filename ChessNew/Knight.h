#pragma once
#include "Piece.h"  // Bao gồm tệp tiêu đề của lớp Piece

// Định nghĩa lớp Knight, một loại của lớp Piece
class Knight :
    public Piece
{
public:
    // Constructor: Khởi tạo một quân mã với đội, vị trí và trình xử lý SDL
    Knight(Team team, std::pair<int, int> pos, SDL_Handler* handler);

    // Gọi hàm name và
    void sayMyName();

    // Tính toán các nước đi có thể của quân mã
    void calcPossibleMoves(Piece* field[8][8], bool checkCheck);
};
