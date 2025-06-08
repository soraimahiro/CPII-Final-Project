#ifndef UI_H
#define UI_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "architecture.h"

// 視窗常數
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define WINDOW_TITLE "Twisted Fables"

// 字體常數
#define FONT_PATH "assets/fonts/jf-openhuninn-2.0.ttf"
#define FONT_SIZE_LARGE 40
#define FONT_SIZE_MEDIUM 20
#define FONT_SIZE_SMALL 16
#define FONT_SIZE_TINY 12

// 畫面狀態
typedef enum {
    SCREEN_MENU = 0,
    SCREEN_GAME
} ScreenState;

// 按鈕結構
typedef struct {
    SDL_Rect rect;
    char* text;
    SDL_Color color;
    SDL_Color hoverColor;
    bool isHovered;
} Button;

void get_init_info(int8_t* mode);
void init_gui();
void close_gui();
TTF_Font* get_font_by_size(int fontSize);
void update_gui();
void draw_gui();
ScreenState get_current_screen();
void set_current_screen(ScreenState screen);
bool is_running();

#endif
