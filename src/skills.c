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

int put_posion(sPlayer* attacker, sPlayer* defender, vector *dest){
    move_card(&attacker->snowWhite.remindPosion, dest);
    defender->life -= attacker->snowWhite.meta4;
}

int handle_snowwhite_skills(sPlayer* attacker, sPlayer* defender, const Card* skill_card, uint8_t level) {
    switch(skill_card->id){
        case CARD_SNOWWHITE_ATK1_CRYSTAL_SHARD:
            if(abs(attacker->locate[0] - defender->locate[0]) >= 1){
                return -1;
            }
            attack(defender, 1 + level);
            move_card(&defender->deck, &defender->graveyard);
            break;
        case CARD_SNOWWHITE_ATK2_CRYSTAL_VORTEX:
            if(abs(attacker->locate[0] - defender->locate[0]) >= 1){
                return -1;
            }
            attack(defender, 2 + level);
            for(int32_t i = 0; i < 2; i++){
                move_card(&defender->deck, &defender->graveyard);
            }
            break;
        case CARD_SNOWWHITE_ATK3_CRYSTAL_STORM:
            if(abs(attacker->locate[0] - defender->locate[0]) >= 1){
                return -1;
            }
            attack(defender, 3 + level);
            for(int32_t i = 0; i < 3; i++){
                move_card(&defender->deck, &defender->graveyard);
            }
            break;
        case CARD_SNOWWHITE_DEF1_TAINTED_GRACE:
            if(abs(attacker->locate[0] - defender->locate[0]) >= 1){
                return -1;
            }
            int32_t posionCardNumber;
            printf("How many posion card you want to put into opponent's graveyard(0~%d): ", level);
            scanf("%d", &posionCardNumber);
            while(1){
                if(posionCardNumber >= 0 && posionCardNumber <= level){
                    break;
                }
                else{
                    printf("error, input again: ");
                    scanf("%d", &posionCardNumber);
                }
            }
            attack(defender, 1);
            char c = 'n';
            if(posionCardNumber >= 1){
                printf("Do you wnat to put one into deck(y/n): ");
                scanf("%c", &c);
                if(c == 'y'){
                    put_posion(attacker, defender, &defender->deck);
                    for(int32_t i = 1; i < posionCardNumber; i++){
                        put_posion(attacker, defender, &defender->graveyard);
                    }
                }
            }
            break;
        case CARD_SNOWWHITE_DEF2_TAINTED_FEAST:
            if(abs(attacker->locate[0] - defender->locate[0]) >= 1){
                return -1;
            }
            int32_t posionCardNumber;
            printf("How many posion card you want to put into opponent's graveyard(0~%d): ", level);
            scanf("%d", &posionCardNumber);
            while(1){
                if(posionCardNumber >= 0 && posionCardNumber <= level){
                    break;
                }
                else{
                    printf("error, input again: ");
                    scanf("%d", &posionCardNumber);
                }
            }
            attack(defender, 2);
            char c = 'n';
            if(posionCardNumber >= 1){
                printf("Do you wnat to put one into deck(y/n): ");
                scanf("%c", &c);
                if(c == 'y'){
                    put_posion(attacker, defender, &defender->deck);
                    for(int32_t i = 1; i < posionCardNumber; i++){
                        put_posion(attacker, defender, &defender->graveyard);
                    }
                }
            }
            break;
        case CARD_SNOWWHITE_DEF3_TAINTED_CARNIVAL:
            if(abs(attacker->locate[0] - defender->locate[0]) >= 1){
                return -1;
            }
            int32_t posionCardNumber;
            printf("How many posion card you want to put into opponent's graveyard(0~%d): ", level);
            scanf("%d", &posionCardNumber);
            while(1){
                if(posionCardNumber >= 0 && posionCardNumber <= level){
                    break;
                }
                else{
                    printf("error, input again: ");
                    scanf("%d", &posionCardNumber);
                }
            }
            attack(defender, 3);
            char c = 'n';
            if(posionCardNumber >= 1){
                printf("Do you wnat to put one into deck(y/n): ");
                scanf("%c", &c);
                if(c == 'y'){
                    put_posion(attacker, defender, &defender->deck);
                    for(int32_t i = 1; i < posionCardNumber; i++){
                        put_posion(attacker, defender, &defender->graveyard);
                    }
                }
            }
            break;
        case CARD_SNOWWHITE_MOVE1_BROKEN_FANTASY:
            if(abs(attacker->locate[0] - defender->locate[0]) >= level){
                return -1;
            }
            attack(defender, 1);
            int32_t dest = 0;
            int32_t left = defender->locate[0]-1;
            int32_t right = defender->locate[0]+1;
            if(left < 1){
                dest = right;
            }
            else if(right > 9){
                dest = left;
            }
            else{
                printf("Opponent is on position %d, where do you wnat to go, %d or %d: ", defender->locate[0], left, right);
                scanf("%d", &dest);
                while(1){
                    if(dest >= 1 && dest <= 9){
                        break;
                    }
                    else{
                        printf("error, input again: ");
                        scanf("%d", &dest);
                    }
                }
            }
            move(attacker, dest - attacker->locate[0]);
            break;
        case CARD_SNOWWHITE_MOVE2_BROKEN_REALITY:
            if(abs(attacker->locate[0] - defender->locate[0]) >= 1 + level){
                return -1;
            }
            attack(defender, 2);
            int32_t dest = 0;
            int32_t left = defender->locate[0]-1;
            int32_t right = defender->locate[0]+1;
            if(left < 1){
                dest = right;
            }
            else if(right > 9){
                dest = left;
            }
            else{
                printf("Opponent is on position %d, where do you wnat to go, %d or %d: ", defender->locate[0], left, right);
                scanf("%d", &dest);
                while(1){
                    if(dest >= 1 && dest <= 9){
                        break;
                    }
                    else{
                        printf("error, input again: ");
                        scanf("%d", &dest);
                    }
                }
            }
            move(attacker, dest - attacker->locate[0]);
            break;
        case CARD_SNOWWHITE_MOVE3_BROKEN_FATE:
            if(abs(attacker->locate[0] - defender->locate[0]) >= 2 + level){
                return -1;
            }
            attack(defender, 3);
            int32_t dest = 0;
            int32_t left = defender->locate[0]-1;
            int32_t right = defender->locate[0]+1;
            if(left < 1){
                dest = right;
            }
            else if(right > 9){
                dest = left;
            }
            else{
                printf("Opponent is on position %d, where do you wnat to go, %d or %d: ", defender->locate[0], left, right);
                scanf("%d", &dest);
                while(1){
                    if(dest >= 1 && dest <= 9){
                        break;
                    }
                    else{
                        printf("error, input again: ");
                        scanf("%d", &dest);
                    }
                }
            }
            move(attacker, dest - attacker->locate[0]);
            break;
        case CARD_SNOWWHITE_SPECIAL1_SEVEN_SNAKES_RAGE:
            if(abs(attacker->locate[0] - defender->locate[0]) >= 1){
                return -1;
            }
            int32_t damage = 0;
            for(int32_t i = 0; i < defender->graveyard.SIZE ; i++){
                if(defender->graveyard.array[i] >= 131 && defender->graveyard.array[i] <= 133){
                    damage++;
                }
            }
            defender->life -= damage;
            break;
        case CARD_SNOWWHITE_SPECIAL2_MAGIC_MIRROR_RAIN:
            if(abs(attacker->locate[0] - defender->locate[0]) >= 1){
                return -1;
            }
            attack(defender, 3);
            while(defender->hand.SIZE > 0){
                move_card(&defender->hand, &defender->graveyard);
            }
            draw_card(defender, 4);
            break;
        case CARD_SNOWWHITE_SPECIAL3_BREWED_DISASTER:
            if(abs(attacker->locate[0] - defender->locate[0]) >= 3){
                return -1;
            }
            attack(defender, 3);
            int32_t card = 0;
            printf("choose first card from oppoent's graveyard to move into deck(0~%d): ", defender->deck.SIZE-1);
            scanf("%d", &card);
            pushbackVector(&defender->deck, defender->graveyard.array[card]);
            eraseVector(&defender->graveyard, card);

            printf("choose second card from oppoent's graveyard to move into deck(0~%d): ", defender->deck.SIZE-1);
            scanf("%d", &card);
            pushbackVector(&defender->deck, defender->graveyard.array[card]);
            eraseVector(&defender->graveyard, card);

            printf("choose third card from oppoent's graveyard to move into deck(0~%d): ", defender->deck.SIZE-1);
            scanf("%d", &card);
            pushbackVector(&defender->deck, defender->graveyard.array[card]);
            eraseVector(&defender->graveyard, card);
            break;
        default:
            break;
    }
    return 0;
}

int handle_kaguya_skills(sPlayer* attacker, sPlayer* defender, const Card* skill_card, uint8_t level) {
    return 0;
}

int handle_matchgirl_skills(sPlayer* attacker, sPlayer* defender, const Card* skill_card, uint8_t level) {
    return 0;
}