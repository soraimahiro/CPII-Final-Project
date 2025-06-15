#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../architecture.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define FONT_PATH "assets/fonts/jf-openhuninn-2.0.ttf"

#define SELECTABLE_CHARACTER_COUNT 6

#define TRANSPRANCY 200

typedef struct _sUiBase {
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;
} sUiBase;

// text
void draw_text(const char* text, int32_t x, int32_t y, SDL_Color color, int32_t font_size);
void draw_text_center(const char* text, int32_t x, int32_t y, SDL_Color color, int32_t font_size);
void draw_text_wrapped(const char* text, int32_t x, int32_t y, SDL_Color color, int32_t font_size, int32_t max_width);

// button
typedef struct {
    SDL_Rect rect;
    char* text;
    SDL_Color *textColor;
    SDL_Color *bgColor;
    SDL_Color *borderColor;
    int32_t fontSize;
    int32_t typeNumber;
} sButton;

sButton *create_button(SDL_Rect rext, const char *text, 
                       SDL_Color *textColor, SDL_Color *bgColor, 
                       SDL_Color *borderColor, int32_t fontSize, int32_t typeNumber);
void free_button(sButton *pButton);
void draw_button(sButton *button, int8_t type);
bool mouse_in_button(sButton *button);