#include "stage.h"

extern sGame game;

int beginning_phase(){
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
    } while(result != 0);
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

    for(int32_t i = 0; i < game.playerMode == 0 ? 2 : 4; i++){
        if(game.players[i].team != playerTeam && abs(game.players[i].locate[0] - pNowTurnPlayer->locate[0]) == 1){ // 檢查是否有敵人在射程1
            attackTarget = i;
            break;
        }
    }
    if(attackTarget == -1){
        printf("No opponent can attack.\n");
        return 1;
    }
    for(int32_t i = 0; i < pNowTurnPlayer->hand.SIZE; i++){
        if(pNowTurnPlayer->hand.array[i] == CARD_BASIC_ATK1){
            printf("Do you wanted to use card ");
        }
    }
}
int defense_action();
int move_action();
int use_a_skill();
int use_an_epic_card();
int power_up_action();
int component_action();