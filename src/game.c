#include "skills.h"
#include "game.h"
#include "card.h"
#include "stage.h"
#include "ui/ui.h"
#include "ui/ui_stage.h"
#include "ui/ui_component.h"
#include "architecture.h"

// 設置初始牌堆
void setup_initial_deck(sPlayer* player) {
    clearVector(&player->deck);
    // 添加基本牌
    for (int i = 0; i < 3; i++) {
        pushbackVector(&player->deck, 1);  // 基本攻擊lv1
        pushbackVector(&player->deck, 4);  // 基本防禦lv1
        pushbackVector(&player->deck, 7);  // 基本移動lv1
    }
    
    // 添加角色牌
    int base_id = (player->character) * 12 + 11;  // 計算角色牌的起始ID
    int top_id = 0;
    pushbackVector(&player->deck, base_id);      // 角色攻擊lv1
    eraseVector(&player->attackSkill, top_id);
    pushbackVector(&player->deck, base_id + 3);  // 角色防禦lv1
    eraseVector(&player->defenseSkill, top_id);
    pushbackVector(&player->deck, base_id + 6);  // 角色移動lv1
    eraseVector(&player->moveSkill, top_id);
    
    
    // 洗牌
    shuffle(&player->deck);
}

// 設置技能牌堆
void setup_skill_decks(sPlayer* player) {
    int base_id = player->character * 12 + 11;  // 角色基本牌起始ID
    int meta_id = player->character * 4 + 135;
    
    pushbackVector(&player->attackSkill, base_id);
    pushbackVector(&player->defenseSkill, base_id + 3);
    pushbackVector(&player->moveSkill, base_id + 6);
    
    for (int i = 0; i < 2; i++) {
        pushbackVector(&player->attackSkill, base_id + 1);
        pushbackVector(&player->defenseSkill, base_id + 4);
        pushbackVector(&player->moveSkill, base_id + 7);
    }
    pushbackVector(&player->attackSkill, meta_id);
    pushbackVector(&player->defenseSkill, meta_id + 1);
    pushbackVector(&player->moveSkill, meta_id + 2);
    
    for (int i = 0; i < 3; i++) {
        pushbackVector(&player->attackSkill, base_id + 2);
        pushbackVector(&player->defenseSkill, base_id + 5);
        pushbackVector(&player->moveSkill, base_id + 8);
    }
    pushbackVector(&player->attackSkill, meta_id + 3);
    pushbackVector(&player->defenseSkill, meta_id + 3);
    pushbackVector(&player->moveSkill, meta_id + 3);
}

// 設置必殺牌
void setup_ultimate_cards(sPlayer* player) {
    int base_id = (player->character) * 12 + 11;
    for (int i = 0; i < 3; i++) {
        pushbackVector(&player->specialDeck, base_id + 9 + i);  // 3張必殺牌
    }
}

// 設置基本供應牌庫
void setup_basic_supply_decks() {
    // 初始化基本供應牌庫
    for (int i = 0; i < 4; i++) {  // 攻擊、防禦、移動、通用
        for (int j = 0; j < 3; j++) {  // 等級1-3
            game.basicBuyDeck[i][j] = initVector();
        }
    }
    
    // 填充攻擊牌
    for (int i = 0; i < 18; i++) {
        pushbackVector(&game.basicBuyDeck[0][0], 1);  // 攻擊lv1
        pushbackVector(&game.basicBuyDeck[0][1], 2);  // 攻擊lv2
        pushbackVector(&game.basicBuyDeck[0][2], 3);  // 攻擊lv3
    }
    
    // 填充防禦牌
    for (int i = 0; i < 18; i++) {
        pushbackVector(&game.basicBuyDeck[1][0], 4);  // 防禦lv1
        pushbackVector(&game.basicBuyDeck[1][1], 5);  // 防禦lv2
        pushbackVector(&game.basicBuyDeck[1][2], 6);  // 防禦lv3
    }
    
    // 填充移動牌
    for (int i = 0; i < 18; i++) {
        pushbackVector(&game.basicBuyDeck[2][0], 7);  // 移動lv1
        pushbackVector(&game.basicBuyDeck[2][1], 8);  // 移動lv2
        pushbackVector(&game.basicBuyDeck[2][2], 9);  // 移動lv3
    }
    
    // 填充通用牌
    for (int i = 0; i < 18; i++) {
        pushbackVector(&game.basicBuyDeck[3][0], 10);  // 通用牌
    }
}

// 初始抽牌
void initial_draw() {
    DEBUG_PRINT("initial draw\n");
    // 玩家1抽4張
    for (int i = 0; i < 4; i++) {
        int32_t card;
        if (getVectorTop(&game.players[0].deck, &card)) {
            DEBUG_PRINT("draw one card: %d\n", card);
            pushbackVector(&game.players[0].hand, card);
            popbackVector(&game.players[0].deck);
        } else {
            break;  // 牌堆空了就停止
        }
    }
    
    // 玩家2抽6張
    for (int i = 0; i < 6; i++) {
        int32_t card;
        if (getVectorTop(&game.players[1].deck, &card)) {
            DEBUG_PRINT("draw one card: %d\n", card);
            pushbackVector(&game.players[1].hand, card);
            popbackVector(&game.players[1].deck);
        } else {
            break;  // 牌堆空了就停止
        }
    }
}

