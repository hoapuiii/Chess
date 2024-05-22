#pragma once
#include "Piece.h"  // Bao gồm tệp tiêu đề của lớp Piece

// Định nghĩa lớp King, một loại của lớp Piece
class King :
    public Piece
{
public:
    // Constructor: Khởi tạo một quân vua với đội, vị trí và trình xử lý SDL
    King(Team team, std::pair<int, int> pos, SDL_Handler* handler);

    // Gọi hàm type và color để gỡ lỗi
    void sayMyName();

    // Tính toán các nước đi có thể của quân vua
    void calcPossibleMoves(Piece* field[8][8], bool checkCheck);

    // Kiểm soát xem quân vua này có bị chiếu hay không
    void setCheck(Piece* field[8][8], int x, int y);

    // Trả về true nếu quân vua bị chiếu
    bool getCheck() { return m_check; };

private:
    // Biến đánh dấu xem quân vua có bị chiếu hay không
    bool m_check;
};

