#include "skills.h"

void knockback(sPlayer* attacker, sPlayer* defender, int distance) {
    int knock_distance;
    int is_pull = (distance < 0);
    int max_distance = is_pull ? -distance : distance;
    
    while (1) {
        printf("%s多少(1-%d): ", is_pull ? "拉近" : "擊退", max_distance);
        scanf("%d", &knock_distance);
        
        if (knock_distance <= 0) {
            printf("距離必須大於0！\n");
            continue;
        }
        if (knock_distance > max_distance) {
            printf("距離不能超過%d！\n", max_distance);
            continue;
        }
        break;
    }
    
    int direction = (defender->locate[0] > attacker->locate[0]) ? 1 : -1;
    if (is_pull) direction = -direction;  // 拉近時方向相反
    
    int new_x = defender->locate[0] + (direction * knock_distance);
    
    // 確保不會穿過或重疊攻擊者位置
    if (is_pull) {
        int attacker_x = attacker->locate[0];
        if ((direction > 0 && new_x >= attacker_x) || 
            (direction < 0 && new_x <= attacker_x)) {
            new_x = attacker_x - direction;  // 停在攻擊者旁邊一格
        }
    }
    
    // 確保在場地範圍內
    if (new_x < 1) new_x = 1;
    if (new_x > 9) new_x = 9;

    defender->locate[0] = new_x;
}

void discard_cards(sPlayer* player, int value) {
    while (value > 0 && player->hand.SIZE > 0) {
        printf("\n需要棄掉 %d 張牌\n", value);
        print_hand_cards(player);
        
        printf("\n選擇要棄掉的牌 (1-%d): ", player->hand.SIZE);
        int choice;
        scanf("%d", &choice);
        
        if (choice < 1 || choice > player->hand.SIZE) {
            printf("無效的選擇！\n");
            continue;
        }
        
        // 取得選擇的牌並移到棄牌堆
        int32_t card_id = player->hand.array[choice - 1];
        pushbackVector(&player->graveyard, card_id);
        eraseVector(&player->hand, choice - 1);
        value--;
    }
}

