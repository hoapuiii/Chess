#include "Queen.h" 
#include <iostream> 
#include <array> 
#include <memory> 

// Định nghĩa các đường dẫn đến tệp hình ảnh của quân Hậu đen và trắng
const std::string BLACK_QUEEN_TEXTURE_PATH = "G:/Game/ChessNew/res/Chess_qdt60.png";
const std::string WHITE_QUEEN_TEXTURE_PATH = "G:/Game/ChessNew/res/Chess_qlt60.png";

// Hàm khởi tạo của lớp Queen
Queen::Queen(Team team, const std::pair<int, int>& pos, std::shared_ptr<SDL_Handler> handler)
    : Piece(team, pos, handler, QUEEN)
{
    // Chọn đường dẫn hình ảnh dựa trên đội của quân Hậu
    const std::string& texturePath = team == BLACK ? BLACK_QUEEN_TEXTURE_PATH : WHITE_QUEEN_TEXTURE_PATH;
    m_texture = handler->loadImage(texturePath); // Tải hình ảnh
    render(); // Vẽ quân Hậu lên màn hình
}

// Hàm in ra tên của quân Hậu (dành cho kiểm tra hoặc debug)
void Queen::sayMyName() const
{
    std::cout << (m_team == BLACK ? "BLACK QUEEN" : "WHITE QUEEN") << std::endl;
}

// Hàm tính toán các nước đi có thể của quân Hậu
void Queen::calcPossibleMoves(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& field, bool checkCheck)
{
    m_possibleMoves.clear(); // Xóa các nước đi có thể trước đó

    // Định nghĩa các hướng đi của quân Hậu
    const std::array<std::pair<int, int>, 8> directions = {
        { -1, -1 }, { -1, 0 }, { -1, 1 },
        {  0, -1 },            {  0, 1 },
        {  1, -1 }, {  1, 0 }, {  1, 1 }
    };

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
bool Queen::isValidPosition(int x, int y) const
{
    return x >= 0 && x < 8 && y >= 0 && y < 8; // Trả về true nếu vị trí nằm trong bàn cờ
}
