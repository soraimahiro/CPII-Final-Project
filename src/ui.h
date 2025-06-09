#ifndef UI_H
#define UI_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// #include "ui_component.h"
#include "architecture.h"
#include "game.h"
#include "ui_component.h"

// 視窗常數
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define WINDOW_TITLE "Twisted Fables"

// 字體常數
#define FONT_PATH "assets/fonts/jf-openhuninn-2.0.ttf"

void init_ui();
void close_ui();

void game_init_menu();
void game_init_charactor(int32_t *nowPlayer);
void game_play_event();
void game_play_show();
void game_over_event();
void game_over_show();

#endif
