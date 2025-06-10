#ifndef CHARACTER_SELECT_H
#define CHARACTER_SELECT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "card_id.h"

#define MAX_PLAYERS 4
#define CARDS_PER_ROW 3

// 選擇畫面顯示的角色ID列表
static const CharacterID selectable_characters[CHARACTER_SELECT_COUNT] = {
    CHARACTER_REDHOOD,       // 小紅帽
    CHARACTER_MULAN,         // 花木蘭
    CHARACTER_SNOWWHITE,     // 白雪公主
    CHARACTER_SLEEPINGBEAUTY,// 睡美人
    CHARACTER_ALICE,         // 愛麗絲
    CHARACTER_KAGUYA        // 輝夜姬
};

typedef struct {
    CharacterID character_id;
    const char* name;
    const char* description;
    int health;
    int shield_max;
    int ultimate_threshold;
} CharacterInfo;

typedef struct {
    int selected_characters[MAX_PLAYERS];
    int current_player;
    int num_players;
    int hover_character;
    SDL_bool is_character_taken[CHARACTER_SELECT_COUNT];
} CharacterSelectState;

// 函數聲明
void init_character_select(CharacterSelectState* state, int num_players);
void render_character_select(SDL_Renderer* renderer, TTF_Font* font, CharacterSelectState* state, int screen_width, int screen_height);
int handle_character_select_event(SDL_Event* event, CharacterSelectState* state, int screen_width, int screen_height);
void cleanup_character_select(CharacterSelectState* state);
CharacterInfo get_character_info(CharacterID id);

#endif
