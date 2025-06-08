#ifndef UI_STAGE_H
#define UI_STAGE_H

#include <stdio.h>
#include <stdint.h>

#include "architecture.h"
#include "ui.h"

void activation_menu(int8_t *active);
void get_active_focus(int32_t *handCard, int32_t *graveCard);

#endif