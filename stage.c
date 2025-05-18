#include "stage.h"

int beginning_phase(game* pGame){
    if (pGame == NULL) {
        return -1;
    }
}

int refresh_phase(game* pGame){
    if (pGame == NULL) {
        return -1;
    }

    int32_t card = 0;
    for(int32_t i = 0; i < 4; i++){
        while (getVectorTop(&(pGame->players[i].usecards), &card)){
            pushbackVector(&(pGame->players[i].graveyard), card);
            popbackVector(&(pGame->players[i].usecards));
        }
        pGame->players[i].defense = 0;
    }
    return 0;
}

int activation_phase(game* pGame){
    if (pGame == NULL) {
        return -1;
    }
}

int ending_phase(game* pGame){
    if (pGame == NULL){
        return -1;
    }

    int32_t card = 0;
    for(int32_t i = 0; i < 4 ; i++){
        pGame->players[i].energy = 0;
        while (getVectorTop(&(pGame->players[i].hand), &card)){
            pushbackVector(&(pGame->players[i].graveyard), card);
            popbackVector(&(pGame->players[i].hand));
        }
    }
}

// action
int focus();
int attack_action();
int defense_action();
int move_action();
int use_a_skill();
int use_an_epic_card();
int power_up_action();
int component_action();