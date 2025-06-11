#include "ui.h"
#include "character_select.h"

extern sGame game;
extern sUiBase uiBase;
extern gameState nowState;
extern bool running;

const SDL_Color white = {255, 255, 255, 255};
const SDL_Color black = {0, 0, 0, 255};
const SDL_Color gray1 = {32, 32, 32, 255};
const SDL_Color gray2 = {64, 64, 64, 255};
const SDL_Color lightblue1 = {160, 245, 255, 255};
const SDL_Color lightblue2 = {120, 190, 200, 255};

bool showConfirmDialog = false;

// Button storage
sButton **menuButtons = NULL;
sButton **initButtons = NULL;
sButton **confirmButtons = NULL;
int32_t menuButtonCount = 0;
int32_t initButtonCount = 0;
int32_t confirmButtonCount = 0;

void cleanup_current_state() {
    printf("clean state\n");
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
            
        case GAME_INIT:
            {
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
            
        case GAME_PLAY:
        case GAME_OVER:
        default:
            // No buttons needed for these states currently
            break;
    }
    printf("change state to %d\n", nowState);
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
    
    uiBase.font = TTF_OpenFont(FONT_PATH, 36);
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

void game_menu(){
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
                change_state(GAME_INIT);
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

void game_init(){
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
                    change_state(GAME_PLAY);
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
    draw_text_center("遊玩順序按照玩家編號，玩家1為先手", SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 150, white, 40);

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
        SDL_SetRenderDrawColor(uiBase.renderer, 64, 64, 64, 255);
        SDL_Rect dialogRect = {SCREEN_WIDTH/2 - 200, SCREEN_HEIGHT/2 - 100, 400, 200};
        SDL_RenderFillRect(uiBase.renderer, &dialogRect);
        SDL_SetRenderDrawColor(uiBase.renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(uiBase.renderer, &dialogRect);
        
        // 統計機器人數量
        int32_t botCount = 0;
        for(int32_t i = 0; i < playerCount; i++){
            if(game.players[i].isBOT) botCount++;
        }
        
        // 顯示確認訊息
        draw_text_center("確認開始遊戲？", SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 50, white, 24);
        
        if(botCount == 0) {
            draw_text_center("沒有機器人", SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 20, white, 20);
        }
        else if(botCount == playerCount) {
            draw_text_center("全部為機器人", SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 20, white, 20);
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
            draw_text_center(botInfo, SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 20, white, 20);
        }
        
        // 繪製確認對話框按鈕
        for(int32_t i = 0; i < confirmButtonCount; i++){
            int8_t buttonType = mouse_in_button(confirmButtons[i]) ? 1 : 0;
            draw_button(confirmButtons[i], buttonType);
        }
    }
    
    SDL_RenderPresent(uiBase.renderer);
}

void game_play(){
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = 0;
        }
    }
    
    SDL_SetRenderDrawColor(uiBase.renderer, 32, 32, 32, 255);
    SDL_RenderClear(uiBase.renderer);

    SDL_Color white = {255, 255, 255, 255};
    draw_text_center("Play", SCREEN_WIDTH/2, SCREEN_HEIGHT/2, white, 36);

    SDL_RenderPresent(uiBase.renderer);
}

void game_over(){
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = 0;
        }
    }
    
    SDL_SetRenderDrawColor(uiBase.renderer, 32, 32, 32, 255);
    SDL_RenderClear(uiBase.renderer);

    SDL_Color white = {255, 255, 255, 255};
    draw_text_center("xxx win", SCREEN_WIDTH/2, SCREEN_HEIGHT/2, white, 36);

    SDL_RenderPresent(uiBase.renderer);
}