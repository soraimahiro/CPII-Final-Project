#include "ui.h"

extern sGame game;
static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static TTF_Font* fontLarge = NULL;
static TTF_Font* fontMedium = NULL;
static TTF_Font* fontSmall = NULL;
static TTF_Font* fontTiny = NULL;
static ScreenState currentScreen = SCREEN_MENU;
static Button singlePlayerButton;
static Button multiPlayerButton;
static bool running = true;

void get_init_info(int8_t* mode) {
    // printf("Enter play mode: ");
    // scanf("%hhd", mode);
    // printf("Mode: %hhd\n", *mode);
}

void init_gui() {
    // 初始化 SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return;
    }
    
    // 初始化 TTF
    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return;
    }
    
    // 創建視窗
    window = SDL_CreateWindow(WINDOW_TITLE,
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             SCREEN_WIDTH,
                             SCREEN_HEIGHT,
                             SDL_WINDOW_SHOWN);
    
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return;
    }
    
    // 創建渲染器
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return;
    }
    
    // 載入不同大小的字體
    fontLarge = TTF_OpenFont(FONT_PATH, FONT_SIZE_LARGE);
    fontMedium = TTF_OpenFont(FONT_PATH, FONT_SIZE_MEDIUM);
    fontSmall = TTF_OpenFont(FONT_PATH, FONT_SIZE_SMALL);
    fontTiny = TTF_OpenFont(FONT_PATH, FONT_SIZE_TINY);
    
    if (fontLarge == NULL || fontMedium == NULL || fontSmall == NULL || fontTiny == NULL) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
    }
    
    // 初始化按鈕 - 改為黑色主題
    singlePlayerButton = (Button){
        .rect = {490, 350, 300, 60},
        .text = "單人模式",
        .color = {64, 64, 64, 255},         // DARKGRAY
        .hoverColor = {96, 96, 96, 255},    // LIGHTER GRAY
        .isHovered = false
    };
    
    multiPlayerButton = (Button){
        .rect = {490, 450, 300, 60},
        .text = "雙人對戰",
        .color = {64, 64, 64, 255},         // DARKGRAY
        .hoverColor = {96, 96, 96, 255},    // LIGHTER GRAY
        .isHovered = false
    };
}

void close_gui() {
    // 清理資源
    if (fontLarge != NULL) {
        TTF_CloseFont(fontLarge);
        fontLarge = NULL;
    }
    if (fontMedium != NULL) {
        TTF_CloseFont(fontMedium);
        fontMedium = NULL;
    }
    if (fontSmall != NULL) {
        TTF_CloseFont(fontSmall);
        fontSmall = NULL;
    }
    if (fontTiny != NULL) {
        TTF_CloseFont(fontTiny);
        fontTiny = NULL;
    }
    
    if (renderer != NULL) {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }
    
    if (window != NULL) {
        SDL_DestroyWindow(window);
        window = NULL;
    }
    
    TTF_Quit();
    SDL_Quit();
}

TTF_Font* get_font_by_size(int fontSize) {
    if (fontSize >= 30) return fontLarge;
    else if (fontSize >= 18) return fontMedium;
    else if (fontSize >= 14) return fontSmall;
    else return fontTiny;
}

ScreenState get_current_screen() {
    return currentScreen;
}

void set_current_screen(ScreenState screen) {
    currentScreen = screen;
}

static bool point_in_rect(int x, int y, SDL_Rect* rect) {
    return (x >= rect->x && x < rect->x + rect->w && 
            y >= rect->y && y < rect->y + rect->h);
}

static void update_button(Button* button) {
    int mouseX, mouseY;
    Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
    
    button->isHovered = point_in_rect(mouseX, mouseY, &button->rect);
    
    if (button->isHovered && (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))) {
        if (button == &singlePlayerButton) {
            game.playerMode = 1;
            set_current_screen(SCREEN_GAME);
        } else if (button == &multiPlayerButton) {
            game.playerMode = 2;
            set_current_screen(SCREEN_GAME);
        }
    }
}

