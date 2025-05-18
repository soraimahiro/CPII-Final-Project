#include "game.h"
#include "card.h"
#include "stage.h"
#include "ui.h"
#include "architecture.h"

int main() {
    game* pGame = malloc(sizeof(game));
    if (!pGame) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    init_game(pGame);

    player* pPlayer = malloc(sizeof(player));
    pGame->players[0] = *pPlayer;
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
