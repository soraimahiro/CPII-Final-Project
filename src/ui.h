#ifndef UI_H
#define UI_H

#include <stdio.h>
#include <stdint.h>
#include <raylib.h>

#include "architecture.h"


// 字體常數
#define FONT_PATH "assets/fonts/jf-openhuninn-2.0.ttf"
#define FONT_SIZE 40

// 畫面狀態
typedef enum {
    SCREEN_MENU = 0,
    SCREEN_GAME
} ScreenState;

// 按鈕結構
typedef struct {
    Rectangle rect;
    char* text;
    Color color;
    Color hoverColor;
    bool isHovered;
} Button;

void get_init_info(int8_t* mode);
void init_gui();
void close_gui();
Font get_custom_font();
void update_gui();
void draw_gui();
ScreenState get_current_screen();
void set_current_screen(ScreenState screen);

#endif
