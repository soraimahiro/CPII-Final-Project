#include "ui_component.h"
#include <stdlib.h>
#include <string.h>

extern sUiBase uiBase;

void draw_text(const char* text, int32_t x, int32_t y, SDL_Color color, int32_t font_size) {
    if (!text || !uiBase.font || !uiBase.renderer) return;
    if (TTF_SetFontSize(uiBase.font, font_size) < 0) {
        printf("failed to set font size: %s\n", TTF_GetError());
        return;
    }
    
    SDL_Surface* textSurface = TTF_RenderUTF8_Solid(uiBase.font, text, color);
    if (!textSurface) return;
    
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(uiBase.renderer, textSurface);
    if (!textTexture) return;

    SDL_Rect destRect = {x, y, textSurface->w, textSurface->h};
    SDL_RenderCopy(uiBase.renderer, textTexture, NULL, &destRect);
    
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

void draw_text_center(const char* text, int32_t x, int32_t y, SDL_Color color, int32_t font_size){
    if (!text || !uiBase.font || !uiBase.renderer) return;
    if (TTF_SetFontSize(uiBase.font, font_size) < 0) {
        printf("failed to set font size: %s\n", TTF_GetError());
        return;
    }
    
    SDL_Surface* textSurface = TTF_RenderUTF8_Solid(uiBase.font, text, color);
    if (!textSurface) return;
    
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(uiBase.renderer, textSurface);
    if (!textTexture) return;

    SDL_Rect destRect = {x - textSurface->w/2, y - textSurface->h/2, textSurface->w, textSurface->h};
    SDL_RenderCopy(uiBase.renderer, textTexture, NULL, &destRect);
    
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

sButton *create_button(SDL_Rect rect, char *text, 
                       SDL_Color textColor, SDL_Color textHoverColor,
                       SDL_Color bgColor, SDL_Color bgHoverColor, 
                       SDL_Color borderColor, int32_t fontSize) {
    sButton *button = (sButton*)malloc(sizeof(sButton));
    if (!button) return NULL;
    
    button->rect = rect;
    button->text = malloc(strlen(text));
    if (!button->text) {
        free(button);
        return NULL;
    }
    strncpy(button->text, text, strlen(text));
    
    button->textColor = textColor;
    button->textHoverColor = textHoverColor;
    button->bgColor = bgColor;
    button->bgHoverColor = bgHoverColor;
    button->borderColor = borderColor;
    button->fontSize = fontSize;
    
    return button;
}

void draw_button(sButton *button) {
    if (!button || !uiBase.renderer) return;
    
    bool isHovered = mouse_in_button(button);
    
    // 繪製按鈕背景
    SDL_Color bgColor = isHovered ? button->bgHoverColor : button->bgColor;
    SDL_SetRenderDrawColor(uiBase.renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderFillRect(uiBase.renderer, &button->rect);
    
    // 繪製按鈕邊框
    SDL_Color borderColor = button->borderColor;
    SDL_SetRenderDrawColor(uiBase.renderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
    SDL_RenderDrawRect(uiBase.renderer, &button->rect);
    
    int32_t textX = button->rect.x + button->rect.w/2;
    int32_t textY = button->rect.y + button->rect.h/2;
    SDL_Color textColor = isHovered ? button->textHoverColor : button->textColor;
    draw_text_center(button->text, textX, textY, textColor, button->fontSize);

}

bool mouse_in_button(sButton *button) {
    if (!button) return false;
    
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    
    return (mouseX >= button->rect.x && 
            mouseX < button->rect.x + button->rect.w &&
            mouseY >= button->rect.y && 
            mouseY < button->rect.y + button->rect.h);
}