void game_init() {
    // 1. 初始化玩家狀態
    for (int i = 0; i < 2; i++) {  // 1v1模式只有兩個玩家
        sPlayer* player = &game.players[i];
        
        // 獲取角色信息
        sCharacterInfo charInfo = get_character_info(player->character);
        
        // 設置初始生命值和必殺閥值
        player->maxlife = charInfo.maxlife;
        player->life = charInfo.maxlife;
        player->maxdefense = charInfo.maxdefense;
        player->defense = 0;
        player->energy = 0;
        player->specialGate = charInfo.ultimate_threshold;
        
        // 設置起始位置
        player->locate[0] = (i == 0) ? 4 : 6;  // 玩家1在4，玩家2在6
        
        // 初始化所有vector
        init_character(player);
        
        // 設置技能牌堆
        setup_skill_decks(player);
        
        // 設置必殺牌
        setup_ultimate_cards(player);

        // 設置初始牌堆
        setup_initial_deck(player);
    }
    
    // 2. 設置基本供應牌庫
    setup_basic_supply_decks();
    
    // 3. 設置初始抽牌
    initial_draw();

    // 4. 設置遊戲狀態
    game.now_turn_player_id = 0;  // 玩家1先攻
    game.status = CHOOSE_MOVE;    // 從選擇移動開始
    game.set_skill_hand = -1;
    game.focused = false;

    // Debug output - print all game.players information
    DEBUG_PRINT("=== GAME INITIALIZATION DEBUG INFO ===\n");
    DEBUG_PRINT("Player Mode: %s\n", game.playerMode == 0 ? "1v1" : "2v2");
    DEBUG_PRINT("Current Turn Player: %d\n", game.now_turn_player_id);
    DEBUG_PRINT("Game Status: %d\n", game.status);
    
    int32_t playerCount = TOTAL_PLAYER;
    for(int32_t i = 0; i < playerCount; i++){
        DEBUG_PRINT("\n");
        DEBUG_PRINT("--- Player %d ---\n", i);
        DEBUG_PRINT("isBOT: %s\n", game.players[i].isBOT ? "true" : "false");
        DEBUG_PRINT("team: %d\n", game.players[i].team);
        DEBUG_PRINT("character: %d\n", game.players[i].character);
        DEBUG_PRINT("locate: [%d, %d]\n", game.players[i].locate[0], game.players[i].locate[1]);
        DEBUG_PRINT("maxlife: %d, life: %d\n", game.players[i].maxlife, game.players[i].life);
        DEBUG_PRINT("maxdefense: %d, defense: %d\n", game.players[i].maxdefense, game.players[i].defense);
        DEBUG_PRINT("energy: %d\n", game.players[i].energy);
        DEBUG_PRINT("specialGate: %d\n", game.players[i].specialGate);
        
        // Print all vectors
        DEBUG_PRINT_VEC(&game.players[i].hand, "hand");
        DEBUG_PRINT_VEC(&game.players[i].deck, "deck");
        DEBUG_PRINT_VEC(&game.players[i].usecards, "usecards");
        DEBUG_PRINT_VEC(&game.players[i].graveyard, "graveyard");
        DEBUG_PRINT_VEC(&game.players[i].metamorphosis, "metamorphosis");
        DEBUG_PRINT_VEC(&game.players[i].attackSkill, "attackSkill");
        DEBUG_PRINT_VEC(&game.players[i].defenseSkill, "defenseSkill");
        DEBUG_PRINT_VEC(&game.players[i].moveSkill, "moveSkill");
        DEBUG_PRINT_VEC(&game.players[i].specialDeck, "specialDeck");
        
        // Print character-specific data based on character type
        switch(game.players[i].character) {
            case CHARACTER_REDHOOD:
                DEBUG_PRINT("Red Hood - saveCard: [%d, %d, %d]\n", 
                    game.players[i].redHood.saveCard[0], 
                    game.players[i].redHood.saveCard[1], 
                    game.players[i].redHood.saveCard[2]);
                break;
                
            case CHARACTER_SNOWWHITE:
                DEBUG_PRINT_VEC(&game.players[i].snowWhite.remindPosion, "Snow White remindPosion");
                break;
                
            case CHARACTER_SLEEPINGBEAUTY:
                DEBUG_PRINT("Sleeping Beauty - AWAKEN_TOKEN: %u, AWAKEN: %d, dayNightmareDrawRemind: %d\n",
                    game.players[i].sleepingBeauty.AWAKEN_TOKEN,
                    game.players[i].sleepingBeauty.AWAKEN,
                    game.players[i].sleepingBeauty.dayNightmareDrawRemind);
                DEBUG_PRINT("Sleeping Beauty - atkRise: %d, atkRiseTime: %d, usedmeta1: %d\n",
                    game.players[i].sleepingBeauty.atkRise,
                    game.players[i].sleepingBeauty.atkRiseTime,
                    game.players[i].sleepingBeauty.usedmeta1);
                break;
                
            case CHARACTER_ALICE:
                DEBUG_PRINT("Alice - identity: %d, riseBasic: %d, restartTurn: %d, havedrestart: %d\n",
                    game.players[i].alice.identity,
                    game.players[i].alice.riseBasic,
                    game.players[i].alice.restartTurn,
                    game.players[i].alice.havedrestart);
                break;
                
            case CHARACTER_MULAN:
                DEBUG_PRINT("Mulan - KI_TOKEN: %u, extraCard: %d, extraDraw: %d\n",
                    game.players[i].mulan.KI_TOKEN,
                    game.players[i].mulan.extraCard,
                    game.players[i].mulan.extraDraw);
                break;
                
            case CHARACTER_KAGUYA:
                DEBUG_PRINT("Kaguya - useDefenseAsATK: %d, useMoveTarget: %d\n",
                    game.players[i].kaguya.useDefenseAsATK,
                    game.players[i].kaguya.useMoveTarget);
                break;
                
            case CHARACTER_MATCHGIRL:
                DEBUG_PRINT("Match Girl - remindMatch: %u, pushedMatch: %u\n",
                    game.players[i].matchGirl.remindMatch,
                    game.players[i].matchGirl.pushedMatch);
                break;
                
            case CHARACTER_DOROTHY:
                DEBUG_PRINT("Dorothy - COMBO_TOKEN: %u, canCombo: %d\n",
                    game.players[i].dorothy.COMBO_TOKEN,
                    game.players[i].dorothy.canCombo);
                break;
                
            case CHARACTER_SCHEHERAZADE:
                DEBUG_PRINT_VEC(&game.players[i].scheherazade.destiny_TOKEN_locate, "Scheherazade destiny_TOKEN_locate");
                DEBUG_PRINT_VEC(&game.players[i].scheherazade.destiny_TOKEN_type, "Scheherazade destiny_TOKEN_type");
                DEBUG_PRINT("Scheherazade - selectToken: %d\n", game.players[i].scheherazade.selectToken);
                break;
                
            default:
                DEBUG_PRINT("Unknown character or no special data\n");
                break;
        }
    }
    
    // Print basic supply decks
    DEBUG_PRINT("\n");
    DEBUG_PRINT("--- Basic Supply Decks ---\n");
    const char* deckNames[] = {"Attack", "Defense", "Move", "Universal"};
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 3; j++) {
            DEBUG_PRINT_VEC(&game.basicBuyDeck[i][j], "%s LV%d", deckNames[i], j+1);
        }
    }
    
    DEBUG_PRINT("=== END GAME INITIALIZATION DEBUG ===\n\n");
    if (game.players[game.now_turn_player_id].isBOT) {
        bot_act();
        next_phase();
    }
}

