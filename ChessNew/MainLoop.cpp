#include <SDL.h> 
#include <stdio.h> 
#include "MainLoop.h" 
#include "SDL_Handler.h" 
#include "Game.h" 

// Định nghĩa phương thức run của lớp MainLoop
void MainLoop::run()
{
    // Tạo một đối tượng SDL_Handler để quản lý các tác vụ liên quan đến SDL
    SDL_Handler* handler = new SDL_Handler();
    // Khởi tạo đối tượng SDL_Handler
    handler->init();

    // Vẽ nền ban đầu
    handler->renderBackground();

    // Tạo một đối tượng Game, truyền handler cho nó
    Game* game = new Game(handler);
    // Biến để kiểm soát vòng lặp chính
    bool quit = false;

    // Các biến để lưu tọa độ sự kiện chuột
    int xStart = -1;
    int yStart = -1;
    int xEnd = -1;
    int yEnd = -1;
    // Con trỏ đến quân cờ được nhấp vào
    Piece* clickedOn = nullptr;

    // Vòng lặp chính, chạy cho đến khi quit được đặt thành true
    while (quit == false)
    {
        // Kiểm tra sự kiện SDL
        while (SDL_PollEvent(&handler->m_event))
        {
            // Nếu sự kiện là SDL_QUIT (như đóng cửa sổ), đặt quit thành true
            if (handler->m_event.type == SDL_QUIT)
            {
                quit = true;
            }

            // Nếu sự kiện là nhấn nút chuột
            if (handler->m_event.type == SDL_MOUSEBUTTONDOWN)
            {
                // Lấy tọa độ lưới của vị trí nhấn chuột
                xStart = handler->m_event.button.x / 80;
                yStart = handler->m_event.button.y / 80;
                // Lấy quân cờ tại vị trí được nhấp vào
                clickedOn = game->getFieldPos(xStart, yStart);
                // Nếu có một quân cờ được nhấp vào và đó là lượt của người chơi hiện tại
                if (clickedOn != nullptr)
                {
                    if (clickedOn->getTeam() == game->getTurn())
                    {
                        // Vẽ các nước đi có thể của quân cờ
                        game->renderPossibleMoves(clickedOn);
                    }
                }
            }

            // Nếu sự kiện là thả nút chuột
            if (handler->m_event.type == SDL_MOUSEBUTTONUP)
            {
                // Nếu có một quân cờ được nhấp vào và đó là lượt của người chơi hiện tại
                if (clickedOn != nullptr)
                {
                    if (clickedOn->getTeam() == game->getTurn())
                    {
                        // Hoàn tác việc vẽ các nước đi có thể của quân cờ
                        game->undoRenderPossibleMoves(clickedOn);
                    }
                }
                // Lấy tọa độ lưới của vị trí thả chuột
                xEnd = handler->m_event.button.x / 80;
                yEnd = handler->m_event.button.y / 80;
                // Nếu có một quân cờ được nhấp vào và tất cả tọa độ đều hợp lệ
                if (clickedOn != nullptr)
                {
                    if ((xStart != -1 && yStart != -1 && xEnd != -1 && yEnd != -1)
                        && (clickedOn->getTeam() == game->getTurn())
                        && (game->isValidMove(xEnd, yEnd, clickedOn)))
                    {
                        // Lấy các nước đi có thể của quân cờ
                        std::vector<std::tuple<int, int, Piece::MoveType>> list = game->getFieldPos(xStart, yStart)->getPossibleMoves();
                        // Duyệt qua các nước đi có thể
                        for (const auto& value : list)
                        {
                            // Nếu tọa độ kết thúc khớp với một nước đi có thể
                            if (std::get<0>(value) == xEnd && std::get<1>(value) == yEnd)
                            {
                                // Di chuyển quân cờ đến vị trí mới
                                game->move(clickedOn, std::tuple<int, int, Piece::MoveType>(xEnd, yEnd, std::get<2>(value)));
                            }
                        }
                        // Đặt lại tọa độ bắt đầu
                        xStart = -1;
                        yStart = -1;
                        yEnd = -1;
                        // Tính toán lại tất cả các nước đi có thể
                        game->calcAllMoves();
                        // Đặt lại con trỏ quân cờ đã nhấp vào
                        clickedOn = nullptr;
                    }
                }
            }
        }
    }

    // Dọn dẹp đối tượng SDL_Handler
    handler->cleanUp();
}
