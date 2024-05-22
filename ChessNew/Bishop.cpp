#include "Bishop.h"
#include <iostream>

const int MIN_BOARD_INDEX = 0;
const int MAX_BOARD_INDEX = 7;
const int BOARD_SIZE = 8;

// Constructor cho lớp Bishop
Bishop::Bishop(Team team, std::pair<int, int> pos, SDL_Handler* handler)
    : Piece(team, pos, handler, BISHOP)
{
    // Xác định tên tệp hình ảnh cho quân tượng dựa trên đội
    std::string filename;
    if (team == BLACK) {
        filename = "G:/Game/ChessNew/res/Chess_bdt60.png";
    }
    else {
        filename = "G:/Game/ChessNew/res/Chess_blt60.png";
    }

    // Khởi tạo các biến thành viên
    m_handler = handler;
    m_texture = handler->loadImage(filename);
    render(); // Vẽ quân tượng
}

// In tên của quân tượng (cho mục đích gỡ lỗi)
void Bishop::sayMyName()
{
    if (m_team == BLACK) {
        std::cout << "QUÂN TƯỢNG ĐEN" << std::endl;
    }
    else {
        std::cout << "QUÂN TƯỢNG TRẮNG" << std::endl;
    }
}

// Kiểm tra xem vị trí đã cho có nằm trong giới hạn bàn cờ không
bool Bishop::isWithinBoardLimits(int row, int col) const {
    return row >= MIN_BOARD_INDEX && row <= MAX_BOARD_INDEX &&
        col >= MIN_BOARD_INDEX && col <= MAX_BOARD_INDEX;
}

// Kiểm tra xem nước đi đến vị trí đã cho có hợp lệ không
bool Bishop::isMoveLegal(int row, int col, Piece* field[BOARD_SIZE][BOARD_SIZE], bool checkCheck) const {
    if (!isWithinBoardLimits(row, col)) {
        return false;
    }
    if (field[row][col] != nullptr && field[row][col]->getTeam() == m_team) {
        return false;
    }
    // Tính toán các nước đi tạm thời đến vị trí đích
    std::vector<std::tuple<int, int, Piece::MoveType>> tempMoves = pushMove(std::vector<std::tuple<int, int, Piece::MoveType>>(), std::make_tuple(row, col, Piece::NORMAL), getOwnKing(field), field, checkCheck);
    return !tempMoves.empty();
}

// Tính toán các nước đi có thể của quân tượng
void Bishop::calcPossibleMoves(Piece* field[BOARD_SIZE][BOARD_SIZE], bool checkCheck)
{
    std::vector<std::tuple<int, int, Piece::MoveType>> moves;

    // Khám phá các hướng chéo
    for (int dx = -1; dx <= 1; dx += 2) {
        for (int dy = -1; dy <= 1; dy += 2) {
            int dx_copy = dx;
            int dy_copy = dy;

            // Tiếp tục di chuyển chéo cho đến khi bị chặn hoặc ra khỏi biên
            while (isWithinBoardLimits(m_pos.first + dx_copy, m_pos.second + dy_copy) && field[m_pos.first + dx_copy][m_pos.second + dy_copy] == nullptr) {
                moves = pushMove(moves, std::make_tuple(m_pos.first + dx_copy, m_pos.second + dy_copy, Piece::NORMAL), getOwnKing(field), field, checkCheck);
                dx_copy += dx;
                dy_copy += dy;
            }

            // Kiểm tra xem vị trí cuối cùng có phải là nước đi hợp lệ không
            if (isMoveLegal(m_pos.first + dx_copy, m_pos.second + dy_copy, field, checkCheck)) {
                moves = pushMove(moves, std::make_tuple(m_pos.first + dx_copy, m_pos.second + dy_copy, Piece::NORMAL), getOwnKing(field), field, checkCheck);
            }
        }
    }

    // Lưu các nước đi có thể tính được
    m_possibleMoves = moves;
}
