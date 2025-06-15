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

void draw_text_wrapped(const char* text, int32_t x, int32_t y, SDL_Color color, int32_t font_size, int32_t max_width) {
    if (!text || !uiBase.font || !uiBase.renderer) return;
    if (TTF_SetFontSize(uiBase.font, font_size) < 0) {
        printf("failed to set font size: %s\n", TTF_GetError());
        return;
    }
    
    // For now, just use regular draw_text without wrapping
    // A full implementation would need to break text into multiple lines
    draw_text(text, x, y, color, font_size);
}

sButton *create_button(SDL_Rect rect, const char *text, 
                       SDL_Color *textColor, SDL_Color *bgColor, 
                       SDL_Color *borderColor, int32_t fontSize, int32_t typeNumber) {
    sButton *pButton = calloc(1, sizeof(sButton));
    if (!pButton) return NULL;
    
    pButton->rect = rect;
    pButton->text = malloc(strlen(text) + 1);
    if (!pButton->text) {
        free(pButton);
        return NULL;
    }
    strcpy(pButton->text, text);
    
    pButton->textColor = calloc(typeNumber, sizeof(SDL_Color));
    if (!pButton->textColor) {
        free(pButton->text);
        free(pButton);
        return NULL;
    }
    memcpy(pButton->textColor, textColor, typeNumber * sizeof(SDL_Color));
    
    pButton->bgColor = calloc(typeNumber, sizeof(SDL_Color));
    if (!pButton->bgColor) {
        free(pButton->textColor);
        free(pButton->text);
        free(pButton);
        return NULL;
    }
    memcpy(pButton->bgColor, bgColor, typeNumber * sizeof(SDL_Color));
    
    pButton->borderColor = calloc(typeNumber, sizeof(SDL_Color));
    if (!pButton->borderColor) {
        free(pButton->bgColor);
        free(pButton->textColor);
        free(pButton->text);
        free(pButton);
        return NULL;
    }
    memcpy(pButton->borderColor, borderColor, typeNumber * sizeof(SDL_Color));
    
    pButton->fontSize = fontSize;
    pButton->typeNumber = typeNumber;
    
    return pButton;
}

void free_button(sButton *pButton){
    if(!pButton) return;
    free(pButton->textColor);
    free(pButton->bgColor);
    free(pButton->borderColor);
    free(pButton->text);
    free(pButton);
}

void draw_button(sButton *pButton, int8_t type) {
    if (!pButton || !uiBase.renderer){
        printf("draw button error!");
        return;
    }
    if(type >= pButton->typeNumber){
        type = pButton->typeNumber-1;
    }
    
    SDL_Color bgColor = pButton->bgColor[type];
    SDL_Color borderColor = pButton->borderColor[type];
    SDL_Color textColor = pButton->textColor[type];
    
    SDL_SetRenderDrawColor(uiBase.renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderFillRect(uiBase.renderer, &pButton->rect);
    
    SDL_SetRenderDrawColor(uiBase.renderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
    SDL_RenderDrawRect(uiBase.renderer, &pButton->rect);
    
    if(strlen(pButton->text) > 0){
        int32_t textX = pButton->rect.x + pButton->rect.w/2;
        int32_t textY = pButton->rect.y + pButton->rect.h/2;
        draw_text_center(pButton->text, textX, textY, textColor, pButton->fontSize);
    }
}

bool mouse_in_button(sButton *pButton) {
    if (!pButton) return false;
    
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    
    return (mouseX >= pButton->rect.x && 
            mouseX < pButton->rect.x + pButton->rect.w &&
            mouseY >= pButton->rect.y && 
            mouseY < pButton->rect.y + pButton->rect.h);
}