void init_character(sPlayer* p){
    p->hand = initVector();
    p->deck = initVector();
    p->usecards = initVector();
    p->graveyard = initVector();
    p->metamorphosis = initVector();
    p->attackSkill = initVector();
    p->defenseSkill = initVector();
    p->moveSkill = initVector();
    p->specialDeck = initVector();

    switch(p->character){
        case CHARACTER_REDHOOD:
            p->redHood.saveCard[0] = -1;
            p->redHood.saveCard[1] = -1;
            p->redHood.saveCard[2] = -1;
            p->redHood.saveAtk = 0;
            p->redHood.saveKnock = 0;
            break;
        case CHARACTER_SNOWWHITE:
            p->snowWhite.remindPosion = initVector();
            for(int32_t i = 0; i < 6; i++){
                pushbackVector(&p->snowWhite.remindPosion, CARD_SNOWWHITE_STATUS_POISON3);
            }
            for(int32_t i = 0; i < 6; i++){
                pushbackVector(&p->snowWhite.remindPosion, CARD_SNOWWHITE_STATUS_POISON2);
            }
            for(int32_t i = 0; i < 6; i++){
                pushbackVector(&p->snowWhite.remindPosion, CARD_SNOWWHITE_STATUS_POISON1);
            }
            break;
        case CHARACTER_SLEEPINGBEAUTY:
            p->sleepingBeauty.AWAKEN_TOKEN = 0;
            p->sleepingBeauty.AWAKEN = 0;
            p->sleepingBeauty.dayNightmareDrawRemind = 0;
            p->sleepingBeauty.atkRise = 0;
            p->sleepingBeauty.atkRiseTime = 0;
            p->sleepingBeauty.usedmeta1 = 0;
            p->sleepingBeauty.caused_damage = 0;
            break;
        case CHARACTER_ALICE:
            p->alice.identity = 0;
            p->alice.riseBasic = 0;
            p->alice.restartTurn = 0;
            p->alice.havedrestart = 0;
            break;
        case CHARACTER_MULAN:
            p->mulan.KI_TOKEN = 0;
            p->mulan.extraCard = 0;
            p->mulan.extraDraw = 0;
            break;
        case CHARACTER_KAGUYA:
            p->kaguya.useDefenseAsATK = 0;
            p->kaguya.useMoveTarget = 0;
            break;
        case CHARACTER_MERMAID:
            break;
        case CHARACTER_MATCHGIRL:
            p->matchGirl.remindMatch = 12;
            p->matchGirl.pushedMatch = 0;
            break;
        case CHARACTER_DOROTHY:
            p->dorothy.COMBO_TOKEN = 0;
            p->dorothy.canCombo = 0;
            break;
        case CHARACTER_SCHEHERAZADE:
            p->scheherazade.destiny_TOKEN_locate = initVector();
            p->scheherazade.destiny_TOKEN_type = initVector();
            p->scheherazade.selectToken = 0;
            break;
        default:
            break;
    }
}

