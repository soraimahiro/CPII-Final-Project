#include "game.h"
<<<<<<< Updated upstream:game.c
#include "ui.h"
#include "card_id.h"
=======
#include "card.h"
#include "stage.h"
#include "ui.h"
#include "ui_stage.h"
#include "ui_component.h"
#include "architecture.h"
>>>>>>> Stashed changes:src/game.c

void init_game(game* pGame) {
    int8_t mode;
    get_init_info(&mode);
    pGame->playerMode = mode;

    pGame->now_turn_player_id = 0;

<<<<<<< Updated upstream:game.c
=======
// 設置初始牌堆
void setup_initial_deck(sPlayer* player) {
    clearVector(&player->deck);
    // 添加基本牌
    for (int i = 0; i < 3; i++) {
        pushbackVector(&player->deck, 1);  // 基本攻擊lv1
        pushbackVector(&player->deck, 2);  // 基本防禦lv1
        pushbackVector(&player->deck, 3);  // 基本移動lv1
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
    // 玩家1抽4張
    for (int i = 0; i < 4; i++) {
        int32_t card;
        if (getVectorTop(&game.players[0].deck, &card) == 0) {
            pushbackVector(&game.players[0].hand, card);
            popbackVector(&game.players[0].deck);
        } else {
            break;  // 牌堆空了就停止
        }
    }
    
    // 玩家2抽6張
    for (int i = 0; i < 6; i++) {
        int32_t card;
        if (getVectorTop(&game.players[1].deck, &card) == 0) {
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
>>>>>>> Stashed changes:src/game.c
}

void init_character(player* p){
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

void init_red_hood(player* p) {
    p->maxlife = 30;
    p->life = 30;
    p->defense = 0;
    p->maxdefense = 6;
    p->energy = 3;
    p->specialGate = 15;

    int i = 0;
    for (i = 11; i <= 13; i++) pushbackVector(&p->attackSkill, i);
    for (i = 14; i <= 16; i++) pushbackVector(&p->defenseSkill, i);
    for (i = 17; i <= 19; i++) pushbackVector(&p->moveSkill, i);
    for (i = 20; i <= 22; i++) pushbackVector(&p->specialDeck, i);
    for (i = 135; i <= 138; i++) pushbackVector(&p->metamorphosis, i);


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
        
        if (!is_basic_card(card_id, TYPE_ATTACK) && card_id != 10) {
            printf("Not an attack card!\n");
            continue;
        }
        
        // Calculate damage and energy
        int card_value = get_card_value(card_id);
        total_damage += card_value;
        total_energy += card_value;
        
        // Move card to usecards
        pushbackVector(&attacker->usecards, card_id);
        eraseVector(&attacker->hand, choice - 1);
        
        printf("Added %d damage and energy\n", card_value);
    }
    
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
        
        if (!is_basic_card(card_id, TYPE_DEFENSE) && card_id != 10) {
            printf("Not a defense card!\n");
            continue;
        }
        
        // Calculate defense and energy
        int card_value = get_card_value(card_id);
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
    // 只有在沒有手牌時才抽牌
    if (current_player->hand.SIZE == 0) {
        printf("Draw 6 cards\n");
        draw_card(current_player, 6);
    } else {
        printf("Already have cards in hand\n");
    }
    
    // 清理階段
    printf("\n=== Cleanup Phase ===\n");
    printf("Reset energy to 0\n");
    current_player->energy = 0;
    
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
                // TODO: Implement skill usage
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
    
    // 回合結束階段
    printf("\n=== End Phase ===\n");
    
    // 1. 將使用過的牌移到棄牌堆
    while (current_player->usecards.SIZE > 0) {
        int32_t card;
        getVectorTop(&current_player->usecards, &card);
        pushbackVector(&current_player->graveyard, card);
        popbackVector(&current_player->usecards);
    }
    printf("Used cards moved to graveyard\n");
    
    // 2. 重置能量
    current_player->energy = 0;
    printf("Energy reset to 0\n");
    
    // 3. 切換玩家
    game.now_turn_player_id = (game.now_turn_player_id + 1) % 2;
    printf("Turn ended. Next player's turn.\n");
    
    // 4. 重置遊戲狀態
    game.status = CHOOSE_MOVE;
    
    // 5. 重置靜態變數，準備下一個回合
    first_render = true;
    waiting_for_input = true;
}
