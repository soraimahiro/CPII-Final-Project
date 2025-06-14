#include "game.h"
#include "card.h"
#include "stage.h"
#include "ui.h"
#include "ui_stage.h"
#include "ui_component.h"
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
    pushbackVector(&player->deck, base_id);      // 角色攻擊lv1
    pushbackVector(&player->deck, base_id + 3);  // 角色防禦lv1
    pushbackVector(&player->deck, base_id + 6);  // 角色移動lv1
    
    // 洗牌
    shuffle(&player->deck);
}

// 設置技能牌堆
void setup_skill_decks(sPlayer* player) {
    int base_id = (player->character - 1) * 10 + 1;  // 角色基本牌起始ID
    int meta_base_id = (player->character - 1) * 4 + 169;  // 蛻變牌起始ID (從169開始，每個角色4張)
    
    // 攻擊技能牌堆
    for (int i = 0; i < 2; i++) {
        pushbackVector(&player->attackSkill, base_id + 1);  // 2張攻擊lv2
    }
    pushbackVector(&player->attackSkill, meta_base_id);  // 蛻變牌1
    for (int i = 0; i < 3; i++) {
        pushbackVector(&player->attackSkill, base_id + 2);  // 3張攻擊lv3
    }
    pushbackVector(&player->attackSkill, meta_base_id + 3);  // 蛻變牌4
    
    // 防禦技能牌堆
    for (int i = 0; i < 2; i++) {
        pushbackVector(&player->defenseSkill, base_id + 4);  // 2張防禦lv2
    }
    pushbackVector(&player->defenseSkill, meta_base_id + 1);  // 蛻變牌2
    for (int i = 0; i < 3; i++) {
        pushbackVector(&player->defenseSkill, base_id + 5);  // 3張防禦lv3
    }
    // 特殊處理火柴女孩
    if (player->character == CHARACTER_MATCHGIRL) {
        pushbackVector(&player->defenseSkill, meta_base_id + 4);  // 蛻變牌5
    } else {
        pushbackVector(&player->defenseSkill, meta_base_id + 3);  // 蛻變牌4
    }
    
    // 移動技能牌堆
    for (int i = 0; i < 2; i++) {
        pushbackVector(&player->moveSkill, base_id + 7);  // 2張移動lv2
    }
    pushbackVector(&player->moveSkill, meta_base_id + 2);  // 蛻變牌3
    for (int i = 0; i < 3; i++) {
        pushbackVector(&player->moveSkill, base_id + 8);  // 3張移動lv3
    }
    // 特殊處理火柴女孩
    if (player->character == CHARACTER_MATCHGIRL) {
        pushbackVector(&player->moveSkill, meta_base_id + 5);  // 蛻變牌6
    } else {
        pushbackVector(&player->moveSkill, meta_base_id + 3);  // 蛻變牌4
    }
}

// 設置必殺牌
void setup_ultimate_cards(sPlayer* player) {
    int base_id = (player->character - 1) * 10 + 1;
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
        
        // 設置初始牌堆
        setup_initial_deck(player);
        
        // 設置技能牌堆
        setup_skill_decks(player);
        
        // 設置必殺牌
        setup_ultimate_cards(player);
    }
    
    // 2. 設置基本供應牌庫
    setup_basic_supply_decks();
    
    // 3. 設置初始抽牌
    initial_draw();
    
    // 4. 設置遊戲狀態
    game.now_turn_player_id = 0;  // 玩家1先攻
    game.status = CHOOSE_MOVE;    // 從選擇移動開始

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
}

