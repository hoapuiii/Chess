#pragma once

#include <utility>
#include <vector>
#include "SDL_Handler.h"
#include <SDL.h>
#include <tuple>

class King;

class Piece {
public:

	enum Team { BLACK, WHITE, NONE };

	enum PieceType { PAWN, ROOK, KNIGHT, BISHOP, KING, QUEEN, EMPTY };

	enum MoveType { NORMAL, CASTLE, ENPASSANT, NEWPIECE, INIT };

	// Trả về danh sách các nước đi có thể
	std::vector<std::tuple <int, int, Piece::MoveType>> getPossibleMoves() { return m_possibleMoves; };

	// Trả về màu sắc của quân cờ (Đen hoặc Trắng)
	Team getTeam() { return m_team; };

	// Đặt vị trí mới cho quân cờ
	void setPosition(std::pair<int, int> newPos) { m_pos = newPos; };

	// Trả về vị trí của quân cờ
	std::pair<int, int> getPos() { return m_pos; };

	// Constructor
	Piece(Team team, std::pair<int, int> pos, SDL_Handler* handler, PieceType type);

	// Constructor sao chép
	Piece(const Piece& piece);

	// Destructor
	~Piece();

	// Render quân cờ này
	void render();

	// In ra tên của quân cờ
	virtual void sayMyName();

	// Tính toán tất cả các nước đi có thể của quân cờ này
	virtual void calcPossibleMoves(Piece* field[8][8], bool checkCheck) = 0;

	// true nếu quân cờ đã di chuyển
	bool m_hasMoved;

	// Trả về loại của quân cờ
	PieceType getType() { return m_type; };

protected:

	// Texture của quân cờ này
	SDL_Texture* m_texture;

	// SDL Handler
	SDL_Handler* m_handler;

	// Màu sắc của quân cờ
	Team m_team;

	// Loại của quân cờ
	PieceType m_type;

	// Danh sách các nước đi có thể của quân cờ <hàng, cột, Loại nước đi>
	std::vector<std::tuple <int, int, Piece::MoveType>> m_possibleMoves;

	// Vị trí của quân cờ
	std::pair<int, int> m_pos;

	// Thêm nước đi vào danh sách (nếu hợp lệ).
	// Mô phỏng nước đi và kiểm tra xem vua của bên mình vẫn có bị chiếu không
	// Nếu vua bị chiếu sau khi mô phỏng nước đi, nước đi đó không hợp lệ
	// Nếu checkCheck là true, việc mô phỏng vua sẽ xác định xem nước đi có hợp lệ hay không
	// Nếu checkCheck là false, chúng ta sẽ chỉ thêm nước đi vào danh sách. checkCheck chỉ là false trong hàm setCheck của King,
	// bởi vì nếu không, nó sẽ gây ra stack overflow (pushMove gọi setCheck, setCheck gọi pushMove và cứ như thế)
	std::vector<std::tuple <int, int, Piece::MoveType>> pushMove(std::vector<std::tuple <int, int, Piece::MoveType>> moveList,
		std::tuple <int, int, Piece::MoveType> move,
		King* king,
		Piece* field[8][8],
		bool checkCheck);

	// Trả về vua của bên mình từ trường field
	King* getOwnKing(Piece* field[8][8]);
};
