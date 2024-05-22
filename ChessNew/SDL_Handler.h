#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

class SDL_Handler
{
public:
    // Kích thước màn hình
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 640;

    // Cửa sổ sẽ render
    SDL_Window* m_window;

    // Bề mặt chứa bởi cửa sổ
    SDL_Surface* m_screenSurface;

    // Renderer
    SDL_Renderer* m_renderer;

    // Sự kiện (click chuột, v.v.)
    SDL_Event m_event;

    // Khởi tạo trường
    // Trả về true nếu mọi thứ hoạt động
    bool init();

    // Dọn dẹp SDL
    void cleanUp();

    // Hủy
    ~SDL_Handler();

    // Vẽ hình chữ nhật nguồn lên hình chữ nhật đích của texture
    void DrawRectangle(SDL_Rect source, SDL_Rect dest, SDL_Texture* text);

    // Loại bỏ PieceRender
    void undoPieceRender(int x, int y);

    // Lấy tên tệp, trả về texture của tệp jpg
    SDL_Texture* loadImage(std::string filename);

    // Vẽ nền của trường
    void renderBackground();

private:
};

