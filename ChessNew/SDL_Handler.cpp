#ifndef SDL_HANDLER_H
#define SDL_HANDLER_H

#include <iostream> 
#include <memory> 
#include <SDL.h> 
#include <SDL_image.h> 

class SDL_Handler
{
public:
    SDL_Handler() = default; // Hàm khởi tạo mặc định
    ~SDL_Handler() = default; // Hàm hủy mặc định

    bool init(); // Hàm khởi tạo SDL và tạo cửa sổ, bộ dựng
    void cleanUp(); // Hàm dọn dẹp tài nguyên SDL

    SDL_Texture* loadImage(const std::string& filename); // Hàm tải hình ảnh từ tệp
    void renderBackground(); // Hàm vẽ nền bàn cờ
    void undoPieceRender(int x, int y); // Hàm xóa hình ảnh quân cờ tại vị trí (x, y)
    void DrawRectangle(SDL_Rect source, SDL_Rect dest, SDL_Texture* text); // Hàm vẽ hình chữ nhật

private:
    SDL_Window* m_window = nullptr; // Con trỏ tới cửa sổ SDL
    SDL_Renderer* m_renderer = nullptr; // Con trỏ tới bộ dựng SDL
    SDL_Surface* m_screenSurface = nullptr; // Con trỏ tới bề mặt màn hình SDL

    // Hằng số cho kích thước màn hình và màu sắc
    constexpr static int SCREEN_WIDTH = 800;
    constexpr static int SCREEN_HEIGHT = 600;
    constexpr static int BOARD_SIZE = 8;
    constexpr static int WHITE_COLOR = 255;
    constexpr static int BROWN_COLOR = 155;
};

bool SDL_Handler::init()
{
    // Khởi tạo SDL video
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        cleanUp();
        return false;
    }

    // Tạo cửa sổ SDL
    m_window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!m_window)
    {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        cleanUp();
        return false;
    }

    // Tạo bộ dựng SDL
    m_renderer = SDL_CreateRenderer(m_window, -1, 0);
    if (!m_renderer)
    {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        cleanUp();
        return false;
    }

    return true; // Trả về true nếu khởi tạo thành công
}

SDL_Texture* SDL_Handler::loadImage(const std::string& filename)
{
    // Tải hình ảnh từ tệp và lưu vào SDL_Surface
    std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> loadedImage(IMG_Load(filename.c_str()), SDL_FreeSurface);
    if (!loadedImage)
    {
        std::cerr << "Couldn't load " << filename << std::endl;
        return nullptr; // Trả về nullptr nếu tải hình ảnh thất bại
    }

    // Tạo SDL_Texture từ SDL_Surface
    return SDL_CreateTextureFromSurface(m_renderer, loadedImage.get());
}

void SDL_Handler::renderBackground()
{
    bool white = true; // Biến để xác định ô vuông màu trắng

    // Thiết lập màu vẽ cho bộ dựng SDL
    SDL_SetRenderDrawColor(m_renderer, WHITE_COLOR, 255);

    // Vòng lặp vẽ nền bàn cờ
    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        for (int j = 0; j < BOARD_SIZE; ++j)
        {
            // Ở đây, ta sẽ thêm mã để vẽ từng ô vuông của bàn cờ
        }
        white = !white; // Đổi màu ô vuông sau mỗi dòng
    }
}

void SDL_Handler::undoPieceRender(int x, int y)
{
    const bool isWhiteSquare = (x + y) % 2 == 0; // Kiểm tra xem ô vuông là màu trắng hay không
    SDL_SetRenderDrawColor(m_renderer, isWhiteSquare ? WHITE_COLOR : BROWN_COLOR, 255);
    // Ở đây, ta sẽ thêm mã để xóa hình ảnh quân cờ tại vị trí (x, y)
}

void SDL_Handler::DrawRectangle(SDL_Rect source, SDL_Rect dest, SDL_Texture* text)
{
    if (text != nullptr)
    {
        // Vẽ hình chữ nhật lên bộ dựng SDL
        SDL_RenderCopy(m_renderer, text, &source, &dest);
        SDL_RenderPresent(m_renderer); // Hiển thị bộ dựng
        SDL_UpdateWindowSurface(m_window); // Cập nhật bề mặt cửa sổ
    }
    else
    {
        std::cerr << "DrawRectangle: text was nullptr" << std::endl; // Thông báo lỗi nếu text là nullptr
    }
}

void SDL_Handler::cleanUp()
{
    // Hủy bộ dựng và cửa sổ SDL
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit(); // Thoát SDL
}

#endif // SDL_HANDLER_H