void print_player_state(sPlayer* player) {
    printf("\n=== Player State ===\n");
    printf("Character ID: %d\n", player->character);
    printf("Position: (%d, %d)\n", player->locate[0], player->locate[1]);
    printf("Health: %d/%d\n", player->life, player->maxlife);
    printf("Defense: %d/%d\n", player->defense, player->maxdefense);
    printf("Energy: %d\n", player->energy);
    printf("Special Gate: %d\n", player->specialGate);  
}

void print_game_state() {
    printf("\n========== Game State ==========\n");
    printf("Current Turn: Player %d\n", game.now_turn_player_id + 1);
    printf("Game Status: %d\n", game.status);
    printf("Player Mode: %s\n", game.playerMode == 0 ? "1v1" : "2v2");

    
    // Print each player's state
    for (int i = 0; i < 4; i++) {
        if (game.players[i].character != -1) {
            printf("\n--- Player %d ---\n", i + 1);
            print_player_state(&game.players[i]);
        }
    }
    printf("\n================================\n");
}

// Helper function to check if a card is a basic card of specific type
bool is_basic_card(int32_t card_id, int type) {
    // Basic cards are 1-10
    // 1-3: Attack
    // 4-6: Defense
    // 7-9: Move
    // 10: Universal
    if (card_id < 1 || card_id > 10) return false;
    
    if (type == TYPE_ATTACK) return card_id >= 1 && card_id <= 3;
    if (type == TYPE_DEFENSE) return card_id >= 4 && card_id <= 6;
    if (type == TYPE_MOVE) return card_id >= 7 && card_id <= 9;
    if (type == TYPE_BASIC) return card_id == 10;
    
    return false;
}

// Helper function to get card value
int get_card_value(int32_t card_id) {
    if (card_id >= 1 && card_id <= 3) return card_id;  // Attack cards
    if (card_id >= 4 && card_id <= 6) return card_id - 3;  // Defense cards
    if (card_id >= 7 && card_id <= 9) return card_id - 6;  // Move cards
    if (card_id == 10) return 1;  // Universal card
    return 0;
}

// Helper function to print hand cards
void print_hand_cards(sPlayer* player) {
    printf("\nYour hand cards:\n");
    for (int i = 0; i < player->hand.SIZE; i++) {
        printf("%d. Card ID: %d\n", i + 1, player->hand.array[i]);
    }
}

void attack(sPlayer* defender, int total_damage) {
    sPlayer* attacker = &game.players[game.now_turn_player_id];
    if (attacker->character == CHARACTER_SLEEPINGBEAUTY) {
        if (attacker->sleepingBeauty.atkRiseTime) {
            total_damage += attacker->sleepingBeauty.atkRise;
            attacker->sleepingBeauty.atkRiseTime -= 1;
        }
    }
    if (defender->character == CHARACTER_MULAN) {
        total_damage -= ki(defender);
        if (total_damage < 0) total_damage = 0;
    }
    DEBUG_PRINT("atk1\n");
    int remaining_damage = 0;
    if (defender->defense > 0) {
        if (defender->defense >= total_damage) {
            defender->defense -= total_damage;
        } else {
            remaining_damage = total_damage - defender->defense;
            defender->defense = 0;
            defender->life = (defender->life > remaining_damage) ? 
                            defender->life - remaining_damage : 0;
        }
    } 
    else {
        remaining_damage = total_damage;
        defender->life = (defender->life > total_damage) ? 
                        defender->life - total_damage : 0;
    }
    // TODO: if (defender->life <= defender->specialGate) 必殺技();
    DEBUG_PRINT("atk2\n");
    DEBUG_PRINT("%d\n",remaining_damage);
    if (remaining_damage) {
        if (attacker->character == CHARACTER_SLEEPINGBEAUTY) {
            attacker->sleepingBeauty.caused_damage += remaining_damage;
        }
        if (defender->character == CHARACTER_SLEEPINGBEAUTY) {
            if (!defender->sleepingBeauty.AWAKEN) {
                defender->sleepingBeauty.AWAKEN_TOKEN += remaining_damage;
                if (defender->sleepingBeauty.AWAKEN_TOKEN >= 6) {
                    defender->sleepingBeauty.AWAKEN_TOKEN = 6;
                    defender->sleepingBeauty.AWAKEN = 1;
                }
            }
            DEBUG_PRINT("atk3\n");
            if (countCard(&defender->usecards, CARD_SLEEPINGBEAUTY_SPECIAL2_DAYMARE)) {
                int draw_count = min(remaining_damage, 6 - defender->sleepingBeauty.dayNightmareDrawRemind);
                draw_card(defender, draw_count);
                defender->sleepingBeauty.dayNightmareDrawRemind = min(defender->sleepingBeauty.dayNightmareDrawRemind + remaining_damage, 6);
            }
        }
    }
    const char* character_data[CHARACTER_COUNT] = {"小紅帽", "白雪公主", "愛麗絲", "花木蘭", "輝夜姬", "火柴女孩"};
    if (defender->life <= defender->specialGate) {
        printf("%s choose 1 ultimate card (1-3): ", character_data[defender->character]);
        int choice;
        scanf("%d", &choice);
        if (choice < 1 || choice > 3) {
            choice = 1;  // 預設選擇第一張
        }
        int ultimate_id;
        for (int i = 0; i < 3; i++) {
            if (choice == 3) {
                getVectorTop(&defender->specialDeck, &ultimate_id);
                popbackVector(&defender->specialDeck);
                break;
            }
            if (i == choice) getVectorTop(&defender->specialDeck, &ultimate_id);
            popbackVector(&defender->specialDeck);
        }
        pushbackVector(&defender->graveyard, ultimate_id);
    }
    if(defender->life <= 0){
        change_state(GAME_OVER);
    }
}

