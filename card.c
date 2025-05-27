#include "card.h"

#define OFFSET 11

static const Card allCards[CARD_TOTAL_COUNT] = {
    {
        .id = CARD_REDHOOD_ATK1_QUICK_SHOT,
        .name = "快速射擊",
        .type = TYPE_ATTACK,
        .level = 1,
        .cost = 0,
        .range = 1,
        .damage = 1,
        .defense = 0,
        .description = "射程1 傷害1+O",
        .characterBound = 0,
        .last_for_turns = 0
    },
    { 12, "精準射擊", TYPE_ATTACK, 2, 2, 2, 2, 0, "O" ,0},
    { 13, "致命狙擊", TYPE_ATTACK, 3, 4, 3, 3, 0, "O" ,0},
    { 14, "能量護盾", TYPE_DEFENSE, 1, 0, 1, 1, 1, "若防禦>0，射程1對手受2傷害" ,0},
    { 15, "電流護盾", TYPE_DEFENSE, 2, 2, 2, 2, 2, "若防禦>0，射程2對手受4傷害" ,0},
    { 16, "終極護盾", TYPE_DEFENSE, 3, 4, 3, 3, 3, "若防禦>0，射程3對手受6傷害" ,0}
};

const Card* getCardData(CardID id) {
    if (id >= 0 && id < CARD_TOTAL_COUNT) {
        printf("%s\n", allCards[id - OFFSET].name);
        return &allCards[id];
    }
    else return NULL;
}


void shuffle(vector* v) {
    srand(time(NULL));
    if (!v || v->SIZE <= 1) return;

    for (uint32_t i = v->SIZE - 1; i > 0; --i) {
        uint32_t j = rand() % (i + 1);

        int32_t temp = v->array[i];
        v->array[i] = v->array[j];
        v->array[j] = temp;
    }
}
