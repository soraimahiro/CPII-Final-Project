#ifndef _SKILLS_H
#define _SKILLS_H

#include "game.h"
#include "card.h"
#include "card_id.h"
#include "stage.h"
#include "ui.h"
#include "ui_stage.h"
#include "ui_component.h"
#include "architecture.h"

int handle_redhood_skills(sPlayer* attacker, sPlayer* defender, const Card* skill_card, uint8_t level);
int handle_redhood_ultimate(sPlayer* attacker, sPlayer* defender, const Card* ultimate_card);

int handle_snowwhite_skills(sPlayer* attacker, sPlayer* defender, const Card* skill_card, uint8_t level);
int handle_snowwhite_ultimate(sPlayer* attacker, sPlayer* defender, const Card* ultimate_card);

int handle_sleepingbeauty_skills(sPlayer* attacker, sPlayer* defender, const Card* skill_card, uint8_t level);
int handle_sleepingbeauty_ultimate(sPlayer* attacker, sPlayer* defender, const Card* ultimate_card);

int handle_mulan_skills(sPlayer* attacker, sPlayer* defender, const Card* skill_card, uint8_t level);
int handle_mulan_ultimate(sPlayer* attacker, sPlayer* defender, const Card* ultimate_card);
int ki(sPlayer* player);

int handle_kaguya_skills(sPlayer* attacker, sPlayer* defender, const Card* skill_card, uint8_t level);

int handle_matchgirl_skills(sPlayer* attacker, sPlayer* defender, const Card* skill_card, uint8_t level);

void knockback(sPlayer* attacker, sPlayer* defender, int distance);
int put_posion(sPlayer* attacker, sPlayer* defender, vector *dest);

#endif