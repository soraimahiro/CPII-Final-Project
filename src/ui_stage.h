#ifndef UI_STAGE_H
#define UI_STAGE_H

#include <stdio.h>
#include <stdint.h>

#include "architecture.h"
#include "ui.h"

// Battle UI layout constants
#define LEFT_COLUMN_WIDTH 200
#define RIGHT_COLUMN_WIDTH 180
#define MIDDLE_COLUMN_WIDTH (SCREEN_WIDTH - LEFT_COLUMN_WIDTH - RIGHT_COLUMN_WIDTH)

#define BATTLE_GRID_HEIGHT 405  // 9 cells * 45px
#define BATTLE_GRID_Y 150
#define GRID_CELL_SIZE 45

// Battle UI state
typedef struct {
    // Action buttons (only 2 now)
    sButton *actionButtons[2];  // Focus and End Turn only
    sButton *shopButton;        // Shop button
    sButton *settingsButton;    // Settings button
    
    // Deck/Graveyard buttons
    sButton *opponentDeckButton;
    sButton *opponentGraveButton;
    sButton *opponentMetamorphButton;
    sButton *myDeckButton;
    sButton *myGraveButton;
    sButton *myMetamorphButton;
    
    sButton *cardButtons[32];   // Hand/skill card buttons
    
    // UI state
    int32_t selectedCard;
    int32_t selectedArea;  // 0=none, 1=hand, 2=skills
    bool showShop;
    bool waitingForInput;
    
    // Focus action state
    bool focusMode;
    int32_t focusSelectedHand;
    int32_t focusSelectedGrave;
} sBattleUIStage;

// Global battle UI instance
extern sBattleUIStage battleUIStage;

// Functions
void init_battle_ui_stage();
void cleanup_battle_ui_stage();
void draw_battle_ui_stage();
void handle_battle_events_stage(SDL_Event* event);

void activation_menu(int8_t *active);
void get_active_focus(int32_t *handCard, int32_t *graveCard);

// Helper functions
void draw_left_column_stage();
void draw_middle_column_stage();
void draw_right_column_stage();
void draw_battle_grid_stage();
void draw_player_info_stage(int32_t player_id, int32_t x, int32_t y, bool is_current);
void draw_card_area_stage(const char* title, vector* cards, int32_t x, int32_t y, int32_t width, int32_t height, bool clickable);

#endif