void defend(sPlayer* player, int total_defense) {
    player->defense += total_defense;
    if (player->defense > player->maxdefense) player->defense = player->maxdefense;
}

void move(sPlayer* player, int total_move, bool snow) {
    sPlayer *oppoent = &game.players[(game.now_turn_player_id+1)%2];
    if (!snow) printf("\nChoose direction (-1: up, 1: down): ");
    int32_t direction;
    if (!snow) scanf("%d", &direction);
    else direction = 1;
    int32_t dest = 0, src = player->locate[0];
    if (player->locate[0] + total_move*direction == oppoent->locate[0]) {
        dest = src + (total_move-1)*direction;
    }
    else if (player->locate[0] + total_move*direction < 1) dest = 1;
    else if (player->locate[0] + total_move*direction > 9) dest = 9;
    else{
        dest = src + total_move*direction;
    }
    player->locate[0] = dest;
    if(player->character == CHARACTER_SNOWWHITE && player->snowWhite.meta3 &&
      (dest > oppoent->locate[0] && src < oppoent->locate[0]) ||
      (dest < oppoent->locate[0] && src > oppoent->locate[0])){
        put_posion(player, oppoent, &oppoent->graveyard);
    }
}

// Attack action
void handle_attack(sPlayer* attacker, sPlayer* defender, int handIndex) {
    DEBUG_PRINT("\033[31mhandle attack\033[0m\n");
    if (abs(attacker->locate[0] - defender->locate[0]) > 1) {
        printf("range is not enough\n");
        return;
    }
    
    int total_damage = 0;
    int total_energy = 0;
    bool continue_attack = true;

    // TODO
    int cost_ki = 0;
    
    // Get the selected card
    int32_t card_id = attacker->hand.array[handIndex];
    const Card* card = getCardData(card_id);
    
    // Calculate damage and energy
    int card_value = card->damage;
    total_damage += card_value;
    attacker->energy += card->level;
    if (attacker->energy > 25) attacker->energy = 25;  

    if (attacker->character == CHARACTER_MULAN) {
        int cost_ki = 0;
        printf("cost ki: ");
        scanf("%d", &cost_ki);
        attacker->mulan.KI_TOKEN -= cost_ki;
        total_damage += cost_ki;
    }

    // Move card to usecards
    pushbackVector(&attacker->usecards, card_id);
    eraseVector(&attacker->hand, handIndex);   

    attack(defender, total_damage);

    if(attacker->character == CHARACTER_SNOWWHITE && attacker->snowWhite.meta1 && total_damage > 2){
        put_posion(attacker, defender, &defender->graveyard);
    }
    
}

// Defense action
void handle_defense(sPlayer* player, int handIndex) {
    DEBUG_PRINT("\033[31mhandle defense\033[0m\n");
    int total_defense = 0;
    int total_energy = 0;
    bool continue_defense = true;
        
    // Get the selected card
    int32_t card_id = player->hand.array[handIndex];
    const Card* card = getCardData(card_id);
    
    // Calculate defense and energy
    int card_value = card->defense;
    total_defense += card_value;
    player->energy += card->level;
    if (player->energy > 25) player->energy = 25;
    
    // Move card to usecards
    pushbackVector(&player->usecards, card_id);
    eraseVector(&player->hand, handIndex);
    
    defend(player, total_defense);
}

// Move action
void handle_move(sPlayer* player, int handIndex) {
    DEBUG_PRINT("\033[31mhandle move\033[0m\n");
    int total_move = 0;
    bool continue_move = true;
        
    // Get the selected card
    int32_t card_id = player->hand.array[handIndex];
    const Card* card = getCardData(card_id);
    
    if (!is_basic_card(card_id, TYPE_MOVE) && card_id != 10) {
        printf("Not a move card!\n");
        return;
    }
    
    // Calculate move distance and energy
    total_move += card->level;
    player->energy += card->level;
    if (player->energy > 25) player->energy = 25;
    
    // Move card to usecards
    pushbackVector(&player->usecards, card_id);
    eraseVector(&player->hand, handIndex);

    if (countCard(&player->metamorphosis, CARD_MULAN_METAMORPH3_CHARGE) && player->mulan.KI_TOKEN) {
        int ki_cost;
        scanf("%d", &ki_cost);
        player->mulan.KI_TOKEN -= ki_cost;
        total_move += ki_cost;
    }
    
    if (total_move > 0) {
        // Apply movement
        move(player, total_move, false);
    }
}

void set_skills(int handIndex) {
    game.set_skill_hand = handIndex;
}

