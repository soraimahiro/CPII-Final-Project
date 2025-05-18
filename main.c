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

    int8_t mode;
    get_init_info(&mode);
    pGame->playerMode = mode;

    //pGame->players[0].character = 0;

    init_game(pGame);

    player* pPlayer = malloc(sizeof(player));
    pGame->players[0] = *pPlayer;
    pPlayer->character = 0;
    //int8_t character = pGame->players[0].character;
    init_character(pPlayer);
    switch (pPlayer->character) {
        case 0:
                init_red_hood(pPlayer);
    }

    int val;
    getVectorTop(&(pPlayer->attackSkill),&val);
    printf("%d\n", val);
    getCardData(val);
}
