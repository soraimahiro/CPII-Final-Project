#include "ui.h"

extern sGame game;
static Font customFont;
static ScreenState currentScreen = SCREEN_MENU;
static Button singlePlayerButton;
static Button multiPlayerButton;

void get_init_info(int8_t* mode) {
    // printf("Enter play mode: ");
    // scanf("%hhd", mode);
    // printf("Mode: %hhd\n", *mode);
}

void init_gui() {
    InitWindow(1280, 720, "Twisted Fables");
    SetTargetFPS(60);
    
    // 禁用 ESC 鍵自動關閉視窗
    SetExitKey(0);
    
    // 加載自定義字體，包含中文字符範圍
    int codepointCount = 0;
    int *codepoints = LoadCodepoints("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()_+-=[]{}|;':\",./<>? 單人模式雙對戰遊戲介面按返回主選TwistedFables", &codepointCount);
    
    customFont = LoadFontEx(FONT_PATH, FONT_SIZE, codepoints, codepointCount);
    
    // 釋放 codepoints 記憶體
    UnloadCodepoints(codepoints);
    
    if (customFont.texture.id == 0) {
        printf("Warning: Failed to load custom font, using default font\n");
        customFont = GetFontDefault();
    }
    
    // 初始化按鈕
    singlePlayerButton = (Button){
        .rect = {490, 350, 300, 60},
        .text = "單人模式",
        .color = LIGHTGRAY,
        .hoverColor = GRAY,
        .isHovered = false
    };
    
    multiPlayerButton = (Button){
        .rect = {490, 450, 300, 60},
        .text = "雙人對戰",
        .color = LIGHTGRAY,
        .hoverColor = GRAY,
        .isHovered = false
    };
}

void close_gui() {
    // 卸載字體 (如果不是默認字體)
    if (customFont.texture.id != GetFontDefault().texture.id) {
        UnloadFont(customFont);
    }
    CloseWindow();
}

Font get_custom_font() {
    return customFont;
}

ScreenState get_current_screen() {
    return currentScreen;
}

void set_current_screen(ScreenState screen) {
    currentScreen = screen;
}

static void update_button(Button* button) {
    Vector2 mousePos = GetMousePosition();
    button->isHovered = CheckCollisionPointRec(mousePos, button->rect);
    
    if (button->isHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
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
    Color buttonColor = button->isHovered ? button->hoverColor : button->color;
    DrawRectangleRec(button->rect, buttonColor);
    DrawRectangleLinesEx(button->rect, 2, BLACK);
    
    Vector2 textSize = MeasureTextEx(customFont, button->text, 30, 1);
    Vector2 textPos = {
        button->rect.x + (button->rect.width - textSize.x) / 2,
        button->rect.y + (button->rect.height - textSize.y) / 2
    };
    DrawTextEx(customFont, button->text, textPos, 30, 1, BLACK);
}

void update_gui() {
    if (currentScreen == SCREEN_MENU) {
        update_button(&singlePlayerButton);
        update_button(&multiPlayerButton);
    }
}

void draw_gui() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    
    if (currentScreen == SCREEN_MENU) {
        // 繪製標題
        Vector2 titleSize = MeasureTextEx(customFont, "Twisted Fables", 60, 2);
        Vector2 titlePos = {(1280 - titleSize.x) / 2, 150};
        DrawTextEx(customFont, "Twisted Fables", titlePos, 60, 2, BLACK);
        
        // 繪製按鈕
        draw_button(&singlePlayerButton);
        draw_button(&multiPlayerButton);
        
    } 
    else if (currentScreen == SCREEN_GAME) {
        // 遊戲介面 (暫時空白)
        DrawTextEx(customFont, "遊戲介面", (Vector2){50, 50}, 40, 2, BLACK);
        DrawTextEx(customFont, "按 ESC 返回主選單", (Vector2){50, 100}, 20, 1, GRAY);
        
        // ESC 鍵返回主選單
        if (IsKeyPressed(KEY_ESCAPE)) {
            set_current_screen(SCREEN_MENU);
        }
    }
    
    EndDrawing();
}