/*
回合開始階段 beginning phase
清理階段 refresh phase
行動階段 activation phase:
    專注 focus
    攻擊 attack action
    防禦 defense action
    移動 move action
    使用一個技能 use a skill
    使用一張必殺 use an epic card
    購買一張牌 power up action
    其他行動（如被動等等）component action
結束階段 ending phase
*/

#ifndef STAGE_H
#define STAGE_H
#include <stdint.h>
#include <stdbool.h>

#include "architecture.h"
#include "card_id.h"
#include "ui/ui_stage.h"

int beginning_phase();
int refresh_phase();
int ending_phase();

// action
int focus();
#endif
