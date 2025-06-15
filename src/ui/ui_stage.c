#include "ui_stage.h"
#include "../card.h"

extern sGame game;
extern sUiBase uiBase;

// Global battle UI instance
sBattleUIStage battleUIStage = {0};

// Color definitions
static const SDL_Color white = {255, 255, 255, 255};
static const SDL_Color black = {0, 0, 0, 255};
static const SDL_Color gray1 = {32, 32, 32, 255};
static const SDL_Color gray2 = {64, 64, 64, 255};
static const SDL_Color lightblue1 = {160, 245, 255, 255};
static const SDL_Color lightblue2 = {120, 190, 200, 255};
static const SDL_Color green = {0, 200, 0, 255};
static const SDL_Color red = {200, 0, 0, 255};

// 函数声明
static const char* getCardTypeName(CardType type);
static void draw_card_detail_popup(const Card* cardData);

// 卡牌区域按钮管理函数（前置定义）
// 初始化卡牌区域按钮
static void init_card_area_buttons(sCardAreaButtons* areaButtons, const char* areaName, SDL_Rect areaRect) {
    strncpy(areaButtons->areaName, areaName, sizeof(areaButtons->areaName) - 1);
    areaButtons->areaName[sizeof(areaButtons->areaName) - 1] = '\0';
    areaButtons->areaRect = areaRect;
    areaButtons->enabled = true;
    areaButtons->buttonCount = 0;
    areaButtons->buttons = NULL;
    
    // 为手牌区域创建按钮
    if (strcmp(areaName, "手牌") == 0) {
        // 清理旧的按钮
        if (areaButtons->buttons) {
            for (int32_t i = 0; i < areaButtons->buttonCount; i++) {
                if (areaButtons->buttons[i]) {
                    free_button(areaButtons->buttons[i]);
                }
            }
            free(areaButtons->buttons);
        }
        
        // 为每张手牌创建一个详细信息按钮
        int32_t current_player = game.now_turn_player_id;
        areaButtons->buttonCount = game.players[current_player].hand.SIZE + 1; // +1 为整理按钮
        areaButtons->buttons = calloc(areaButtons->buttonCount, sizeof(sButton*));
        
        SDL_Color textColors[] = {white, white, black, black};
        SDL_Color bgColors[] = {gray1, gray2, lightblue1, lightblue2};
        SDL_Color borderColors[] = {white, white, white, white};
        
        // 创建整理按钮
        SDL_Rect sortButtonRect = {
            areaRect.x + areaRect.w - 80,  // 距离右边80像素
            areaRect.y + 5,                // 距离上边5像素
            70,                            // 宽度
            25                             // 高度
        };
        areaButtons->buttons[0] = create_button(sortButtonRect, "整理", textColors, bgColors, borderColors, 14, 2);
        
        // 為每張手牌創建按鈕
        int32_t maxCardsPerRow = areaRect.w / 150; // 每行最多顯示的卡牌數
        if(maxCardsPerRow < 1) maxCardsPerRow = 1;
        
        int32_t lineHeight = 15;
        int32_t currentRow = 0;
        int32_t currentCol = 0;
        
        for(int32_t i = 0; i < game.players[current_player].hand.SIZE; i++) {
            const Card* cardData = getCardData(game.players[current_player].hand.array[i]);
            if(cardData) {
                // 計算按鈕位置
                int32_t buttonX = areaRect.x + 10 + currentCol * 150;
                int32_t buttonY = areaRect.y + 25 + currentRow * lineHeight;
                
                // 檢查是否需要換行
                if(buttonX + 140 > areaRect.x + areaRect.w) {
                    currentRow++;
                    currentCol = 0;
                    buttonX = areaRect.x + 10;
                    buttonY = areaRect.y + 25 + currentRow * lineHeight;
                }
                
                // 創建按鈕
                SDL_Rect cardButtonRect = {
                    buttonX,
                    buttonY,
                    140,  // 寬度
                    15    // 高度
                };
                areaButtons->buttons[i + 1] = create_button(cardButtonRect, cardData->name, textColors, bgColors, borderColors, 12, 1);
                
                currentCol++;
            }
        }
    }
    else if (strcmp(areaName, "反轉牌") == 0) {
        // 清理旧的按钮
        if (areaButtons->buttons) {
            for (int32_t i = 0; i < areaButtons->buttonCount; i++) {
                if (areaButtons->buttons[i]) {
                    free_button(areaButtons->buttons[i]);
                }
            }
            free(areaButtons->buttons);
        }
        
        // 为每张反轉创建一个详细信息按钮
        int32_t current_player = game.now_turn_player_id;
        areaButtons->buttonCount = game.players[current_player].metamorphosis.SIZE + 1; // +1 为整理按钮
        areaButtons->buttons = calloc(areaButtons->buttonCount, sizeof(sButton*));
        
        SDL_Color textColors[] = {white, white, black, black};
        SDL_Color bgColors[] = {gray1, gray2, lightblue1, lightblue2};
        SDL_Color borderColors[] = {white, white, white, white};
        
        // 创建整理按钮
        SDL_Rect sortButtonRect = {
            areaRect.x + areaRect.w - 80,  // 距离右边80像素
            areaRect.y + 5,                // 距离上边5像素
            70,                            // 宽度
            25                             // 高度
        };
        areaButtons->buttons[0] = create_button(sortButtonRect, "整理", textColors, bgColors, borderColors, 14, 2);
        
        // 為每張反轉牌創建按鈕
        int32_t maxCardsPerRow = areaRect.w / 150; // 每行最多顯示的卡牌數
        if(maxCardsPerRow < 1) maxCardsPerRow = 1;
        
        int32_t lineHeight = 15;
        int32_t currentRow = 0;
        int32_t currentCol = 0;
        
        for(int32_t i = 0; i < game.players[current_player].metamorphosis.SIZE; i++) {
            const Card* cardData = getCardData(game.players[current_player].metamorphosis.array[i]);
            if(cardData) {
                // 計算按鈕位置
                int32_t buttonX = areaRect.x + 10 + currentCol * 150;
                int32_t buttonY = areaRect.y + 25 + currentRow * lineHeight;
                
                // 檢查是否需要換行
                if(buttonX + 140 > areaRect.x + areaRect.w) {
                    currentRow++;
                    currentCol = 0;
                    buttonX = areaRect.x + 10;
                    buttonY = areaRect.y + 25 + currentRow * lineHeight;
                }
                
                // 創建按鈕
                SDL_Rect cardButtonRect = {
                    buttonX,
                    buttonY,
                    140,  // 寬度
                    15    // 高度
                };
                areaButtons->buttons[i + 1] = create_button(cardButtonRect, cardData->name, textColors, bgColors, borderColors, 12, 1);
                
                currentCol++;
            }
        }
    }
}

// 清理卡牌區域按鈕
static void cleanup_card_area_buttons(sCardAreaButtons* areaButtons) {
    if (areaButtons->buttons) {
        for (int32_t i = 0; i < areaButtons->buttonCount; i++) {
            if (areaButtons->buttons[i]) {
                free_button(areaButtons->buttons[i]);
                areaButtons->buttons[i] = NULL;
            }
        }
        free(areaButtons->buttons);
        areaButtons->buttons = NULL;
    }
    areaButtons->buttonCount = 0;
    areaButtons->enabled = false;
}

// 繪製卡牌區域按鈕
static void draw_card_area_buttons(sCardAreaButtons* areaButtons) {
    if (!areaButtons->enabled || !areaButtons->buttons) {
        return;
    }
    
    for (int32_t i = 0; i < areaButtons->buttonCount; i++) {
        if (areaButtons->buttons[i]) {
            int8_t buttonType = mouse_in_button(areaButtons->buttons[i]) ? 1 : 0;
            draw_button(areaButtons->buttons[i], buttonType);
        }
    }
}

// 處理卡牌區域按鈕事件
static bool handle_card_area_button_events(sCardAreaButtons* areaButtons, SDL_Event* event) {
    if (!areaButtons->enabled || !areaButtons->buttons || event->type != SDL_MOUSEBUTTONDOWN) {
        return false;
    }
    
    if (event->button.button == SDL_BUTTON_LEFT) {
        for (int32_t i = 0; i < areaButtons->buttonCount; i++) {
            if (areaButtons->buttons[i] && mouse_in_button(areaButtons->buttons[i])) {
                DEBUG_PRINT("Card area button clicked in %s area: button %d\n", areaButtons->areaName, i);
                
                // 如果是卡牌按鈕（i > 0），顯示詳細信息
                if (i > 0) {
                    battleUIStage.showCardDetailPopup = true;
                    battleUIStage.selectedCardIndex = i - 1;
                }
                
                return true;  // 事件已處理
            }
        }
    }
    
    return false;  // 事件未處理
}

void draw_left_column_stage() {
    // Draw background
    SDL_Rect leftRect = {0, 0, LEFT_COLUMN_WIDTH, SCREEN_HEIGHT};
    SDL_SetRenderDrawColor(uiBase.renderer, 40, 40, 40, 255);
    SDL_RenderFillRect(uiBase.renderer, &leftRect);
    
    // Draw opponent info (top)
    int32_t opponent_id = (game.now_turn_player_id + 1) % 2;
    draw_player_info_stage(opponent_id, 10, 20, false);
    
    // Draw battle grid (middle)
    draw_battle_grid_stage();
    
    // Draw current player info (bottom)
    draw_player_info_stage(game.now_turn_player_id, 10, SCREEN_HEIGHT - 120, true);
}

