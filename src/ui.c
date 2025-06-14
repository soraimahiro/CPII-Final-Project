#include "ui.h"
#include "ui_stage.h"

extern sGame game;
extern sUiBase uiBase;
extern gameState nowState;
extern bool running;
extern int8_t winner;
extern int32_t total_turns;           // 使用 main.c 中的全域變數
extern int32_t game_duration_seconds; // 使用 main.c 中的全域變數

const SDL_Color white = {255, 255, 255, 255};
const SDL_Color black = {0, 0, 0, 255};
const SDL_Color gray1 = {32, 32, 32, 255};
const SDL_Color gray2 = {64, 64, 64, 255};
const SDL_Color lightblue1 = {160, 245, 255, 255};
const SDL_Color lightblue2 = {120, 190, 200, 255};
const SDL_Color darkblue1 = {60, 80, 200, 255};

bool showConfirmDialog = false;

// Button storage
sButton **menuButtons = NULL;
sButton **initButtons = NULL;
sButton **confirmButtons = NULL;
int32_t menuButtonCount = 0;
int32_t initButtonCount = 0;
int32_t confirmButtonCount = 0;

uStateComponent stateComponent;

static const CharacterID selectable_characters[6] = {
    CHARACTER_REDHOOD,       // 小紅帽
    CHARACTER_MULAN,         // 花木蘭
    CHARACTER_SNOWWHITE,     // 白雪公主
    CHARACTER_SLEEPINGBEAUTY,// 睡美人
    CHARACTER_ALICE,         // 愛麗絲
    CHARACTER_KAGUYA        // 輝夜姬
};

void cleanup_current_state() {
    // Clean up menu buttons
    if (menuButtons) {
        for (int32_t i = 0; i < menuButtonCount; i++) {
            free_button(menuButtons[i]);
        }
        free(menuButtons);
        menuButtons = NULL;
        menuButtonCount = 0;
    }
    
    // Clean up init buttons
    if (initButtons) {
        for (int32_t i = 0; i < initButtonCount; i++) {
            free_button(initButtons[i]);
        }
        free(initButtons);
        initButtons = NULL;
        initButtonCount = 0;
    }
    
    // Clean up confirm buttons
    if (confirmButtons) {
        for (int32_t i = 0; i < confirmButtonCount; i++) {
            free_button(confirmButtons[i]);
        }
        free(confirmButtons);
        confirmButtons = NULL;
        confirmButtonCount = 0;
    }
    
    showConfirmDialog = false;

    if(nowState == GAME_INIT_CHARACTER_SELECT && stateComponent.characterSelect){
        int32_t PlayerCount = TOTAL_PLAYER;
        for(int32_t i = 0; i < SELECTABLE_CHARACTER_COUNT; i++){
            free_button(stateComponent.characterSelect->ppShowInfoButton[i]);
            for(int32_t j = 0; j < PlayerCount; j++){
                free_button(stateComponent.characterSelect->pppPlayerSelect[j][i]);
            }
        }
        free_button(stateComponent.characterSelect->pAcceptButton);
        free(stateComponent.characterSelect);
    }
    
    // Clean up battle UI stage when leaving GAME_PLAY state
    if(nowState == GAME_PLAY) {
        cleanup_battle_ui_stage();
    }
}

