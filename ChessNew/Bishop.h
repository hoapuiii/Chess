#pragma once
#include "Piece.h" // Bao gồm tệp tiêu đề của lớp cơ sở Piece
#include "King.h"  // Bao gồm tệp tiêu đề của lớp King (có thể được sử dụng để kiểm tra tình trạng chiếu)

// Định nghĩa lớp Bishop kế thừa từ lớp Piece
class Bishop : public Piece
{
public:
    // Hàm dựng: khởi tạo một quân Tượng với đội, vị trí, và trình xử lý SDL
    Bishop(Team team, std::pair<int, int> pos, SDL_Handler* handler);

    // Phương thức để in chi tiết của quân Tượng, dùng để gỡ lỗi
    void sayMyName();

    // Phương thức để tính toán các nước đi có thể của quân Tượng
    // field là trạng thái hiện tại của bàn cờ, checkCheck là cờ để kiểm tra xem nước đi có đặt vua của mình vào tình huống bị chiếu hay không
    void calcPossibleMoves(Piece* field[8][8], bool checkCheck);
};
