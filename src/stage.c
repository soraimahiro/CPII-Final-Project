#include "stage.h"

extern sGame game;
extern int8_t winner;

#define TOTAL_PLAYER game.playerMode == 0 ? 2 : 4


int check_game_winner(){
    int32_t team1_life = 0;
    int32_t team2_life = 0;
    for(int32_t i = 0; i < TOTAL_PLAYER; i++){
        if(game.players[i].team == 1){
            team1_life += game.players[i].life;
        }
        else if(game.players[i].team == 2){
            team2_life += game.players[i].life;
        }
    }
    if(team1_life <= 0){
        winner = 2;
        return 1;
    }
    if(team2_life <= 0){
        winner = 1;
        return 1;
    }
    return 0;
}

int beginning_phase(){
    sPlayer *pCurrentPlayer = &(game.players[game.now_turn_player_id]);
    
    // 處理小紅帽護盾持續效果
    if(pCurrentPlayer->character == 0) { // Little Red Riding Hood
        if(pCurrentPlayer->defense > 0) {
            // 檢查使用過的防禦技能是否有持續效果
            // 根據防禦技能等級決定射程和傷害
            int32_t damage = 0;
            int32_t range = 0;
            
            // 檢查上一回合使用的防禦技能
            // 等級一 能量護盾: 射程1 傷害2
            // 等級二 電流護盾: 射程2 傷害4  
            // 等級三 終極護盾: 射程3 傷害6
            
            // 簡化實現：假設使用了護盾技能，檢查防禦值來判斷等級
            if(pCurrentPlayer->defense >= 3) {
                damage = 6;
                range = 3;
            } else if(pCurrentPlayer->defense >= 2) {
                damage = 4;
                range = 2;
            } else if(pCurrentPlayer->defense >= 1) {
                damage = 2;
                range = 1;
            }
            
            if(damage > 0) {
                // 對射程內的敵人造成傷害
                for(int32_t i = 0; i < TOTAL_PLAYER; i++) {
                    if(game.players[i].team != pCurrentPlayer->team) {
                        int32_t distance = abs(game.players[i].locate[0] - pCurrentPlayer->locate[0]) + 
                                         abs(game.players[i].locate[1] - pCurrentPlayer->locate[1]);
                        if(distance <= range) {
                            game.players[i].life = game.players[i].life > damage ? 
                                                 game.players[i].life - damage : 0;
                        }
                    }
                }
            }
        }
    }
    
    // 處理白雪公主的至純之毒效果
    if(pCurrentPlayer->character == 1) { // Snow White
        // 檢查是否有至純之毒蛻變牌，用於處理中毒牌進入棄牌堆的額外傷害
        // 這個效果在中毒牌實際進入棄牌堆時觸發，這裡只是記錄狀態
        // 實際的傷害處理會在相關的動作函數中實現
    }
    
    return 0;
}

int refresh_phase(){
    int32_t card = 0;
    for(int32_t i = 0; i < 4; i++){
        while (getVectorTop(&(game.players[i].usecards), &card)){
            pushbackVector(&(game.players[i].graveyard), card);
            popbackVector(&(game.players[i].usecards));
        }
        game.players[i].defense = 0;
    }
    return 0;
}

int activation_phase(){
    int8_t active = 0;
    int32_t result = 0;
    do{
        activation_menu(&active);
        switch (active) {
            case 1:
                result = focus();
        }
    } while(result != 0 && !winner);
}

int ending_phase(){
    int32_t card = 0;
    for(int32_t i = 0; i < 4 ; i++){
        game.players[i].energy = 0;
        while (getVectorTop(&(game.players[i].hand), &card)){
            pushbackVector(&(game.players[i].graveyard), card);
            popbackVector(&(game.players[i].hand));
        }
    }
}

// action
int focus(){
    int32_t handCard = -1, graveCard = -1;
    get_active_focus(&handCard, &graveCard);
    if(handCard != -1){
        eraseVector(&(game.players[game.now_turn_player_id].hand), handCard);
    }
    else{
        eraseVector(&(game.players[game.now_turn_player_id].graveyard), graveCard);
    }
    return 0;
}

int attack_action(){
    int32_t attackTarget = -1;
    sPlayer *pNowTurnPlayer = &(game.players[game.now_turn_player_id]);
    int8_t playerTeam = pNowTurnPlayer->team; 

    for(int32_t i = 0; i < TOTAL_PLAYER; i++){
        if(game.players[i].team != playerTeam && abs(game.players[i].locate[0] - pNowTurnPlayer->locate[0]) == 1){ // 檢查是否有敵人在射程1
            attackTarget = i;
            break;
        }
    }
    if(attackTarget == -1){
        printf("沒有對手在射程內\n");
        return 1;
    }
}
int defense_action();
int move_action();
int use_a_skill();
int use_an_epic_card();
int power_up_action();
int component_action();