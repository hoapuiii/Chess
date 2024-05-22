#include "Rook.h" 
#include <iostream> 
#include <array> 
#include <memory> 

// Định nghĩa các đường dẫn đến tệp hình ảnh của quân Xe đen và trắng
const std::string BLACK_ROOK_TEXTURE_PATH = "G:/Game/ChessNew/res/Chess_rdt60.png";
const std::string WHITE_ROOK_TEXTURE_PATH = "G:/Game/ChessNew/res/Chess_rlt60.png";

// Hàm khởi tạo của lớp Rook
Rook::Rook(Team team, const std::pair<int, int>& pos, std::shared_ptr<SDL_Handler> handler)
    : Piece(team, pos, handler, ROOK)
{
    // Chọn đường dẫn hình ảnh dựa trên đội của quân Xe
    const std::string& texturePath = team == BLACK ? BLACK_ROOK_TEXTURE_PATH : WHITE_ROOK_TEXTURE_PATH;
    m_texture = handler->loadImage(texturePath); // Tải hình ảnh
    render(); // Vẽ quân Xe lên màn hình
}

// Hàm in ra tên của quân Xe (dành cho kiểm tra hoặc debug)
void Rook::sayMyName() const
{
    std::cout << (m_team == BLACK ? "BLACK ROOK" : "WHITE ROOK") << std::endl;
}

// Hàm tính toán các nước đi có thể của quân Xe
void Rook::calcPossibleMoves(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& field, bool checkCheck)
{
    m_possibleMoves.clear(); // Xóa các nước đi có thể trước đó

    // Định nghĩa các hướng đi của quân Xe
    const std::array<std::pair<int, int>, 4> directions = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } };

    // Duyệt qua từng hướng
    for (const auto& dir : directions) {
        int dx = dir.first; // Hướng x
        int dy = dir.second; // Hướng y
        int newX = m_pos.first + dx; // Tọa độ x mới
        int newY = m_pos.second + dy; // Tọa độ y mới

        // Tiếp tục di chuyển theo hướng cho đến khi gặp biên hoặc quân cờ khác
        while (isValidPosition(newX, newY)) {
            if (field[newX][newY]) { // Nếu có quân cờ ở vị trí mới
                if (field[newX][newY]->getTeam() != m_team) { // Nếu quân cờ đó là của đối phương
                    m_possibleMoves.push_back(std::make_tuple(newX, newY, Piece::NORMAL)); // Thêm nước đi vào danh sách
                }
                break; // Dừng lại nếu gặp quân cờ
            }
            m_possibleMoves.push_back(std::make_tuple(newX, newY, Piece::NORMAL)); // Thêm nước đi vào danh sách
            newX += dx; // Cập nhật tọa độ x mới
            newY += dy; // Cập nhật tọa độ y mới
        }
    }

    // Tính toán lại các nước đi hợp pháp
    m_possibleMoves = calculateLegalMoves(m_possibleMoves, getOwnKing(field), field, checkCheck);
}

// Hàm kiểm tra vị trí có hợp lệ hay không
bool Rook::isValidPosition(int x, int y) const
{
    return x >= 0 và x < 8 và y >= 0 và y < 8; // Trả về true nếu vị trí nằm trong bàn cờ
}

// Hàm hủy (destructor) của lớp Rook, sử dụng mặc định
Rook::~Rook() = default;
