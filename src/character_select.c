#include "character_select.h"
#include <stdio.h>

static const CharacterInfo character_data[CHARACTER_COUNT] = {
    {CHARACTER_REDHOOD, "小紅帽", "科技射手，擅長遠程攻擊", 30, 6, 15},
    {CHARACTER_SNOWWHITE, "白雪公主", "毒術師，以毒攻敵", 34, 6, 17},
    {CHARACTER_SLEEPINGBEAUTY, "睡美人", "血魔法師，覺醒與沉睡", 42, 6, 21},
    {CHARACTER_ALICE, "愛麗絲", "多重身份，變幻莫測", 32, 6, 16},
    {CHARACTER_MULAN, "花木蘭", "武術大師，以氣為力量", 34, 3, 17},
    {CHARACTER_KAGUYA, "輝夜姬", "防禦專家，以守為攻", 32, 6, 16},
    {CHARACTER_MERMAID, "美人魚", "深海女王，觸手操控", 36, 3, 18},
    {CHARACTER_MATCHGIRL, "火柴女孩", "惡魔契約者，以火為媒", 36, 6, 18},
    {CHARACTER_DOROTHY, "桃樂絲", "機械殺手，連擊專家", 40, 6, 20},
    {CHARACTER_SCHEHERAZADE, "山魯佐德", "命運編織者，故事操控", 36, 6, 18}
};

void init_character_select(CharacterSelectState* state, int num_players) {
    state->num_players = num_players;
    state->current_player = 0;
    state->hover_character = -1;
    
    for (int i = 0; i < MAX_PLAYERS; i++) {
        state->selected_characters[i] = -1;
    }
    
    for (int i = 0; i < CHARACTER_SELECT_COUNT; i++) {
        state->is_character_taken[i] = SDL_FALSE;
    }
}

CharacterInfo get_character_info(CharacterID id) {
    if (id >= 0 && id < CHARACTER_COUNT) {
        return character_data[id];
    }
    return character_data[0]; // 預設返回小紅帽
}

void render_character_select(SDL_Renderer* renderer, TTF_Font* font, CharacterSelectState* state, int screen_width, int screen_height) {
    // 使用螢幕比例計算卡片位置和尺寸
    int card_width = screen_width * 0.2;    // 卡片寬度為螢幕寬度的20%
    int card_height = screen_height * 0.15; // 卡片高度為螢幕高度的15%
    
    // 計算總寬度和總高度來置中排列
    int total_width = CARDS_PER_ROW * card_width + (CARDS_PER_ROW - 1) * (screen_width * 0.05);
    int total_height = 2 * card_height + screen_height * 0.1; // 2行 + 間距
    
    int start_x = (screen_width - total_width) / 2;
    int start_y = (screen_height - total_height) / 2 + screen_height * 0.1; // 向下偏移一點給標題留空間
    
    int spacing_x = card_width + screen_width * 0.05;  // 卡片間距為螢幕寬度的5%
    int spacing_y = card_height + screen_height * 0.1; // 行間距為螢幕高度的10%
    
    // 渲染角色卡片（只顯示選擇的6個角色）
    for (int i = 0; i < CHARACTER_SELECT_COUNT; i++) {
        CharacterID char_id = selectable_characters[i];
        CharacterInfo char_info = get_character_info(char_id);
        
        int col = i % CARDS_PER_ROW;
        int row = i / CARDS_PER_ROW;
        
        int x = start_x + col * spacing_x;
        int y = start_y + row * spacing_y;
        
        SDL_Rect card_rect = {x, y, card_width, card_height};
        
        // 設定卡片顏色
        if (state->is_character_taken[i]) {
            // 已被選擇 - 灰色
            SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        } else if (state->hover_character == i) {
            // 滑鼠懸停 - 亮藍色
            SDL_SetRenderDrawColor(renderer, 100, 150, 255, 255);
        } else {
            // 可選擇 - 深藍色
            SDL_SetRenderDrawColor(renderer, 60, 80, 120, 255);
        }
        
        SDL_RenderFillRect(renderer, &card_rect);
        
        // 卡片邊框
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &card_rect);
        
        // 渲染角色名稱（置中顯示）
        if (font) {
            SDL_Color white = {255, 255, 255, 255};
            SDL_Surface* text_surface = TTF_RenderUTF8_Solid(font, char_info.name, white);
            if (text_surface) {
                SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
                
                int text_w, text_h;
                SDL_QueryTexture(text_texture, NULL, NULL, &text_w, &text_h);
                
                SDL_Rect text_rect = {
                    x + (card_width - text_w) / 2,
                    y + (card_height - text_h) / 2,
                    text_w,
                    text_h
                };
                
                SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
                
                SDL_DestroyTexture(text_texture);
                SDL_FreeSurface(text_surface);
            }
        }
    }
    
    // 渲染目前玩家指示
    if (font) {
        char player_text[50];
        snprintf(player_text, sizeof(player_text), "玩家 %d 選擇角色", state->current_player + 1);
        
        SDL_Color yellow = {255, 255, 0, 255};
        SDL_Surface* player_surface = TTF_RenderUTF8_Solid(font, player_text, yellow);
        if (player_surface) {
            SDL_Texture* player_texture = SDL_CreateTextureFromSurface(renderer, player_surface);
            
            SDL_Rect player_rect = {
                screen_width/2 - player_surface->w/2, 
                screen_height * 0.1, // 螢幕高度的10%位置
                player_surface->w, 
                player_surface->h
            };
            SDL_RenderCopy(renderer, player_texture, NULL, &player_rect);
            
            SDL_DestroyTexture(player_texture);
            SDL_FreeSurface(player_surface);
        }
    }
    
    SDL_RenderPresent(renderer);
}

