#include "skills.h"

void knockback(sPlayer* attacker, sPlayer* defender, int distance) {
    int direction = (defender->locate[0] > attacker->locate[0]) ? 1 : -1;
    
    int new_x = defender->locate[0] + (direction * distance);
    
    if (new_x < 1) new_x = 1;
    if (new_x > 9) new_x = 9;

    defender->locate[0] = new_x;
}

int handle_redhood_skills(sPlayer* attacker, sPlayer* defender, const Card* skill_card, uint8_t level) {
    int extra_damage = 0;
    int extra_range = 0;
    if (skill_card->type == TYPE_ATTACK ||skill_card->type == TYPE_MOVE) { // check過載燃燒 and 變異感應
        printf("可以捨棄至多一張技能牌來獲得傷害+X\n");
        print_hand_cards(attacker);
        while (1) {
            printf("\nChoose a skill card (1-%d) or 0 to skip: ", attacker->hand.SIZE);
            int choice;
            scanf("%d", &choice);
            if (choice == 0) break;
            if (choice < 1 || choice > attacker->hand.SIZE) {
                printf("Invalid choice!\n");
                continue;
            }
            // Get the selected card
            int32_t card_id = attacker->hand.array[choice - 1];
            const Card* card = getCardData(card_id);

            if (card->id <= 10 || (card->id - 11) % 12 > 9) {
                printf("Not a skill card!\n");
                continue;
            }
            else {
                extra_damage = card->level;
                pushbackVector(&attacker->usecards, card_id);
                eraseVector(&attacker->hand, choice - 1);
                break;
            }
        }

        printf("可以捨棄至多一張技能牌來獲得射程+X\n");
        print_hand_cards(attacker);
        while (1) {
            printf("\nChoose a skill card (1-%d) or 0 to skip: ", attacker->hand.SIZE);
            int choice;
            scanf("%d", &choice);
            if (choice == 0) break;
            if (choice < 1 || choice > attacker->hand.SIZE) {
                printf("Invalid choice!\n");
                continue;
            }
            // Get the selected card
            int32_t card_id = attacker->hand.array[choice - 1];
            const Card* card = getCardData(card_id);

            if (card->id <= 10 || (card->id - 11) % 12 > 9) {
                printf("Not a skill card!\n");
                continue;
            }
            else {
                extra_range = card->level;
                pushbackVector(&attacker->usecards, card_id);
                eraseVector(&attacker->hand, choice - 1);
                break;
            }
        }
        
        switch (skill_card->id) {
            case CARD_REDHOOD_ATK1_QUICK_SHOT:
            case CARD_REDHOOD_ATK2_PRECISION_SHOT:
            case CARD_REDHOOD_ATK3_FATAL_SHOT: {
                int range = abs(attacker->locate[0] - defender->locate[0]);
                if (range > skill_card->level + extra_range) return -1;
                else {
                    int total_damage = skill_card->level + level + extra_damage;
                    attack(defender, total_damage);
                    int distance;
                    printf("擊退多少(max: %u): ", skill_card->level);
                    scanf("%d", &distance);
                    knockback(attacker, defender, distance);
                    return 0;
                }
            }
            case CARD_REDHOOD_MOVE1_JET:
            case CARD_REDHOOD_MOVE2_FIRE_JET:
            case CARD_REDHOOD_MOVE3_RAGE_JET: {
                int range = abs(attacker->locate[0] - defender->locate[0]);
                if (range > skill_card->level + extra_range) return -1;
                else {
                    int total_damage = skill_card->level + extra_damage;
                    attack(defender, total_damage);
                    return 0;
                }
            }
            default:
                break;
        }
    }
    return 0;
}

int handle_sleepingbeauty_skills(sPlayer* attacker, sPlayer* defender, const Card* skill_card, uint8_t level) {
    return 0;
}

int handle_mulan_skills(sPlayer* attacker, sPlayer* defender, const Card* skill_card, uint8_t level) {
    return 0;
}

int handle_snowwhite_skills(sPlayer* attacker, sPlayer* defender, const Card* skill_card, uint8_t level) {
    return 0;
}

int handle_kaguya_skills(sPlayer* attacker, sPlayer* defender, const Card* skill_card, uint8_t level) {
    return 0;
}

int handle_matchgirl_skills(sPlayer* attacker, sPlayer* defender, const Card* skill_card, uint8_t level) {
    return 0;
}