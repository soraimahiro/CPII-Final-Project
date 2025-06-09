#include "game.h"
#include "card.h"
#include "stage.h"
#include "ui.h"
#include "ui_stage.h"
#include "ui_component.h"
#include "architecture.h"

sGame game;
sUiBase uiBase;
gameState nowState = GAME_INIT_MENU;
bool running = 0;

int8_t winner = 0;

int main() {

    init_ui();

    nowState = GAME_INIT_MENU;
    running = 1;
    while (running) {
        if(nowState == GAME_INIT_MENU){
            game_init_menu();
        }
        else if(nowState == GAME_INIT_CHARACTOR){
            int32_t nowPlayer;
            game_init_charactor(&nowPlayer);
        }
        else if(nowState == GAME_PLAY){
            game_play();
        }
        else{
            game_over();
        }
    }

    close_ui();

    // sPlayer* pPlayer = malloc(sizeof(sPlayer));
    // game.players[0] = *pPlayer;
    // pPlayer->character = 0;
    // init_character(pPlayer);
    // switch (pPlayer->character) {
    //     case 0:
    //         init_red_hood(pPlayer);
    // }

    // int val;
    // shuffle(&(pPlayer->attackSkill));
    // getVectorTop(&(pPlayer->attackSkill),&val);
    // printf("%d\n", val);
    // getCardData(val);
    // free(pPlayer);

    

    return 0;
}
