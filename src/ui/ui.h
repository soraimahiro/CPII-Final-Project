#ifndef UI_H
#define UI_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// #include "ui_component.h"
#include "../architecture.h"
#include "../card.h"
#include "../game.h"
#include "ui_component.h"
#include "ui_stage.h"

// Confirmation dialog state
extern bool showConfirmDialog;

// State management
void change_state(gameState newState);
void cleanup_current_state();

void init_ui();
void close_ui();

typedef struct _sCharacterSelect{
    int32_t showCharacterInfo;
    bool showWarningDialog;
    sButton *ppShowInfoButton[SELECTABLE_CHARACTER_COUNT];
    sButton *pppPlayerSelect[4][SELECTABLE_CHARACTER_COUNT];
    sButton *pAcceptButton;
} sCharacterSelect;

typedef union _uStateComponent{
    sCharacterSelect *characterSelect;
} uStateComponent;

void game_menu_ui();
void game_init_bot_select_ui();
void game_init_character_select_ui();
void game_play_ui();
void game_over_ui();

#endif
