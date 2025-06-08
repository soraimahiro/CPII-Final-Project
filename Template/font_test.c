#include "raylib.h"
#include <stdio.h>

int main() {
    printf("Font Test Program\n");
    printf("=================\n");
    
    InitWindow(400, 300, "Font Test");
    
    const char* fontPaths[] = {
        "assets/fonts/game.ttf",
        "./assets/fonts/game.ttf", 
        "../assets/fonts/game.ttf",
        "fonts/game.ttf",
        "./fonts/game.ttf",
        "game.ttf"
    };
    
    int pathCount = sizeof(fontPaths) / sizeof(fontPaths[0]);
    
    for (int i = 0; i < pathCount; i++) {
        printf("Testing path: %s\n", fontPaths[i]);
        
        Font testFont = LoadFontEx(fontPaths[i], 18, 0, 0);
        if (testFont.texture.id != 0 && testFont.texture.id != GetFontDefault().texture.id) {
            printf("  ✓ SUCCESS: Font loaded!\n");
            UnloadFont(testFont);
        } else {
            printf("  ✗ FAILED: Could not load font\n");
            if (testFont.texture.id != 0) {
                UnloadFont(testFont);
            }
        }
    }
    
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Check console for font test results", 10, 10, 20, BLACK);
        DrawText("Press ESC to close", 10, 40, 20, GRAY);
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}