void change_state(gameState newState) {
    cleanup_current_state();
    nowState = newState;
    
    SDL_Color textColors[] = {white, white, black, black};
    SDL_Color bgColors[] = {gray1, gray2, lightblue1, lightblue2};
    SDL_Color borderColors[] = {white, white, white, white};
    
    switch (newState) {
        case GAME_MENU:
            menuButtonCount = 2;
            menuButtons = calloc(menuButtonCount, sizeof(sButton*));
            
            SDL_Rect start1v1ButtonRect = {SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2, 200, 60};
            menuButtons[0] = create_button(start1v1ButtonRect, "開始1v1", textColors, bgColors, borderColors, 24, 2);
            
            SDL_Rect exitButtonRect = {SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 + 100, 200, 60};
            menuButtons[1] = create_button(exitButtonRect, "離開遊戲", textColors, bgColors, borderColors, 24, 2);
            break;
            
        case GAME_INIT_BOT_SELECT: {
            int32_t playerCount = TOTAL_PLAYER;
            initButtonCount = playerCount + 1; // player buttons + accept button
            initButtons = calloc(initButtonCount, sizeof(sButton*));
            
            // Create player buttons
            for(int32_t i = 0; i < playerCount; i++){
                SDL_Rect playRect = {SCREEN_WIDTH/2 - 200 + i*220, SCREEN_HEIGHT/2, 180, 100};
                char playerText[10] = "";
                snprintf(playerText, 10, "玩家%d", i+1);
                initButtons[i] = create_button(playRect, playerText, textColors, bgColors, borderColors, 36, 4);
            }
            
            // Create accept button
            SDL_Rect acceptRect = {SCREEN_WIDTH/2 - 50, SCREEN_HEIGHT - 200, 100, 40};
            initButtons[playerCount] = create_button(acceptRect, "確認", textColors, bgColors, borderColors, 24, 2);
            
            // Initialize confirm dialog buttons (will be created when needed)
            confirmButtonCount = 2;
            confirmButtons = calloc(confirmButtonCount, sizeof(sButton*));
            SDL_Rect confirmRect = {SCREEN_WIDTH/2 - 80, SCREEN_HEIGHT/2 + 200, 70, 40};
            confirmButtons[0] = create_button(confirmRect, "確認", textColors, bgColors, borderColors, 20, 2);
            SDL_Rect cancelRect = {SCREEN_WIDTH/2 + 10, SCREEN_HEIGHT/2 + 200, 70, 40};
            confirmButtons[1] = create_button(cancelRect, "取消", textColors, bgColors, borderColors, 20, 2);
        }
            break;
            
        case GAME_INIT_CHARACTER_SELECT: {
            int32_t playerCount = TOTAL_PLAYER;
            int32_t botCount = 0;
            for(int32_t i = 0; i < playerCount; i++){
                game.players[i].character = -1;
                if(game.players[i].isBOT){
                    botCount++;
                }
            }
            
            // 如果全部都是機器人，直接分配角色後進入play階段
            if(botCount == playerCount){
                for(int32_t i = 0; i < playerCount; i++){
                    game.players[i].character = selectable_characters[i % SELECTABLE_CHARACTER_COUNT];
                }
                game_init();
                change_state(GAME_PLAY);
                return;
            }

            // 創建角色選擇界面
            stateComponent.characterSelect = calloc(1, sizeof(sCharacterSelect));
            stateComponent.characterSelect->showCharacterInfo = -1;
            stateComponent.characterSelect->showWarningDialog = false;
            

            for(int32_t i = 0; i < SELECTABLE_CHARACTER_COUNT; i++){
                // 300 ~ 1000
                SDL_Rect buttonRect = {250 + (900/SELECTABLE_CHARACTER_COUNT)*i, 220, 900/SELECTABLE_CHARACTER_COUNT-10, 50};
                stateComponent.characterSelect->ppShowInfoButton[i] = create_button(buttonRect, "角色資訊", textColors, bgColors, borderColors, 20, 2);

                buttonRect.h = 80;
                buttonRect.y += 80;

                SDL_Color textColorsCH[] = {white, white, black, gray2};
                SDL_Color bgColorsCH[] = {gray1, gray2, lightblue1, gray1};
                SDL_Color borderColorsCH[] = {white, white, white, gray2};
                for(int32_t j = 0; j < playerCount; j++){
                    stateComponent.characterSelect->pppPlayerSelect[j][i] = create_button(buttonRect, "選擇", textColorsCH, bgColorsCH, borderColorsCH, 20, 4);
                    buttonRect.y += 100;
                }
            }
            SDL_Rect acceptRect = {SCREEN_WIDTH/2 - 50, SCREEN_HEIGHT - 100, 100, 40};
            stateComponent.characterSelect->pAcceptButton = create_button(acceptRect, "確認", textColors, bgColors, borderColors, 24, 2);
        }
            break;
        case GAME_PLAY:
            // Initialize battle UI stage
            init_battle_ui_stage();
            break;
        case GAME_OVER:
        default:
            break;
    }
    DEBUG_PRINT("[UI] change state to %d\n", nowState);
}

