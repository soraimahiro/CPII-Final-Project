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

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define FONT_PATH "assets/fonts/jf-openhuninn-2.0.ttf"

void init_ui();
void close_ui();

void game_menu();
void game_init();
void game_play();
void game_over();

#endif