int handle_character_select_event(SDL_Event* event, CharacterSelectState* state, int screen_width, int screen_height) {
    if (event->type == SDL_MOUSEMOTION) {
        int mouse_x = event->motion.x;
        int mouse_y = event->motion.y;
        
        // 檢查滑鼠是否在某個角色卡片上
        state->hover_character = -1;
        
        // 使用與渲染相同的計算邏輯
        int card_width = screen_width * 0.2;
        int card_height = screen_height * 0.15;
        
        int total_width = CARDS_PER_ROW * card_width + (CARDS_PER_ROW - 1) * (screen_width * 0.05);
        int total_height = 2 * card_height + screen_height * 0.1;
        
        int start_x = (screen_width - total_width) / 2;
        int start_y = (screen_height - total_height) / 2 + screen_height * 0.1;
        
        int spacing_x = card_width + screen_width * 0.05;
        int spacing_y = card_height + screen_height * 0.1;
        
        for (int i = 0; i < CHARACTER_SELECT_COUNT; i++) {
            int col = i % CARDS_PER_ROW;
            int row = i / CARDS_PER_ROW;
            
            int x = start_x + col * spacing_x;
            int y = start_y + row * spacing_y;
            
            if (mouse_x >= x && mouse_x < x + card_width &&
                mouse_y >= y && mouse_y < y + card_height) {
                state->hover_character = i;
                break;
            }
        }
    }
    
    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        if (state->hover_character >= 0 && !state->is_character_taken[state->hover_character]) {
            // 選擇角色（存儲實際的CharacterID）
            state->selected_characters[state->current_player] = selectable_characters[state->hover_character];
            state->is_character_taken[state->hover_character] = SDL_TRUE;
            
            state->current_player++;
            
            // 檢查是否所有玩家都已選擇
            if (state->current_player >= state->num_players) {
                return 1; // 選擇完成
            }
        }
    }
    
    return 0; // 繼續選擇
}

void cleanup_character_select(CharacterSelectState* state) {
    // 目前沒有需要清理的資源
    (void)state; // 避免未使用參數警告
}
