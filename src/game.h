#ifndef GAME_H
#define GAME_H

#include "architecture.h"
#include "card_id.h"

#define TOTAL_PLAYER game.playerMode == 0 ? 2 : 4

typedef enum _gameState{
    GAME_MENU = 0,
    GAME_INIT_BOT_SELECT,
    GAME_INIT_CHARACTER_SELECT,
    GAME_PLAY,
    GAME_OVER
} gameState;

void game_init();
void init_character(sPlayer* pPlayer);
void init_red_hood(sPlayer* pPlayer);

// 新增的函數聲明
void setup_initial_deck(sPlayer* player);
void setup_skill_decks(sPlayer* player);
void setup_ultimate_cards(sPlayer* player);
void setup_basic_supply_decks();
void initial_draw();

// Game logic functions
void game_play_logic();  // Main game logic
void print_game_state(); // Debug function to print current game state
void print_player_state(sPlayer* player); // Debug function to print player state

#endif

