#include "Pawn.h" 
#include <iostream> 
#include <array> 
#include <memory> 

// Đường dẫn tới hình ảnh quân tốt đen và trắng
const std::string BLACK_PAWN_TEXTURE_PATH = "G:/Game/ChessNew/res/Chess_pdt60.png";
const std::string WHITE_PAWN_TEXTURE_PATH = "G:/Game/ChessNew/res/Chess_plt60.png";

// Hàm khởi tạo của lớp Pawn
Pawn::Pawn(Team team, const std::pair<int, int>& pos, std::shared_ptr<SDL_Handler> handler)
    : Piece(team, pos, handler, PAWN), m_enPassantTarget(std::make_pair(false, 0)) // Gọi hàm khởi tạo của lớp cơ sở Piece và khởi tạo m_enPassantTarget
{
    // Xác định đường dẫn đến hình ảnh dựa trên đội
    const std::string& texturePath = team == BLACK ? BLACK_PAWN_TEXTURE_PATH : WHITE_PAWN_TEXTURE_PATH;
    // Tải hình ảnh và lưu vào m_texture
    m_texture = handler->loadImage(texturePath);
    // Đặt hướng di chuyển của quân tốt (tiến lên hay lùi xuống)
    m_dy = team == BLACK ? -1 : 1;
    // Vẽ quân tốt
    render();
}

// Hàm in ra tên của quân cờ (dành cho kiểm tra hoặc debug)
void Pawn::sayMyName() const
{
    std::cout << (m_team == BLACK ? "BLACK PAWN" : "WHITE PAWN") << std::endl;
}

// Hàm tính toán các nước đi có thể của quân tốt
void Pawn::calcPossibleMoves(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& field, bool checkCheck)
{
    m_possibleMoves.clear(); // Xóa các nước đi cũ
    const int targetRow = m_pos.second + m_dy; // Tính hàng đích mà quân tốt sẽ di chuyển tới

    // Kiểm tra vị trí hợp lệ và ô trống phía trước
    if (isValidPosition(m_pos.first, targetRow)) {
        if (!field[m_pos.first][targetRow]) {
            m_possibleMoves.push_back(std::make_tuple(m_pos.first, targetRow, Piece::NORMAL)); // Thêm nước đi bình thường
            // Nếu quân tốt chưa di chuyển, kiểm tra và thêm nước đi hai ô
            if (!m_hasMoved && isValidPosition(m_pos.first, targetRow + m_dy) && !field[m_pos.first][targetRow + m_dy]) {
                m_possibleMoves.push_back(std::make_tuple(m_pos.first, targetRow + m_dy, Piece::NORMAL));
            }
        }

        // Kiểm tra và thêm nước đi bắt quân chéo phải
        if (isValidPosition(m_pos.first + 1, targetRow) && field[m_pos.first + 1][targetRow] && field[m_pos.first + 1][targetRow]->getTeam() != m_team) {
            m_possibleMoves.push_back(std::make_tuple(m_pos.first + 1, targetRow, targetRow == 0 || targetRow == 7 ? Piece::NEWPIECE : Piece::NORMAL));
        }

        // Kiểm tra và thêm nước đi bắt quân chéo trái
        if (isValidPosition(m_pos.first - 1, targetRow) && field[m_pos.first - 1][targetRow] && field[m_pos.first - 1][targetRow]->getTeam() != m_team) {
            m_possibleMoves.push_back(std::make_tuple(m_pos.first - 1, targetRow, targetRow == 0 || targetRow == 7 ? Piece::NEWPIECE : Piece::NORMAL));
        }

        // Kiểm tra và thêm nước đi bắt quân qua đường
        checkEnPassantMoves(field);
    }

    // Nếu cần kiểm tra vua có bị chiếu hay không, lọc các nước đi hợp lệ
    if (checkCheck) {
        filterIllegalMoves(field);
    }
}

// Hàm kiểm tra và thêm nước đi bắt quân qua đường
void Pawn::checkEnPassantMoves(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& field)
{
    if (m_enPassantTarget.first && isValidPosition(m_pos.first + m_enPassantTarget.second, m_pos.second)) {
        const int captureRow = m_pos.second + m_dy; // Hàng đích khi bắt quân qua đường
        m_possibleMoves.push_back(std::make_tuple(m_pos.first + m_enPassantTarget.second, captureRow, Piece::ENPASSANT));
    }
}

// Hàm kiểm tra vị trí có hợp lệ hay không
bool Pawn::isValidPosition(int x, int y) const
{
    return x >= 0 && x < 8 && y >= 0 && y < 8;
}

// Hàm lọc các nước đi bất hợp pháp
void Pawn::filterIllegalMoves(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& field)
{
    std::vector<std::tuple<int, int, Piece::MoveType>> legalMoves; // Mảng lưu các nước đi hợp pháp
    for (const auto& move : m_possibleMoves) {
        const int newX = std::get<0>(move);
        const int newY = std::get<1>(move);
        // Nếu nước đi hợp pháp, thêm vào mảng
        if (isMoveLegal(field, newX, newY)) {
            legalMoves.push_back(move);
        }
    }
    m_possibleMoves = std::move(legalMoves); // Cập nhật mảng các nước đi có thể
}

// Hàm kiểm tra nước đi có hợp pháp hay không
bool Pawn::isMoveLegal(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& field, int newX, int newY) const
{
    std::array<std::array<std::shared_ptr<Piece>, 8>, 8> hypotheticalField = field;
    std::swap(hypotheticalField[m_pos.first][m_pos.second], hypotheticalField[newX][newY]); // Giả lập nước đi
    return !isKingInCheck(hypotheticalField); // Kiểm tra vua có bị chiếu hay không
}

// Hàm kiểm tra vua có bị chiếu hay không
bool Pawn::isKingInCheck(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& field) const
{
    const std::shared_ptr<King> ownKing = getOwnKing(field); // Lấy vị trí vua của đội mình
    return ownKing->isKingInCheck(field, ownKing->getPos().first, ownKing->getPos().second); // Kiểm tra vua có bị chiếu hay không
}

// Hàm lấy con trỏ đến quân vua của đội mình
std::shared_ptr<King> Pawn::getOwnKing(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& field) const
{
    for (const auto& row : field) {
        for (const auto& piece : row) {
            if (piece && piece->getType() == KING && piece->getTeam() == m_team) {
                return std::static_pointer_cast<King>(piece); // Trả về con trỏ đến quân vua
            }
        }
    }
    return nullptr; // Trả về nullptr nếu không tìm thấy vua
}
