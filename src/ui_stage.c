#include "ui_stage.h"
#include "card.h"

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
    
    // Opponent areas (top) - symmetric to my areas
    // Opponent hand cards
    draw_card_area_stage("手牌", &game.players[opponent].hand, 
                        startX + 10, 20, width - 20, 80, false); // Increased height to 80
    
    // Opponent skill areas and used cards - positioned symmetrically
    int32_t opponentAreasY = 110; // Position for opponent skill areas
    
    vector emptyVector = {0};
    
    // Opponent skill areas (4 vertical sections)
    draw_card_area_stage("必殺牌", &game.players[opponent].specialDeck, 
                        startX + 10, opponentAreasY, skillCardWidth, 80, false);
    
    draw_card_area_stage("攻擊技能", &emptyVector, 
                        startX + 10 + skillCardWidth, opponentAreasY, skillCardWidth, 80, false);
    
    draw_card_area_stage("防禦技能", &emptyVector, 
                        startX + 10 + skillCardWidth * 2, opponentAreasY, skillCardWidth, 80, false);
    
    draw_card_area_stage("移動技能", &emptyVector, 
                        startX + 10 + skillCardWidth * 3, opponentAreasY, skillCardWidth, 80, false);
    
    // Opponent used cards area (right side)
    draw_card_area_stage("反轉牌", &game.players[opponent].usecards, 
                        startX + largeAreaWidth + 30, opponentAreasY, largeAreaWidth, 80, false);
    
    // Main battle area (center of screen) - enlarged
    int32_t battleAreaY = 210; // Start after opponent areas with spacing
    int32_t battleAreaHeight = 280; // Enlarged battle area
    SDL_Rect battleArea = {startX + 50, battleAreaY, width - 100, battleAreaHeight};
    SDL_SetRenderDrawColor(uiBase.renderer, 60, 60, 60, 255);
    SDL_RenderFillRect(uiBase.renderer, &battleArea);
    SDL_SetRenderDrawColor(uiBase.renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(uiBase.renderer, &battleArea);
    
    draw_text_center("主戰鬥區域", startX + width/2, battleAreaY + battleAreaHeight/2, white, 20);
    
    // My skill areas and used cards - positioned symmetrically to opponent
    int32_t myAreasY = 510; // Position symmetric to opponent areas
    
    // Four skill areas from left to right: Ultimate, Attack, Defense, Move
    draw_card_area_stage("必殺牌", &game.players[current_player].specialDeck, 
                        startX + 10, myAreasY, skillCardWidth, 80, true);
    
    draw_card_area_stage("攻擊技能", &emptyVector, 
                        startX + 10 + skillCardWidth, myAreasY, skillCardWidth, 80, true);
    
    draw_card_area_stage("防禦技能", &emptyVector, 
                        startX + 10 + skillCardWidth * 2, myAreasY, skillCardWidth, 80, true);
    
    draw_card_area_stage("移動技能", &emptyVector, 
                        startX + 10 + skillCardWidth * 3, myAreasY, skillCardWidth, 80, true);
    
    // My used cards area (right side)
    draw_card_area_stage("反轉牌", &game.players[current_player].usecards, 
                        startX + largeAreaWidth + 30, myAreasY, largeAreaWidth, 80, false);
    
    // My hand cards - symmetric to opponent hand cards
    int32_t handY = 600; // Position symmetric to opponent hand
    draw_card_area_stage("手牌", &game.players[current_player].hand, 
                        startX + 10, handY, width - 20, 80, true); // Same height as opponent
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
    int32_t topSectionHeight = centerY - 40; // Reserve 40px margin from center
    int32_t bottomSectionHeight = SCREEN_HEIGHT - centerY - 40; // Reserve 40px margin from center
    
    // Top section - opponent buttons with proper spacing
    int32_t topButtonSpacing = topSectionHeight / 6; // Space for 3 buttons + margins
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
    
    // Opponent metamorphosis display area - between buttons and center
    int32_t opponentMetamorphY = topStartY + topButtonSpacing * 3 + 10;
    int32_t opponentMetamorphHeight = centerY - 50 - opponentMetamorphY; // Fill remaining space above center
    SDL_Rect opponentMetamorphRect = {startX + 10, opponentMetamorphY, RIGHT_COLUMN_WIDTH - 20, opponentMetamorphHeight};
    SDL_SetRenderDrawColor(uiBase.renderer, 70, 70, 70, 255);
    SDL_RenderFillRect(uiBase.renderer, &opponentMetamorphRect);
    SDL_SetRenderDrawColor(uiBase.renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(uiBase.renderer, &opponentMetamorphRect);
    
    draw_text("對手蛻變牌", startX + 15, opponentMetamorphY + 5, white, 16);
    char opponentMetamorphText[30];
    snprintf(opponentMetamorphText, 30, "(%d張)", game.players[opponent].metamorphosis.SIZE);
    draw_text(opponentMetamorphText, startX + 15, opponentMetamorphY + 25, white, 12);
    
    // Basic card shop button (center) - vertically centered
    if(battleUIStage.shopButton) {
        int8_t buttonType = mouse_in_button(battleUIStage.shopButton) ? 1 : 0;
        draw_button(battleUIStage.shopButton, buttonType);
    }
    
    // My metamorphosis display area - between center and bottom buttons
    int32_t myMetamorphY = centerY + 50;
    int32_t bottomButtonsStartY = SCREEN_HEIGHT - (bottomSectionHeight / 6) * 4 - 10; // Space for 4 buttons
    int32_t myMetamorphHeight = bottomButtonsStartY - myMetamorphY - 10;
    SDL_Rect myMetamorphRect = {startX + 10, myMetamorphY, RIGHT_COLUMN_WIDTH - 20, myMetamorphHeight};
    SDL_SetRenderDrawColor(uiBase.renderer, 70, 70, 70, 255);
    SDL_RenderFillRect(uiBase.renderer, &myMetamorphRect);
    SDL_SetRenderDrawColor(uiBase.renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(uiBase.renderer, &myMetamorphRect);
    
    draw_text("我的蛻變牌", startX + 15, myMetamorphY + 5, white, 16);
    char myMetamorphText[30];
    snprintf(myMetamorphText, 30, "(%d張)", game.players[current_player].metamorphosis.SIZE);
    draw_text(myMetamorphText, startX + 15, myMetamorphY + 25, white, 12);
    
    // Bottom section - my buttons with proper spacing
    int32_t bottomButtonSpacing = bottomSectionHeight / 6; // Space for 4 buttons + margins
    
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
    
    // Top section button spacing and positions
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
    
    // Card count - 12px font
    char countText[20];
    snprintf(countText, 20, "(%d張)", cards->SIZE);
    draw_text(countText, x + width - 60, y + 5, white, 12);
    
    if(!clickable || cards->SIZE == 0) return;
    
    // Show first few cards as text for now (max 8 cards)
    int32_t cardY = y + 25;
    int32_t maxCards = (height - 30) / 15; // Approximately 15px per line
    if(maxCards > 8) maxCards = 8;
    if(maxCards > cards->SIZE) maxCards = cards->SIZE;
    
    for(int32_t i = 0; i < maxCards; i++) {
        const Card* cardData = getCardData(cards->array[i]);
        if(cardData) {
            char cardText[100];
            // Limit card name to 8 characters as specified
            snprintf(cardText, 100, "%d. %.8s", i + 1, cardData->name);
            draw_text(cardText, x + 10, cardY + i * 15, white, 12);
        }
    }
    
    // If more than 8 cards, show "..."
    if(cards->SIZE > 8) {
        draw_text("...", x + 10, cardY + 8 * 15, white, 12);
    }
}

void draw_battle_ui_stage() {
    // Clear screen with main background color
    SDL_SetRenderDrawColor(uiBase.renderer, 30, 30, 30, 255);
    SDL_RenderClear(uiBase.renderer);
    
    // Draw three columns
    draw_left_column_stage();
    draw_middle_column_stage();
    draw_right_column_stage();
    
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
    
    SDL_RenderPresent(uiBase.renderer);
}

void handle_battle_events_stage(SDL_Event* event) {
    if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
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
        
        if(battleUIStage.shopButton && mouse_in_button(battleUIStage.shopButton)) {
            printf("Shop button clicked\n");
            battleUIStage.showShop = !battleUIStage.showShop;
            return;
        }
        
        // Handle deck/graveyard button clicks
        if(battleUIStage.opponentDeckButton && mouse_in_button(battleUIStage.opponentDeckButton)) {
            printf("Opponent deck button clicked\n");
            return;
        }
        
        if(battleUIStage.opponentGraveButton && mouse_in_button(battleUIStage.opponentGraveButton)) {
            printf("Opponent graveyard button clicked\n");
            return;
        }
        
        if(battleUIStage.myDeckButton && mouse_in_button(battleUIStage.myDeckButton)) {
            printf("My deck button clicked\n");
            return;
        }
        
        if(battleUIStage.myGraveButton && mouse_in_button(battleUIStage.myGraveButton)) {
            printf("My graveyard button clicked\n");
            return;
        }
        
        // Handle metamorphosis area clicks (updated positions with relative coordinates)
        int32_t mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        
        int32_t startX = SCREEN_WIDTH - RIGHT_COLUMN_WIDTH;
        int32_t centerY = SCREEN_HEIGHT / 2;
        int32_t topSectionHeight = centerY - 40;
        int32_t topButtonSpacing = topSectionHeight / 6;
        int32_t topStartY = topButtonSpacing;
        
        // Check opponent metamorphosis area click
        int32_t opponentMetamorphY = topStartY + topButtonSpacing * 3 + 10;
        int32_t opponentMetamorphHeight = centerY - 50 - opponentMetamorphY;
        SDL_Rect opponentMetamorphRect = {startX + 10, opponentMetamorphY, RIGHT_COLUMN_WIDTH - 20, opponentMetamorphHeight};
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
        SDL_Rect myMetamorphRect = {startX + 10, myMetamorphY, RIGHT_COLUMN_WIDTH - 20, myMetamorphHeight};
        if(mouseX >= myMetamorphRect.x && mouseX < myMetamorphRect.x + myMetamorphRect.w &&
           mouseY >= myMetamorphRect.y && mouseY < myMetamorphRect.y + myMetamorphRect.h) {
            printf("My metamorphosis area clicked\n");
            // TODO: Show detailed metamorphosis list
            return;
        }
    }
    
    if(event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_ESCAPE) {
        battleUIStage.focusMode = false;
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