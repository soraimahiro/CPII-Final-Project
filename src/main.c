#include "game.h"
#include "card.h"
#include "stage.h"
#include "ui/ui.h"
#include "ui/ui_stage.h"
#include "ui/ui_component.h"
#include "architecture.h"
#include <time.h>

sGame game;
sUiBase uiBase;
gameState nowState = GAME_MENU;
bool running = 0;

int8_t winner = 0;
time_t game_start_time = 0;
time_t game_end_time = 0;

// 遊戲統計全域變數
int32_t total_turns = 0;           // 總回合數
int32_t game_duration_seconds = 0; // 遊戲時長（秒）

int main(int argc, char **argv) {

    init_ui();

    change_state(GAME_MENU);
    running = 1;
    while (running) {
        if(nowState == GAME_MENU){
            game_menu_ui();
        }
        else if(nowState == GAME_INIT_BOT_SELECT){
            game_init_bot_select_ui();
        }
        else if(nowState == GAME_INIT_CHARACTER_SELECT){
            game_init_character_select_ui();
        }
        else if(nowState == GAME_PLAY){
            // 記錄遊戲開始時間
            if(game_start_time == 0) {
                game_start_time = time(NULL);
            }
            game_play_ui();
        }
        else{
            // 記錄遊戲結束時間
            if(game_end_time == 0) {
                game_end_time = time(NULL);
                game_duration_seconds = (int32_t)(game_end_time - game_start_time);
            }
            game_over_ui();
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
