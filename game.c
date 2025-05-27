#include "game.h"

extern sGame game;

void init_game() {
    int8_t mode;
    get_init_info(&mode);
    game.playerMode = mode;

    game.now_turn_player_id = 0;

}

void init_character(sPlayer* p){
    p->hand = initVector();
    p->deck = initVector();
    p->usecards = initVector();
    p->graveyard = initVector();
    p->metamorphosis = initVector();
    p->attackSkill = initVector();
    p->defenseSkill = initVector();
    p->moveSkill = initVector();
    p->specialDeck = initVector();
}

void init_red_hood(sPlayer* p) {
    p->maxlife = 30;
    p->life = 30;
    p->defense = 0;
    p->maxdefense = 6;
    p->energy = 3;
    p->specialGate = 15;

    int i = 0;
    for (i = 11; i <= 13; i++) pushbackVector(&p->attackSkill, i);
    for (i = 14; i <= 16; i++) pushbackVector(&p->defenseSkill, i);
    for (i = 17; i <= 19; i++) pushbackVector(&p->moveSkill, i);
    for (i = 20; i <= 22; i++) pushbackVector(&p->specialDeck, i);
    for (i = 135; i <= 138; i++) pushbackVector(&p->metamorphosis, i);


    p->redHood.saveCard[0] = -1;
    p->redHood.saveCard[1] = -1;
    p->redHood.saveCard[2] = -1;

}
