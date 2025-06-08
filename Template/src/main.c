#include "raylib.h"
#include <stdbool.h>

typedef struct {
    const char *name;
    int hp;
    int mp;
    Color color;
} Character;

int main(void) {
    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "Twisted Fables GUI");

    SetTargetFPS(60);

    // 我方與對方角色
    Character player = { "Red Riding Hood", 80, 40, RED };
    Character enemy = { "Snow White", 75, 50, BLUE };

    // 控制顯示的狀態
    bool showEnemyDiscard = false;
    bool showPlayerDiscard = false;
    bool showSkillPile = false;
    bool showTonePile = false;
    bool showFinisher = false;

    while (!WindowShouldClose()) {
        // ==== Input ====
        if (IsKeyPressed(KEY_ONE)) showEnemyDiscard = !showEnemyDiscard;
        if (IsKeyPressed(KEY_TWO)) showSkillPile = !showSkillPile;
        if (IsKeyPressed(KEY_THREE)) showTonePile = !showTonePile;
        if (IsKeyPressed(KEY_FOUR)) showFinisher = !showFinisher;
        if (IsKeyPressed(KEY_FIVE)) showPlayerDiscard = !showPlayerDiscard;

        BeginDrawing();
        ClearBackground(DARKGRAY);

        // ==== 敵方角色資訊（上方）====
        DrawRectangle(20, 20, 300, 100, Fade(enemy.color, 0.3f));
        DrawText(TextFormat("Enemy: %s", enemy.name), 30, 30, 20, enemy.color);
        DrawText(TextFormat("HP: %d", enemy.hp), 30, 60, 18, WHITE);
        DrawText(TextFormat("MP: %d", enemy.mp), 150, 60, 18, WHITE);

        // ==== 玩家角色資訊（下方）====
        DrawRectangle(20, screenHeight - 120, 300, 100, Fade(player.color, 0.3f));
        DrawText(TextFormat("Player: %s", player.name), 30, screenHeight - 110, 20, player.color);
        DrawText(TextFormat("HP: %d", player.hp), 30, screenHeight - 80, 18, WHITE);
        DrawText(TextFormat("MP: %d", player.mp), 150, screenHeight - 80, 18, WHITE);

        // ==== 戰鬥場路徑（中間）====
        for (int i = 0; i < 5; i++) {
            DrawRectangle(480 + i * 60, screenHeight / 2 - 30, 50, 60, GRAY);
            DrawText(TextFormat("%d", i), 495 + i * 60, screenHeight / 2, 20, LIGHTGRAY);
        }
        DrawText("BATTLE PATH", 600, screenHeight / 2 + 40, 18, LIGHTGRAY);

        // ==== 我方出牌區與反轉牌區（左下）====
        DrawRectangle(20, screenHeight - 240, 120, 50, LIGHTGRAY);
        DrawText("出牌區", 30, screenHeight - 225, 18, BLACK);
        DrawRectangle(160, screenHeight - 240, 120, 50, SKYBLUE);
        DrawText("反轉區", 170, screenHeight - 225, 18, BLACK);

        // ==== 技能供應牌庫（左中）====
        DrawRectangle(20, screenHeight / 2 - 50, 100, 140, PURPLE);
        DrawText("技能供應", 25, screenHeight / 2 - 40, 16, WHITE);
        if (showSkillPile) {
            DrawText("後面技能牌顯示中...", 20, screenHeight / 2 + 60, 14, WHITE);
        }

        // ==== 語氣牌堆與必殺牌（右下）====
        DrawRectangle(screenWidth - 220, screenHeight - 240, 100, 50, ORANGE);
        DrawText("語氣牌堆", screenWidth - 210, screenHeight - 225, 16, BLACK);
        if (showTonePile) {
            DrawText("語氣牌: (更多...)", screenWidth - 210, screenHeight - 170, 16, WHITE);
        }

        DrawRectangle(screenWidth - 100, screenHeight - 240, 80, 50, RED);
        DrawText("必殺", screenWidth - 90, screenHeight - 225, 16, BLACK);
        if (showFinisher) {
            DrawText("必殺牌資訊...", screenWidth - 130, screenHeight - 170, 16, WHITE);
        }

        // ==== 棄牌堆（右上 / 左下）====
        if (showEnemyDiscard) {
            DrawRectangle(screenWidth - 200, 20, 140, 40, LIGHTGRAY);
            DrawText("敵方棄牌堆", screenWidth - 190, 30, 16, BLACK);
        }

        if (showPlayerDiscard) {
            DrawRectangle(320, screenHeight - 120, 140, 40, LIGHTGRAY);
            DrawText("我方棄牌堆", 330, screenHeight - 110, 16, BLACK);
        }

        // ==== 控制說明 ====
        DrawText("按鍵操作：", 20, screenHeight - 280, 18, RAYWHITE);
        DrawText("[1] 顯示敵方棄牌堆", 20, screenHeight - 260, 16, WHITE);
        DrawText("[2] 顯示技能供應牌", 20, screenHeight - 240, 16, WHITE);
        DrawText("[3] 顯示語氣牌堆", 20, screenHeight - 220, 16, WHITE);
        DrawText("[4] 顯示必殺牌", 20, screenHeight - 200, 16, WHITE);
        DrawText("[5] 顯示我方棄牌堆", 20, screenHeight - 180, 16, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}