void draw_middle_column_stage() {
    int32_t startX = LEFT_COLUMN_WIDTH;
    int32_t width = MIDDLE_COLUMN_WIDTH;
    
    // Draw background
    SDL_Rect middleRect = {startX, 0, width, SCREEN_HEIGHT};
    SDL_SetRenderDrawColor(uiBase.renderer, 50, 50, 50, 255);
    SDL_RenderFillRect(uiBase.renderer, &middleRect);
    
    int32_t current_player = game.now_turn_player_id;
    int32_t opponent = (current_player + 1) % 2;
    
    // Calculate symmetric areas
    int32_t largeAreaWidth = width / 2 - 20;
    int32_t skillCardWidth = largeAreaWidth / 4;
    
    // Opponent areas (top) - 恢復對手手牌區域顯示
    // Opponent hand cards - 以按鈕方式顯示張數
    SDL_Rect opponentHandRect = {startX + 10, 20, width - 20, 80};
    SDL_SetRenderDrawColor(uiBase.renderer, 70, 70, 70, 255);
    SDL_RenderFillRect(uiBase.renderer, &opponentHandRect);
    SDL_SetRenderDrawColor(uiBase.renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(uiBase.renderer, &opponentHandRect);
    
    // 對方手牌標題和張數
    draw_text("對手手牌", startX + 15, 25, white, 16);
    char opponentHandText[30];
    snprintf(opponentHandText, 30, "(%d張)", game.players[opponent].hand.SIZE);
    draw_text(opponentHandText, startX + width - 80, 25, white, 12);
    
    // 在區域中央顯示"無法查看"
    draw_text_center("無法查看對手手牌", startX + width/2, 60, gray2, 14);
    
    // Opponent skill areas and used cards - positioned symmetrically
    int32_t opponentAreasY = 110; // Position for opponent skill areas
    
    // Create vectors for opponent skill cards
    vector opponentAttackSkills = initVector();
    vector opponentDefenseSkills = initVector();
    vector opponentMoveSkills = initVector();
    
    // Filter opponent's hand and graveyard cards by type
    for(int32_t i = 0; i < game.players[opponent].hand.SIZE; i++) {
        const Card* cardData = getCardData(game.players[opponent].hand.array[i]);
        if(cardData) {
            if(cardData->type == 1) { // Attack skill
                pushbackVector(&opponentAttackSkills, game.players[opponent].attackSkill.array[i]);
            } else if(cardData->type == 2) { // Defense skill
                pushbackVector(&opponentDefenseSkills, game.players[opponent].defenseSkill.array[i]);
            } else if(cardData->type == 3) { // Move skill
                pushbackVector(&opponentMoveSkills, game.players[opponent].moveSkill.array[i]);
            }
        }
        else {
            // If card data is missing, show a placeholder
            draw_text("未知卡牌", startX + 10, opponentAreasY + i * 15, red, 12);
        }
    }

    // Opponent skill areas (4 vertical sections)
    draw_card_area_stage("必殺牌", &game.players[opponent].specialDeck, 
                        startX + 10, opponentAreasY, skillCardWidth, 80, true);
    
    draw_card_area_stage("攻擊技能", &game.players[opponent].attackSkill, 
                        startX + 10 + skillCardWidth, opponentAreasY, skillCardWidth, 80, true);
    
    draw_card_area_stage("防禦技能", &game.players[opponent].defenseSkill, 
                        startX + 10 + skillCardWidth * 2, opponentAreasY, skillCardWidth, 80, true);
    
    draw_card_area_stage("移動技能", &game.players[opponent].moveSkill, 
                        startX + 10 + skillCardWidth * 3, opponentAreasY, skillCardWidth, 80, true);
    
    // Opponent used cards area (right side)
    draw_card_area_stage("反轉牌", &game.players[opponent].metamorphosis, 
                        startX + largeAreaWidth + 30, opponentAreasY, largeAreaWidth, 80, true);
    
    // // Main battle area (center of screen) - enlarged
    // int32_t battleAreaY = 210; // Start after opponent areas with spacing
    // int32_t battleAreaHeight = 280; // Enlarged battle area
    // SDL_Rect battleArea = {startX + 50, battleAreaY, width - 100, battleAreaHeight};
    // SDL_SetRenderDrawColor(uiBase.renderer, 60, 60, 60, 255);
    // SDL_RenderFillRect(uiBase.renderer, &battleArea);
    // SDL_SetRenderDrawColor(uiBase.renderer, 255, 255, 255, 255);
    // SDL_RenderDrawRect(uiBase.renderer, &battleArea);
    
    // draw_text_center("主戰鬥區域", startX + width/2, battleAreaY + battleAreaHeight/2, white, 20);

    //  Main battle area (center of screen)
    int32_t battleAreaY = 210; // Start after opponent areas with spacing
    int32_t battleAreaHeight = 280; // Enlarged battle area
    SDL_Rect battleArea = {startX + 50, battleAreaY, width - 100, battleAreaHeight};
    
    draw_card_area_stage("出牌區", &game.players[current_player].usecards, 
                        startX + 50, battleAreaY, width - 100, battleAreaHeight, true); // Same height as opponent
    
    // My skill areas and used cards - positioned symmetrically to opponent
    int32_t myAreasY = 510; // Position symmetric to opponent areas
    
    // Create vectors for my skill cards
    vector myAttackSkills = initVector();
    vector myDefenseSkills = initVector();
    vector myMoveSkills = initVector();
    
    // Filter current player's hand and graveyard cards by type
    for(int32_t i = 0; i < game.players[current_player].hand.SIZE; i++) {
        const Card* cardData = getCardData(game.players[current_player].hand.array[i]);
        if(cardData) {
            if(cardData->type == 1) { // Attack skill
                pushbackVector(&myAttackSkills, game.players[current_player].attackSkill.array[i]);
            } else if(cardData->type == 2) { // Defense skill
                pushbackVector(&myDefenseSkills, game.players[current_player].defenseSkill.array[i]);
            } else if(cardData->type == 3) { // Move skill
                pushbackVector(&myMoveSkills, game.players[current_player].moveSkill.array[i]);
            }
        }
        else {
            // If card data is missing, show a placeholder
            draw_text("未知卡牌", startX + 10, myAreasY + i * 15, red, 12);
        }
    }
    // Four skill areas from left to right: Ultimate, Attack, Defense, Move
    draw_card_area_stage("必殺牌", &game.players[current_player].specialDeck, 
                        startX + 10, myAreasY, skillCardWidth, 80, true);
    
    // 攻擊技能牌
    SDL_Rect attackRect = {startX + 10 + skillCardWidth, myAreasY, skillCardWidth, 80};
    SDL_SetRenderDrawColor(uiBase.renderer, 80, 80, 80, 255);
    SDL_RenderFillRect(uiBase.renderer, &attackRect);
    SDL_SetRenderDrawColor(uiBase.renderer, 200, 200, 200, 255);
    SDL_RenderDrawRect(uiBase.renderer, &attackRect);
    draw_text("攻擊技能", attackRect.x + 5, attackRect.y + 5, white, 14);
    
    if (game.players[current_player].attackSkill.SIZE > 0) {
        const Card* attackCard = getCardData(game.players[current_player].attackSkill.array[0]);
        if (attackCard) {
            draw_text_center(attackCard->name, attackRect.x + attackRect.w/2, attackRect.y + 30, white, 14);
            char costText[20];
            snprintf(costText, 20, "費用: %d", attackCard->cost);
            draw_text_center(costText, attackRect.x + attackRect.w/2, attackRect.y + 50, lightblue1, 12);
        }
    }
    
    // 防禦技能牌
    SDL_Rect defenseRect = {startX + 10 + skillCardWidth * 2, myAreasY, skillCardWidth, 80};
    SDL_SetRenderDrawColor(uiBase.renderer, 80, 80, 80, 255);
    SDL_RenderFillRect(uiBase.renderer, &defenseRect);
    SDL_SetRenderDrawColor(uiBase.renderer, 200, 200, 200, 255);
    SDL_RenderDrawRect(uiBase.renderer, &defenseRect);
    draw_text("防禦技能", defenseRect.x + 5, defenseRect.y + 5, white, 14);
    
    if (game.players[current_player].defenseSkill.SIZE > 0) {
        const Card* defenseCard = getCardData(game.players[current_player].defenseSkill.array[0]);
        if (defenseCard) {
            draw_text_center(defenseCard->name, defenseRect.x + defenseRect.w/2, defenseRect.y + 30, white, 14);
            char costText[20];
            snprintf(costText, 20, "費用: %d", defenseCard->cost);
            draw_text_center(costText, defenseRect.x + defenseRect.w/2, defenseRect.y + 50, lightblue1, 12);
        }
    }
    
    // 移動技能牌
    SDL_Rect moveRect = {startX + 10 + skillCardWidth * 3, myAreasY, skillCardWidth, 80};
    SDL_SetRenderDrawColor(uiBase.renderer, 80, 80, 80, 255);
    SDL_RenderFillRect(uiBase.renderer, &moveRect);
    SDL_SetRenderDrawColor(uiBase.renderer, 200, 200, 200, 255);
    SDL_RenderDrawRect(uiBase.renderer, &moveRect);
    draw_text("移動技能", moveRect.x + 5, moveRect.y + 5, white, 14);
    
    if (game.players[current_player].moveSkill.SIZE > 0) {
        const Card* moveCard = getCardData(game.players[current_player].moveSkill.array[0]);
        if (moveCard) {
            draw_text_center(moveCard->name, moveRect.x + moveRect.w/2, moveRect.y + 30, white, 14);
            char costText[20];
            snprintf(costText, 20, "費用: %d", moveCard->cost);
            draw_text_center(costText, moveRect.x + moveRect.w/2, moveRect.y + 50, lightblue1, 12);
        }
    }
    
    // My used cards area (right side)
    SDL_Rect metamorphosisRect = {startX + largeAreaWidth + 30, myAreasY, largeAreaWidth, 80};
    draw_card_area_stage("反轉牌", &game.players[current_player].metamorphosis, 
                        metamorphosisRect.x, metamorphosisRect.y, metamorphosisRect.w, metamorphosisRect.h, true);
    
    // My hand cards - symmetric to opponent hand cards
    int32_t handY = 600; // Position symmetric to opponent hand
    SDL_Rect handRect = {startX + 10, handY, width - 20, 80};
    
    draw_card_area_stage("手牌", &game.players[current_player].hand, 
                        startX + 10, handY, width - 20, 80, true); // Same height as opponent
    
    // 初始化手牌區域按鈕和反轉牌區域按鈕（如果還沒初始化）
    if (battleUIStage.handAreaButtons.buttonCount == 0) {
        init_card_area_buttons(&battleUIStage.handAreaButtons, "手牌", handRect);
        
        SDL_Color textColors[] = {white, white};
        SDL_Color bgColors[] = {gray1, gray2};
        SDL_Color borderColors[] = {white, white};
        SDL_Rect useHandRect = {SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 + 210, 200, 50};
        battleUIStage.useHandButton = create_button(useHandRect, "使用此牌", textColors, bgColors, borderColors, 20, 2);
    }
    if (battleUIStage.metamorphosisAreaButtons.buttonCount == 0) {
        init_card_area_buttons(&battleUIStage.metamorphosisAreaButtons, "反轉牌", metamorphosisRect);
    }
    
    // 繪製手牌區域按鈕和反轉牌區域按鈕
    draw_card_area_buttons(&battleUIStage.handAreaButtons);
    draw_card_area_buttons(&battleUIStage.metamorphosisAreaButtons);
}

void init_battle_ui_stage() {
    // Initialize button colors
    SDL_Color textColors[] = {white, white, black, black};
    SDL_Color bgColors[] = {gray1, gray2, lightblue1, lightblue2};
    SDL_Color borderColors[] = {white, white, white, white};
    
    int32_t startX = SCREEN_WIDTH - RIGHT_COLUMN_WIDTH + 10;
    int32_t buttonWidth = RIGHT_COLUMN_WIDTH - 20;
    
    // Calculate relative positions
    int32_t centerY = SCREEN_HEIGHT / 2;
    int32_t topSectionHeight = centerY - 40;
    int32_t bottomSectionHeight = SCREEN_HEIGHT - centerY - 40;
    
    // Top section button spacing and positions - 調整為6個區塊（移除對手手牌按鈕）
    int32_t topButtonSpacing = topSectionHeight / 6;
    int32_t topStartY = topButtonSpacing;
    
    // Settings button (top)
    SDL_Rect settingsRect = {startX, topStartY, buttonWidth, 30};
    battleUIStage.settingsButton = create_button(settingsRect, "設定",
                                                 textColors, bgColors, borderColors, 16, 2);
    
    // Opponent deck button
    SDL_Rect opponentDeckRect = {startX, topStartY + topButtonSpacing, buttonWidth, 30};
    battleUIStage.opponentDeckButton = create_button(opponentDeckRect, "對手牌庫",
                                                    textColors, bgColors, borderColors, 16, 2);
    
    // Opponent graveyard button
    SDL_Rect opponentGraveRect = {startX, topStartY + topButtonSpacing * 2, buttonWidth, 30};
    battleUIStage.opponentGraveButton = create_button(opponentGraveRect, "對手棄牌區",
                                                     textColors, bgColors, borderColors, 16, 2);
    
    // Shop button (center) - vertically centered
    SDL_Rect shopRect = {startX, centerY - 15, buttonWidth, 30};
    battleUIStage.shopButton = create_button(shopRect, "基礎牌商店",
                                            textColors, bgColors, borderColors, 16, 2);
    
    // Bottom section button spacing and positions
    int32_t bottomButtonSpacing = bottomSectionHeight / 6;
    int32_t bottomStartY = SCREEN_HEIGHT - bottomButtonSpacing * 4;
    
    // My graveyard button
    SDL_Rect myGraveRect = {startX, bottomStartY, buttonWidth, 30};
    battleUIStage.myGraveButton = create_button(myGraveRect, "我的棄牌區",
                                               textColors, bgColors, borderColors, 16, 2);
    
    // My deck button
    SDL_Rect myDeckRect = {startX, bottomStartY + bottomButtonSpacing, buttonWidth, 30};
    battleUIStage.myDeckButton = create_button(myDeckRect, "我的牌庫",
                                              textColors, bgColors, borderColors, 16, 2);
    
    // Action buttons (bottom)
    const char* actionTexts[] = {"專注", "結束回合"};
    for(int32_t i = 0; i < 2; i++) {
        SDL_Rect rect = {startX, bottomStartY + bottomButtonSpacing * (2 + i), buttonWidth, 30};
        battleUIStage.actionButtons[i] = create_button(rect, (char*)actionTexts[i], 
                                                      textColors, bgColors, borderColors, 18, 2);
    }
    
    // Remove metamorphosis buttons (now display areas only)
    battleUIStage.opponentMetamorphButton = NULL;
    battleUIStage.myMetamorphButton = NULL;
    
    // 初始化對手手牌按鈕為 NULL（不再使用）
    battleUIStage.opponentHandButton = NULL;
    
    // Initialize card buttons (will be created dynamically)
    for(int32_t i = 0; i < 32; i++) {
        battleUIStage.cardButtons[i] = NULL;
    }
    
    // Reset state
    battleUIStage.selectedCard = -1;
    battleUIStage.selectedArea = 0;
    battleUIStage.showShop = false;
    battleUIStage.waitingForInput = false;
    battleUIStage.focusMode = false;
    battleUIStage.focusSelectedHand = -1;
    battleUIStage.focusSelectedGrave = -1;
    
    // Initialize new popup states
    battleUIStage.showShopPopup = false;
    battleUIStage.showOpponentGravePopup = false;
    battleUIStage.showMyDeckPopup = false;
    battleUIStage.showMyGravePopup = false;
    
    // Initialize skill popup states
    battleUIStage.showOpponentSpecialPopup = false;
    battleUIStage.showOpponentAttackSkillPopup = false;
    battleUIStage.showOpponentDefenseSkillPopup = false;
    battleUIStage.showOpponentMoveSkillPopup = false;
    battleUIStage.showMySpecialPopup = false;
    battleUIStage.showMyAttackSkillPopup = false;
    battleUIStage.showMyDefenseSkillPopup = false;
    battleUIStage.showMyMoveSkillPopup = false;
    
    // Initialize hover states
    battleUIStage.hoveringShop = false;
    battleUIStage.hoveringOpponentDeck = false;    // 恢復懸停狀態初始化
    battleUIStage.hoveringOpponentGrave = false;
    battleUIStage.hoveringMyDeck = false;
    battleUIStage.hoveringMyGrave = false;
    
    // Initialize skill hover states
    battleUIStage.hoveringOpponentSpecial = false;
    battleUIStage.hoveringOpponentAttackSkill = false;
    battleUIStage.hoveringOpponentDefenseSkill = false;
    battleUIStage.hoveringOpponentMoveSkill = false;
    battleUIStage.hoveringMySpecial = false;
    battleUIStage.hoveringMyAttackSkill = false;
    battleUIStage.hoveringMyDefenseSkill = false;
    battleUIStage.hoveringMyMoveSkill = false;
}

void draw_right_column_stage() {
    int32_t startX = SCREEN_WIDTH - RIGHT_COLUMN_WIDTH;
    
    // Draw background - same color as middle column
    SDL_Rect rightRect = {startX, 0, RIGHT_COLUMN_WIDTH, SCREEN_HEIGHT};
    SDL_SetRenderDrawColor(uiBase.renderer, 50, 50, 50, 255);
    SDL_RenderFillRect(uiBase.renderer, &rightRect);
    
    int32_t current_player = game.now_turn_player_id;
    int32_t opponent = (current_player + 1) % 2;
    
    // Calculate relative positions
    int32_t centerY = SCREEN_HEIGHT / 2;
    int32_t topSectionHeight = centerY - 40;
    int32_t bottomSectionHeight = SCREEN_HEIGHT - centerY - 40;
    
    // Top section - opponent buttons with proper spacing（調整為3個按鈕）
    int32_t topButtonSpacing = topSectionHeight / 6;
    int32_t topStartY = topButtonSpacing;
    
    // Settings button (top)
    if(battleUIStage.settingsButton) {
        int8_t buttonType = mouse_in_button(battleUIStage.settingsButton) ? 1 : 0;
        draw_button(battleUIStage.settingsButton, buttonType);
    }
    
    // Opponent deck button
    if(battleUIStage.opponentDeckButton) {
        int8_t buttonType = mouse_in_button(battleUIStage.opponentDeckButton) ? 1 : 0;
        draw_button(battleUIStage.opponentDeckButton, buttonType);
    }
    
    // Opponent graveyard button
    if(battleUIStage.opponentGraveButton) {
        int8_t buttonType = mouse_in_button(battleUIStage.opponentGraveButton) ? 1 : 0;
        draw_button(battleUIStage.opponentGraveButton, buttonType);
    }
    
    // Opponent metamorphosis display area - 調整位置與我方蛻變牌相同大小
    // int32_t opponentMetamorphY = topStartY + topButtonSpacing * 3 + 10;
    // int32_t bottomButtonsStartY = SCREEN_HEIGHT - (bottomSectionHeight / 6) * 4 - 10;
    // int32_t shopButtonY = centerY - 15;
    // int32_t myMetamorphY = centerY + 50;
    
    // 對手蛻變牌高度與我方蛻變牌相同
    // int32_t myMetamorphHeight = bottomButtonsStartY - myMetamorphY - 10;
    // int32_t opponentMetamorphHeight = shopButtonY - 30 - opponentMetamorphY;
    
    // 使用相同的高度
    // int32_t metamorphHeight = myMetamorphHeight;
    
    // SDL_Rect opponentMetamorphRect = {startX + 10, opponentMetamorphY, RIGHT_COLUMN_WIDTH - 20, metamorphHeight};
    // SDL_SetRenderDrawColor(uiBase.renderer, 70, 70, 70, 255);
    // SDL_RenderFillRect(uiBase.renderer, &opponentMetamorphRect);
    // SDL_SetRenderDrawColor(uiBase.renderer, 255, 255, 255, 255);
    // SDL_RenderDrawRect(uiBase.renderer, &opponentMetamorphRect);
    
    // draw_text("對手蛻變牌", startX + 15, opponentMetamorphY + 5, white, 16);
    
    // Basic card shop button (center) - vertically centered
    if(battleUIStage.shopButton) {
        int8_t buttonType = mouse_in_button(battleUIStage.shopButton) ? 1 : 0;
        draw_button(battleUIStage.shopButton, buttonType);
    }
    
    // My metamorphosis display area - 與對手蛻變牌相同大小
    // SDL_Rect myMetamorphRect = {startX + 10, myMetamorphY, RIGHT_COLUMN_WIDTH - 20, metamorphHeight};
    // SDL_SetRenderDrawColor(uiBase.renderer, 70, 70, 70, 255);
    // SDL_RenderFillRect(uiBase.renderer, &myMetamorphRect);
    // SDL_SetRenderDrawColor(uiBase.renderer, 255, 255, 255, 255);
    // SDL_RenderDrawRect(uiBase.renderer, &myMetamorphRect);
    
    // draw_text("我的蛻變牌", startX + 15, myMetamorphY + 5, white, 16);
    
    // Bottom section - my buttons with proper spacing
    int32_t bottomButtonSpacing = bottomSectionHeight / 6;
    
    // My graveyard button
    if(battleUIStage.myGraveButton) {
        int8_t buttonType = mouse_in_button(battleUIStage.myGraveButton) ? 1 : 0;
        draw_button(battleUIStage.myGraveButton, buttonType);
    }
    
    // My deck button
    if(battleUIStage.myDeckButton) {
        int8_t buttonType = mouse_in_button(battleUIStage.myDeckButton) ? 1 : 0;
        draw_button(battleUIStage.myDeckButton, buttonType);
    }
    
    // Action buttons (bottom)
    for(int32_t i = 0; i < 2; i++) {
        if(battleUIStage.actionButtons[i]) {
            int8_t buttonType = mouse_in_button(battleUIStage.actionButtons[i]) ? 1 : 0;
            draw_button(battleUIStage.actionButtons[i], buttonType);
        }
    }
}

void cleanup_battle_ui_stage() {
    // Free action buttons (only 2 now)
    for(int32_t i = 0; i < 2; i++) {
        if(battleUIStage.actionButtons[i]) {
            free_button(battleUIStage.actionButtons[i]);
            battleUIStage.actionButtons[i] = NULL;
        }
    }
    
    // Free other buttons
    if(battleUIStage.settingsButton) {
        free_button(battleUIStage.settingsButton);
        battleUIStage.settingsButton = NULL;
    }
    
    if(battleUIStage.shopButton) {
        free_button(battleUIStage.shopButton);
        battleUIStage.shopButton = NULL;
    }
    
    if(battleUIStage.opponentHandButton) {
        free_button(battleUIStage.opponentHandButton);
        battleUIStage.opponentHandButton = NULL;
    }
    
    if(battleUIStage.opponentDeckButton) {
        free_button(battleUIStage.opponentDeckButton);
        battleUIStage.opponentDeckButton = NULL;
    }
    
    if(battleUIStage.opponentGraveButton) {
        free_button(battleUIStage.opponentGraveButton);
        battleUIStage.opponentGraveButton = NULL;
    }
    
    // Remove metamorphosis button cleanup since they're now display areas
    battleUIStage.opponentMetamorphButton = NULL;
    battleUIStage.myMetamorphButton = NULL;
    
    if(battleUIStage.myGraveButton) {
        free_button(battleUIStage.myGraveButton);
        battleUIStage.myGraveButton = NULL;
    }
    
    if(battleUIStage.myDeckButton) {
        free_button(battleUIStage.myDeckButton);
        battleUIStage.myDeckButton = NULL;
    }
    
    // Free card buttons
    for(int32_t i = 0; i < 32; i++) {
        if(battleUIStage.cardButtons[i]) {
            free_button(battleUIStage.cardButtons[i]);
            battleUIStage.cardButtons[i] = NULL;
        }
    }
    
    // 清理卡牌區域按鈕
    cleanup_card_area_buttons(&battleUIStage.handAreaButtons);
    cleanup_card_area_buttons(&battleUIStage.opponentHandAreaButtons);
    cleanup_card_area_buttons(&battleUIStage.skillAreaButtons);
    cleanup_card_area_buttons(&battleUIStage.metamorphosisAreaButtons);
}

void draw_battle_grid_stage() {
    int32_t gridX = 20;
    int32_t gridY = BATTLE_GRID_Y;
    int32_t cellSize = GRID_CELL_SIZE;
    
    // Draw 9 vertical cells (45x160px each)
    for(int32_t i = 0; i < 9; i++) {
        SDL_Rect cell = {gridX, gridY + i * cellSize, LEFT_COLUMN_WIDTH - 40, cellSize};
        
        // Check if any player is in this position
        bool hasPlayer = false;
        int32_t playerInCell = -1;
        
        for(int32_t j = 0; j < 2; j++) {
            if(game.players[j].locate[0] == i + 1) {
                hasPlayer = true;
                playerInCell = j;
                break;
            }
        }
        
        if(hasPlayer) {
            // Draw player in cell
            if(playerInCell == game.now_turn_player_id) {
                SDL_SetRenderDrawColor(uiBase.renderer, 0, 150, 0, 255); // Green for current player
            } else {
                SDL_SetRenderDrawColor(uiBase.renderer, 150, 0, 0, 255); // Red for opponent
            }
            SDL_RenderFillRect(uiBase.renderer, &cell);
        } else {
            // Empty cell - gray background
            SDL_SetRenderDrawColor(uiBase.renderer, 80, 80, 80, 255);
            SDL_RenderFillRect(uiBase.renderer, &cell);
        }
        
        // Draw border
        SDL_SetRenderDrawColor(uiBase.renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(uiBase.renderer, &cell);
        
        // Draw position number
        char posText[5];
        snprintf(posText, 5, "%d", i + 1);
        draw_text_center(posText, gridX + (LEFT_COLUMN_WIDTH - 40)/2, 
                        gridY + i * cellSize + cellSize/2, white, 16);
    }
}

void draw_player_info_stage(int32_t player_id, int32_t x, int32_t y, bool is_current) {
    if(game.players[player_id].character == -1) return;
    
    sCharacterInfo charInfo = get_character_info(game.players[player_id].character);
    
    // Background
    SDL_Rect bgRect = {x, y, LEFT_COLUMN_WIDTH - 20, 90};
    if(is_current) {
        SDL_SetRenderDrawColor(uiBase.renderer, 0, 80, 0, 255); // Green for current player
    } else {
        SDL_SetRenderDrawColor(uiBase.renderer, 80, 0, 0, 255); // Red for opponent
    }
    SDL_RenderFillRect(uiBase.renderer, &bgRect);
    SDL_SetRenderDrawColor(uiBase.renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(uiBase.renderer, &bgRect);
    
    // Player name and character - 16px font
    char nameText[50];
    snprintf(nameText, 50, "玩家%d - %s", player_id + 1, charInfo.name);
    draw_text(nameText, x + 5, y + 5, white, 16);
    
    // Health - 12px font
    char healthText[30];
    snprintf(healthText, 30, "HP: %d/%d", 
             game.players[player_id].life, game.players[player_id].maxlife);
    draw_text(healthText, x + 5, y + 25, white, 12);
    
    // Defense - 12px font
    char defenseText[30];
    snprintf(defenseText, 30, "DEF: %d/%d", 
             game.players[player_id].defense, game.players[player_id].maxdefense);
    draw_text(defenseText, x + 5, y + 45, white, 12);
    
    // Energy - 12px font
    char energyText[20];
    snprintf(energyText, 20, "能量: %d/25", game.players[player_id].energy);
    draw_text(energyText, x + 5, y + 65, white, 12);
}

void draw_card_area_stage(const char* title, vector* cards, int32_t x, int32_t y, 
                         int32_t width, int32_t height, bool clickable) {
    // Background
    SDL_Rect areaRect = {x, y, width, height};
    SDL_SetRenderDrawColor(uiBase.renderer, 70, 70, 70, 255);
    SDL_RenderFillRect(uiBase.renderer, &areaRect);
    SDL_SetRenderDrawColor(uiBase.renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(uiBase.renderer, &areaRect);
    
    // Title - 16px font
    draw_text(title, x + 5, y + 5, white, 16);
    
    // Store hover state for clickable areas
    if(clickable) {
        int32_t mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        bool isHovering = (mouseX >= x && mouseX < x + width && 
                          mouseY >= y && mouseY < y + height);
        
        // Update hover states based on title
        if(strcmp(title, "必殺牌") == 0) {
            if(y < SCREEN_HEIGHT/2) {
                battleUIStage.hoveringOpponentSpecial = isHovering;
            } else {
                battleUIStage.hoveringMySpecial = isHovering;
            }
        } else if(strcmp(title, "攻擊技能") == 0) {
            if(y < SCREEN_HEIGHT/2) {
                battleUIStage.hoveringOpponentAttackSkill = isHovering;
            } else {
                battleUIStage.hoveringMyAttackSkill = isHovering;
            }
        } else if(strcmp(title, "防禦技能") == 0) {
            if(y < SCREEN_HEIGHT/2) {
                battleUIStage.hoveringOpponentDefenseSkill = isHovering;
            } else {
                battleUIStage.hoveringMyDefenseSkill = isHovering;
            }
        } else if(strcmp(title, "移動技能") == 0) {
            if(y < SCREEN_HEIGHT/2) {
                battleUIStage.hoveringOpponentMoveSkill = isHovering;
            } else {
                battleUIStage.hoveringMyMoveSkill = isHovering;
            }
        }
    }
    
    if(!clickable || cards->SIZE == 0) return;
    
    int32_t cardY = y + 25;
    
    // 檢查是否為我方手牌（位於下方且標題為"手牌"）
    bool isMyHandCards = (strcmp(title, "手牌") == 0) && (y > SCREEN_HEIGHT/2);
    // 檢查是否為我方反轉牌（位於下方且標題為"反轉牌"）
    bool isMyMetamorphosisCards = (strcmp(title, "反轉牌") == 0) && (y > SCREEN_HEIGHT/2);
    
    if(isMyHandCards) {
        // 获取鼠标位置
        int32_t mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        
        // 我方手牌顯示全部卡牌
        int32_t maxCardsPerRow = width / 150; // 每行最多顯示的卡牌數（假設每張卡需要150px寬度）
        if(maxCardsPerRow < 1) maxCardsPerRow = 1;
        
        int32_t lineHeight = 15;
        int32_t currentRow = 0;
        int32_t currentCol = 0;
        
        for(int32_t i = 0; i < cards->SIZE; i++) {
            const Card* cardData = getCardData(cards->array[i]);
            if(cardData) {
                // 計算卡牌顯示位置
                int32_t cardX = x + 10 + currentCol * 150;
                int32_t cardDisplayY = cardY + currentRow * lineHeight;
                
                // 檢查是否需要換行
                if(cardX + 140 > x + width) {
                    currentRow++;
                    currentCol = 0;
                    cardX = x + 10;
                    cardDisplayY = cardY + currentRow * lineHeight;
                }
                
                // 檢查是否超出區域高度
                if(cardDisplayY + lineHeight > y + height - 5) {
                    // 顯示省略號並停止
                    draw_text("...", x + 10, cardDisplayY, white, 12);
                    break;
                }
                
                // 檢查滑鼠是否懸停在卡牌上
                bool isHovering = (mouseX >= cardX && mouseX <= cardX + 140 &&
                                 mouseY >= cardDisplayY && mouseY <= cardDisplayY + 15);
                
                // 如果懸停，顯示預覽
                if(isHovering) {
                    draw_hover_preview(cardData->name, cards, mouseX, mouseY);
                }
                
                currentCol++;
            }
            else {
                // If card data is missing, show a placeholder
                char cardText[50];
                snprintf(cardText, 50, "未知卡牌");
                
                int32_t cardX = x + 10 + currentCol * 150;
                int32_t cardDisplayY = cardY + currentRow * lineHeight;
                
                if(cardX + 140 > x + width) {
                    currentRow++;
                    currentCol = 0;
                    cardX = x + 10;
                    cardDisplayY = cardY + currentRow * lineHeight;
                }
                
                if(cardDisplayY + lineHeight > y + height - 5) {
                    draw_text("...", x + 10, cardDisplayY, red, 12);
                    break;
                }
                
                draw_text(cardText, cardX, cardDisplayY, red, 12);
                currentCol++;
            }
        }
    }
    else if(isMyMetamorphosisCards) {
        // 獲取滑鼠位置
        int32_t mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        
        // 我方反轉牌顯示全部卡牌
        int32_t maxCardsPerRow = width / 150; // 每行最多顯示的卡牌數（假設每張卡需要150px寬度）
        if(maxCardsPerRow < 1) maxCardsPerRow = 1;
        
        int32_t lineHeight = 15;
        int32_t currentRow = 0;
        int32_t currentCol = 0;
        
        for(int32_t i = 0; i < cards->SIZE; i++) {
            const Card* cardData = getCardData(cards->array[i]);
            // 計算卡牌顯示位置
            int32_t cardX = x + 10 + currentCol * 150;
            int32_t cardDisplayY = cardY + currentRow * lineHeight;
            
            // 檢查是否需要換行
            if(cardX + 140 > x + width) {
                currentRow++;
                currentCol = 0;
                cardX = x + 10;
                cardDisplayY = cardY + currentRow * lineHeight;
            }
            
            // 檢查是否超出區域高度
            if(cardDisplayY + lineHeight > y + height - 5) {
                // 顯示省略號並停止
                draw_text("...", x + 10, cardDisplayY, white, 12);
                break;
            }
            
            if(cardData) {
                // 檢查滑鼠是否懸停在卡牌上
                bool isHovering = (mouseX >= cardX && mouseX <= cardX + 140 &&
                                 mouseY >= cardDisplayY && mouseY <= cardDisplayY + 15);
                
                // 如果懸停，顯示預覽
                if(isHovering) {
                    draw_hover_preview(cardData->name, cards, mouseX, mouseY);
                }
                
                // 顯示卡牌名稱
                draw_text(cardData->name, cardX, cardDisplayY, white, 12);
            } else {
                // 如果卡牌數據缺失，顯示未知卡牌
                draw_text("未知卡牌", cardX, cardDisplayY, red, 12);
            }
            
            currentCol++;
        }
    }
    else {
        // 其他區域保持原來的顯示方式（只顯示前3張）
        int32_t maxCards = 3; // Show only first 3 cards
        if(maxCards > cards->SIZE) maxCards = cards->SIZE;
        
        for(int32_t i = 0; i < maxCards; i++) {
            const Card* cardData = getCardData(cards->array[i]);
            if(cardData) {
                char cardText[150];
                // Display full card name without character limit
                snprintf(cardText, 150, "%d. %s", i + 1, cardData->name);
                
                // Use simple text drawing for now
                draw_text(cardText, x + 10, cardY + i * 15, white, 12);
            }
            else {
                // If card data is missing, show a placeholder
                draw_text("未知卡牌", x + 10, cardY + i * 15, red, 12);
            }
        }
        
        // If more than 3 cards, show "..."
        if(cards->SIZE > 3) {
            draw_text("...", x + 10, cardY + 3 * 15, white, 12);
        }
    }
}

void draw_hover_preview(const char* title, vector* cards, int32_t mouseX, int32_t mouseY) {
    // Small preview window that follows mouse
    int32_t previewWidth = 200;
    int32_t previewHeight = 120;
    
    // Position preview to avoid screen edges
    int32_t previewX = mouseX + 20;
    int32_t previewY = mouseY - 60;
    
    if (previewX + previewWidth > SCREEN_WIDTH) {
        previewX = mouseX - previewWidth - 20;
    }
    if (previewY < 0) {
        previewY = 20;
    }
    if (previewY + previewHeight > SCREEN_HEIGHT) {
        previewY = SCREEN_HEIGHT - previewHeight - 20;
    }
    
    // Draw preview background
    SDL_Rect previewRect = {previewX, previewY, previewWidth, previewHeight};
    SDL_SetRenderDrawColor(uiBase.renderer, 40, 40, 40, 240);
    SDL_RenderFillRect(uiBase.renderer, &previewRect);
    SDL_SetRenderDrawColor(uiBase.renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(uiBase.renderer, &previewRect);
    
    // Draw title
    draw_text(title, previewX + 10, previewY + 10, white, 16);
    
    if (cards) {
        // 移除卡牌數量顯示，改為顯示"詳細內容"
        draw_text("詳細內容", previewX + 10, previewY + 35, lightblue1, 14);
        
        // Show first few cards
        int32_t maxShow = 3;
        for (int32_t i = 0; i < maxShow && i < cards->SIZE; i++) {
            const Card* cardData = getCardData(cards->array[i]);
            if (cardData) {
                draw_text(cardData->name, previewX + 10, previewY + 60 + i * 18, white, 12);
            }
        }
        
        if (cards->SIZE > maxShow) {
            draw_text("...", previewX + 10, previewY + 60 + maxShow * 18, gray2, 12);
        }
    } else {
        // 對於對手牌庫或商店等特殊情況
        if (strcmp(title, "對手牌庫") == 0) {
            draw_text("無法查看對手牌庫", previewX + 10, previewY + 35, gray2, 14);
        } else if (strcmp(title, "對手手牌") == 0) {
            draw_text("無法查看對手手牌", previewX + 10, previewY + 35, gray2, 14);
        } else if (strcmp(title, "基礎牌商店") == 0) {
            draw_text("基本攻擊/防禦/移動牌", previewX + 10, previewY + 35, lightblue1, 14);
            draw_text("點擊查看詳細內容", previewX + 10, previewY + 60, white, 12);
        }
    }
}

void handle_battle_events_stage(SDL_Event* event) {
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        int32_t mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        
        // 處理棄牌堆點擊
        if (battleUIStage.opponentGraveButton && mouse_in_button(battleUIStage.opponentGraveButton)) {
            battleUIStage.showOpponentGravePopup = true;
            return;
        }
        if (battleUIStage.myGraveButton && mouse_in_button(battleUIStage.myGraveButton)) {
            battleUIStage.showMyGravePopup = true;
            return;
        }
        
        // 處理棄牌堆彈窗關閉
        if (battleUIStage.showOpponentGravePopup || battleUIStage.showMyGravePopup) {
            SDL_Rect closeButtonRect = {
                SCREEN_WIDTH/2 + 310,
                SCREEN_HEIGHT/2 - 240,
                30, 30
            };
            
            if (mouseX >= closeButtonRect.x && mouseX < closeButtonRect.x + closeButtonRect.w &&
                mouseY >= closeButtonRect.y && mouseY < closeButtonRect.y + closeButtonRect.h) {
                battleUIStage.showOpponentGravePopup = false;
                battleUIStage.showMyGravePopup = false;
                return;
            }
        }
        
        // 處理技能牌商店點擊
        if (battleUIStage.showMyAttackSkillPopup || 
            battleUIStage.showMyDefenseSkillPopup || 
            battleUIStage.showMyMoveSkillPopup) {
            
            // 檢查是否點擊關閉按鈕
            SDL_Rect closeButtonRect = {
                SCREEN_WIDTH/2 + 310,
                SCREEN_HEIGHT/2 - 240,
                30, 30
            };
            
            if (mouseX >= closeButtonRect.x && mouseX < closeButtonRect.x + closeButtonRect.w &&
                mouseY >= closeButtonRect.y && mouseY < closeButtonRect.y + closeButtonRect.h) {
                battleUIStage.showMyAttackSkillPopup = false;
                battleUIStage.showMyDefenseSkillPopup = false;
                battleUIStage.showMyMoveSkillPopup = false;
                return;
            }
            
            // 檢查是否點擊購買按鈕
            int32_t startY = SCREEN_HEIGHT/2 - 170;
            int32_t cardHeight = 100;
            int32_t cardWidth = 200;
            int32_t cardsPerRow = 3;
            int32_t padding = 20;
            
            // 獲取對應的技能牌vector
            vector* skillCards;
            if (battleUIStage.showMyAttackSkillPopup) {
                skillCards = &game.players[game.now_turn_player_id].attackSkill;
            } else if (battleUIStage.showMyDefenseSkillPopup) {
                skillCards = &game.players[game.now_turn_player_id].defenseSkill;
            } else {
                skillCards = &game.players[game.now_turn_player_id].moveSkill;
            }
            
            int32_t validCardCount = 0;
            for (int32_t i = 0; i < skillCards->SIZE; i++) {
                const Card* cardData = getCardData(skillCards->array[i]);
                if (cardData) {
                    int32_t row = validCardCount / cardsPerRow;
                    int32_t col = validCardCount % cardsPerRow;
                    
                    int32_t cardX = SCREEN_WIDTH/2 - 340 + padding + col * (cardWidth + padding);
                    int32_t cardY = startY + row * (cardHeight + padding);
                    
                    // 整個卡片作為按鈕
                    SDL_Rect cardRect = {cardX, cardY, cardWidth, cardHeight};
                    
                    if (mouseX >= cardRect.x && mouseX < cardRect.x + cardRect.w &&
                        mouseY >= cardRect.y && mouseY < cardRect.y + cardRect.h) {
                        
                        // 只有第一張牌可以購買
                        if (cardData-> cost != 0 && validCardCount == 0 && game.players[game.now_turn_player_id].energy >= cardData->cost) {
                            // 扣除能量
                            game.players[game.now_turn_player_id].energy -= cardData->cost;
                            
                            // 將卡片加入棄牌堆
                            pushbackVector(&game.players[game.now_turn_player_id].graveyard, skillCards->array[i]);
                            
                            // 將技能牌堆的該張卡片標記為已使用（暗掉）
                            skillCards->array[i] = -1;  // 使用 -1 表示卡片已使用
                            const Card* nextCardData = getCardData(skillCards->array[i + 1]);
                            if (nextCardData->cost == 0 && validCardCount == 0 && game.players[game.now_turn_player_id].energy >= nextCardData->cost) {
                                // 將卡片加入反轉牌區域
                                pushbackVector(&game.players[game.now_turn_player_id].metamorphosis, skillCards->array[i + 1]);
                                DEBUG_PRINT_VEC((&game.players[game.now_turn_player_id].metamorphosis), "metamorphosis");
                                // 將技能牌堆的該張卡片標記為已使用（暗掉）
                                skillCards->array[i + 1] = -1;  // 使用 -1 表示卡片已使用
                            }
                        }
                        return;
                    }
                    validCardCount++;
                }
            }
            return;
        }
        
        // 處理技能牌區域點擊
        int32_t middleStartX = LEFT_COLUMN_WIDTH;
        int32_t width = MIDDLE_COLUMN_WIDTH;
        int32_t largeAreaWidth = width / 2 - 20;
        int32_t skillCardWidth = largeAreaWidth / 4;
        int32_t myAreasY = 510;
        
        SDL_Rect myAttackRect = {middleStartX + 10 + skillCardWidth, myAreasY, skillCardWidth, 80};
        SDL_Rect myDefenseRect = {middleStartX + 10 + skillCardWidth * 2, myAreasY, skillCardWidth, 80};
        SDL_Rect myMoveRect = {middleStartX + 10 + skillCardWidth * 3, myAreasY, skillCardWidth, 80};
        
        if (mouseX >= myAttackRect.x && mouseX < myAttackRect.x + myAttackRect.w &&
            mouseY >= myAttackRect.y && mouseY < myAttackRect.y + myAttackRect.h) {
            battleUIStage.showMyAttackSkillPopup = true;
            return;
        }
        if (mouseX >= myDefenseRect.x && mouseX < myDefenseRect.x + myDefenseRect.w &&
            mouseY >= myDefenseRect.y && mouseY < myDefenseRect.y + myDefenseRect.h) {
            battleUIStage.showMyDefenseSkillPopup = true;
            return;
        }
        if (mouseX >= myMoveRect.x && mouseX < myMoveRect.x + myMoveRect.w &&
            mouseY >= myMoveRect.y && mouseY < myMoveRect.y + myMoveRect.h) {
            battleUIStage.showMyMoveSkillPopup = true;
            return;
        }
    }
    
    if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        int32_t mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        
        // 如果卡片詳細信息彈窗正在顯示，檢查是否點擊了彈窗外部
        if (battleUIStage.showCardDetailPopup) {
            if(mouse_in_button(battleUIStage.useHandButton)){
                DEBUG_PRINT("use hand card\n");
                activation_phase(battleUIStage.selectedCardIndex);
                SDL_Rect handRect = {LEFT_COLUMN_WIDTH + 10, 600, MIDDLE_COLUMN_WIDTH - 20, 80};
                init_card_area_buttons(&battleUIStage.handAreaButtons, "手牌", handRect);
            }
            SDL_Rect dialogRect = {SCREEN_WIDTH/2 - 200, SCREEN_HEIGHT/2 - 200, 400, 400};
            if (mouseX < dialogRect.x || mouseX > dialogRect.x + dialogRect.w ||
                mouseY < dialogRect.y || mouseY > dialogRect.y + dialogRect.h) {
                battleUIStage.showCardDetailPopup = false;
                return;
            }
        }
        
        // 處理卡牌區域按鈕事件（優先處理）
        if (handle_card_area_button_events(&battleUIStage.handAreaButtons, event)) {
            return;  // 事件已被處理，不繼續處理其他事件
        }
        
        // 處理反轉牌區域按鈕事件
        if (handle_card_area_button_events(&battleUIStage.metamorphosisAreaButtons, event)) {
            return;  // 事件已被處理，不繼續處理其他事件
        }
        
        // Handle popup close events first
        bool anyPopupOpen = (battleUIStage.showShopPopup || 
                           battleUIStage.showOpponentGravePopup || battleUIStage.showMyDeckPopup || 
                           battleUIStage.showMyGravePopup || battleUIStage.showOpponentSpecialPopup ||
                           battleUIStage.showOpponentAttackSkillPopup || battleUIStage.showOpponentDefenseSkillPopup ||
                           battleUIStage.showOpponentMoveSkillPopup || battleUIStage.showMySpecialPopup ||
                           battleUIStage.showMyAttackSkillPopup || battleUIStage.showMyDefenseSkillPopup ||
                           battleUIStage.showMyMoveSkillPopup || battleUIStage.showCardDetailPopup);
        
        if(anyPopupOpen) {
            // 檢查是否點擊了彈窗外部
            SDL_Rect dialogRect = {SCREEN_WIDTH/2 - 450, SCREEN_HEIGHT/2 - 250, 900, 500};
            if (mouseX < dialogRect.x || mouseX > dialogRect.x + dialogRect.w ||
                mouseY < dialogRect.y || mouseY > dialogRect.y + dialogRect.h) {
                // 關閉所有彈窗
                battleUIStage.showShopPopup = false;
                battleUIStage.showOpponentGravePopup = false;
                battleUIStage.showMyDeckPopup = false;
                battleUIStage.showMyGravePopup = false;
                battleUIStage.showOpponentSpecialPopup = false;
                battleUIStage.showOpponentAttackSkillPopup = false;
                battleUIStage.showOpponentDefenseSkillPopup = false;
                battleUIStage.showOpponentMoveSkillPopup = false;
                battleUIStage.showMySpecialPopup = false;
                battleUIStage.showMyAttackSkillPopup = false;
                battleUIStage.showMyDefenseSkillPopup = false;
                battleUIStage.showMyMoveSkillPopup = false;
                battleUIStage.showCardDetailPopup = false;
                return;
            }
            
            // 處理商店彈窗中的卡片點擊
            if (battleUIStage.showShopPopup) {
                int32_t colWidth = 200;
                int32_t startX = dialogRect.x + 50;
                int32_t startY = dialogRect.y + 120;
                
                for (int32_t type = 0; type < 4; type++) {
                    int32_t colX = startX + type * colWidth;
                    int32_t maxLevel = (type == 3) ? 1 : 3;
                    
                    for (int32_t level = 1; level <= maxLevel; level++) {
                        int32_t cardY = startY + 40 + (level - 1) * 70;
                        SDL_Rect cardRect = {colX + 10, cardY, colWidth - 20, 60};
                        
                        if (mouseX >= cardRect.x && mouseX <= cardRect.x + cardRect.w &&
                            mouseY >= cardRect.y && mouseY <= cardRect.y + cardRect.h) {
                            
                            int32_t cardId = (type == 3) ? 10 : (type * 3 + level);
                            const Card* cardData = getCardData(cardId);
                            
                            if (cardData) {
                                bool canBuy = false;
                                if (type < 3) {
                                    canBuy = game.basicBuyDeck[type][level-1].SIZE > 0 && 
                                            game.players[game.now_turn_player_id].energy >= cardData->cost;
                                } else {
                                    canBuy = game.basicBuyDeck[3][0].SIZE > 0 && 
                                            game.players[game.now_turn_player_id].energy >= cardData->cost;
                                }
                                
                                if (canBuy) {
                                    if (type < 3) {
                                        buyBasicCard(type, level);
                                    } else {
                                        buyBasicCard(3, 1);
                                    }
                                }
                            }
                            return;
                        }
                    }
                }
            }
        }
        
        // 檢查商店按鈕點擊
        if (battleUIStage.shopButton && mouse_in_button(battleUIStage.shopButton)) {
            battleUIStage.showShopPopup = true;
            return;
        }
        
        // Handle skill area clicks
        int32_t middleStartX = LEFT_COLUMN_WIDTH;
        int32_t width = MIDDLE_COLUMN_WIDTH;
        int32_t largeAreaWidth = width / 2 - 20;
        int32_t skillCardWidth = largeAreaWidth / 4;
        
        // Opponent skill areas
        int32_t opponentAreasY = 110;
        SDL_Rect opponentSpecialRect = {middleStartX + 10, opponentAreasY, skillCardWidth, 80};
        SDL_Rect opponentAttackRect = {middleStartX + 10 + skillCardWidth, opponentAreasY, skillCardWidth, 80};
        SDL_Rect opponentDefenseRect = {middleStartX + 10 + skillCardWidth * 2, opponentAreasY, skillCardWidth, 80};
        SDL_Rect opponentMoveRect = {middleStartX + 10 + skillCardWidth * 3, opponentAreasY, skillCardWidth, 80};
        
        // My skill areas
        int32_t myAreasY = 510;
        SDL_Rect mySpecialRect = {middleStartX + 10, myAreasY, skillCardWidth, 80};
        SDL_Rect myAttackRect = {middleStartX + 10 + skillCardWidth, myAreasY, skillCardWidth, 80};
        SDL_Rect myDefenseRect = {middleStartX + 10 + skillCardWidth * 2, myAreasY, skillCardWidth, 80};
        SDL_Rect myMoveRect = {middleStartX + 10 + skillCardWidth * 3, myAreasY, skillCardWidth, 80};
        
        // Check skill area clicks
        if (mouseX >= opponentSpecialRect.x && mouseX < opponentSpecialRect.x + opponentSpecialRect.w &&
            mouseY >= opponentSpecialRect.y && mouseY < opponentSpecialRect.y + opponentSpecialRect.h) {
            battleUIStage.showOpponentSpecialPopup = true;
            return;
        }
        if (mouseX >= opponentAttackRect.x && mouseX < opponentAttackRect.x + opponentAttackRect.w &&
            mouseY >= opponentAttackRect.y && mouseY < opponentAttackRect.y + opponentAttackRect.h) {
            battleUIStage.showOpponentAttackSkillPopup = true;
            return;
        }
        if (mouseX >= opponentDefenseRect.x && mouseX < opponentDefenseRect.x + opponentDefenseRect.w &&
            mouseY >= opponentDefenseRect.y && mouseY < opponentDefenseRect.y + opponentDefenseRect.h) {
            battleUIStage.showOpponentDefenseSkillPopup = true;
            return;
        }
        if (mouseX >= opponentMoveRect.x && mouseX < opponentMoveRect.x + opponentMoveRect.w &&
            mouseY >= opponentMoveRect.y && mouseY < opponentMoveRect.y + opponentMoveRect.h) {
            battleUIStage.showOpponentMoveSkillPopup = true;
            return;
        }
        if (mouseX >= mySpecialRect.x && mouseX < mySpecialRect.x + mySpecialRect.w &&
            mouseY >= mySpecialRect.y && mouseY < mySpecialRect.y + mySpecialRect.h) {
            battleUIStage.showMySpecialPopup = true;
            return;
        }
        if (mouseX >= myAttackRect.x && mouseX < myAttackRect.x + myAttackRect.w &&
            mouseY >= myAttackRect.y && mouseY < myAttackRect.y + myAttackRect.h) {
            battleUIStage.showMyAttackSkillPopup = true;
            return;
        }
        if (mouseX >= myDefenseRect.x && mouseX < myDefenseRect.x + myDefenseRect.w &&
            mouseY >= myDefenseRect.y && mouseY < myDefenseRect.y + myDefenseRect.h) {
            battleUIStage.showMyDefenseSkillPopup = true;
            return;
        }
        if (mouseX >= myMoveRect.x && mouseX < myMoveRect.x + myMoveRect.w &&
            mouseY >= myMoveRect.y && mouseY < myMoveRect.y + myMoveRect.h) {
            battleUIStage.showMyMoveSkillPopup = true;
            return;
        }
        
        // Handle action button clicks (only 2 buttons now)
        for(int32_t i = 0; i < 2; i++) {
            if(battleUIStage.actionButtons[i] && mouse_in_button(battleUIStage.actionButtons[i])) {
                printf("Action button %d clicked\n", i + 1);
                return;
            }
        }
        
        // Handle other button clicks
        if(battleUIStage.settingsButton && mouse_in_button(battleUIStage.settingsButton)) {
            printf("Settings button clicked\n");
            return;
        }
        
        if(battleUIStage.opponentDeckButton && mouse_in_button(battleUIStage.opponentDeckButton)) {
            printf("Opponent deck button clicked (no action)\n");
            // 不執行任何動作，只是打印訊息
            return;
        }
        
        if(battleUIStage.opponentGraveButton && mouse_in_button(battleUIStage.opponentGraveButton)) {
            printf("Opponent graveyard button clicked\n");
            battleUIStage.showOpponentGravePopup = true;
            return;
        }
        
        if(battleUIStage.myDeckButton && mouse_in_button(battleUIStage.myDeckButton)) {
            printf("My deck button clicked (no action)\n");
            // 我的牌庫按鈕不執行任何動作
            return;
        }
        
        if(battleUIStage.myGraveButton && mouse_in_button(battleUIStage.myGraveButton)) {
            printf("My graveyard button clicked\n");
            battleUIStage.showMyGravePopup = true;
            return;
        }
        
        // Handle metamorphosis area clicks (updated positions with relative coordinates)
        int32_t rightStartX = SCREEN_WIDTH - RIGHT_COLUMN_WIDTH;
        int32_t centerY = SCREEN_HEIGHT / 2;
        int32_t topSectionHeight = centerY - 40;
        int32_t topButtonSpacing = topSectionHeight / 6;
        int32_t topStartY = topButtonSpacing;
        
        // Check opponent metamorphosis area click
        int32_t opponentMetamorphY = topStartY + topButtonSpacing * 4 + 10;
        int32_t opponentMetamorphHeight = centerY - 50 - opponentMetamorphY;
        SDL_Rect opponentMetamorphRect = {rightStartX + 10, opponentMetamorphY, RIGHT_COLUMN_WIDTH - 20, opponentMetamorphHeight};
        if(mouseX >= opponentMetamorphRect.x && mouseX < opponentMetamorphRect.x + opponentMetamorphRect.w &&
           mouseY >= opponentMetamorphRect.y && mouseY < opponentMetamorphRect.y + opponentMetamorphRect.h) {
            printf("Opponent metamorphosis area clicked\n");
            // TODO: Show detailed metamorphosis list
            return;
        }
        
        // Check my metamorphosis area click
        int32_t myMetamorphY = centerY + 50;
        int32_t bottomSectionHeight = SCREEN_HEIGHT - centerY - 40;
        int32_t bottomButtonsStartY = SCREEN_HEIGHT - (bottomSectionHeight / 6) * 4 - 10;
        int32_t myMetamorphHeight = bottomButtonsStartY - myMetamorphY - 10;
        SDL_Rect myMetamorphRect = {rightStartX + 10, myMetamorphY, RIGHT_COLUMN_WIDTH - 20, myMetamorphHeight};
        if(mouseX >= myMetamorphRect.x && mouseX < myMetamorphRect.x + myMetamorphRect.w &&
           mouseY >= myMetamorphRect.y && mouseY < myMetamorphRect.y + myMetamorphRect.h) {
            printf("My metamorphosis area clicked\n");
            // TODO: Show detailed metamorphosis list
            return;
        }
    }
    
    if(event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_ESCAPE) {
        // Close all popups on ESC
        battleUIStage.showShopPopup = false;
        battleUIStage.showOpponentGravePopup = false;
        battleUIStage.showMyDeckPopup = false;
        battleUIStage.showMyGravePopup = false;
        battleUIStage.showOpponentSpecialPopup = false;
        battleUIStage.showOpponentAttackSkillPopup = false;
        battleUIStage.showOpponentDefenseSkillPopup = false;
        battleUIStage.showOpponentMoveSkillPopup = false;
        battleUIStage.showMySpecialPopup = false;
        battleUIStage.showMyAttackSkillPopup = false;
        battleUIStage.showMyDefenseSkillPopup = false;
        battleUIStage.showMyMoveSkillPopup = false;
        battleUIStage.showCardDetailPopup = false;
        
        battleUIStage.focusMode = false;
    }
    
    // 處理手牌區域按鈕事件
    if (battleUIStage.handAreaButtons.enabled && event->type == SDL_MOUSEBUTTONDOWN) {
        int32_t mouseX = event->button.x;
        int32_t mouseY = event->button.y;
        
        // 檢查是否點擊了整理按鈕
        if (battleUIStage.handAreaButtons.buttons[0] && 
            mouse_in_button(battleUIStage.handAreaButtons.buttons[0])) {
            // 處理整理按鈕點擊
            return;
        }
        
        // 檢查是否點擊了卡牌按鈕
        for (int32_t i = 1; i < battleUIStage.handAreaButtons.buttonCount; i++) {
            if (battleUIStage.handAreaButtons.buttons[i] && 
                mouse_in_button(battleUIStage.handAreaButtons.buttons[i])) {
                // 顯示卡牌詳細信息
                battleUIStage.showCardDetailPopup = true;
                battleUIStage.selectedCardIndex = i - 1;
                return;
            }
        }
    }
    
    // 處理反轉牌區域按鈕事件
    if (battleUIStage.metamorphosisAreaButtons.enabled && event->type == SDL_MOUSEBUTTONDOWN) {
        int32_t mouseX = event->button.x;
        int32_t mouseY = event->button.y;
        
        // 檢查是否點擊了整理按鈕
        if (battleUIStage.metamorphosisAreaButtons.buttons[0] && 
            mouse_in_button(battleUIStage.metamorphosisAreaButtons.buttons[0])) {
            // 處理整理按鈕點擊
            return;
        }
        
        // 檢查是否點擊了卡牌按鈕
        for (int32_t i = 1; i < battleUIStage.metamorphosisAreaButtons.buttonCount; i++) {
            if (battleUIStage.metamorphosisAreaButtons.buttons[i] && 
                mouse_in_button(battleUIStage.metamorphosisAreaButtons.buttons[i])) {
                // 顯示卡牌詳細信息
                battleUIStage.showCardDetailPopup = true;
                battleUIStage.selectedCardIndex = i - 1;
                return;
            }
        }
    }
}

void activation_menu(int8_t *active) {
    battleUIStage.waitingForInput = true;
    *active = 0;
    
    while(battleUIStage.waitingForInput) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                *active = 8; // Exit
                battleUIStage.waitingForInput = false;
                return;
            }
            
            if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                // Check action buttons
                for(int32_t i = 0; i < 8; i++) {
                    if(battleUIStage.actionButtons[i] && mouse_in_button(battleUIStage.actionButtons[i])) {
                        *active = i + 1;
                        battleUIStage.waitingForInput = false;
                        return;
                    }
                }
            }
        }
        
        // Draw the battle UI
        draw_battle_ui_stage();
    }
}

