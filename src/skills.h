#ifndef _SKILLS_H
#define _SKILLS_H

#include "game.h"
#include "card.h"
#include "stage.h"
#include "ui.h"
#include "ui_stage.h"
#include "ui_component.h"
#include "architecture.h"

void handle_redhood_skills(const Card* skill_card, uint8_t level);
void handle_snowwhite_skills(const Card* skill_card, uint8_t level);
void handle_sleepingbeauty_skills(const Card* skill_card, uint8_t level);
void handle_mulan_skills(const Card* skill_card, uint8_t level);
void handle_kaguya_skills(const Card* skill_card, uint8_t level);
void handle_matchgirl_skills(const Card* skill_card, uint8_t level);

#endif