void handle_skills(sPlayer* attacker, sPlayer* defender, int basicIndex) {
    DEBUG_PRINT("\033[31mhandle skill\033[0m\n");
    int skillIndex = game.set_skill_hand;

    int total_damage = 0;
    int total_defense = 0;
    int total_move = 0;
    int total_energy = 0;
    
    int32_t skill_card_id = attacker->hand.array[skillIndex];
    const Card* skill_card = getCardData(skill_card_id);
    int32_t basic_card_id = attacker->hand.array[basicIndex];
    const Card* basic_card = getCardData(basic_card_id);

    
    
    if (skill_card_id <= 19) {
        if (handle_redhood_skills(attacker, defender, skill_card, basic_card->level)) {
            DEBUG_PRINT("range not enough\n");
            return;
        };
    }
    else if (skill_card_id <= 31) {
        if (handle_snowwhite_skills(attacker, defender, skill_card, basic_card->level)) {
            DEBUG_PRINT("range not enough\n");
            return;
        }
    }
    else if (skill_card_id <= 43) {
        int level = basic_card->level;
        if (!(attacker->sleepingBeauty.usedmeta1 & 0b10) && countCard(&attacker->metamorphosis, CARD_SLEEPINGBEAUTY_METAMORPH1_BLOODLETTING)) {
            int choice;
            printf("放血療法(0/2/4/6): ");
            scanf("%d", &choice);
            if (!choice) {
                level = choice / 2; 
                attacker->sleepingBeauty.usedmeta1 ^= 0b10;
            }
        }
        if (handle_sleepingbeauty_skills(attacker, defender, skill_card, level)) {
            printf("range not enough\n");
            return;
        }
    }
    else if (skill_card_id <= 67) {
        if (handle_mulan_skills(attacker, defender, skill_card, basic_card->level)) {
            printf("range not enough\n");
            return;
        }
    }
    else if (skill_card_id <= 79) {
        if (handle_kaguya_skills(attacker, defender, skill_card, basic_card->level)) {
            printf("range not enough\n");
            return;
        }
    }
    else {
        if (handle_matchgirl_skills(attacker, defender, skill_card, basic_card->level)) {
            printf("range not enough\n");
            return;
        }
    }
    pushbackVector(&attacker->usecards, skill_card_id);
    eraseVector(&attacker->hand, skillIndex);
    pushbackVector(&attacker->usecards, basic_card_id);
    eraseVector(&attacker->hand, basicIndex);
}

void handle_ultimate(sPlayer* attacker, sPlayer* defender, int handIndex) {
    int total_damage = 0;
    int total_defense = 0;
    int total_move = 0;
    int total_energy = 0; //
    
    // Get the selected card
    int32_t ultimate_card_id = attacker->hand.array[handIndex];
    const Card* ultimate_card = getCardData(ultimate_card_id);
        
    //TODO: 檢查是否為必殺牌 
    if (ultimate_card_id <= 19) {
        if (handle_redhood_ultimate(attacker, defender, ultimate_card)) {
            printf("invalid!\n");
            return;
        }
    }
    else if(ultimate_card_id <= 31){
        if(handle_snowwhite_ultimate(attacker, defender, ultimate_card)){
            printf("invalid!\n");
            return;
        }
    }
    if (ultimate_card_id <= 46) {
        if (handle_sleepingbeauty_ultimate(attacker, defender, ultimate_card)) {
            printf("invalid!\n");
            return;
        }
    }
    else {
        if(handle_mulan_ultimate(attacker, defender, ultimate_card)){
            printf("invalid!\n");
            return;
        }
    }
    pushbackVector(&attacker->usecards, ultimate_card_id);
    eraseVector(&attacker->hand, handIndex);
}

// 抽牌函數
void draw_card(sPlayer* player, int count) {
    for (int i = 0; i < count; i++) {
        int32_t card;
        // 先嘗試從牌堆抽牌
        if (getVectorTop(&player->deck, &card)) {
            pushbackVector(&player->hand, card);
            popbackVector(&player->deck);
        } else {
            // 牌堆空了，檢查棄牌堆
            if (player->graveyard.SIZE > 0) {
                // 洗棄牌堆
                shuffle(&player->graveyard);
                // 把棄牌堆的牌移到牌堆
                while (player->graveyard.SIZE > 0) {
                    getVectorTop(&player->graveyard, &card);
                    pushbackVector(&player->deck, card);
                    popbackVector(&player->graveyard);
                }
                // 再試一次抽牌
                if (getVectorTop(&player->deck, &card)) {
                    pushbackVector(&player->hand, card);
                    popbackVector(&player->deck);
                } else {
                    // 如果還是抽不到牌，就結束
                    break;
                }
            } else {
                // 棄牌堆也空了，就結束
                break;
            }
        }
    }
}

