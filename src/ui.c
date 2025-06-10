#include "ui.h"

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
    SDL_Color textColors[] = {white, white};
    SDL_Color bgColors[] = {gray1, gray2};
    SDL_Color borderColors[] = {white, white};
    
    SDL_Rect start1v1ButtonRect = {SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2, 200, 60};
    sButton *start1v1Button = create_button(start1v1ButtonRect, "開始1v1", textColors, bgColors, borderColors, 24, 2);
    SDL_Rect exitButtonRext = {SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 + 100, 200, 60};
    sButton *exitButton = create_button(exitButtonRext, "離開遊戲", textColors, bgColors, borderColors, 24, 2);
    
    bool isHoveringButton = mouse_in_button(start1v1Button) || mouse_in_button(exitButton);
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
            if (mouse_in_button(start1v1Button)) {
                game.playerMode = 0;
                nowState = GAME_INIT;
            } 
            else if (mouse_in_button(exitButton)) {
                running = 0;
            }
        }
    }

    // draw
    SDL_SetRenderDrawColor(uiBase.renderer, 32, 32, 32, 255);
    SDL_RenderClear(uiBase.renderer);

    draw_text_center("Twisted Fables", SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 150, white, 60);
    draw_text_center("扭曲寓言", SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 80, white, 60);
    
    if(mouse_in_button(start1v1Button)) draw_button(start1v1Button, 1);
    else draw_button(start1v1Button, 0);

    if(mouse_in_button(exitButton)) draw_button(exitButton, 1);
    else draw_button(exitButton, 0);

    SDL_RenderPresent(uiBase.renderer);
    free_button(start1v1Button);
    free_button(exitButton);
}
void game_init(){
    SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW));

    int32_t playerCount = TOTAL_PLAYER;
    
    SDL_Color textColors[] = {white, white, black, black};
    SDL_Color bgColors[] = {gray1, gray2, lightblue1, lightblue2};
    SDL_Color borderColors[] = {white, white, white, white};
    
    sButton **playerButton = NULL;
    playerButton = calloc(playerCount, sizeof(sButton*));
    bool isHoveringButton = false;
    for(int32_t i = 0; i < playerCount; i++){
        SDL_Rect playRect = {SCREEN_WIDTH/2 - 200 + i*220, SCREEN_HEIGHT/2, 180, 100};
        char playerText[10] = "";
        snprintf(playerText, 10, "玩家%d", i+1);
        playerButton[i] = create_button(playRect, playerText, textColors, bgColors, borderColors, 36, 4);
        if(mouse_in_button(playerButton[i])){
            isHoveringButton = true;
        }
    }

    SDL_Color acceptTextColors[] = {white, white};
    SDL_Color acceptBgColors[] = {gray1, gray2};
    SDL_Color acceptBorderColors[] = {white, white};
    SDL_Rect acceptRect = {SCREEN_WIDTH/2 - 50, SCREEN_HEIGHT - 200, 100, 40};
    sButton *accetpButton = create_button(acceptRect, "確認", acceptTextColors, acceptBgColors, acceptBorderColors, 24, 2);
    if(mouse_in_button(accetpButton)){
        isHoveringButton = true;
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
            for(int32_t i = 0; i < playerCount; i++){
                if(mouse_in_button(playerButton[i])){
                    game.players[i].isBOT = !(game.players[i].isBOT);
                }
            }
        }
    }

    SDL_SetRenderDrawColor(uiBase.renderer, 32, 32, 32, 255);
    SDL_RenderClear(uiBase.renderer);

    SDL_Color white = {255, 255, 255, 255};
    draw_text_center("選擇機器人玩家", SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 200, white, 40);  
    draw_text_center("遊玩順序按照玩家編號，玩家1為先手", SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 150, white, 40);

    for(int32_t i = 0; i < playerCount; i++){
        int32_t type = (int32_t)(game.players[i].isBOT) * 2;
        if(mouse_in_button(playerButton[i])){
            type++;
        }
        draw_button(playerButton[i], type);
    }
    if(mouse_in_button(accetpButton)){
        draw_button(accetpButton, 1);
    }
    else{
        draw_button(accetpButton, 0);
    }
    
    SDL_RenderPresent(uiBase.renderer);

    for(int32_t i = 0; i < playerCount; i++){
        free_button(playerButton[i]);
    }
    free(playerButton);
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