void init_ui() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return;
    }
    
    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return;
    }
    
    // 嘗試載入自定義字體
    uiBase.font = TTF_OpenFont(FONT_PATH, 24);  // 24是默認字體大小
    
    // 如果載入失敗，嘗試使用系統字體
    if (!uiBase.font) {
        printf("Failed to load custom font: %s\n", TTF_GetError());
        printf("Trying to load system font...\n");
        
        // 嘗試載入系統字體
        const char* system_fonts[] = {
            "/System/Library/Fonts/STHeiti Light.ttc",  // macOS 系統字體
            "/System/Library/Fonts/PingFang.ttc",       // macOS 系統字體
            "/System/Library/Fonts/AppleGothic.ttf",    // macOS 系統字體
            "/Library/Fonts/Arial Unicode.ttf",         // 通用字體
            NULL
        };
        
        for (int i = 0; system_fonts[i] != NULL; i++) {
            uiBase.font = TTF_OpenFont(system_fonts[i], 24);
            if (uiBase.font) {
                printf("Successfully loaded system font: %s\n", system_fonts[i]);
                break;
            }
        }
        
        // 如果所有字體都無法載入，輸出錯誤信息
        if (!uiBase.font) {
            printf("Failed to load any font! SDL_ttf Error: %s\n", TTF_GetError());
            return;
        }
    }
    
    uiBase.window = SDL_CreateWindow("Twisted Fables",
                             SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED,
                             SCREEN_WIDTH,
                             SCREEN_HEIGHT,
                             SDL_WINDOW_SHOWN);
    if (uiBase.window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return;
    }
    
    uiBase.renderer = SDL_CreateRenderer(uiBase.window, -1, SDL_RENDERER_ACCELERATED);
    if (uiBase.renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return;
    }
}

void close_ui() {
    cleanup_current_state();
    
    if(uiBase.font){
        TTF_CloseFont(uiBase.font);
    }
    if(uiBase.renderer){
        SDL_DestroyRenderer(uiBase.renderer);
    }
    if(uiBase.window){
        SDL_DestroyWindow(uiBase.window);
    }
    
    TTF_Quit();
    SDL_Quit();
}

void game_menu_ui(){
    bool isHoveringButton = false;
    for (int32_t i = 0; i < menuButtonCount; i++) {
        if (mouse_in_button(menuButtons[i])) {
            isHoveringButton = true;
            break;
        }
    }
    
    if (isHoveringButton) {
        SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND));
    }
    else {
        SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW));
    }
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = 0;
        } 
        else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            if (mouse_in_button(menuButtons[0])) { // start1v1Button
                game.playerMode = 0;
                change_state(GAME_INIT_BOT_SELECT);
                return;
            } 
            else if (mouse_in_button(menuButtons[1])) { // exitButton
                running = 0;
                return;
            }
        }
    }

    // draw
    SDL_SetRenderDrawColor(uiBase.renderer, 32, 32, 32, 255);
    SDL_RenderClear(uiBase.renderer);

    draw_text_center("Twisted Fables", SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 150, white, 60);
    draw_text_center("扭曲寓言", SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 80, white, 60);
    
    for (int32_t i = 0; i < menuButtonCount; i++) {
        int8_t buttonType = mouse_in_button(menuButtons[i]) ? 1 : 0;
        draw_button(menuButtons[i], buttonType);
    }

    SDL_RenderPresent(uiBase.renderer);
}