int handle_redhood_skills(sPlayer* attacker, sPlayer* defender, const Card* skill_card, uint8_t level) {
    int extra_damage = 0;
    int extra_range = 0;
    if (skill_card->type == TYPE_ATTACK || skill_card->type == TYPE_MOVE) {
        if (countCard(&attacker->metamorphosis, CARD_REDHOOD_METAMORPH1_OVERLOAD_BURN)) { // check 過載燃燒
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
        }
        if (countCard(&attacker->metamorphosis, CARD_REDHOOD_METAMORPH3_MUTATION_SENSE)) { // check 變異感應
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
        }
        int distance = abs(attacker->locate[0] - defender->locate[0]);
        switch (skill_card->id) {
            case CARD_REDHOOD_MOVE1_JET:
            case CARD_REDHOOD_MOVE2_FIRE_JET:
            case CARD_REDHOOD_MOVE3_RAGE_JET: {
                if (distance > skill_card->level + extra_range) return -1;
                else {
                    int total_damage = skill_card->level + extra_damage;
                    attack(defender, total_damage);
                    attacker->redHood.saveAtk = total_damage;
                    knockback(attacker, defender, skill_card->level);
                    attacker->redHood.saveKnock = skill_card->level;
                    return 0;
                }
            }
            case CARD_REDHOOD_DEF1_ENERGY_SHIELD:
            case CARD_REDHOOD_DEF2_CURRENT_SHIELD:
            case CARD_REDHOOD_DEF3_ULTIMATE_SHIELD: {
                if (distance > skill_card->level + extra_range) return -1;
                else {
                    attack(defender, skill_card->level);
                    defend(attacker, skill_card->level);
                    return 0;
                }
            }
            case CARD_REDHOOD_ATK1_QUICK_SHOT:
            case CARD_REDHOOD_ATK2_PRECISION_SHOT:
            case CARD_REDHOOD_ATK3_FATAL_SHOT: {
                if (distance > skill_card->level + extra_range) return -1;
                else {
                    int total_damage = skill_card->level + level + extra_damage;
                    attack(defender, total_damage);
                    attacker->redHood.saveAtk = total_damage;
                    attacker->redHood.saveKnock = 0;
                    return 0;
                }
            }
            default:
                return -1;
        }
    }
    return 0;
}
int handle_redhood_ultimate(sPlayer* attacker, sPlayer* defender, const Card* ultimate_card) {
    switch (ultimate_card->id) {
        case CARD_REDHOOD_SPECIAL1_WOLF_HUNGER:
            // get_2_cards_for_free();
            return 0;
        case CARD_REDHOOD_SPECIAL2_SYSTEM_HACK:
            if (attacker->usecards.SIZE == 0) {
                printf("本回合尚未使用任何牌！\n");
                return -1;
            }
            int32_t card_id = attacker->usecards.array[attacker->usecards.SIZE - 1];
            const Card* card = getCardData(card_id);
            if (card_id <= 10) {  // 基本牌
                switch (card->type) {
                    case TYPE_ATTACK:
                        attack(defender, card->level);
                        break;
                    case TYPE_DEFENSE:
                        defend(attacker, card->level);
                        break;
                    case TYPE_MOVE:
                        move(attacker, card->level);
                        break;
                    default:
                        printf("選擇要執行的效果：\n");
                        printf("1. 攻擊 (傷害1)\n");
                        printf("2. 防禦\n");
                        printf("3. 移動\n");
                        int choice;
                        scanf("%d", &choice);
                        switch (choice) {
                            case 1:
                                attack(defender, 1);  // lv1 攻擊
                                break;
                            case 2:
                                defend(attacker, 1);
                                break;
                            case 3:
                                move(attacker, 1);
                                break;
                        }
                        break;
                }
                return 0;
            }
            else if (card_id >= 11 && card_id <= 13 || (card_id >= 17 && card_id <= 19)) {
                attack(defender, attacker->redHood.saveAtk);
                if (attacker->redHood.saveKnock) knockback(attacker, defender, attacker->redHood.saveKnock);
                return 0;
            }
            else {
                printf("剛剛的行動不是基本牌、攻擊或移動技能\n");
                return -1;
            }
            return 0;
        case CARD_REDHOOD_SPECIAL3_RAIN_OF_VENGEANCE:
            if (abs(attacker->locate[0] - defender->locate[0]) > 3) return -1;
            attack(defender, 3);
            knockback(attacker, defender, 3);
            discard_cards(defender, 3);
            return 0;
        default:
            break;
    }
    return 0;
}

int handle_sleepingbeauty_skills(sPlayer* attacker, sPlayer* defender, const Card* skill_card, uint8_t level) {
    int cost_token = 0;
    // TODO: 是否花費 TOKEN
    int distance = abs(attacker->locate[0] - defender->locate[0]);
    switch (skill_card->id) {
        case CARD_SLEEPINGBEAUTY_ATK1_MIND_SHOCK:
        case CARD_SLEEPINGBEAUTY_ATK2_MIND_RAGE:
        case CARD_SLEEPINGBEAUTY_ATK3_MIND_FURY: {
            if (abs(attacker->locate[0] - defender->locate[0]) == 1) {
                int choice;
                printf("犧牲血量(0-%d): ", skill_card->level);
                scanf("%d", &choice);
                attack(attacker, choice);
                attack(defender, skill_card->level * level + choice);
                return 0;
            }
            else return -1;
        }
        case CARD_SLEEPINGBEAUTY_DEF1_BURST_CHAIN:
        case CARD_SLEEPINGBEAUTY_DEF2_BURST_BONE:
        case CARD_SLEEPINGBEAUTY_DEF3_BURST_SOUL: {
            attacker->sleepingBeauty.atkRise = skill_card->level;
            attacker->sleepingBeauty.atkRiseTime = cost_token + level;
            return 0;
        }
        case CARD_SLEEPINGBEAUTY_MOVE1_DARK_TOUCH:
        case CARD_SLEEPINGBEAUTY_MOVE2_DARK_ENTANGLE:
        case CARD_SLEEPINGBEAUTY_MOVE3_DARK_STRANGLE: {
            if (distance > skill_card->level + 1) return -1;
            knockback(attacker, defender, (-1 * level));
            attack(defender, level + cost_token);
            return 0;
        }
        default:
            return -1;
    }
}

