#ifndef UI_STAGE_H
#define UI_STAGE_H

#include <stdio.h>
#include <stdint.h>

#include "architecture.h"
#include "ui.h"

// 卡牌區域按鈕管理結構
typedef struct {
    sButton **buttons;      // 按鈕陣列
    int32_t buttonCount;    // 按鈕數量
    char areaName[32];      // 區域名稱
    SDL_Rect areaRect;      // 區域矩形
    bool enabled;           // 是否啟用
} sCardAreaButtons;

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
    sButton *opponentDeckButton;    // 恢復對手牌庫按鈕
    sButton *opponentGraveButton;
    sButton *opponentHandButton;    // 對手手牌按鈕
    sButton *opponentMetamorphButton;
    sButton *myDeckButton;
    sButton *myGraveButton;
    sButton *myMetamorphButton;
    
    sButton *cardButtons[32];   // Hand/skill card buttons
    
    // 卡牌區域按鈕管理
    sCardAreaButtons handAreaButtons;      // 手牌區域按鈕
    sCardAreaButtons opponentHandAreaButtons;  // 對手手牌區域按鈕
    sCardAreaButtons skillAreaButtons;     // 技能區域按鈕
    
    // UI state
    int32_t selectedCard;
    int32_t selectedArea;  // 0=none, 1=hand, 2=skills
    bool showShop;
    bool waitingForInput;
    
    // Focus action state
    bool focusMode;
    int32_t focusSelectedHand;
    int32_t focusSelectedGrave;
    
    // New popup window states
    bool showShopPopup;
    // 移除 showOpponentDeckPopup
    bool showOpponentGravePopup;
    bool showMyDeckPopup;
    bool showMyGravePopup;
    
    // Skill and special deck popup states
    bool showOpponentSpecialPopup;
    bool showOpponentAttackSkillPopup;
    bool showOpponentDefenseSkillPopup;
    bool showOpponentMoveSkillPopup;
    bool showMySpecialPopup;
    bool showMyAttackSkillPopup;
    bool showMyDefenseSkillPopup;
    bool showMyMoveSkillPopup;
    
    // Hover states
    bool hoveringShop;
    bool hoveringOpponentDeck;    // 恢復懸停狀態
    bool hoveringOpponentGrave;
    bool hoveringMyDeck;
    bool hoveringMyGrave;
    
    // Skill hover states
    bool hoveringOpponentSpecial;
    bool hoveringOpponentAttackSkill;
    bool hoveringOpponentDefenseSkill;
    bool hoveringOpponentMoveSkill;
    bool hoveringMySpecial;
    bool hoveringMyAttackSkill;
    bool hoveringMyDefenseSkill;
    bool hoveringMyMoveSkill;
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

// New popup window functions
void draw_shop_popup();
void draw_deck_popup(const char* title, vector* deck, vector* graveyard);
void draw_hover_preview(const char* title, vector* cards, int32_t mouseX, int32_t mouseY);
void handle_popup_events(SDL_Event* event);

#endif