void game_init_bot_select_ui(){
    int32_t playerCount = TOTAL_PLAYER;
    
    bool isHoveringButton = false;
    for(int32_t i = 0; i < initButtonCount; i++){
        if(mouse_in_button(initButtons[i])){
            isHoveringButton = true;
            break;
        }
    }
    
    if(showConfirmDialog) {
        for(int32_t i = 0; i < confirmButtonCount; i++){
            if(mouse_in_button(confirmButtons[i])){
                isHoveringButton = true;
                break;
            }
        }
    }

    if (isHoveringButton) {
        SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND));
    }
    else {
        SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW));
    }

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = 0;
            return;
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            if(showConfirmDialog) {
                if(mouse_in_button(confirmButtons[0])) { // confirm button
                    showConfirmDialog = false;
                    change_state(GAME_INIT_CHARACTER_SELECT);
                    return;
                }
                else if(mouse_in_button(confirmButtons[1])) { // cancel button
                    showConfirmDialog = false;
                }
            }
            else {
                if(mouse_in_button(initButtons[playerCount])) { // accept button
                    showConfirmDialog = true;
                }
                else {
                    for(int32_t i = 0; i < playerCount; i++){
                        if(mouse_in_button(initButtons[i])){
                            game.players[i].isBOT = !(game.players[i].isBOT);
                        }
                    }
                }
            }
        }
    }

    SDL_SetRenderDrawColor(uiBase.renderer, 32, 32, 32, 255);
    SDL_RenderClear(uiBase.renderer);

    draw_text_center("選擇機器人玩家", SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 200, white, 40);  
    draw_text_center("遊玩順序按照玩家編號，玩家1為先手", SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 150, white, 20);
    draw_text_center("選取到的為機器人玩家，可以沒有機器人或全為機器人", SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 120, white, 20);

    // Draw player buttons
    for(int32_t i = 0; i < playerCount; i++){
        int32_t type = (int32_t)(game.players[i].isBOT) * 2;
        if(mouse_in_button(initButtons[i])){
            type++;
        }
        draw_button(initButtons[i], type);
    }
    
    // Draw accept button
    int8_t acceptType = mouse_in_button(initButtons[playerCount]) ? 1 : 0;
    draw_button(initButtons[playerCount], acceptType);

    // 顯示確認對話框
    if(showConfirmDialog) {
        // 半透明背景
        SDL_SetRenderDrawColor(uiBase.renderer, 0, 0, 0, 128);
        SDL_Rect overlay = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderFillRect(uiBase.renderer, &overlay);
        
        // 對話框背景
        SDL_Rect dialogRect = {SCREEN_WIDTH/2 - 200, SCREEN_HEIGHT/2 - 100, 400, 200};
        SDL_SetRenderDrawColor(uiBase.renderer, 64, 64, 64, 255);
        SDL_RenderFillRect(uiBase.renderer, &dialogRect);
        SDL_SetRenderDrawColor(uiBase.renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(uiBase.renderer, &dialogRect);
        
        // 統計機器人數量
        int32_t botCount = 0;
        for(int32_t i = 0; i < playerCount; i++){
            if(game.players[i].isBOT) botCount++;
        }
        
        // 顯示確認訊息
        draw_text_center("確定選好了？", SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 35, white, 30);
        
        if(botCount == 0) {
            draw_text_center("沒有機器人", SCREEN_WIDTH/2, SCREEN_HEIGHT/2+5, white, 30);
        }
        else if(botCount == playerCount) {
            draw_text_center("全部為機器人", SCREEN_WIDTH/2, SCREEN_HEIGHT/2+5, white, 30);
        }
        else {
            char botInfo[100] = "機器人玩家：";
            for(int32_t i = 0; i < playerCount; i++){
                if(game.players[i].isBOT) {
                    char playerNum[10];
                    snprintf(playerNum, 10, "玩家%d ", i+1);
                    strcat(botInfo, playerNum);
                }
            }
            draw_text_center(botInfo, SCREEN_WIDTH/2, SCREEN_HEIGHT/2+5, white, 30);
        }
        
        // 繪製確認對話框按鈕
        for(int32_t i = 0; i < confirmButtonCount; i++){
            int8_t buttonType = mouse_in_button(confirmButtons[i]) ? 1 : 0;
            draw_button(confirmButtons[i], buttonType);
        }
    }
    
    SDL_RenderPresent(uiBase.renderer);
}