void get_active_focus(int32_t *handCard, int32_t *graveCard) {
    battleUIStage.focusMode = true;
    battleUIStage.focusSelectedHand = -1;
    battleUIStage.focusSelectedGrave = -1;
    battleUIStage.waitingForInput = true;
    
    *handCard = -1;
    *graveCard = -1;
    
    while(battleUIStage.waitingForInput) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                battleUIStage.focusMode = false;
                battleUIStage.waitingForInput = false;
                return;
            }
            
            // Handle focus mode card selection
            if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                // For now, use simple number input as fallback
                // TODO: Implement visual card selection
                printf("Click on hand card (0-%d) or graveyard card (%d-%d): ", 
                       game.players[game.now_turn_player_id].hand.SIZE - 1,
                       game.players[game.now_turn_player_id].hand.SIZE,
                       game.players[game.now_turn_player_id].hand.SIZE + game.players[game.now_turn_player_id].graveyard.SIZE - 1);
            }
            
            if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                // Cancel focus mode
                battleUIStage.focusMode = false;
                battleUIStage.waitingForInput = false;
                return;
            }
        }
        
        // Draw the battle UI
        draw_battle_ui_stage();
    }
    
    // Fallback to console input for now
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
        *graveCard = option - game.players[game.now_turn_player_id].hand.SIZE - 1;
    }
    
    battleUIStage.focusMode = false;
}

