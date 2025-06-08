#include "ui_stage.h"

extern sGame game;

void activation_menu(int8_t *active) {
    printf("1 focus\n");
    printf("2 attack\n");
    printf("3 defense\n");
    printf("4 move\n");
    printf("5 use a skill\n");
    printf("6 use an epic card\n");
    printf("7 power up\n");
    // printf("8 component action\n");
    printf("Choose an activation: ");
    scanf("%hhd", active);
}

void get_active_focus(int32_t *handCard, int32_t *graveCard) {
    int32_t option = 1;
    printf("In hand:\n");
    for(int32_t i = 0; i < game.players[game.now_turn_player_id].hand.SIZE; i++) {
        printf("%d\n", option);
        option++;
    }
    printf("In graveyard:\n");
    for(int32_t i = 0; i < game.players[game.now_turn_player_id].graveyard.SIZE; i++) {
        printf("%d\n", option);
        option++;
    }
    printf("Which card do you want to draw: ");
    scanf("%d", &option);
    if(option <= game.players[game.now_turn_player_id].hand.SIZE){
        *handCard = option - 1;
    }
    else{
        *graveCard = option - game.players[game.now_turn_player_id].hand.SIZE;
    }
}