static void draw_button(Button* button) {
    SDL_Color buttonColor = button->isHovered ? button->hoverColor : button->color;
    
    // 繪製按鈕背景
    SDL_SetRenderDrawColor(renderer, buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
    SDL_RenderFillRect(renderer, &button->rect);
    
    // 繪製按鈕邊框
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // WHITE
    SDL_RenderDrawRect(renderer, &button->rect);
    
    // 繪製按鈕文字
    TTF_Font* font = get_font_by_size(30);
    if (font != NULL) {
        SDL_Color textColor = {255, 255, 255, 255}; // WHITE
        SDL_Surface* textSurface = TTF_RenderUTF8_Solid(font, button->text, textColor);
        if (textSurface != NULL) {
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            if (textTexture != NULL) {
                int textW = textSurface->w;
                int textH = textSurface->h;
                SDL_Rect textRect = {
                    button->rect.x + (button->rect.w - textW) / 2,
                    button->rect.y + (button->rect.h - textH) / 2,
                    textW,
                    textH
                };
                SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                SDL_DestroyTexture(textTexture);
            }
            SDL_FreeSurface(textSurface);
        }
    }
}

static void draw_text(const char* text, int x, int y, int fontSize, SDL_Color color) {
    TTF_Font* font = get_font_by_size(fontSize);
    if (font != NULL) {
        SDL_Surface* textSurface = TTF_RenderUTF8_Solid(font, text, color);
        if (textSurface != NULL) {
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            if (textTexture != NULL) {
                SDL_Rect textRect = {x, y, textSurface->w, textSurface->h};
                SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                SDL_DestroyTexture(textTexture);
            }
            SDL_FreeSurface(textSurface);
        }
    }
}

static void draw_rect_filled(int x, int y, int w, int h, SDL_Color color) {
    SDL_Rect rect = {x, y, w, h};
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}

static void draw_rect_outline(int x, int y, int w, int h, SDL_Color color) {
    SDL_Rect rect = {x, y, w, h};
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawRect(renderer, &rect);
}

static void draw_game_interface() {
    // 定義顏色
    SDL_Color red = {255, 0, 0, 255};
    SDL_Color blue = {0, 0, 255, 255};
    SDL_Color gray = {128, 128, 128, 255};
    SDL_Color lightGray = {192, 192, 192, 255};
    SDL_Color purple = {128, 0, 128, 255};
    SDL_Color orange = {255, 165, 0, 255};
    SDL_Color skyBlue = {135, 206, 235, 255};
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color black = {0, 0, 0, 255};
    SDL_Color fadeRed = {255, 0, 0, 76};
    SDL_Color fadeBlue = {0, 0, 100, 76};
    
    // ==== 敵方角色資訊（上方）====
    draw_rect_filled(20, 20, 300, 100, fadeBlue);
    draw_text("Enemy: Snow White", 30, 30, 10, blue);
    draw_text("HP: 75", 30, 55, 5, white);
    draw_text("MP: 50", 30, 80, 5, white);
    
    // ==== 玩家角色資訊（下方）====
    draw_rect_filled(20, SCREEN_HEIGHT - 120, 300, 100, fadeRed);
    draw_text("Player: Red Riding Hood", 30, SCREEN_HEIGHT - 110, 16, red);
    draw_text("HP: 80", 30, SCREEN_HEIGHT - 85, 14, white);
    draw_text("MP: 40", 150, SCREEN_HEIGHT - 85, 14, white);
    
    // ==== 戰鬥場路徑（中間）====
    for (int i = 0; i < 5; i++) {
        draw_rect_filled(480 + i * 60, SCREEN_HEIGHT / 2 - 30, 50, 60, gray);
        char posText[10];
        sprintf(posText, "%d", i);
        draw_text(posText, 495 + i * 60, SCREEN_HEIGHT / 2, 16, lightGray);
    }
    draw_text("BATTLE PATH", 600, SCREEN_HEIGHT / 2 + 40, 14, lightGray);
    
    // ==== 我方出牌區與反轉牌區（左下）====
    draw_rect_filled(20, SCREEN_HEIGHT - 240, 120, 50, lightGray);
    draw_text("出牌區", 30, SCREEN_HEIGHT - 225, 14, black);
    
    draw_rect_filled(160, SCREEN_HEIGHT - 240, 120, 50, skyBlue);
    draw_text("反轉區", 170, SCREEN_HEIGHT - 225, 14, black);
    
    // ==== 技能供應牌庫（左中）====
    draw_rect_filled(20, SCREEN_HEIGHT / 2 - 50, 100, 140, purple);
    draw_text("技能供應", 25, SCREEN_HEIGHT / 2 - 40, 14, white);
    draw_text("技能牌顯示中...", 25, SCREEN_HEIGHT / 2 + 60, 12, white);
    
    // ==== 語氣牌堆與必殺牌（右下）====
    draw_rect_filled(SCREEN_WIDTH - 220, SCREEN_HEIGHT - 240, 100, 50, orange);
    draw_text("語氣牌堆", SCREEN_WIDTH - 210, SCREEN_HEIGHT - 225, 14, black);
    draw_text("語氣牌: (更多...)", SCREEN_WIDTH - 210, SCREEN_HEIGHT - 170, 12, white);
    
    draw_rect_filled(SCREEN_WIDTH - 100, SCREEN_HEIGHT - 240, 80, 50, red);
    draw_text("必殺", SCREEN_WIDTH - 90, SCREEN_HEIGHT - 225, 14, black);
    draw_text("必殺牌資訊...", SCREEN_WIDTH - 95, SCREEN_HEIGHT - 170, 12, white);
    
    // ==== 棄牌堆 ====
    // 敵方棄牌堆（右上）
    draw_rect_filled(SCREEN_WIDTH - 200, 20, 140, 40, lightGray);
    draw_text("敵方棄牌堆", SCREEN_WIDTH - 190, 30, 14, black);
    
    // 我方棄牌堆（左下）
    draw_rect_filled(320, SCREEN_HEIGHT - 120, 140, 40, lightGray);
    draw_text("我方棄牌堆", 330, SCREEN_HEIGHT - 110, 14, black);
    
    // ==== 控制說明 ====
    draw_text("按 ESC 返回主選單", 20, SCREEN_HEIGHT - 300, 14, white);
}

void update_gui() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            running = false;
        } else if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_ESCAPE && currentScreen == SCREEN_GAME) {
                set_current_screen(SCREEN_MENU);
            }
        } else if (e.type == SDL_MOUSEBUTTONDOWN) {
            if (currentScreen == SCREEN_MENU) {
                update_button(&singlePlayerButton);
                update_button(&multiPlayerButton);
            }
        }
    }
    
    // 更新按鈕懸停狀態
    if (currentScreen == SCREEN_MENU) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        singlePlayerButton.isHovered = point_in_rect(mouseX, mouseY, &singlePlayerButton.rect);
        multiPlayerButton.isHovered = point_in_rect(mouseX, mouseY, &multiPlayerButton.rect);
    }
}