void game_init_character_select_ui(){
    SDL_Event event;
    int32_t playerCount = TOTAL_PLAYER;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = 0;
            return;
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){
            if(stateComponent.characterSelect->showWarningDialog) {
                // 點擊警告視窗外區域關閉視窗
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                SDL_Rect dialogRect = {SCREEN_WIDTH/2 - 200, SCREEN_HEIGHT/2 - 100, 400, 200};
                
                if (!(mouseX >= dialogRect.x && mouseX < dialogRect.x + dialogRect.w &&
                      mouseY >= dialogRect.y && mouseY < dialogRect.y + dialogRect.h)) {
                    stateComponent.characterSelect->showWarningDialog = false;
                }
            }
            else if(stateComponent.characterSelect->showCharacterInfo != -1){
                // 點擊浮空視窗外區域關閉視窗
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                SDL_Rect dialogRect = {SCREEN_WIDTH/2 - 300, SCREEN_HEIGHT/2 - 200, 600, 400};
                
                if (!(mouseX >= dialogRect.x && mouseX < dialogRect.x + dialogRect.w &&
                      mouseY >= dialogRect.y && mouseY < dialogRect.y + dialogRect.h)) {
                    stateComponent.characterSelect->showCharacterInfo = -1;
                }
            }
            else{
                if(mouse_in_button(stateComponent.characterSelect->pAcceptButton)){
                    // 檢查是否有人類玩家未選擇角色
                    bool hasUnselectedPlayer = false;
                    for(int32_t i = 0; i < playerCount; i++){
                        if(!game.players[i].isBOT && game.players[i].character == -1) {
                            hasUnselectedPlayer = true;
                            break;
                        }
                    }
                    
                    if(hasUnselectedPlayer) {
                        stateComponent.characterSelect->showWarningDialog = true;
                        break;
                    }
                    else {
                        // 先收集人類玩家已選的角色
                        bool usedCharacters[SELECTABLE_CHARACTER_COUNT] = {false};
                        for(int32_t i = 0; i < playerCount; i++){
                            if(!game.players[i].isBOT && game.players[i].character != -1) {
                                for(int32_t j = 0; j < SELECTABLE_CHARACTER_COUNT; j++) {
                                    if(selectable_characters[j] == game.players[i].character) {
                                        usedCharacters[j] = true;
                                        break;
                                    }
                                }
                            }
                        }
                        
                        // 為機器人分配未被使用的角色
                        int32_t botCharacterIndex = 0;
                        for(int32_t i = 0; i < playerCount; i++){
                            if(game.players[i].isBOT){
                                // 找到下一個未被使用的角色
                                while(botCharacterIndex < SELECTABLE_CHARACTER_COUNT && usedCharacters[botCharacterIndex]) {
                                    botCharacterIndex++;
                                }
                                
                                if(botCharacterIndex < SELECTABLE_CHARACTER_COUNT) {
                                    game.players[i].character = selectable_characters[botCharacterIndex];
                                    usedCharacters[botCharacterIndex] = true;
                                    botCharacterIndex++;
                                }
                            }
                        }
                    }
                    
                    // 最後檢查是否還有重複（應該不會有了）
                    bool hasDuplicate = false;
                    for(int32_t i = 0; i < playerCount && !hasDuplicate; i++){
                        if(game.players[i].character != -1) {
                            for(int32_t j = i + 1; j < playerCount; j++){
                                if(game.players[j].character == game.players[i].character) {
                                    hasDuplicate = true;
                                    break;
                                }
                            }
                        }
                    }
                    
                    if(hasDuplicate) {
                        stateComponent.characterSelect->showWarningDialog = true;
                    }
                    else {
                        // 只在這裡調用一次 game_init()
                        game_init();
                        change_state(GAME_PLAY);
                        return;
                    }
                }
                for(int32_t i = 0; i < SELECTABLE_CHARACTER_COUNT; i++){
                    if(mouse_in_button(stateComponent.characterSelect->ppShowInfoButton[i])){
                        stateComponent.characterSelect->showCharacterInfo = i;
                        break;
                    }
                    for(int32_t j = 0; j < playerCount ; j++){
                        if(mouse_in_button(stateComponent.characterSelect->pppPlayerSelect[j][i])){
                            game.players[j].character = selectable_characters[i];
                        }
                    }
                }
            }
        }
    }
    
    SDL_SetRenderDrawColor(uiBase.renderer, 32, 32, 32, 255);
    SDL_RenderClear(uiBase.renderer);

    char playerText[10];
    for(int32_t i = 0; i < playerCount ; i++){
        snprintf(playerText, 10, "玩家%d", i+1);
        draw_text(playerText, 150, 320+100*i, white, 30);
    }

    for(int32_t i = 0; i < SELECTABLE_CHARACTER_COUNT; i++){
        sCharacterInfo nowch = get_character_info(selectable_characters[i]);
        int32_t col_width = 900/SELECTABLE_CHARACTER_COUNT;
        draw_text_center(nowch.name, 250+(col_width-10)/2 + col_width*i, 170, white, 30);
        if(mouse_in_button(stateComponent.characterSelect->ppShowInfoButton[i])){
            draw_button(stateComponent.characterSelect->ppShowInfoButton[i], 1);
        }
        else{
            draw_button(stateComponent.characterSelect->ppShowInfoButton[i], 0);
        }
        
        for(int32_t j = 0; j < playerCount ; j++){
            if(game.players[j].isBOT){
                draw_button(stateComponent.characterSelect->pppPlayerSelect[j][i], 3);
            }
            else if(game.players[j].character == selectable_characters[i]){
                draw_button(stateComponent.characterSelect->pppPlayerSelect[j][i], 2);
            }
            else if(mouse_in_button(stateComponent.characterSelect->pppPlayerSelect[j][i])){
                draw_button(stateComponent.characterSelect->pppPlayerSelect[j][i], 1);
            }
            else{
                draw_button(stateComponent.characterSelect->pppPlayerSelect[j][i], 0);
            }
        }
    }
    if(mouse_in_button(stateComponent.characterSelect->pAcceptButton)){
        draw_button(stateComponent.characterSelect->pAcceptButton, 1);
    }
    else{
        draw_button(stateComponent.characterSelect->pAcceptButton, 0);
    }

    // 顯示警告對話框
    if(stateComponent.characterSelect->showWarningDialog) {
        // 半透明背景覆蓋層
        SDL_SetRenderDrawColor(uiBase.renderer, 0, 0, 0, 128);
        SDL_Rect overlay = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderFillRect(uiBase.renderer, &overlay);
        
        // 警告視窗背景
        SDL_Rect dialogRect = {SCREEN_WIDTH/2 - 200, SCREEN_HEIGHT/2 - 100, 400, 200};
        SDL_SetRenderDrawColor(uiBase.renderer, 64, 64, 64, 255);
        SDL_RenderFillRect(uiBase.renderer, &dialogRect);
        SDL_SetRenderDrawColor(uiBase.renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(uiBase.renderer, &dialogRect);
        
        // 顯示警告訊息
        int32_t centerX = SCREEN_WIDTH/2;
        int32_t centerY = SCREEN_HEIGHT/2;
        
        draw_text_center("警告", centerX, centerY - 50, white, 36);
        
        // 檢查是角色重複還是未選擇的問題
        bool hasUnselectedPlayer = false;
        bool hasDuplicate = false;
        
        for(int32_t i = 0; i < playerCount; i++){
            if(!game.players[i].isBOT && game.players[i].character == -1) {
                hasUnselectedPlayer = true;
                break;
            }
        }
        
        if(!hasUnselectedPlayer) {
            for(int32_t i = 0; i < playerCount && !hasDuplicate; i++){
                if(game.players[i].character != -1) {
                    for(int32_t j = i + 1; j < playerCount; j++){
                        if(game.players[j].character == game.players[i].character) {
                            hasDuplicate = true;
                            break;
                        }
                    }
                }
            }
        }
        
        if(hasUnselectedPlayer) {
            draw_text_center("有玩家尚未選擇角色！", centerX, centerY - 10, white, 28);
            draw_text_center("請完成角色選擇", centerX, centerY + 30, white, 24);
        }
        else if(hasDuplicate) {
            draw_text_center("不能選擇相同角色！", centerX, centerY - 10, white, 28);
            draw_text_center("請重新選擇", centerX, centerY + 30, white, 24);
        }
        
        // 提示文字
        draw_text_center("點擊視窗外區域關閉", centerX, centerY + 70, lightblue1, 20);
    }

    // 顯示角色資訊浮空視窗
    if(stateComponent.characterSelect->showCharacterInfo != -1) {
        // 半透明背景覆蓋層
        SDL_SetRenderDrawColor(uiBase.renderer, 0, 0, 0, 128);
        SDL_Rect overlay = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderFillRect(uiBase.renderer, &overlay);
        
        // 浮空視窗背景
        SDL_Rect dialogRect = {SCREEN_WIDTH/2 - 300, SCREEN_HEIGHT/2 - 200, 600, 400};
        SDL_SetRenderDrawColor(uiBase.renderer, 64, 64, 64, 255);
        SDL_RenderFillRect(uiBase.renderer, &dialogRect);
        SDL_SetRenderDrawColor(uiBase.renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(uiBase.renderer, &dialogRect);
        
        // 獲取角色資訊
        sCharacterInfo charInfo = get_character_info(selectable_characters[stateComponent.characterSelect->showCharacterInfo]);
        
        // 顯示角色資訊
        int32_t centerX = SCREEN_WIDTH/2;
        int32_t startY = SCREEN_HEIGHT/2 - 150;

        char infoText[200];
        snprintf(infoText, 200, "%s", charInfo.name);
        draw_text_center(infoText, centerX, startY, white, 36);
        
        snprintf(infoText, 200, "說明：%s", charInfo.description);
        draw_text_center(infoText, centerX, startY + 100, white, 24);
        
        snprintf(infoText, 200, "生命上限：%2d", charInfo.maxlife);
        draw_text_center(infoText, centerX, startY + 140, white, 24);
        
        snprintf(infoText, 200, "護盾上限：%2d", charInfo.maxdefense);
        draw_text_center(infoText, centerX, startY + 180, white, 24);
        
        snprintf(infoText, 200, "必殺閥值：%2d", charInfo.ultimate_threshold);
        draw_text_center(infoText, centerX, startY + 220, white, 24);
        
        // 提示文字
        draw_text_center("點擊視窗外區域關閉", centerX, startY + 300, lightblue1, 20);
    }

    SDL_RenderPresent(uiBase.renderer);
}

void game_play_ui(){
    SDL_Event event;
    
    // Handle SDL events
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = 0;
            return;
        }
        
        // Handle battle events using new stage UI
        handle_battle_events_stage(&event);
    }
    
    // Draw the battle UI using new stage implementation
    draw_battle_ui_stage();
}