int handle_sleepingbeauty_ultimate(sPlayer* attacker, sPlayer* defender, const Card* ultimate_card) {
    switch (ultimate_card->id) {
        case CARD_SLEEPINGBEAUTY_METAMORPH4_FORCED_TREATMENT:
            attacker->life += 5;
            if (attacker->life > attacker->maxlife) attacker->life = attacker->maxlife;
            return 0;
        case CARD_SLEEPINGBEAUTY_SPECIAL1_AWAKEN_SLEEP:
            attacker->sleepingBeauty.AWAKEN_TOKEN += 3;
            if (attacker->sleepingBeauty.AWAKEN_TOKEN > 9) attacker->sleepingBeauty.AWAKEN_TOKEN = 9;
            if (attacker->sleepingBeauty.AWAKEN_TOKEN < 6 && !attacker->sleepingBeauty.AWAKEN) {
                int choice;
                printf("是否進入覺醒狀態(1/0): ");
                scanf("%d", &choice);
                if (choice) attacker->sleepingBeauty.AWAKEN = 1;
            }
            else attacker->sleepingBeauty.AWAKEN = 1;
            return 0;
        case CARD_SLEEPINGBEAUTY_SPECIAL2_DAYMARE:
            attacker->sleepingBeauty.dayNightmareDrawRemind = 0;
            return 0;
        case CARD_SLEEPINGBEAUTY_SPECIAL3_BLOOD_RECAST:
            attacker->life += attacker->sleepingBeauty.AWAKEN_TOKEN;
            if (attacker->life > attacker->maxlife) attacker->life = attacker->maxlife;
            attacker->sleepingBeauty.AWAKEN_TOKEN = 0;
            attacker->sleepingBeauty.AWAKEN = 0;
            return 0;
        default:
            return -1;
    }
}

int handle_mulan_skills(sPlayer* attacker, sPlayer* defender, const Card* skill_card, uint8_t level) {
    return 0;
}

int ki(sPlayer* player) {
    // 檢查手牌
    if (player->hand.SIZE == 0) {
        printf("沒有手牌可以捨棄！\n");
        return 0;
    }

    // 顯示手牌
    printf("\n選擇要捨棄的防禦或通用牌：\n");
    print_hand_cards(player);

    // 選擇要捨棄的牌
    while (1) {
        printf("\n選擇要捨棄的牌 (1-%d) 或 0 取消: ", player->hand.SIZE);
        int choice;
        scanf("%d", &choice);

        if (choice == 0) {
            return 0;
        }

        if (choice < 1 || choice > player->hand.SIZE) {
            printf("無效的選擇！\n");
            continue;
        }

        // 取得選擇的牌
        int32_t card_id = player->hand.array[choice - 1];
        const Card* card = getCardData(card_id);

        // 檢查是否為防禦或通用牌
        if (card->type != TYPE_DEFENSE && card->type != TYPE_BASIC) {
            printf("只能捨棄防禦或通用牌！\n");
            continue;
        }

        // 計算可抵擋的傷害和獲得的 KI token
        int ki_value = card->defense;
        
        // 捨棄牌並獲得 KI token
        pushbackVector(&player->graveyard, card_id);
        eraseVector(&player->hand, choice - 1);
        player->mulan.KI_TOKEN += ki_value;
        if (player->mulan.KI_TOKEN > 12) player->mulan.KI_TOKEN = 12;
        return ki_value;
    }
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