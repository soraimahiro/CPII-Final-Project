#include "ui.h"

extern sGame game;
extern sUiBase uiBase;
extern gameState nowState;
extern bool running;

void init_ui() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return;
    }
    
    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return;
    }
    
    uiBase.window = SDL_CreateWindow(WINDOW_TITLE,
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

void game_init_menu(){
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color darkGray = {32, 32, 32, 255};
    SDL_Color lightGray = {64, 64, 64, 255};
    SDL_Rect pveButtonRect = {SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2, 200, 60};
    sButton *pveButton = create_button(pveButtonRect, "單人對電腦", white, white, darkGray, lightGray, white, 24);
    SDL_Rect pvpButtonRect = {SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 + 100, 200, 60};
    sButton *pvpButton = create_button(pvpButtonRect, "雙人對戰", white, white, darkGray, lightGray, white, 24);
    
    bool isHoveringButton = mouse_in_button(pveButton) || mouse_in_button(pvpButton);
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
            if (mouse_in_button(pveButton)) {
                game.playerMode = 0;
                nowState = GAME_INIT_CHARACTOR;
                
            } 
            else if (mouse_in_button(pvpButton)) {
                game.playerMode = 1;
                nowState = GAME_INIT_CHARACTOR;
            }
        }
    }

    // draw
    SDL_SetRenderDrawColor(uiBase.renderer, 32, 32, 32, 255);
    SDL_RenderClear(uiBase.renderer);

    draw_text_center("Twisted Fables", SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 150, white, 100);

    draw_button(pveButton);
    draw_button(pvpButton);

    SDL_RenderPresent(uiBase.renderer);
    if (pveButton) {
        free(pveButton->text);
        free(pveButton);
    }
    if (pvpButton) {
        free(pvpButton->text);
        free(pvpButton);
    }
}
void game_init_charactor(int32_t *nowPlayer){
    SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW));

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = 0;
        }
    }

    SDL_SetRenderDrawColor(uiBase.renderer, 32, 32, 32, 255);
    SDL_RenderClear(uiBase.renderer);

    SDL_Color white = {255, 255, 255, 255};
    draw_text_center("玩家1選擇角色", SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 200, white, 40);

    SDL_RenderPresent(uiBase.renderer);
}

void game_play_event(){
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = 0;
        }
    }
}
void game_play_show(){
    SDL_SetRenderDrawColor(uiBase.renderer, 32, 32, 32, 255);
    SDL_RenderClear(uiBase.renderer);

    SDL_Color white = {255, 255, 255, 255};
    draw_text_center("Play", SCREEN_WIDTH/2, SCREEN_HEIGHT/2, white, 36);

    SDL_RenderPresent(uiBase.renderer);
}

void game_over_event(){
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = 0;
        }
    }
}
void game_over_show(){
    SDL_SetRenderDrawColor(uiBase.renderer, 32, 32, 32, 255);
    SDL_RenderClear(uiBase.renderer);

    SDL_Color white = {255, 255, 255, 255};
    draw_text_center("xxx win", SCREEN_WIDTH/2, SCREEN_HEIGHT/2, white, 36);

    SDL_RenderPresent(uiBase.renderer);
}