void game_over_ui(){
    SDL_Event event;
    
    // 檢查是否有按鈕需要處理滑鼠事件
    bool isHoveringButton = false;
    
    // 創建按鈕（如果還沒創建）
    if(menuButtons == NULL) {
        menuButtonCount = 2;
        menuButtons = calloc(menuButtonCount, sizeof(sButton*));
        
        SDL_Color textColors[] = {white, white, black, black};
        SDL_Color bgColors[] = {gray1, gray2, lightblue1, lightblue2};
        SDL_Color borderColors[] = {white, white, white, white};
        
        // 重新開始按鈕
        SDL_Rect restartButtonRect = {SCREEN_WIDTH/2 - 120, SCREEN_HEIGHT - 120, 100, 50};
        menuButtons[0] = create_button(restartButtonRect, "重新開始", textColors, bgColors, borderColors, 20, 2);
        
        // 回主選單按鈕
        SDL_Rect mainMenuButtonRect = {SCREEN_WIDTH/2 + 20, SCREEN_HEIGHT - 120, 100, 50};
        menuButtons[1] = create_button(mainMenuButtonRect, "回主選單", textColors, bgColors, borderColors, 20, 2);
    }
    
    // 檢查滑鼠懸停
    for (int32_t i = 0; i < menuButtonCount; i++) {
        if (mouse_in_button(menuButtons[i])) {
            isHoveringButton = true;
            break;
        }
    }
    
    if (isHoveringButton) {
        SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND));
    } else {
        SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW));
    }
    
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = 0;
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            if (mouse_in_button(menuButtons[0])) { // 重新開始
                // 重置遊戲統計數據
                extern time_t game_start_time, game_end_time;
                game_start_time = 0;
                game_end_time = 0;
                total_turns = 0;
                game_duration_seconds = 0;
                winner = 0;
                
                change_state(GAME_INIT_CHARACTER_SELECT);
                return;
            }
            else if (mouse_in_button(menuButtons[1])) { // 回主選單
                // 重置遊戲統計數據
                extern time_t game_start_time, game_end_time;
                game_start_time = 0;
                game_end_time = 0;
                total_turns = 0;
                game_duration_seconds = 0;
                winner = 0;
                
                change_state(GAME_MENU);
                return;
            }
        }
    }
    
    // 繪製背景
    SDL_SetRenderDrawColor(uiBase.renderer, 32, 32, 32, 255);
    SDL_RenderClear(uiBase.renderer);
    
    // 遊戲結束標題 - 更大更居中
    int32_t startY = SCREEN_HEIGHT/2 - 150;
    draw_text_center("遊戲結束", SCREEN_WIDTH/2, startY, white, 64);
    
    startY += 120;
    
    // 顯示勝負結果 - 更大更突出
    char resultText[200];
    int32_t playerCount = (game.playerMode == 0) ? 2 : 4;
    
    for(int32_t i = 0; i < playerCount; i++) {
        sCharacterInfo charInfo = get_character_info(game.players[i].character);
        if(game.players[i].team == winner) {
            snprintf(resultText, 200, "玩家%d %s 勝利！", i+1, charInfo.name);
            draw_text_center(resultText, SCREEN_WIDTH/2, startY, lightblue1, 40);
        } else {
            snprintf(resultText, 200, "玩家%d %s 輸了", i+1, charInfo.name);
            draw_text_center(resultText, SCREEN_WIDTH/2, startY, gray2, 32);
        }
        startY += 50;
    }
    
    startY += 60;
    
    // 遊戲統計資訊 - 一行顯示，時間格式為 HH:MM
    int32_t hours = game_duration_seconds / 3600;
    int32_t minutes = (game_duration_seconds % 3600) / 60;
    snprintf(resultText, 200, "遊戲時長：%02d:%02d    回合數：%d", hours, minutes, total_turns);
    draw_text_center(resultText, SCREEN_WIDTH/2, startY, white, 28);
    
    // 繪製按鈕
    for (int32_t i = 0; i < menuButtonCount; i++) {
        int8_t buttonType = mouse_in_button(menuButtons[i]) ? 1 : 0;
        draw_button(menuButtons[i], buttonType);
    }

    SDL_RenderPresent(uiBase.renderer);
}
