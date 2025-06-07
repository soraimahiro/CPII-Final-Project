#include "game.h"
#include "card.h"
#include "stage.h"
#include "ui.h"
#include "ui_stage.h"
#include "architecture.h"

sGame game;
int8_t winner = 0;

int main() {
    init_game();

    sPlayer* pPlayer = malloc(sizeof(sPlayer));
    game.players[0] = *pPlayer;
    pPlayer->character = 0;
    init_character(pPlayer);
    switch (pPlayer->character) {
        case 0:
                init_red_hood(pPlayer);
    }

    int val;
    shuffle(&(pPlayer->attackSkill));
    getVectorTop(&(pPlayer->attackSkill),&val);
    printf("%d\n", val);
    getCardData(val);
}
