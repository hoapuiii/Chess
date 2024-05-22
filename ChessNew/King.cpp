#include "King.h"
#include "Pawn.h"
#include <iostream>
#include <array>
#include <memory>

// Đường dẫn đến file ảnh cho quân vua đen và trắng
const std::string BLACK_KING_TEXTURE_PATH = "G:/Game/ChessNew/res/Chess_kdt60.png";
const std::string WHITE_KING_TEXTURE_PATH = "G:/Game/ChessNew/res/Chess_klt60.png";

King::King(Team team, const std::pair<int, int>& pos, std::shared_ptr<SDL_Handler> handler)
    : Piece(team, pos, handler, KING), m_isInCheck(false)
{
    const std::string& texturePath = team == BLACK ? BLACK_KING_TEXTURE_PATH : WHITE_KING_TEXTURE_PATH; // Lấy đường dẫn ảnh tương ứng với đội đen hay trắng
    m_texture = handler->loadImage(texturePath); // Tải ảnh cho quân vua
    render(); // Render quân vua lên màn hình
}

void King::sayMyName() const
{
    std::cout << (m_team == BLACK ? "BLACK KING" : "WHITE KING") << std::endl; // In ra tên quân vua (đen hay trắng)
}

void King::calcPossibleMoves(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& field, bool checkCheck)
{
    m_possibleMoves.clear(); // Xóa danh sách các nước đi có thể của quân vua
    checkCastlingMoves(field); // Tính toán các nước đi phong tỏa có thể
    checkNormalMoves(field, checkCheck); // Tính toán các nước đi bình thường có thể
}

void King::setCheck(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& field, int x, int y)
{
    m_isInCheck = isKingInCheck(field, x, y); // Đặt trạng thái chiếu tướng cho quân vua tại vị trí (x, y)
}

bool King::isKingInCheck(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& field, int x, int y) const
{
    for (const auto& row : field) {
        for (const auto& piece : row) {
            if (piece && piece->getTeam() != m_team) { // Nếu có quân cờ của đối phương
                piece->calcPossibleMoves(field, false); // Tính toán các nước đi có thể của quân cờ đó
                const auto& possibleMoves = piece->getPossibleMoves();
                if (std::find(possibleMoves.begin(), possibleMoves.end(), std::make_tuple(x, y, Piece::NORMAL)) != possibleMoves.end()) {
                    return true; // Nếu quân vua ở vị trí (x, y) bị chiếu tướng, trả về true
                }
            }
        }
    }
    return false; // Không bị chiếu tướng, trả về false
}

void King::checkNormalMoves(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& field, bool checkCheck)
{
    const std::array<int, 8> directions = { -1, -1, -1, 0, 0, 1, 1, 1 }; // Mảng các hướng di chuyển của quân vua
    for (int i = 0; i < 8; ++i) {
        const int dx = directions[i];
        const int dy = directions[i == 7 ? 0 : i + 1];
        const int newX = m_pos.first + dx;
        const int newY = m_pos.second + dy;

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

void King::checkCastlingMoves(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& field)
{
    if (!m_hasMoved) { // Nếu quân vua chưa di chuyển
        checkCastlingMove(field, 0, 0, 3, 0, 0); // Kiểm tra nước đi phong tỏa ở góc trên bên trái
        checkCastlingMove(field, 7, 0, 5, 0, 7); // Kiểm tra nước đi phong tỏa ở góc trên bên phải
        checkCastlingMove(field, 0, 7, 3, 7, 0); // Kiểm tra nước đi phong tỏa ở góc dưới bên trái
        checkCastlingMove(field, 7, 7, 5, 7, 7); // Kiểm tra nước đi phong tỏa ở góc dưới bên phải
    }
}

void King::checkCastlingMove(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& field,
    int rookX, int rookY, int kingX, int kingY, int oppRookX)
{
    if (field[rookX][rookY] && field[rookX][rookY]->getType() == ROOK && // Nếu có quân tượng tại vị trí (rookX, rookY)
        field[rookX][rookY]->getTeam() == m_team && !field[rookX][rookY]->hasMoved()) { // và cùng đội với quân vua, và chưa di chuyển
        bool canCastle = true;
        for (int x = kingX + 1; x < oppRookX; ++x) { // Kiểm tra các ô giữa quân vua và quân tượng
            if (field[x][kingY]) { // Nếu có quân cờ nào đó ở giữa
                canCastle = false; // Không thể phong tỏa
                break;
            }
        }
        if (canCastle && !isSquareUnderAttack(field, kingX, kingY) && !isSquareUnderAttack(field, oppRookX, kingY)) { // Nếu có thể phong tỏa và không bị chiếu tướng tại vị trí mới
            m_possibleMoves.push_back(std::make_tuple(oppRookX, kingY, Piece::CASTLE)); // Thêm nước đi phong tỏa vào danh sách nước đi có thể
        }
    }
}

bool King::isSquareUnder