void draw_deck_popup(const char* title, vector* deck, vector* graveyard) {
    // Semi-transparent overlay
    SDL_SetRenderDrawColor(uiBase.renderer, 0, 0, 0, 128);
    SDL_Rect overlay = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderFillRect(uiBase.renderer, &overlay);
    
    // Main popup window
    SDL_Rect dialogRect = {SCREEN_WIDTH/2 - 350, SCREEN_HEIGHT/2 - 250, 700, 500};
    SDL_SetRenderDrawColor(uiBase.renderer, 64, 64, 64, 255);
    SDL_RenderFillRect(uiBase.renderer, &dialogRect);
    SDL_SetRenderDrawColor(uiBase.renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(uiBase.renderer, &dialogRect);
    
    // Title
    draw_text_center(title, SCREEN_WIDTH/2, dialogRect.y + 30, white, 32);
    
    // Card list area
    int32_t listY = dialogRect.y + 70;
    int32_t listHeight = 350;
    
    if (deck && deck->SIZE > 0) {
        // 移除卡牌數量顯示
        draw_text("內容:", dialogRect.x + 20, listY, lightblue1, 18);
        
        // Scrollable card list
        int32_t maxVisible = 15; // Maximum visible cards
        int32_t cardY = listY + 30;
        
        for (int32_t i = 0; i < maxVisible && i < deck->SIZE; i++) {
            const Card* cardData = getCardData(deck->array[i]);
            if (cardData) {
                char cardText[100];
                snprintf(cardText, 100, "%d. %s", i + 1, cardData->name);
                draw_text(cardText, dialogRect.x + 30, cardY + i * 20, white, 14);
                
                // Show card description on the right
                draw_text(cardData->description, dialogRect.x + 300, cardY + i * 20, gray2, 12);
            }
        }
        
        if (deck->SIZE > maxVisible) {
            char moreText[30];
            snprintf(moreText, 30, "... 還有更多");
            draw_text(moreText, dialogRect.x + 30, cardY + maxVisible * 20, gray2, 14);
        }
    } else {
        draw_text_center("空的", SCREEN_WIDTH/2, listY + 100, gray2, 24);
    }
    
    // Instructions
    draw_text_center("點擊視窗外區域關閉", SCREEN_WIDTH/2, dialogRect.y + dialogRect.h - 30, lightblue1, 18);
}

void draw_battle_ui_stage() {
    // Clear screen with main background color
    SDL_SetRenderDrawColor(uiBase.renderer, 30, 30, 30, 255);
    SDL_RenderClear(uiBase.renderer);
    
    // Update hover states
    battleUIStage.hoveringShop = (battleUIStage.shopButton && mouse_in_button(battleUIStage.shopButton));
    battleUIStage.hoveringOpponentDeck = (battleUIStage.opponentDeckButton && mouse_in_button(battleUIStage.opponentDeckButton));
    battleUIStage.hoveringOpponentGrave = (battleUIStage.opponentGraveButton && mouse_in_button(battleUIStage.opponentGraveButton));
    battleUIStage.hoveringMyGrave = (battleUIStage.myGraveButton && mouse_in_button(battleUIStage.myGraveButton));
    
    // Draw three columns
    draw_left_column_stage();
    draw_middle_column_stage();
    draw_right_column_stage();
    
    // Draw hover previews
    int32_t mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    
    int32_t opponent = (game.now_turn_player_id + 1) % 2;
    
    // 對手牌庫懸停預覽
    if (battleUIStage.hoveringOpponentDeck) {
        draw_hover_preview("對手牌庫", NULL, mouseX, mouseY);
    }
    
    // 棄牌堆懸停預覽
    if (battleUIStage.hoveringOpponentGrave && !battleUIStage.showOpponentGravePopup) {
        draw_hover_preview("對手棄牌區", &game.players[opponent].graveyard, mouseX, mouseY);
    }
    
    if (battleUIStage.hoveringMyGrave && !battleUIStage.showMyGravePopup) {
        draw_hover_preview("我的棄牌區", &game.players[game.now_turn_player_id].graveyard, mouseX, mouseY);
    }
    
    // 技能牌區域懸停預覽
    if (battleUIStage.hoveringMyAttackSkill) {
        draw_hover_preview("攻擊技能商店", &game.players[game.now_turn_player_id].attackSkill, mouseX, mouseY);
    }
    if (battleUIStage.hoveringMyDefenseSkill) {
        draw_hover_preview("防禦技能商店", &game.players[game.now_turn_player_id].defenseSkill, mouseX, mouseY);
    }
    if (battleUIStage.hoveringMyMoveSkill) {
        draw_hover_preview("移動技能商店", &game.players[game.now_turn_player_id].moveSkill, mouseX, mouseY);
    }
    
    // Draw popups
    if (battleUIStage.showShopPopup) {
        draw_shop_popup();
    }
    
    // 棄牌堆彈窗
    if (battleUIStage.showOpponentGravePopup) {
        draw_deck_popup("對手棄牌區", &game.players[opponent].graveyard, NULL);
    }
    if (battleUIStage.showMyGravePopup) {
        draw_deck_popup("我的棄牌區", &game.players[game.now_turn_player_id].graveyard, NULL);
    }
    
    // 技能牌商店彈窗
    if (battleUIStage.showMyAttackSkillPopup) {
        draw_skill_shop_popup("攻擊技能商店", 1);
    }
    if (battleUIStage.showMyDefenseSkillPopup) {
        draw_skill_shop_popup("防禦技能商店", 2);
    }
    if (battleUIStage.showMyMoveSkillPopup) {
        draw_skill_shop_popup("移動技能商店", 3);
    }
    
    // Draw focus mode overlay if active
    if(battleUIStage.focusMode) {
        // Semi-transparent overlay
        SDL_SetRenderDrawColor(uiBase.renderer, 0, 0, 0, 128);
        SDL_Rect overlay = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderFillRect(uiBase.renderer, &overlay);
        
        // Focus instruction - 24px font
        draw_text_center("專注模式：選擇一張牌移除", SCREEN_WIDTH/2, 50, white, 24);
        draw_text_center("點擊手牌或棄牌堆中的牌，再次點擊確認選擇", SCREEN_WIDTH/2, 80, lightblue1, 18);
        draw_text_center("按 ESC 取消", SCREEN_WIDTH/2, 110, gray2, 16);
    }
    
    if(!battleUIStage.showCardDetailPopup){
        // --- 新增：手牌懸停預覽（在所有UI繪製完畢後）---
        int32_t handX = battleUIStage.handAreaButtons.areaRect.x;
        int32_t handY = battleUIStage.handAreaButtons.areaRect.y;
        int32_t handW = battleUIStage.handAreaButtons.areaRect.w;
        int32_t handH = battleUIStage.handAreaButtons.areaRect.h;
        int32_t maxCardsPerRow = handW / 150;
        if(maxCardsPerRow < 1) maxCardsPerRow = 1;
        int32_t lineHeight = 15;
        int32_t currentRow = 0;
        int32_t currentCol = 0;
        for(int32_t i = 0; i < game.players[game.now_turn_player_id].hand.SIZE; i++) {
            int32_t cardX = handX + 10 + currentCol * 150;
            int32_t cardY = handY + 25 + currentRow * lineHeight;
            if(cardX + 140 > handX + handW) {
                currentRow++;
                currentCol = 0;
                cardX = handX + 10;
                cardY = handY + 25 + currentRow * lineHeight;
            }
            if(mouseX >= cardX && mouseX <= cardX + 140 && mouseY >= cardY && mouseY <= cardY + 15) {
                const Card* cardData = getCardData(game.players[game.now_turn_player_id].hand.array[i]);
                if(cardData) {
                    draw_hover_preview(cardData->name, &game.players[game.now_turn_player_id].hand, mouseX, mouseY);
                }
                break;
            }
            currentCol++;
        }

        // --- 新增：反轉牌懸停預覽 ---
        int32_t metamorphX = battleUIStage.metamorphosisAreaButtons.areaRect.x;
        int32_t metamorphY = battleUIStage.metamorphosisAreaButtons.areaRect.y;
        int32_t metamorphW = battleUIStage.metamorphosisAreaButtons.areaRect.w;
        int32_t metamorphH = battleUIStage.metamorphosisAreaButtons.areaRect.h;
        maxCardsPerRow = metamorphW / 150;
        if(maxCardsPerRow < 1) maxCardsPerRow = 1;
        currentRow = 0;
        currentCol = 0;
        for(int32_t i = 0; i < game.players[game.now_turn_player_id].metamorphosis.SIZE; i++) {
            int32_t cardX = metamorphX + 10 + currentCol * 150;
            int32_t cardY = metamorphY + 25 + currentRow * lineHeight;
            if(cardX + 140 > metamorphX + metamorphW) {
                currentRow++;
                currentCol = 0;
                cardX = metamorphX + 10;
                cardY = metamorphY + 25 + currentRow * lineHeight;
            }
            if(mouseX >= cardX && mouseX <= cardX + 140 && mouseY >= cardY && mouseY <= cardY + 15) {
                const Card* cardData = getCardData(game.players[game.now_turn_player_id].metamorphosis.array[i]);
                if(cardData) {
                    draw_hover_preview(cardData->name, &game.players[game.now_turn_player_id].metamorphosis, mouseX, mouseY);
                }
                break;
            }
            currentCol++;
        }
    }
    // 繪製卡片詳細信息彈窗
    if (battleUIStage.showCardDetailPopup) {
        const Card* cardData = getCardData(game.players[game.now_turn_player_id].hand.array[battleUIStage.selectedCardIndex]);
        if (cardData) {
            draw_card_detail_popup(cardData);
        }
    }
    SDL_RenderPresent(uiBase.renderer);
}

void draw_shop_popup() {
    // Semi-transparent overlay
    SDL_SetRenderDrawColor(uiBase.renderer, 0, 0, 0, 128);
    SDL_Rect overlay = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderFillRect(uiBase.renderer, &overlay);
    
    // Main popup window - adjusted size to fit properly
    SDL_Rect dialogRect = {SCREEN_WIDTH/2 - 450, SCREEN_HEIGHT/2 - 250, 900, 500};
    SDL_SetRenderDrawColor(uiBase.renderer, 64, 64, 64, 255);
    SDL_RenderFillRect(uiBase.renderer, &dialogRect);
    SDL_SetRenderDrawColor(uiBase.renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(uiBase.renderer, &dialogRect);
    
    // Title
    draw_text_center("基礎牌商店", SCREEN_WIDTH/2, dialogRect.y + 30, white, 32);
    
    // Show current player's energy
    char energyText[50];
    snprintf(energyText, 50, "我的能量: %d/25", game.players[game.now_turn_player_id].energy);
    draw_text_center(energyText, SCREEN_WIDTH/2, dialogRect.y + 70, lightblue1, 24);
    
    // Draw basic card types in columns - adjusted layout
    int32_t colWidth = 200;
    int32_t startX = dialogRect.x + 50;
    int32_t startY = dialogRect.y + 120;
    
    const char* cardTypes[] = {"攻擊牌", "防禦牌", "移動牌", "通用牌"};
    
    for (int32_t type = 0; type < 4; type++) {
        int32_t colX = startX + type * colWidth;
        
        // Column title
        draw_text_center(cardTypes[type], colX + colWidth/2, startY, lightblue1, 20);
        
        // 通用牌只顯示一種
        int32_t maxLevel = (type == 3) ? 1 : 3;
        
        // Draw cards for each level
        for (int32_t level = 1; level <= maxLevel; level++) {
            int32_t cardY = startY + 40 + (level - 1) * 70;
            
            // Card background
            SDL_Rect cardRect = {colX + 10, cardY, colWidth - 20, 60};
            
            // 檢查是否可以購買
            bool canBuy = false;
            if (type < 3) {
                if (game.basicBuyDeck[type][level-1].SIZE == 0) {
                    canBuy = false;
                }
                else {
                    canBuy = game.players[game.now_turn_player_id].energy >= getCardData((type * 3 + level))->cost;
                }
            } else {
                if (game.basicBuyDeck[3][0].SIZE == 0) {
                    canBuy = false;
                }
                else {
                    canBuy = game.players[game.now_turn_player_id].energy >= getCardData(10)->cost;
                }
            }
            
            // 根據是否可以購買設置不同的背景顏色
            if (canBuy) {
                SDL_SetRenderDrawColor(uiBase.renderer, 80, 80, 80, 255);
            } else {
                SDL_SetRenderDrawColor(uiBase.renderer, 50, 50, 50, 255);
            }
            SDL_RenderFillRect(uiBase.renderer, &cardRect);
            SDL_SetRenderDrawColor(uiBase.renderer, 200, 200, 200, 255);
            SDL_RenderDrawRect(uiBase.renderer, &cardRect);
            
            // Card info
            int32_t cardId = (type == 3) ? 10 : (type * 3 + level);
            const Card* cardData = getCardData(cardId);
            if (cardData) {
                draw_text_center(cardData->name, colX + colWidth/2, cardY + 10, white, 12);
                
                char costText[20];
                snprintf(costText, 20, "費用: %d", cardData->cost);
                draw_text_center(costText, colX + colWidth/2, cardY + 25, lightblue1, 11);
                
                // Show remaining count
                char countText[30];
                if (type < 3) {
                    snprintf(countText, 30, "剩餘: %d張", game.basicBuyDeck[type][level-1].SIZE);
                } else {
                    snprintf(countText, 30, "剩餘: %d張", game.basicBuyDeck[3][0].SIZE);
                }
                draw_text_center(countText, colX + colWidth/2, cardY + 40, lightblue1, 10);
            }
        }
        
        // Show total available count for this type
        if (type < 3) {
            char availText[30];
            int32_t totalCount = game.basicBuyDeck[type][0].SIZE + 
                               game.basicBuyDeck[type][1].SIZE + 
                               game.basicBuyDeck[type][2].SIZE;
            snprintf(availText, 30, "總計: %d張", totalCount);
            draw_text_center(availText, colX + colWidth/2, startY + 250, gray2, 12);
        } else {
            char availText[30];
            snprintf(availText, 30, "總計: %d張", game.basicBuyDeck[3][0].SIZE);
            draw_text_center(availText, colX + colWidth/2, startY + 180, gray2, 12);
        }
    }
    
    // Instructions
    draw_text_center("點擊視窗外區域關閉", SCREEN_WIDTH/2, dialogRect.y + dialogRect.h - 30, lightblue1, 18);
}

// 顯示卡片詳細信息彈窗
static void draw_card_detail_popup(const Card* cardData) {
    // 创建半透明背景
    SDL_SetRenderDrawColor(uiBase.renderer, 0, 0, 0, 128);
    SDL_Rect overlay = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderFillRect(uiBase.renderer, &overlay);
    
    // 创建弹窗背景
    SDL_Rect popupRect = {
        SCREEN_WIDTH/2 - 200,
        SCREEN_HEIGHT/2 - 200,
        400,
        400
    };
    
    // 绘制弹窗背景
    SDL_SetRenderDrawColor(uiBase.renderer, 40, 40, 40, 255);
    SDL_RenderFillRect(uiBase.renderer, &popupRect);
    
    // 绘制弹窗边框
    SDL_SetRenderDrawColor(uiBase.renderer, 100, 100, 100, 255);
    SDL_RenderDrawRect(uiBase.renderer, &popupRect);
    
    // 绘制卡片信息
    int32_t y = popupRect.y + 20;
    draw_text_center(cardData->name, SCREEN_WIDTH/2, y, white, 24);
    y += 40;
    
    // 卡片类型
    const char* typeName = getCardTypeName(cardData->type);
    char typeText[50];
    snprintf(typeText, 50, "類型: %s", typeName);
    draw_text_center(typeText, SCREEN_WIDTH/2, y, lightblue1, 18);
    y += 30;
    
    // 使用费用
    char costText[20];
    snprintf(costText, 20, "使用費用: %d", cardData->cost);
    draw_text_center(costText, SCREEN_WIDTH/2, y, lightblue1, 18);
    y += 30;
    
    // 效果描述
    draw_text_center("效果:", SCREEN_WIDTH/2, y, lightblue1, 18);
    y += 30;
    draw_text_center(cardData->description, SCREEN_WIDTH/2, y, white, 16);
    y += 40;
    
    // 攻击力
    char damageText[20];
    if(cardData->damage > 0) {
        snprintf(damageText, 20, "攻擊力: %d", cardData->damage);
        draw_text_center(damageText, SCREEN_WIDTH/2, y, lightblue1, 18);
        y += 30;
    }
    
    // 防御力
    char defenseText[20];
    if(cardData->defense > 0) {
        snprintf(defenseText, 20, "防禦力: %d", cardData->defense);
        draw_text_center(defenseText, SCREEN_WIDTH/2, y, lightblue1, 18);
        y += 30;
    }
    
    // 移动力
    char rangeText[20];
    if(cardData->range > 0) {
        snprintf(rangeText, 20, "射程: %d", cardData->range);
        draw_text_center(rangeText, SCREEN_WIDTH/2, y, lightblue1, 18);
    }

    int32_t mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    if(mouse_in_button(battleUIStage.useHandButton)){
        draw_button(battleUIStage.useHandButton, 1);
    }
    else{
        draw_button(battleUIStage.useHandButton, 0);
    }
}

// 添加getCardTypeName函数实现
static const char* getCardTypeName(CardType type) {
    switch(type) {
        case TYPE_ATTACK: return "攻擊牌";
        case TYPE_DEFENSE: return "防禦牌";
        case TYPE_MOVE: return "移動牌";
        case TYPE_SPECIAL: return "特殊牌";
        case TYPE_METAMORPH: return "蛻變牌";
        default: return "基本牌";
    }
}

// 新增：技能牌商店彈窗函數
void draw_skill_shop_popup(const char* title, int32_t skillType) {
    // 半透明背景
    SDL_SetRenderDrawColor(uiBase.renderer, 0, 0, 0, 128);
    SDL_Rect overlay = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderFillRect(uiBase.renderer, &overlay);
    
    // 主彈窗
    SDL_Rect dialogRect = {SCREEN_WIDTH/2 - 350, SCREEN_HEIGHT/2 - 250, 700, 500};
    SDL_SetRenderDrawColor(uiBase.renderer, 64, 64, 64, 255);
    SDL_RenderFillRect(uiBase.renderer, &dialogRect);
    SDL_SetRenderDrawColor(uiBase.renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(uiBase.renderer, &dialogRect);
    
    // 標題
    draw_text_center(title, SCREEN_WIDTH/2, dialogRect.y + 30, white, 32);
    
    // 顯示技能牌
    int32_t startY = dialogRect.y + 80;
    int32_t cardHeight = 100;
    int32_t cardWidth = 200;
    int32_t cardsPerRow = 3;
    int32_t padding = 20;
    
    // 根據技能類型獲取對應的vector
    vector* skillCards;
    switch(skillType) {
        case 1: // 攻擊技能
            skillCards = &game.players[game.now_turn_player_id].attackSkill;
            break;
        case 2: // 防禦技能
            skillCards = &game.players[game.now_turn_player_id].defenseSkill;
            break;
        case 3: // 移動技能
            skillCards = &game.players[game.now_turn_player_id].moveSkill;
            break;
        default:
            return;
    }
    
    // 顯示所有技能牌
    int32_t validCardCount = 0;  // 用于计算有效的卡片数量
    for (int32_t i = 0; i < skillCards->SIZE; i++) {
        const Card* cardData = getCardData(skillCards->array[i]);
        if (cardData) {
            int32_t row = validCardCount / cardsPerRow;
            int32_t col = validCardCount % cardsPerRow;
            
            int32_t cardX = dialogRect.x + padding + col * (cardWidth + padding);
            int32_t cardY = startY + row * (cardHeight + padding);
            
            // 卡牌背景（整個卡片作為按鈕）
            SDL_Rect cardRect = {cardX, cardY, cardWidth, cardHeight};
            bool isFirstCard = validCardCount == 0;
            bool canBuy = isFirstCard && game.players[game.now_turn_player_id].energy >= cardData->cost;
            
            // 根據是否可以購買設置不同的背景顏色
            if (canBuy) {
                SDL_SetRenderDrawColor(uiBase.renderer, 80, 80, 80, 255);
            } else if (isFirstCard) {
                SDL_SetRenderDrawColor(uiBase.renderer, 60, 60, 60, 255);
            } else {
                SDL_SetRenderDrawColor(uiBase.renderer, 40, 40, 40, 255);
            }
            SDL_RenderFillRect(uiBase.renderer, &cardRect);
            SDL_SetRenderDrawColor(uiBase.renderer, 200, 200, 200, 255);
            SDL_RenderDrawRect(uiBase.renderer, &cardRect);
            
            // 卡牌信息
            SDL_Color textColor;
            if (canBuy) {
                textColor = white;
            } else if (isFirstCard) {
                textColor = (SDL_Color){180, 180, 180, 255};
            } else {
                textColor = (SDL_Color){120, 120, 120, 255};
            }
            draw_text(cardData->name, cardX + 10, cardY + 10, textColor, 16);
            char costText[20];
            snprintf(costText, 20, "費用: %d", cardData->cost);
            draw_text(costText, cardX + 10, cardY + 30, canBuy ? lightblue1 : (SDL_Color){100, 100, 100, 255}, 14);
            draw_text(cardData->description, cardX + 10, cardY + 50, textColor, 12);
            
            // 購買按鈕
            SDL_Rect buyButtonRect = {cardX + 10, cardY + cardHeight - 30, 80, 20};
            SDL_Color buttonColor;
            if (canBuy) {
                buttonColor = (SDL_Color){0, 200, 0, 255};
            } else if (isFirstCard) {
                buttonColor = (SDL_Color){100, 100, 100, 255};
            } else {
                buttonColor = (SDL_Color){60, 60, 60, 255};
            }
            SDL_SetRenderDrawColor(uiBase.renderer, buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
            SDL_RenderFillRect(uiBase.renderer, &buyButtonRect);
            // 根據價格顯示不同的按鈕文字
            const char* buttonText = cardData->cost == 0 ? "未解鎖" : "購買";
            draw_text_center(buttonText, cardX + 50, cardY + cardHeight - 20, white, 12);
            
            validCardCount++;
        }
    }
    
    // 關閉按鈕
    SDL_Rect closeButtonRect = {dialogRect.x + dialogRect.w - 40, dialogRect.y + 10, 30, 30};
    SDL_SetRenderDrawColor(uiBase.renderer, 200, 0, 0, 255);
    SDL_RenderFillRect(uiBase.renderer, &closeButtonRect);
    draw_text_center("X", dialogRect.x + dialogRect.w - 25, dialogRect.y + 25, white, 16);
}