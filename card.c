#include "card.h"

static const Card allCards[CARD_TOTAL_COUNT] = {
    [CARD_REDHOOD_ATK1_QUICK_SHOT] = {
        .id = CARD_REDHOOD_ATK1_QUICK_SHOT,
        .name = "快速射擊",
        .type = TYPE_ATTACK,
        .level = 1,
        .cost = 0,
        .range = 1,
        .damage = 1,
        .defense = 0,
        .description = "射程1 傷害1+O",
        .characterBound = 0
    },
    [CARD_REDHOOD_ATK2_PRECISION_SHOT] = {
        .id = CARD_REDHOOD_ATK2_PRECISION_SHOT,
        .name = "精準射擊",
        .type = TYPE_ATTACK,
        .level = 2,
        .cost = 2,
        .range = 2,
        .damage = 2,
        .defense = 0,
        .description = "射程2 傷害2+O",
        .characterBound = 0
    }
};

const Card* getCardData(CardID id) {
    if (id >= 0 && id < CARD_TOTAL_COUNT) {
        return &allCards[id];
    }
    return NULL;
}

