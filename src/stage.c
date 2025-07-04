#include "stage.h"

extern sGame game;
extern int8_t winner;
extern int32_t total_turns; // 使用 main.c 中的全域變數

int check_game_winner(){
    int32_t team1_life = 0;
    int32_t team2_life = 0;
    int32_t playerCount = TOTAL_PLAYER;
    for(int32_t i = 0; i < playerCount; i++){
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

int beginning_phase() {
    sPlayer *pCurrentPlayer = &(game.players[game.now_turn_player_id]);
    sPlayer *pOpponentPlayer = &(game.players[(game.now_turn_player_id + 1) % 2]);
    pCurrentPlayer->sleepingBeauty.usedmeta1 = 0;
    
    DEBUG_PRINT("--- Beginning Phase ---\n");
    DEBUG_PRINT("Current Player ID: %d\n", game.now_turn_player_id);
    DEBUG_PRINT("Character: %d\n", pCurrentPlayer->character);
    DEBUG_PRINT("Life: %d/%d\n", pCurrentPlayer->life, pCurrentPlayer->maxlife);
    DEBUG_PRINT("Defense: %d/%d\n", pCurrentPlayer->defense, pCurrentPlayer->maxdefense);
    DEBUG_PRINT("Energy: %d\n", pCurrentPlayer->energy);
    DEBUG_PRINT_VEC(&pCurrentPlayer->hand, "Hand Cards");
    DEBUG_PRINT_VEC(&pCurrentPlayer->deck, "Deck Cards");
    DEBUG_PRINT_VEC(&pCurrentPlayer->graveyard, "Graveyard Cards");
    DEBUG_PRINT_VEC(&pCurrentPlayer->metamorphosis, "Metamorphosis Cards");
    DEBUG_PRINT_VEC(&pCurrentPlayer->attackSkill, "Attack Skills");
    DEBUG_PRINT_VEC(&pCurrentPlayer->defenseSkill, "Defense Skills");
    DEBUG_PRINT_VEC(&pCurrentPlayer->moveSkill, "Move Skills");
    DEBUG_PRINT_VEC(&pCurrentPlayer->specialDeck, "Special Deck");
    
    // 統計回合數 (只在玩家1的回合開始時增加，避免重複計算)
    if (game.now_turn_player_id == 0) {
        total_turns++;
    }
    
    // 處理小紅帽護盾持續效果
    if (pCurrentPlayer->character == CHARACTER_REDHOOD) { // Little Red Riding Hood
        int distance = abs(pCurrentPlayer->locate[0] - pOpponentPlayer->locate[0]);
        for (int i = 0; i < countCard(&pCurrentPlayer->usecards, CARD_REDHOOD_DEF1_ENERGY_SHIELD); i++) {
            if (distance <= 1) attack(pOpponentPlayer, 1);
        }
        for (int i = 0; i < countCard(&pCurrentPlayer->usecards, CARD_REDHOOD_DEF2_CURRENT_SHIELD); i++) {
            if (distance <= 2) attack(pOpponentPlayer, 2);
        }
        for (int i = 0; i < countCard(&pCurrentPlayer->usecards, CARD_REDHOOD_DEF3_ULTIMATE_SHIELD); i++) {
            if (distance <= 3) attack(pOpponentPlayer, 3);
        }
    }

    if(pCurrentPlayer->character == CHARACTER_MULAN) {
        for (int i = 0; i < countCard(&pCurrentPlayer->usecards, CARD_REDHOOD_DEF2_CURRENT_SHIELD); i++) {
            pCurrentPlayer->mulan.KI_TOKEN += 1;
        }
    }

    game.set_skill_hand = -1;
    
    return 0;
}

int refresh_phase(){
    sPlayer *current_player = &(game.players[game.now_turn_player_id]);
    sPlayer *pOpponentPlayer = &(game.players[(game.now_turn_player_id + 1) % 2]);
    current_player->defense = 0;
    int card_id;
    while (current_player->usecards.SIZE > 0) {
        getVectorTop(&current_player->usecards, &card_id);
        pushbackVector(&current_player->graveyard, card_id);
        popbackVector(&current_player->usecards);
    }
    printf("Used cards moved to graveyard\n");
    return 0;
}

int ending_phase(){
    sPlayer *current_player = &(game.players[game.now_turn_player_id]);
    sPlayer *pOpponentPlayer = &(game.players[(game.now_turn_player_id + 1) % 2]);
    current_player->energy = 0;
    printf("Energy reset to 0\n");

    if (current_player->character == CHARACTER_SLEEPINGBEAUTY) {
        current_player->sleepingBeauty.atkRise = 0;
        current_player->sleepingBeauty.atkRiseTime = 0;
        current_player->sleepingBeauty.caused_damage = 0;
    }

    int32_t card_id = 0;
    for (int i = 0; i < current_player->usecards.SIZE; i++) {
        card_id = current_player->usecards.array[i];
        const Card* card = getCardData(card_id);
        if (!card->last_for_turns) {
            pushbackVector(&current_player->graveyard, card_id);
            eraseVector(&current_player->usecards, i);
            i--;
        }
        else i++; // skip next card
    }
    printf("Used cards moved to graveyard\n");
    
    while (current_player->hand.SIZE > 0) {
        getVectorTop(&current_player->hand, &card_id);
        pushbackVector(&current_player->graveyard, card_id);
        popbackVector(&current_player->hand);
    }
    printf("Hand cards moved to graveyard\n");
    
    printf("Draw 6 cards\n");
    draw_card(current_player, 6);
    DEBUG_PRINT_VEC(&current_player->hand, "hand");
    if (countCard(&current_player->usecards, CARD_MULAN_SPECIAL1_SOARING)) draw_card(current_player, 4);

    if (current_player->mulan.extraDraw && current_player->mulan.KI_TOKEN) {
        printf("可以花費X點氣來額外抽取X張牌(max :%d): ", min(current_player->mulan.extraDraw, current_player->mulan.KI_TOKEN));
        int choice;
        scanf("%d", &choice);
        draw_card(current_player, choice);
        current_player->mulan.extraDraw = 0;
    }

    game.now_turn_player_id = (game.now_turn_player_id + 1) % 2;
    printf("Turn ended. Next player's turn.\n");

    return 0;
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