void game_play_logic() {
    static bool first_render = true;
    static bool waiting_for_input = true;
    
    // 獲取當前玩家和對手
    sPlayer* current_player = &game.players[game.now_turn_player_id];
    sPlayer* opponent = &game.players[(game.now_turn_player_id + 1) % 2];
    
    printf("\n=== Player %d's Turn ===\n", game.now_turn_player_id + 1);
    
    // 開始階段
    printf("\n=== Start Phase ===\n");
    beginning_phase();

    // 清理階段
    printf("\n=== Refresh Phase ===\n");
    refresh_phase(); 

    // 行動階段


    bool action_phase_end = false;
    while (!action_phase_end) {
        printf("\n=== Action Phase ===\n");
        printf("1. Focus\n");
        printf("2. Attack\n");
        printf("3. Defense\n");
        printf("4. Move\n");
        printf("5. Use Skill\n");
        printf("6. Use Ultimate\n");
        printf("7. Buy Card\n");
        printf("8. End Action Phase\n");
        printf("9. chara special act\n");
        printf("-1. Exit Game\n");
        
        printf("Choose action (1-10): ");
        
        int choice;
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: // Focus
                printf("Focus: Choose a card to remove from hand or graveyard\n");
                // TODO: Implement focus logic
                action_phase_end = true;
                break;
                
            case 2: // Attack
                handle_attack(current_player, opponent, 0);
                break;
                
            case 3: // Defense
                handle_defense(current_player, 0);
                break;
                
            case 4: // Move
                handle_move(current_player, 0);
                break;
                
            case 5: // Use Skill
                printf("Use Skill: Choose a skill card to use\n");
                // handle_skills(current_player, opponent);
                break;
                
            case 6: // Use Ultimate
                printf("Use Ultimate: Choose an ultimate card to use\n");
                // handle_ultimate(current_player, opponent);
                break;
                
            case 7: // Buy Card
                printf("Buy Card: Choose a card to buy\n");
                // TODO: Implement card buying
                break;
                
            case 8: // End Action Phase
                action_phase_end = true;
                break;
            
            case 9:
                switch (current_player->character) {
                    case CHARACTER_REDHOOD: {
                        int value = countCard(&current_player->metamorphosis, CARD_REDHOOD_METAMORPH4_ONBOARD_CACHE);
                        if (value) {
                            printf("板載緩存\n");
                            int i;
                            for (i = 1; i <= value; i++) printf("%d. %d", i, current_player->redHood.saveCard[i - 1]);
                            printf("0. exit");
                            while (1) {
                                int choice;
                                scanf("%d", &choice);
                                if (!choice) break;
                                else if (choice <= i) {
                                    if (current_player->redHood.saveCard[choice - 1]) {
                                        // 如果已經有存牌，則將牌加入手牌
                                        int32_t saved_card = current_player->redHood.saveCard[choice - 1];
                                        pushbackVector(&current_player->hand, saved_card);
                                        current_player->redHood.saveCard[choice - 1] = -1;  // 清空儲存
                                        printf("將儲存的牌加入手牌\n");
                                    }
                                    else {
                                        if (current_player->hand.SIZE == 0) {
                                            printf("沒有手牌可以儲存！\n");
                                            break;
                                        }
                                        
                                        printf("選擇要儲存的手牌：\n");
                                        print_hand_cards(current_player);
                                        
                                        int card_choice;
                                        while (1) {
                                            printf("\n選擇要儲存的牌 (1-%d): ", current_player->hand.SIZE);
                                            scanf("%d", &card_choice);
                                            
                                            if (card_choice < 1 || card_choice > current_player->hand.SIZE) {
                                                printf("無效的選擇！\n");
                                                continue;
                                            }
                                            break;
                                        }
                                        
                                        // 儲存選擇的牌
                                        int32_t card_to_save = current_player->hand.array[card_choice - 1];
                                        current_player->redHood.saveCard[choice - 1] = card_to_save;
                                        eraseVector(&current_player->hand, card_choice - 1);
                                        printf("已儲存選擇的牌\n");
                                    }
                                }
                                else continue;
                            }
                        }
                        else {
                            printf("no available skills\n");
                            break;
                        }
                    }
                    case CHARACTER_SLEEPINGBEAUTY: {
                        int value = countCard(&current_player->metamorphosis, CARD_SLEEPINGBEAUTY_METAMORPH2_BLOOD_RITUAL);
                        if (!(current_player->sleepingBeauty.usedmeta1 & 0b01) && value) {
                            int damage = current_player->sleepingBeauty.caused_damage;
                            int max_level = 0;
                            
                            // 根據傷害決定可拿的牌等級
                            if (damage >= 6) max_level = 3;
                            else if (damage >= 4) max_level = 2;
                            else if (damage >= 2) max_level = 1;
                            
                            if (max_level > 0) {
                                printf("血祭之禮(max lv: %d)\n", max_level);
                                // 檢查棄牌堆中是否有符合條件的牌
                                int has_valid_card = 0;
                                for (int i = 0; i < current_player->graveyard.SIZE; i++) {
                                    int32_t card_id = current_player->graveyard.array[i];
                                    const Card* card = getCardData(card_id);
                                    if (card->type == TYPE_ATTACK && card->level <= max_level) {
                                        has_valid_card = 1;
                                        break;
                                    }
                                }
                                
                                if (has_valid_card) {
                                    printf("棄牌堆中的攻擊牌：\n");
                                    for (int i = 0; i < current_player->graveyard.SIZE; i++) {
                                        int32_t card_id = current_player->graveyard.array[i];
                                        const Card* card = getCardData(card_id);
                                        if (card->type == TYPE_ATTACK && card->level <= max_level) {
                                            printf("%d. %s (Lv%d)\n", i + 1, card->name, card->level);
                                        }
                                    }
                                    
                                    int choice;
                                    while (1) {
                                        printf("\n選擇要加入手牌的牌 (1-%d): ", current_player->graveyard.SIZE);
                                        scanf("%d", &choice);
                                        
                                        if (!choice) break;
                                        if (choice < 0 || choice > current_player->graveyard.SIZE) {
                                            printf("無效的選擇！\n");
                                            continue;
                                        }
                                        
                                        int32_t selected_card = current_player->graveyard.array[choice - 1];
                                        const Card* card = getCardData(selected_card);
                                        
                                        if (card->type != TYPE_ATTACK || card->level > max_level) {
                                            printf("只能選擇等級不超過 Lv%d 的攻擊牌！\n", max_level);
                                            continue;
                                        }
                                        
                                        // 將選擇的牌加入手牌
                                        pushbackVector(&current_player->hand, selected_card);
                                        eraseVector(&current_player->graveyard, choice - 1);
                                        printf("已將 %s 加入手牌\n", card->name);
                                        break;
                                    }
                                } else {
                                    printf("棄牌堆中沒有符合條件的攻擊牌！\n");
                                }
                            }
                            current_player->sleepingBeauty.usedmeta1 ^= 0b01;
                        }
                    }
                    default:
                        break;
                }
                default:
                    printf("no available skills\n");
                    break;
                
            case -1: // Exit Game
                change_state(GAME_MENU);
                return;
        }
    }
    
    // 結束階段
    printf("\n=== End Phase ===\n");
    ending_phase();
    first_render = true;
    waiting_for_input = true;
}

