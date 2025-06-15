#ifndef CARD_H
#define CARD_H

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "architecture.h"
#include "card_id.h"

typedef enum {
    TYPE_BASIC,
    TYPE_ATTACK,
    TYPE_DEFENSE,
    TYPE_MOVE,
    TYPE_SPECIAL,
    TYPE_METAMORPH,
} CardType;

typedef struct {
    CardID id;
    const char* name;
    CardType type;
    uint8_t level; // 0 for special
    uint8_t cost;
    uint8_t range;
    uint8_t damage;
    uint8_t defense;
    const char* description;
    uint8_t characterBound; // 255 for universal
    uint8_t last_for_turns;
} Card;

const Card* getCardData(CardID id);
void shuffle(vector* v);
int move_card(vector *dest, vector *src);

// character
typedef struct {
    CharacterID character_id;
    const char* name;
    const char* description;
    uint8_t maxlife;
    uint8_t maxdefense;
    uint8_t ultimate_threshold;
} sCharacterInfo;

sCharacterInfo get_character_info(CharacterID id);
#endif

