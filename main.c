#include <stdio.h>
#include "game.h"
#include "stage.h"
#include "ui.h"
#include "architecture.h"

int main() {
    game* pGame;

    int8_t* mode;
    get_init_info(mode);
    pGame->playerMode = mode;

    init_game(pGame);


}