void init_red_hood(sPlayer* p) {
    p->maxlife = 30;
    p->life = 30;
    p->defense = 0;
    p->maxdefense = 6;
    p->energy = 0;
    p->specialGate = 15;

    // 攻擊技能牌庫
    pushbackVector(&p->attackSkill, 138); // 板載緩存 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->attackSkill, 13); // 致命狙擊 x3
    pushbackVector(&p->attackSkill, 135); // 過載燃燒 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->attackSkill, 12); // 精準射擊 x2
    pushbackVector(&p->attackSkill, 11); // 快速射擊 x1

    // 防禦技能牌庫
    pushbackVector(&p->defenseSkill, 138); // 板載緩存 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->defenseSkill, 16); // 終極護盾 x3
    pushbackVector(&p->defenseSkill, 136); // 兜帽系統 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->defenseSkill, 15); // 電流護盾 x2
    pushbackVector(&p->defenseSkill, 14); // 能量護盾 x1

    // 移動技能牌庫
    pushbackVector(&p->moveSkill, 138); // 板載緩存 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->moveSkill, 19); // 暴怒噴射 x3
    pushbackVector(&p->moveSkill, 137); // 變異感應 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->moveSkill, 18); // 火力噴射 x2
    pushbackVector(&p->moveSkill, 17); // 彈道噴射 x1

    // 必殺牌
    pushbackVector(&p->specialDeck, 20); // 餓狼吞噬
    pushbackVector(&p->specialDeck, 21); // 系統入侵
    pushbackVector(&p->specialDeck, 22); // 復仇之雨

    p->redHood.saveCard[0] = -1;
    p->redHood.saveCard[1] = -1;
    p->redHood.saveCard[2] = -1;

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

// Attack action
void handle_attack(sPlayer* attacker, sPlayer* defender) {
    printf("\nAttack Action:\n");
    print_hand_cards(attacker);
    
    int total_damage = 0;
    int total_energy = 0;
    bool continue_attack = true;
    
    while (continue_attack) {
        printf("\nChoose an attack card (1-%d) or 0 to stop: ", attacker->hand.SIZE);
        int choice;
        scanf("%d", &choice);
        
        if (choice == 0) {
            continue_attack = false;
            continue;
        }
        
        if (choice < 1 || choice > attacker->hand.SIZE) {
            printf("Invalid choice!\n");
            continue;
        }
        
        // Get the selected card
        int32_t card_id = attacker->hand.array[choice - 1];
        const Card* card = getCardData(card_id);
        
        // 檢查是否為攻擊牌或通用牌
        if (card->type != TYPE_ATTACK && card->type != TYPE_BASIC) {
            printf("Not an attack card!\n");
            continue;
        }
        
        // Calculate damage and energy
        int card_value = card->damage;
        total_damage += card_value;
        total_energy += card_value;
        
        // Move card to usecards
        pushbackVector(&attacker->usecards, card_id);
        eraseVector(&attacker->hand, choice - 1);
        
        printf("Added %d damage and energy\n", card_value);
    }
   
    // TODO: Hua Mu-Lan

    if (total_damage > 0) {
        // Apply damage
        if (defender->defense > 0) {
            if (defender->defense >= total_damage) {
                defender->defense -= total_damage;
            } else {
                int remaining_damage = total_damage - defender->defense;
                defender->defense = 0;
                defender->life = (defender->life > remaining_damage) ? 
                               defender->life - remaining_damage : 0;
            }
        } else {
            defender->life = (defender->life > total_damage) ? 
                           defender->life - total_damage : 0;
        }
        
        // Add energy
        attacker->energy += total_energy;
        if (attacker->energy > 25) attacker->energy = 25;
        
        printf("\nAttack Summary:\n");
        printf("Total Damage: %d\n", total_damage);
        printf("Energy Gained: %d\n", total_energy);
        printf("Defender's HP: %d/%d\n", defender->life, defender->maxlife);
        printf("Defender's Defense: %d/%d\n", defender->defense, defender->maxdefense);
    }
}

// Defense action
void handle_defense(sPlayer* player) {
    printf("\nDefense Action:\n");
    print_hand_cards(player);
    
    int total_defense = 0;
    int total_energy = 0;
    bool continue_defense = true;
    
    while (continue_defense) {
        printf("\nChoose a defense card (1-%d) or 0 to stop: ", player->hand.SIZE);
        int choice;
        scanf("%d", &choice);
        
        if (choice == 0) {
            continue_defense = false;
            continue;
        }
        
        if (choice < 1 || choice > player->hand.SIZE) {
            printf("Invalid choice!\n");
            continue;
        }
        
        // Get the selected card
        int32_t card_id = player->hand.array[choice - 1];
        const Card* card = getCardData(card_id);
        
        // 檢查是否為防禦牌或通用牌
        if (card->type != TYPE_DEFENSE && card->type != TYPE_BASIC) {
            printf("Not a defense card!\n");
            continue;
        }
        
        // Calculate defense and energy
        int card_value = card->defense;
        total_defense += card_value;
        total_energy += card_value;
        
        // Move card to usecards
        pushbackVector(&player->usecards, card_id);
        eraseVector(&player->hand, choice - 1);
        
        printf("Added %d defense and energy\n", card_value);
    }
    
    if (total_defense > 0) {
        // Apply defense
        player->defense += total_defense;
        if (player->defense > player->maxdefense) {
            player->defense = player->maxdefense;
        }
        
        // Add energy
        player->energy += total_energy;
        if (player->energy > 25) player->energy = 25;
        
        printf("\nDefense Summary:\n");
        printf("Total Defense Added: %d\n", total_defense);
        printf("Energy Gained: %d\n", total_energy);
        printf("Current Defense: %d/%d\n", player->defense, player->maxdefense);
    }
}

// Move action
void handle_move(sPlayer* player) {
    printf("\nMove Action:\n");
    print_hand_cards(player);
    
    int total_move = 0;
    int total_energy = 0;
    bool continue_move = true;
    
    while (continue_move) {
        printf("\nChoose a move card (1-%d) or 0 to stop: ", player->hand.SIZE);
        int choice;
        scanf("%d", &choice);
        
        if (choice == 0) {
            continue_move = false;
            continue;
        }
        
        if (choice < 1 || choice > player->hand.SIZE) {
            printf("Invalid choice!\n");
            continue;
        }
        
        // Get the selected card
        int32_t card_id = player->hand.array[choice - 1];
        
        if (!is_basic_card(card_id, TYPE_MOVE) && card_id != 10) {
            printf("Not a move card!\n");
            continue;
        }
        
        // Calculate move distance and energy
        int card_value = get_card_value(card_id);
        total_move += card_value;
        total_energy += card_value;
        
        // Move card to usecards
        pushbackVector(&player->usecards, card_id);
        eraseVector(&player->hand, choice - 1);
        
        printf("Added %d move distance and energy\n", card_value);
    }
    
    if (total_move > 0) {
        // Apply movement
    printf("\nChoose direction (1: left, 2: right): ");
        int direction;
        scanf("%d", &direction);
        
        int new_position = player->locate[0];
        if (direction == 1) {
            new_position -= total_move;
        } else {
            new_position += total_move;
        }
        
        // Check boundaries
        if (new_position < 1) new_position = 1;
        if (new_position > 9) new_position = 9;
        
        player->locate[0] = new_position;
        
        // Add energy
        player->energy += total_energy;
        if (player->energy > 25) player->energy = 25;
        
        printf("\nMove Summary:\n");
        printf("Total Move Distance: %d\n", total_move);
        printf("Energy Gained: %d\n", total_energy);
        printf("New Position: %d\n", player->locate[0]);
    }
}

void handle_skills(sPlayer* attacker, sPlayer* defender) {
    printf("\nSkill Action:\n");
    print_hand_cards(attacker);
    
    int total_damage = 0;
    int total_defense = 0;
    int total_move = 0;
    int total_energy = 0;
    
    int choice;
    while (1) {
        printf("\nChoose an skill card (1-%d) or 0 to stop: ", attacker->hand.SIZE);
        
        scanf("%d", &choice);
        if (choice == 0)  return;
        else if (choice < 1 || choice > attacker->hand.SIZE) {
            printf("Invalid choice!\n");
            continue;
        }
        else break; // TODO: check if type is skill
    }
        
    // Get the selected card
    int32_t skill_card_id = attacker->hand.array[choice - 1];
    const Card* skill_card = getCardData(skill_card_id);
        
    //TODO: 檢查是否為技能牌    

    pushbackVector(&attacker->usecards, skill_card_id);
    eraseVector(&attacker->hand, choice - 1);

    int32_t basic_card_id = -1;
    
    while (1) {
        printf("\nChoose an basic card (1-%d) or 0 to stop: ", attacker->hand.SIZE);
        scanf("%d", &choice);
            
        if (choice == 0)  return;
        else if (choice < 1 || choice > attacker->hand.SIZE) {
            printf("Invalid choice!\n");
            continue;
        }
        else {
            basic_card_id = attacker->hand.array[choice - 1];
            const Card* basic_card = getCardData(basic_card_id);
            if (basic_card->type != TYPE_BASIC && basic_card->type != skill_card->type) {
                printf("basic_card->type != skill_card->type\n");
                continue;
            }
            else break;
        }
    }
    const Card* basic_card = getCardData(basic_card_id);

 
    pushbackVector(&attacker->usecards, basic_card_id);
    eraseVector(&attacker->hand, choice - 1);
        
   
    // TODO: Hua Mu-Lan

    switch (skill_card_id) {
        // case SKILL: skill01(skill_card, basic_card->level);
    }

}

// 抽牌函數
void draw_card(sPlayer* player, int count) {
    for (int i = 0; i < count; i++) {
        int32_t card;
        // 先嘗試從牌堆抽牌
        if (getVectorTop(&player->deck, &card) == 0) {
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
                if (getVectorTop(&player->deck, &card) == 0) {
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
     
    
    // 清理階段
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
        printf("9. Exit Game\n");
        printf("Choose action (1-9): ");
        
        int choice;
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: // Focus
                printf("Focus: Choose a card to remove from hand or graveyard\n");
                // TODO: Implement focus logic
                action_phase_end = true;
                break;
                
            case 2: // Attack
                handle_attack(current_player, opponent);
                break;
                
            case 3: // Defense
                handle_defense(current_player);
                break;
                
            case 4: // Move
                handle_move(current_player);
                break;
                
            case 5: // Use Skill
                printf("Use Skill: Choose a skill card to use\n");
                handle_skills(current_player, opponent);
                break;
                
            case 6: // Use Ultimate
                printf("Use Ultimate: Choose an ultimate card to use\n");
                // TODO: Implement ultimate usage
                break;
                
            case 7: // Buy Card
                printf("Buy Card: Choose a card to buy\n");
                // TODO: Implement card buying
                break;
                
            case 8: // End Action Phase
                action_phase_end = true;
                break;
                
            case 9: // Exit Game
                change_state(GAME_MENU);
                return;
        }
    }
    
    // 結束階段
    printf("\n=== End Phase ===\n");
    
    // 1. 將使用過的牌移到棄牌堆
    while (current_player->usecards.SIZE > 0) {
        int32_t card;
        getVectorTop(&current_player->usecards, &card);
        pushbackVector(&current_player->graveyard, card);
        popbackVector(&current_player->usecards);
    }
    printf("Used cards moved to graveyard\n");
    
    // 2. 將手牌移到棄牌堆
    while (current_player->hand.SIZE > 0) {
        int32_t card;
        getVectorTop(&current_player->hand, &card);
        pushbackVector(&current_player->graveyard, card);
        popbackVector(&current_player->hand);
    }
    printf("Hand cards moved to graveyard\n");
    
    // 3. 重置能量
    current_player->energy = 0;
    printf("Energy reset to 0\n");
    // 4. 抽牌
    printf("Draw 6 cards\n");
    draw_card(current_player, 6);
    // 5. 切換玩家
    game.now_turn_player_id = (game.now_turn_player_id + 1) % 2;
    printf("Turn ended. Next player's turn.\n");
    first_render = true;
    waiting_for_input = true;
}
void init_snow_white(sPlayer* p) {
    p->maxlife = 34;
    p->life = 34;
    p->defense = 0;
    p->maxdefense = 6;
    p->energy = 0;
    p->specialGate = 17;

    // 攻擊技能牌庫
    pushbackVector(&p->attackSkill, 142); // 至純之毒 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->attackSkill, 25); // 水晶風暴 x3
    pushbackVector(&p->attackSkill, 139); // 水晶之棺 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->attackSkill, 24); // 水晶漩渦 x2
    pushbackVector(&p->attackSkill, 23); // 水晶碎片 x1

    // 防禦技能牌庫
    pushbackVector(&p->defenseSkill, 142); // 至純之毒 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->defenseSkill, 28); // 玷污的狂歡 x3
    pushbackVector(&p->defenseSkill, 140); // 墮落之劫 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->defenseSkill, 27); // 玷污的盛筵 x2
    pushbackVector(&p->defenseSkill, 26); // 玷污的恩惠 x1

    // 移動技能牌庫
    pushbackVector(&p->moveSkill, 142); // 至純之毒 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->moveSkill, 31); // 破碎的命運 x3
    pushbackVector(&p->moveSkill, 141); // 劇毒之蝕 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->moveSkill, 30); // 破碎的現實 x2
    pushbackVector(&p->moveSkill, 29); // 破碎的幻想 x1

    // 必殺牌
    pushbackVector(&p->specialDeck, 32); // 七蛇之怒
    pushbackVector(&p->specialDeck, 33); // 魔鏡之雨
    pushbackVector(&p->specialDeck, 34); // 醞釀之災

    p->snowWhite.remindPosion = initVector();
}

void init_sleeping_beauty(sPlayer* p) {
    p->maxlife = 42;
    p->life = 42;
    p->defense = 0;
    p->maxdefense = 6;
    p->energy = 0;
    p->specialGate = 21;

    // 攻擊技能牌庫
    pushbackVector(&p->attackSkill, 146); // 強制治療 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->attackSkill, 37); // 心靈狂怒 x3
    pushbackVector(&p->attackSkill, 143); // 放血療法 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->attackSkill, 36); // 心靈之怒 x2
    pushbackVector(&p->attackSkill, 35); // 心靈震顫 x1

    // 防禦技能牌庫
    pushbackVector(&p->defenseSkill, 146); // 強制治療 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->defenseSkill, 40); // 爆裂之魂 x3
    pushbackVector(&p->defenseSkill, 144); // 血祭之禮 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->defenseSkill, 39); // 爆裂之骨 x2
    pushbackVector(&p->defenseSkill, 38); // 爆裂之鎖 x1

    // 移動技能牌庫
    pushbackVector(&p->moveSkill, 146); // 強制治療 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->moveSkill, 43); // 黑暗絞殺 x3
    pushbackVector(&p->moveSkill, 145); // 精神屏障 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->moveSkill, 42); // 黑暗糾纏 x2
    pushbackVector(&p->moveSkill, 41); // 黑暗碰觸 x1

    // 必殺牌
    pushbackVector(&p->specialDeck, 44); // 喚醒沉睡
    pushbackVector(&p->specialDeck, 45); // 白日夢魘
    pushbackVector(&p->specialDeck, 46); // 血脈重鑄

    p->sleepingBeauty.AWAKEN_TOKEN = 0;
    p->sleepingBeauty.AWAKEN = 0;
    p->sleepingBeauty.dayNightmareDrawRemind = 0;
    p->sleepingBeauty.atkRise = 0;
    p->sleepingBeauty.atkRiseTime = 0;
    p->sleepingBeauty.usedmeta1 = 0;
}

void init_alice(sPlayer* p) {
    p->maxlife = 32;
    p->life = 32;
    p->defense = 0;
    p->maxdefense = 6;
    p->energy = 0;
    p->specialGate = 16;

    // 攻擊技能牌庫
    pushbackVector(&p->attackSkill, 150); // 開始我的表演 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->attackSkill, 49); // 操控牌局 x3
    pushbackVector(&p->attackSkill, 147); // 砍掉她的頭 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->attackSkill, 48); // 扭轉牌局 x2
    pushbackVector(&p->attackSkill, 47); // 開啟牌局 x1

    // 防禦技能牌庫
    pushbackVector(&p->defenseSkill, 150); // 開始我的表演 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->defenseSkill, 52); // 帽子戲法 x3
    pushbackVector(&p->defenseSkill, 148); // 仙境降臨 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->defenseSkill, 51); // 精神幻術 x2
    pushbackVector(&p->defenseSkill, 50); // 魔力技巧 x1

    // 移動技能牌庫
    pushbackVector(&p->moveSkill, 150); // 開始我的表演 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->moveSkill, 55); // 詭異的詭異 x3
    pushbackVector(&p->moveSkill, 149); // 我們全是瘋子 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->moveSkill, 54); // 詭異的隱蔽 x2
    pushbackVector(&p->moveSkill, 53); // 詭異的敏捷 x1

    // 必殺牌
    pushbackVector(&p->specialDeck, 56); // 無休止的派對
    pushbackVector(&p->specialDeck, 57); // 精彩的奇妙日
    pushbackVector(&p->specialDeck, 58); // 遊戲盡在掌控

    p->alice.identity = 0;
    p->alice.riseBasic = 0;
    p->alice.restartTurn = 0;
    p->alice.havedrestart = 0;
}

void init_mulan(sPlayer* p) {
    p->maxlife = 34;
    p->life = 34;
    p->defense = 0;
    p->maxdefense = 3;
    p->energy = 0;
    p->specialGate = 17;

    // 攻擊技能牌庫
    pushbackVector(&p->attackSkill, 154); // 暴風前夕 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->attackSkill, 61); // 堅不可摧 x3
    pushbackVector(&p->attackSkill, 151); // 氣慣全身 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->attackSkill, 60); // 勢不可擋 x2
    pushbackVector(&p->attackSkill, 59); // 不容小覷 x1

    // 防禦技能牌庫
    pushbackVector(&p->defenseSkill, 154); // 暴風前夕 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->defenseSkill, 64); // 以弱勝強 x3
    pushbackVector(&p->defenseSkill, 152); // 主宰命運 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->defenseSkill, 63); // 以柔克剛 x2
    pushbackVector(&p->defenseSkill, 62); // 以靜制動 x1

    // 移動技能牌庫
    pushbackVector(&p->moveSkill, 154); // 暴風前夕 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->moveSkill, 67); // 絕不饒恕 x3
    pushbackVector(&p->moveSkill, 153); // 長驅直入 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->moveSkill, 66); // 毫不留情 x2
    pushbackVector(&p->moveSkill, 65); // 永不退縮 x1

    // 必殺牌
    pushbackVector(&p->specialDeck, 68); // 氣沖雲霄
    pushbackVector(&p->specialDeck, 69); // 直面混沌
    pushbackVector(&p->specialDeck, 70); // 雷霆一擊

    p->mulan.KI_TOKEN = 0;
    p->mulan.extraCard = 0;
    p->mulan.extraDraw = 0;
}

void init_kaguya(sPlayer* p) {
    p->maxlife = 32;
    p->life = 32;
    p->defense = 0;
    p->maxdefense = 6;
    p->energy = 0;
    p->specialGate = 16;

    // 攻擊技能牌庫
    pushbackVector(&p->attackSkill, 158); // 月下沉思 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->attackSkill, 73); // 領悟的化身 x3
    pushbackVector(&p->attackSkill, 155); // 懲戒時刻 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->attackSkill, 72); // 領悟的榮耀 x2
    pushbackVector(&p->attackSkill, 71); // 領悟的光芒 x1

    // 防禦技能牌庫
    pushbackVector(&p->defenseSkill, 158); // 月下沉思 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->defenseSkill, 76); // 神性的召換 x3
    pushbackVector(&p->defenseSkill, 156); // 血色月光 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->defenseSkill, 75); // 久遠的回響 x2
    pushbackVector(&p->defenseSkill, 74); // 困惑的回聲 x1

    // 移動技能牌庫
    pushbackVector(&p->moveSkill, 158); // 月下沉思 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->moveSkill, 79); // 痛徹的淨化 x3
    pushbackVector(&p->moveSkill, 157); // 靈性本能 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->moveSkill, 78); // 頓悟的決心 x2
    pushbackVector(&p->moveSkill, 77); // 專注的自省 x1

    // 必殺牌
    pushbackVector(&p->specialDeck, 80); // 炙熱的竹刀
    pushbackVector(&p->specialDeck, 81); // 注定的審判
    pushbackVector(&p->specialDeck, 82); // 躁動的血性

    p->kaguya.useDefenseAsATK = 0;
    p->kaguya.useMoveTarget = 0;
}

void init_mermaid(sPlayer* p) {
    p->maxlife = 36;
    p->life = 36;
    p->defense = 0;
    p->maxdefense = 3;
    p->energy = 0;
    p->specialGate = 18;

    // 攻擊技能牌庫
    pushbackVector(&p->attackSkill, 162); // 暗潮湧動 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->attackSkill, 85); // 海妖的尖嘯 x3
    pushbackVector(&p->attackSkill, 159); // 暴風之蝕 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->attackSkill, 84); // 海妖的歌聲 x2
    pushbackVector(&p->attackSkill, 83); // 海妖的召喚 x1

    // 防禦技能牌庫
    pushbackVector(&p->defenseSkill, 162); // 暗潮湧動 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->defenseSkill, 88); // 復仇之怒 x3
    pushbackVector(&p->defenseSkill, 160); // 神秘共鳴 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->defenseSkill, 87); // 噴薄之怒 x2
    pushbackVector(&p->defenseSkill, 86); // 洶湧之怒 x1

    // 移動技能牌庫
    pushbackVector(&p->moveSkill, 162); // 暗潮湧動 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->moveSkill, 91); // 深淵的征服 x3
    pushbackVector(&p->moveSkill, 161); // 海的女兒 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->moveSkill, 90); // 深淵的入侵 x2
    pushbackVector(&p->moveSkill, 89); // 深淵的蠶食 x1

    // 必殺牌
    pushbackVector(&p->specialDeck, 92); // 人魚復興
    pushbackVector(&p->specialDeck, 93); // 遠古甦醒
    pushbackVector(&p->specialDeck, 94); // 淨化之潮

}

void init_match_girl(sPlayer* p) {
    p->maxlife = 36;
    p->life = 36;
    p->defense = 0;
    p->maxdefense = 6;
    p->energy = 0;
    p->specialGate = 18;

    // 攻擊技能牌庫
    pushbackVector(&p->attackSkill, 166); // 火焰的捉弄 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->attackSkill, 97); // 無厭的奢望 x3
    pushbackVector(&p->attackSkill, 163); // 痛苦的儀式 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->attackSkill, 96); // 隱密的期望 x2
    pushbackVector(&p->attackSkill, 95); // 虛幻的願望 x1

    // 防禦技能牌庫
    pushbackVector(&p->defenseSkill, 167); // 欲望的捉弄 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->defenseSkill, 100); // 惡魔的契約 x3
    pushbackVector(&p->defenseSkill, 164); // 放縱的渴望 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->defenseSkill, 99); // 惡魔的賭注 x2
    pushbackVector(&p->defenseSkill, 98); // 惡魔的祭品 x1

    // 移動技能牌庫
    pushbackVector(&p->moveSkill, 168); // 命運的捉弄 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->moveSkill, 103); // 殘破的靈魂 x3
    pushbackVector(&p->moveSkill, 165); // 魔鬼的凝視 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->moveSkill, 102); // 虧欠的靈魂 x2
    pushbackVector(&p->moveSkill, 101); // 失重的靈魂 x1

    // 必殺牌
    pushbackVector(&p->specialDeck, 104); // 地獄烈焰
    pushbackVector(&p->specialDeck, 105); // 厄運降臨
    pushbackVector(&p->specialDeck, 106); // 貪婪詛咒

    p->matchGirl.remindMatch = 12;
    p->matchGirl.pushedMatch = 0;
}

void init_dorothy(sPlayer* p) {
    p->maxlife = 40;
    p->life = 40;
    p->defense = 0;
    p->maxdefense = 6;
    p->energy = 0;
    p->specialGate = 20;

    // 攻擊技能牌庫
    pushbackVector(&p->attackSkill, 172); // 無所遁形 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->attackSkill, 109); // 目標清除 x3
    pushbackVector(&p->attackSkill, 169); // 殺戮指令 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->attackSkill, 108); // 目標鎖定 x2
    pushbackVector(&p->attackSkill, 107); // 目標確認 x1

    // 防禦技能牌庫
    pushbackVector(&p->defenseSkill, 172); // 無所遁形 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->defenseSkill, 112); // 讀取完畢 x3
    pushbackVector(&p->defenseSkill, 170); // 超越機器 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->defenseSkill, 111); // 深度搜索 x2
    pushbackVector(&p->defenseSkill, 110); // 思想刺探 x1

    // 移動技能牌庫
    pushbackVector(&p->moveSkill, 172); // 無所遁形 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->moveSkill, 115); // 使命終結 x3
    pushbackVector(&p->moveSkill, 171); // 獲准極刑 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->moveSkill, 114); // 進入視野 x2
    pushbackVector(&p->moveSkill, 113); // 發現敵蹤 x1

    // 必殺牌
    pushbackVector(&p->specialDeck, 116); // 獅子
    pushbackVector(&p->specialDeck, 117); // 鐵皮人
    pushbackVector(&p->specialDeck, 118); // 稻草人

    p->dorothy.COMBO_TOKEN = 0;
    p->dorothy.canCombo = 0;
}

void init_scheherazade(sPlayer* p) {
    p->maxlife = 36;
    p->life = 36;
    p->defense = 0;
    p->maxdefense = 6;
    p->energy = 0;
    p->specialGate = 18;

    // 攻擊技能牌庫
    pushbackVector(&p->attackSkill, 176); // 童話編織者 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->attackSkill, 121); // 扼殺存在 x3
    pushbackVector(&p->attackSkill, 173); // 命運之手 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->attackSkill, 120); // 銷毀記憶 x2
    pushbackVector(&p->attackSkill, 119); // 消除夢境 x1

    // 防禦技能牌庫
    pushbackVector(&p->defenseSkill, 176); // 童話編織者 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->defenseSkill, 124); // 消融之網 x3
    pushbackVector(&p->defenseSkill, 174); // 改寫欲望 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->defenseSkill, 123); // 沉迷之網 x2
    pushbackVector(&p->defenseSkill, 122); // 浸沒之網 x1

    // 移動技能牌庫
    pushbackVector(&p->moveSkill, 176); // 童話編織者 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->moveSkill, 127); // 支配之腦 x3
    pushbackVector(&p->moveSkill, 175); // 重組思想 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->moveSkill, 126); // 操縱之手 x2
    pushbackVector(&p->moveSkill, 125); // 監視之眼 x1

    // 必殺牌
    pushbackVector(&p->specialDeck, 128); // 系統刪除
    pushbackVector(&p->specialDeck, 129); // 無法自拔
    pushbackVector(&p->specialDeck, 130); // 切斷通路

    p->scheherazade.destiny_TOKEN_locate = initVector();
    p->scheherazade.destiny_TOKEN_type = initVector();
    p->scheherazade.selectToken = 0;
}
