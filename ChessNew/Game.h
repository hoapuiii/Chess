#pragma once
#include "Piece.h"        // Bao gồm tệp tiêu đề của lớp Piece
#include "SDL_Handler.h"  // Bao gồm tệp tiêu đề của lớp SDL_Handler
#include "Pawn.h"         // Bao gồm tệp tiêu đề của lớp Pawn
#include "Rook.h"         // Bao gồm tệp tiêu đề của lớp Rook
#include "Knight.h"       // Bao gồm tệp tiêu đề của lớp Knight
#include "Bishop.h"       // Bao gồm tệp tiêu đề của lớp Bishop
#include "King.h"         // Bao gồm tệp tiêu đề của lớp King
#include "Queen.h"        // Bao gồm tệp tiêu đề của lớp Queen

// Định nghĩa lớp Game để quản lý trạng thái và luật của trò chơi cờ vua
class Game
{
public:
    // Constructor: Khởi tạo trò chơi với một trình xử lý SDL
    Game(SDL_Handler* handler);

    // Destructor: Hủy bỏ trò chơi
    ~Game();

    // Trả về con cờ ở vị trí (row, col) trên bàn cờ
    Piece* getFieldPos(int row, int col);

    // Di chuyển một quân cờ
    void move(Piece* piece, std::tuple<int, int, Piece::MoveType> move);

    // Trả về lượt của người chơi hiện tại
    Piece::Team getTurn() { return m_turn; };

    // Kiểm tra xem một nước đi có hợp lệ không
    bool isValidMove(int x, int y, Piece* piece);

    // Tính toán tất cả các nước đi được phép của mọi quân cờ
    void calcAllMoves();

    // Hiển thị các nước đi có thể của một quân cờ
    void renderPossibleMoves(Piece* piece);

    // Hủy bỏ việc hiển thị các nước đi có thể của một quân cờ
    void undoRenderPossibleMoves(Piece* piece);

private:
    // Mảng 2D của bàn cờ, mỗi vị trí có một đội và một quân cờ
    Piece* m_field[8][8];

    // Vô hiệu hóa En Passant cho mọi quân cờ
    void disableEnPassant();

    // Di chuyển thông thường
    void normal(int xStart, int yStart, int xEnd, int yEnd);

    // Di chuyển En Passant
    void enPassant(int xStart, int yStart, int xEnd, int yEnd);

    // Di chuyển trao đổi quân cờ
    void exchange(int xStart, int yStart, int xEnd, int yEnd);

    // Di chuyển Castles
    void castles(int xStart, int yStart, int xEnd, int yEnd);

    // Tên tệp nền
    std::string m_backgroundFilename;

    // Xác định xem lượt của người chơi hiện tại là màu đen hay trắng
    Piece::Team m_turn;

    // Trình xử lý SDL
    SDL_Handler* m_handler;

    // Nếu là true, vô hiệu hóa En Passant! Nếu là false, thì không
    bool m_checkEnPassant;

    // Kiểm tra trạng thái trò chơi hiện tại, xác định người chiến thắng hoặc hòa
    void gameState();

    // Các quân cờ riêng lẻ
    Pawn* pl1;
    Pawn* pl2;
   Pawn* pl3;
    Pawn* pl4;
    Pawn* pl5;
    Pawn* pl6;
    Pawn* pl7;
    Pawn* pl8;
    Pawn* pb1;
    Pawn* pb2;
    Pawn* pb3;
    Pawn* pb4;
    Pawn* pb5;
    Pawn* pb6;
    Pawn* pb7;
    Pawn* pb8;
    Rook* rb1;
    Rook* rb2;
    Rook* rl1;
    Rook* rl2;
    Knight* nb1;
    Knight* nb2;
    Knight* nl1;
    Knight* nl2;
    Bishop* bb1;
    Bishop* bb2;
    Bishop* bl1;
    Bishop* bl2;
    King* kb1;
    King* kl1;
    Queen* qb1;
    Queen* ql1;
};


