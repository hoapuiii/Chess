#include "Knight.h"

#include <iostream>
#include <array>
#include <memory>

// Đường dẫn đến file ảnh cho quân mã đen và trắng
const std::string BLACK_KNIGHT_TEXTURE_PATH = "G:/Game/ChessNew/res/Chess_ndt60.png";
const std::string WHITE_KNIGHT_TEXTURE_PATH = "G:/Game/ChessNew/res/Chess_nlt60.png";

Knight::Knight(Team team, const std::pair<int, int>& pos, std::shared_ptr<SDL_Handler> handler)
    : Piece(team, pos, handler, KNIGHT)
{
    const std::string& texturePath = team == BLACK ? BLACK_KNIGHT_TEXTURE_PATH : WHITE_KNIGHT_TEXTURE_PATH; // Lấy đường dẫn ảnh tương ứng với đội đen hay trắng
    m_texture = handler->loadImage(texturePath); // Tải ảnh cho quân mã
    render(); // Render quân mã lên màn hình
}

void Knight::sayMyName() const
{
    std::cout << (m_team == BLACK ? "BLACK KNIGHT" : "WHITE KNIGHT") << std::endl; // In ra tên quân mã (đen hay trắng)
}

void Knight::calcPossibleMoves(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& field, bool checkCheck)
{
    m_possibleMoves.clear(); // Xóa danh sách các nước đi có thể của quân mã

    const std::array<std::pair<int, int>, 8> knightMoves = {
        std::make_pair(-2, -1), std::make_pair(-1, -2),
        std::make_pair(1, -2), std::make_pair(2, -1),
        std::make_pair(2, 1), std::make_pair(1, 2),
        std::make_pair(-1, 2), std::make_pair(-2, 1)
    }; // Mảng các hướng di chuyển có thể của quân mã

    for (const auto& move : knightMoves) {
        const int newX = m_pos.first + move.first;
        const int newY = m_pos.second + move.second;

        if (isValidPosition(newX, newY)) { // Nếu vị trí mới hợp lệ
            if (!field[newX][newY] || (field[newX][newY] && field[newX][newY]->getTeam() != m_team)) { // Nếu ô đó trống hoặc có quân cờ đối phương
                m_possibleMoves.push_back(std::make_tuple(newX, newY, Piece::NORMAL)); // Thêm vào danh sách nước đi có thể
            }
        }
    }

    if (checkCheck) {
        filterIllegalMoves(field); // Loại bỏ các nước đi không hợp lệ (gây ra tình trạng chiếu tướng)
    }
}

bool Knight::isValidPosition(int x, int y) const
{
    return x >= 0 && x < 8 && y >= 0 && y < 8; // Kiểm tra xem vị trí (x, y) có hợp lệ (nằm trong bàn cờ) hay không
}

void Knight::filterIllegalMoves(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& field)
{
    std::vector<std::tuple<int, int, Piece::MoveType>> legalMoves;
    for (const auto& move : m_possibleMoves) {
        const int newX = std::get<0>(move);
        const int newY = std::get<1>(move);
        if (isMoveLegal(field, newX, newY)) { // Nếu nước đi là hợp lệ (không gây ra tình trạng chiếu tướng)
            legalMoves.push_back(move); // Thêm vào danh sách nước đi hợp lệ
        }
    }
    m_possibleMoves = std::move(legalMoves); // Cập nhật danh sách nước đi có thể bằng danh sách nước đi hợp lệ
}

bool Knight::isMoveLegal(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& field, int newX, int newY) const
{
    std::array<std::array<std::shared_ptr<Piece>, 8>, 8> hypotheticalField = field; // Tạo bản sao bàn cờ hiện tại
    std::swap(hypotheticalField[m_pos.first][m_pos.second], hypotheticalField[newX][newY]); // Di chuyển quân mã đến vị trí mới trên bản sao
    return !isKingInCheck(hypotheticalField); // Kiểm tra xem sau khi di chuyển, quân vua có bị chiếu tướng hay không
}

bool Knight::isKingInCheck(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& field) const
{
    const std::shared_ptr<King> ownKing = getOwnKing(field); // Lấy quân vua của đội mình
    return ownKing->isKingInCheck(field, ownKing->getPos().first, ownKing->getPos().second); // Kiểm tra xem quân vua có bị chiếu tướng hay không
}

std::shared_ptr<King> Knight::getOwnKing(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& field) const
{
    for (const auto& row : field) {
        for (const auto& piece : row) {
            if (piece && piece->getType() == KING && piece->getTeam() == m_team) { // Nếu đó là quân vua của đội mình
                return std::static_pointer_cast<King>(piece); // Trả về con trỏ đến quân vua
            }
        }
    }
    return nullptr; // Nếu không tìm thấy quân vua, trả về nullptr
}