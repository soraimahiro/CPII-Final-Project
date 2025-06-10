#ifndef GAME_H
#define GAME_H

#include "architecture.h"
#include "ui.h"
#include "card_id.h"

#define TOTAL_PLAYER game.playerMode == 0 ? 2 : 4

typedef enum _gameState{
    GAME_MENU = 0,
    GAME_INIT,
    GAME_PLAY,
    GAME_OVER
} gameState;

void init_game();
void init_character(sPlayer* pPlayer);
void init_red_hood(sPlayer* pPlayer);

#endif

