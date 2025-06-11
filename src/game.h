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
void init_snow_white(sPlayer* pPlayer);
void init_sleeping_beauty(sPlayer* pPlayer);
void init_alice(sPlayer* pPlayer);
void init_mulan(sPlayer* pPlayer);
void init_kaguya(sPlayer* pPlayer);
void init_mermaid(sPlayer* pPlayer);
void init_match_girl(sPlayer* pPlayer);
void init_dorothy(sPlayer* pPlayer);
void init_scheherazade(sPlayer* pPlayer);

#endif

