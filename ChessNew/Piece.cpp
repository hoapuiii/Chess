#include "Piece.h" 
#include "King.h" 
#include <string> 
#include <iostream>
#include <memory>
#include <algorithm> 

// Hàm tính toán các nước đi hợp lệ
std::vector<std::tuple<int, int, Piece::MoveType>> Piece::calculateLegalMoves(
    const std::vector<std::tuple<int, int, Piece::MoveType>>& moves,
    const std::shared_ptr<King>& king,
    const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& field,
    bool checkCheck) const
{
    // Nếu không cần kiểm tra vua có bị chiếu hay không, trả về tất cả các nước đi
    if (!checkCheck) {
        return moves;
    }

    std::vector<std::tuple<int, int, Piece::MoveType>> legalMoves; // Mảng lưu các nước đi hợp pháp
    for (const auto& move : moves) {
        // Nếu nước đi hợp lệ, thêm vào mảng
        if (isMoveValid(field, king, move)) {
            legalMoves.push_back(move);
        }
    }
    return legalMoves; // Trả về mảng các nước đi hợp lệ
}

// Hàm kiểm tra nước đi có hợp lệ hay không
bool Piece::isMoveValid(
    const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& field,
    const std::shared_ptr<King>& king,
    const std::tuple<int, int, Piece::MoveType>& move) const
{
    // Sao chép bàn cờ hiện tại để giả lập nước đi
    std::array<std::array<std::shared_ptr<Piece>, 8>, 8> hypotheticalField = field;
    const int newX = std::get<0>(move); // Lấy tọa độ mới của nước đi
    const int newY = std::get<1>(move); // Lấy tọa độ mới của nước đi
    std::swap(hypotheticalField[m_pos.first][m_pos.second], hypotheticalField[newX][newY]); // Giả lập nước đi

    // Nếu quân cờ là vua, kiểm tra vua có bị chiếu ở vị trí mới hay không
    if (m_type == KING) {
        return !king->isKingInCheck(hypotheticalField, newX, newY);
    }
    else { // Nếu không phải vua, kiểm tra vua của đội có bị chiếu ở vị trí hiện tại hay không
        return !king->isKingInCheck(hypotheticalField, king->getPos().first, king->getPos().second);
    }
}

// Hàm lấy con trỏ đến quân vua của đội mình
std::shared_ptr<King> Piece::getOwnKing(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& field) const
{
    // Tìm quân vua của đội mình trên bàn cờ
    const auto& kingPiece = std::find_if(
        field.cbegin(), field.cend(),
        [this](const auto& row) {
            return std::any_of(row.cbegin(), row.cend(), [this](const auto& piece) {
                return piece && piece->getTeam() == m_team && piece->getType() == KING;
                });
        });

    if (kingPiece != field.cend()) {
        const auto& kingRow = *kingPiece; // Lấy hàng chứa quân vua
        const auto& king = std::find_if(kingRow.cbegin(), kingRow.cend(), [this](const auto& piece) {
            return piece && piece->getTeam() == m_team && piece->getType() == KING;
            });
        return *king; // Trả về con trỏ đến quân vua
    }

    return nullptr; // Trả về nullptr nếu không tìm thấy vua
}

// Hàm khởi tạo của lớp Piece
Piece::Piece(Team team, const std::pair<int, int>& pos, std::shared_ptr<SDL_Handler> handler, PieceType type)
    : m_team(team), m_pos(pos), m_handler(handler), m_texture(nullptr), m_hasMoved(false), m_type(type)
{
}

// Hàm khởi tạo sao chép của lớp Piece
Piece::Piece(const Piece& piece)
    : m_team(piece.m_team), m_pos(piece.m_pos), m_handler(piece.m_handler), m_texture(nullptr), m_hasMoved(false), m_type(piece.m_type)
{
}

// Hàm hủy của lớp Piece
Piece::~Piece()
{
    SDL_DestroyTexture(m_texture); // Hủy texture của quân cờ
    m_handler->undoPieceRender(m_pos.first, m_pos.second); // Hoàn tác việc vẽ quân cờ
}

// Hàm vẽ quân cờ
void Piece::render() const
{
    const SDL_Rect src = { 0, 0, 60, 60 }; // Hình chữ nhật nguồn
    if (m_handler == nullptr) {
        sayMyName(); // Nếu handler là nullptr, in ra tên quân cờ (dành cho debug)
        return;
    }

    const SDL_Rect dest = {
        static_cast<int>(m_handler->SCREEN_WIDTH / 8 * m_pos.first - 2),
        static_cast<int>(m_handler->SCREEN_HEIGHT / 8 * m_pos.second),
        static_cast<int>(m_handler->SCREEN_WIDTH / 8),
        static_cast<int>(m_handler->SCREEN_HEIGHT / 8)
    }; // Hình chữ nhật đích
    m_handler->DrawRectangle(src, dest, m_texture); // Vẽ hình chữ nhật
}

// Hàm in ra tên của quân cờ (dành cho kiểm tra hoặc debug)
void Piece::sayMyName() const
{
    std::cout << "No Piece here" << std::endl;
}
