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
int handle_snowwhite_skills(sPlayer* attacker, sPlayer* defender, const Card* skill_card, uint8_t level);
int handle_sleepingbeauty_skills(sPlayer* attacker, sPlayer* defender, const Card* skill_card, uint8_t level);
int handle_mulan_skills(sPlayer* attacker, sPlayer* defender, const Card* skill_card, uint8_t level);
int handle_kaguya_skills(sPlayer* attacker, sPlayer* defender, const Card* skill_card, uint8_t level);
int handle_matchgirl_skills(sPlayer* attacker, sPlayer* defender, const Card* skill_card, uint8_t level);

#endif