int activation_phase(int32_t handIndex){
    sPlayer* current_player = &game.players[game.now_turn_player_id];
    sPlayer* opponent = &game.players[(game.now_turn_player_id + 1) % 2];
    int32_t cardID = current_player->hand.array[handIndex];
    const Card* card = getCardData(cardID);
    DEBUG_PRINT("my locate: %d, oppoent locate: %d\n", current_player->locate[0], opponent->locate[0]);

    if (game.focused) {
        eraseVector(&current_player->hand, handIndex);
        game.focused = false;
        next_phase();
        return 0;
    }
    if (game.set_skill_hand != -1) {
        int32_t skillCardID = current_player->hand.array[game.set_skill_hand];
        const Card* skill_card = getCardData(skillCardID);
        if (card->id <= 10 && (card->type == TYPE_BASIC || skill_card->type == card->type)) {
            handle_skills(current_player, opponent, handIndex);
            return 0;
        }
        else {
            set_skills(-1);
            return 0;
        }
    }
    else set_skills(-1);
    if(cardID >= CARD_BASIC_ATK1 && cardID <= CARD_BASIC_ATK3){
        handle_attack(current_player, opponent, handIndex);
    }
    else if(cardID >= CARD_BASIC_DEF1 && cardID <= CARD_BASIC_DEF3){
        handle_defense(current_player, handIndex);
    }
    else if(cardID >= CARD_BASIC_MOVE1 && cardID <= CARD_BASIC_MOVE3){
        handle_move(current_player, handIndex);
    }
    else if(cardID >= 131 && cardID <= 133){
        current_player->energy += 1;
        if (current_player->energy > 25) current_player->energy = 25;
        pushbackVector(&current_player->graveyard, cardID);
        eraseVector(&current_player->hand, handIndex);
    }
    else if (card->type != TYPE_SPECIAL) { // skill cards
        set_skills(handIndex);
    }
    else handle_ultimate(current_player, opponent, handIndex);
    DEBUG_PRINT_VEC(&current_player->hand, "hand");
    DEBUG_PRINT_VEC(&current_player->graveyard, "graveyard");
    DEBUG_PRINT("%d\n", game.set_skill_hand);
    
    return 0;
}
 

void bot_act() {
    sPlayer* current_player = &game.players[game.now_turn_player_id];
    
    // 創建一個新陣列來存儲手牌
    int32_t hand_cards[256];  // 假設最多256張手牌
    int32_t hand_size = current_player->hand.SIZE;
    
    // 複製手牌到新陣列
    int i = 0;
    int j = 0;
    while (j < hand_size) {
        if (current_player->hand.array[i] <= 6) {
            hand_cards[i] = j;
            i++;
        }
        j++;
    }
    
    // 依序執行每張牌
    for (j = 0; j < i; j++) {
        activation_phase(hand_cards[j]);
    }
} 

int next_phase() {
    ending_phase();
    beginning_phase();
    refresh_phase();
    if (game.players[game.now_turn_player_id].isBOT) {
        bot_act();
        next_phase();
    }
    
    return 0;
}

void buyBasicCard(int32_t type, int32_t level) {
    // 獲取卡牌ID
    int32_t cardId = (type == 3) ? 10 : (type * 3 + level);
    const Card* cardData = getCardData(cardId);
    
    if (!cardData) return;
    
    // 從基礎牌商店移除卡牌
    int32_t buyCardId;
    if (type < 3) {
        buyCardId = game.basicBuyDeck[type][level-1].array[0];
        eraseVector(&game.basicBuyDeck[type][level-1], 0);
    } else {
        buyCardId = game.basicBuyDeck[3][0].array[0];
        eraseVector(&game.basicBuyDeck[3][0], 0);
    }
    
    // 扣除能量
    game.players[game.now_turn_player_id].energy -= cardData->cost;
    
    // 將卡牌加入棄牌堆
    pushbackVector(&game.players[game.now_turn_player_id].graveyard, buyCardId);
}