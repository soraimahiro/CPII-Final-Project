#include "game.h"
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

    init_game(pGame);


}