void draw_gui() {
    // 清除螢幕
    SDL_SetRenderDrawColor(renderer, 32, 32, 32, 255); // DARK BACKGROUND
    SDL_RenderClear(renderer);
    
    if (currentScreen == SCREEN_MENU) {
        // 繪製標題
        SDL_Color titleColor = {255, 255, 255, 255}; // WHITE
        
        TTF_Font* titleFont = get_font_by_size(60);
        if (titleFont != NULL) {
            SDL_Surface* titleSurface = TTF_RenderUTF8_Solid(titleFont, "Twisted Fables", titleColor);
            if (titleSurface != NULL) {
                SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
                if (titleTexture != NULL) {
                    int titleW = titleSurface->w;
                    int titleH = titleSurface->h;
                    SDL_Rect titleRect = {
                        (SCREEN_WIDTH - titleW) / 2,
                        150,
                        titleW,
                        titleH
                    };
                    SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
                    SDL_DestroyTexture(titleTexture);
                }
                SDL_FreeSurface(titleSurface);
            }
        }
        
        // 繪製按鈕
        draw_button(&singlePlayerButton);
        draw_button(&multiPlayerButton);
        
    } else if (currentScreen == SCREEN_GAME) {
        // 繪製遊戲介面
        draw_game_interface();
    }
    
    // 更新螢幕
    SDL_RenderPresent(renderer);
}

bool is_running() {
    return running;
}