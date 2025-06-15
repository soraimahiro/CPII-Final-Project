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

int place(sPlayer* attacker, sPlayer* defender) {
    int left = attacker->locate[0] - 1;
    int right = attacker->locate[0] + 1;
    
    printf("選擇要放置的位置：\n");
    int lf = 0;
    if (left >= 1 && left != defender->locate[0]) {
        printf("0. 位置 %d\n", left);
        lf = 1;
    }
    if (right <= 9 && right != defender->locate[0]) {
        printf("1. 位置 %d\n", right);
        lf = 1;
    }
    if (!lf) {
        printf("沒有位置\n");
        return 0;
    }
    
    int choice;
    printf("\n選擇放置左右(0/1): ");
    scanf("%d", &choice);
    
    if (choice < 1 || choice > 2) {
        printf("invalid choice!\n");
    }
    defender->locate[0] = (choice == 0) ? left : right;
    if (defender->locate[0] == 1 || defender->locate[0] == 9) return 1;
    else return 0;
}

void discard_cards(sPlayer* player, int value, bool is_random) {
    while (value > 0 && player->hand.SIZE > 0) {
        if (is_random) {
            // Randomly select a card to discard
            int random_index = rand() % player->hand.SIZE;
            int32_t card_id = player->hand.array[random_index];
            const Card* card = getCardData(card_id);
            printf("\n隨機棄掉 %s\n", card->name);
            pushbackVector(&player->graveyard, card_id);
            eraseVector(&player->hand, random_index);
            value--;
        } else {
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
                        move(attacker, card->level, false);
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
                                move(attacker, 1, false);
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
            discard_cards(defender, 3, false);
            return 0;
        default:
            break;
    }
    return 0;
}

int handle_sleepingbeauty_skills(sPlayer* attacker, sPlayer* defender, const Card* skill_card, uint8_t level) {
    int cost_token = 0;
    // TODO: 是否花費 TOKEN ,check 是否覺醒， exit覺醒
    if (attacker->sleepingBeauty.AWAKEN && (skill_card->id <= CARD_SLEEPINGBEAUTY_ATK3_MIND_FURY 
    || skill_card->id >= CARD_SLEEPINGBEAUTY_MOVE1_DARK_TOUCH)) {
        printf("cost token(0-%d): ", min(3, attacker->sleepingBeauty.AWAKEN_TOKEN));
        int choice;
        scanf("%d", &choice);
        cost_token = choice;
        attacker->sleepingBeauty.AWAKEN_TOKEN -= choice;
        if (attacker->sleepingBeauty.AWAKEN_TOKEN <= 0) attacker->sleepingBeauty.AWAKEN = 0;
    }
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
            if (countCard(&attacker->usecards, CARD_SLEEPINGBEAUTY_METAMORPH3_MENTAL_BARRIER)) defend(attacker, skill_card->level);
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
    int distance = abs(attacker->locate[0] - defender->locate[0]);
    switch (skill_card->id) {
        case CARD_MULAN_ATK1_UNDERESTIMATED:
        case CARD_MULAN_ATK2_IRRESISTIBLE:
        case CARD_MULAN_ATK3_UNBREAKABLE: {
            if (distance > 1) return -1;
            int cost_ki = 0;
            printf("cost ki: ");
            scanf("%d", &cost_ki);
            attacker->mulan.KI_TOKEN -= cost_ki;
            if (place(attacker, defender)) {
                // 檢查對手是否有手牌
                if (defender->hand.SIZE == 0) printf("對手沒有手牌可以棄掉！\n");
                else {
                    discard_cards(defender, 1, true);  // Randomly discard one card
                }
            }
            attack(defender, skill_card->level + level + cost_ki);
            return 0;
        }
        case CARD_MULAN_DEF1_STILLNESS:
        case CARD_MULAN_DEF2_SOFTNESS:
        case CARD_MULAN_DEF3_WEAK_OVERCOME_STRONG: {
            defend(attacker, level);
            attacker->mulan.extraDraw += skill_card->level;
            return 0;
        }
        case CARD_MULAN_MOVE1_NEVER_RETREAT:
        case CARD_MULAN_MOVE2_NO_MERCY:
        case CARD_MULAN_MOVE3_NO_FORGIVENESS: {
            if (distance > 1) return -1;
            attack(defender, skill_card->level);
            knockback(attacker, defender, skill_card->level);
            if (defender->locate[0] == 1 || defender->locate[0] == 9) discard_cards(defender, 1 , true);
            place(defender, attacker);
        }
        default:
            return -1;
    }
}
// TODO: move move_meta1
int handle_mulan_ultimate(sPlayer* attacker, sPlayer* defender, const Card* ultimate_card) {
    int distance = abs(attacker->locate[0] - defender->locate[0]);
    switch (ultimate_card->id) {
        case CARD_MULAN_SPECIAL1_SOARING:
            return 0;
        case CARD_MULAN_SPECIAL2_FACE_CHAOS:
            place(defender, attacker);
            attacker->mulan.KI_TOKEN += 3;
            if (attacker->mulan.KI_TOKEN > 12) attacker->mulan.KI_TOKEN = 12;
        case CARD_MULAN_SPECIAL3_THUNDER_STRIKE:
            if (distance > 1) return -1;
            else {
                attack(defender, attacker->mulan.KI_TOKEN);
                attacker->mulan.KI_TOKEN = 0;
                return 0;
            }
        default:
            return -1;
    }
}

int put_posion(sPlayer* attacker, sPlayer* defender, vector *dest){
    int32_t card;
    if(getVectorTop(&attacker->snowWhite.remindPosion, &card)){
        pushbackVector(dest, card);
        if(card == CARD_SNOWWHITE_STATUS_POISON2){
            defender->life -= 1;
        }
        else if(card == CARD_SNOWWHITE_STATUS_POISON3){
            defender->life -= 2;
        }
        popbackVector(&attacker->snowWhite.remindPosion);
        return 1;
    }
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

        if (choice == 0) return 0;

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
    switch(skill_card->id){
        case CARD_SNOWWHITE_ATK1_CRYSTAL_SHARD:{
            if(abs(attacker->locate[0] - defender->locate[0]) > 1){
                return -1;
            }
            int32_t total_damage = 1 + level;
            attack(defender, total_damage);
            if(attacker->snowWhite.meta1 && total_damage > 2){
                put_posion(attacker, defender, &defender->graveyard);
            }
            move_card(&defender->graveyard, &defender->deck);
            break;
        }
        case CARD_SNOWWHITE_ATK2_CRYSTAL_VORTEX:{
            if(abs(attacker->locate[0] - defender->locate[0]) > 1){
                return -1;
            }
            int32_t total_damage = 2 + level;
            attack(defender, total_damage);
            if(attacker->snowWhite.meta1 && total_damage > 2){
                put_posion(attacker, defender, &defender->graveyard);
            }
            for(int32_t i = 0; i < 2; i++){
                move_card(&defender->graveyard, &defender->deck);
            }
            break;
        }
        case CARD_SNOWWHITE_ATK3_CRYSTAL_STORM:{
            if(abs(attacker->locate[0] - defender->locate[0]) > 1){
                return -1;
            }
            int32_t total_damage = 3 + level;
            attack(defender, total_damage);
            if(attacker->snowWhite.meta1 && total_damage > 2){
                put_posion(attacker, defender, &defender->graveyard);
            }
            for(int32_t i = 0; i < 3; i++){
                move_card(&defender->graveyard, &defender->deck);
            }
            break;
        }
        case CARD_SNOWWHITE_DEF1_TAINTED_GRACE:{
            if(abs(attacker->locate[0] - defender->locate[0]) > 1){
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
            DEBUG_PRINT("attack com\n");
            int c;
            if(posionCardNumber >= 1 && countCard(&attacker->metamorphosis, CARD_SNOWWHITE_METAMORPH2_FALLEN_CALAMITY)){
                printf("Do you wnat to put one into deck(1/0): ");
                scanf("%d", &c);
                if(c){
                    put_posion(attacker, defender, &defender->deck);
                    for(int32_t i = 1; i < posionCardNumber; i++){
                        put_posion(attacker, defender, &defender->graveyard);
                    }
                }
                else{
                    for(int32_t i = 0; i < posionCardNumber; i++){
                        put_posion(attacker, defender, &defender->graveyard);
                    }
                }
            }
            else{
                for(int32_t i = 0; i < posionCardNumber; i++){
                    put_posion(attacker, defender, &defender->graveyard);
                }
            }
            break;
        }
        case CARD_SNOWWHITE_DEF2_TAINTED_FEAST:{
            if(abs(attacker->locate[0] - defender->locate[0]) > 1){
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
            DEBUG_PRINT("attack com\n");
            char c = 'n';
            if(posionCardNumber >= 1 && countCard(&attacker->metamorphosis, CARD_SNOWWHITE_METAMORPH2_FALLEN_CALAMITY)){
                printf("Do you wnat to put one into deck(y/n): ");
                scanf("%c", &c);
                if(c == 'y'){
                    put_posion(attacker, defender, &defender->deck);
                    for(int32_t i = 1; i < posionCardNumber; i++){
                        put_posion(attacker, defender, &defender->graveyard);
                    }
                }
                else{
                    for(int32_t i = 0; i < posionCardNumber; i++){
                        put_posion(attacker, defender, &defender->graveyard);
                    }
                }
            }
            else{
                for(int32_t i = 0; i < posionCardNumber; i++){
                    put_posion(attacker, defender, &defender->graveyard);
                }
            }
            break;
        }
        case CARD_SNOWWHITE_DEF3_TAINTED_CARNIVAL:{
            if(abs(attacker->locate[0] - defender->locate[0]) > 1){
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
            DEBUG_PRINT("attack com\n");
            char c = 'n';
            if(posionCardNumber >= 1 && countCard(&attacker->metamorphosis, CARD_SNOWWHITE_METAMORPH2_FALLEN_CALAMITY)){
                printf("Do you wnat to put one into deck(y/n): ");
                scanf("%c", &c);
                if(c == 'y'){
                    put_posion(attacker, defender, &defender->deck);
                    for(int32_t i = 1; i < posionCardNumber; i++){
                        put_posion(attacker, defender, &defender->graveyard);
                    }
                }
                else{
                    for(int32_t i = 0; i < posionCardNumber; i++){
                        put_posion(attacker, defender, &defender->graveyard);
                    }
                }
            }
            else{
                for(int32_t i = 0; i < posionCardNumber; i++){
                    put_posion(attacker, defender, &defender->graveyard);
                }
            }
            break;
        }
        case CARD_SNOWWHITE_MOVE1_BROKEN_FANTASY:{
            if(abs(attacker->locate[0] - defender->locate[0]) > level){
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
            move(attacker, dest - attacker->locate[0], true);
            break;
        }
        case CARD_SNOWWHITE_MOVE2_BROKEN_REALITY:{
            if(abs(attacker->locate[0] - defender->locate[0]) > 1 + level){
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
            move(attacker, dest - attacker->locate[0], true);
            break;
        }
        case CARD_SNOWWHITE_MOVE3_BROKEN_FATE:{
            if(abs(attacker->locate[0] - defender->locate[0]) > 2 + level){
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
            move(attacker, dest - attacker->locate[0], true);
            break;
        }
        default:
            break;
    }
    return 0;
}

int handle_snowwhite_ultimate(sPlayer* attacker, sPlayer* defender, const Card* ultimate_card){
    switch(ultimate_card->id) {
        case CARD_SNOWWHITE_SPECIAL1_SEVEN_SNAKES_RAGE:
            if(abs(attacker->locate[0] - defender->locate[0]) > 1){
                return -1;
            }
            int32_t damage = 0;
            for(int32_t i = 0; i < defender->graveyard.SIZE ; i++){
                if(defender->graveyard.array[i] >= 131 && defender->graveyard.array[i] <= 133){
                    damage++;
                }
            }
            defender->life -= damage;
            return 0;
        case CARD_SNOWWHITE_SPECIAL2_MAGIC_MIRROR_RAIN:
            if(abs(attacker->locate[0] - defender->locate[0]) > 1){
                return -1;
            }
            attack(defender, 3);
            while(defender->hand.SIZE > 0){
                move_card(&defender->hand, &defender->graveyard);
            }
            draw_card(defender, 4);
            return 0;
        case CARD_SNOWWHITE_SPECIAL3_BREWED_DISASTER:
            if(abs(attacker->locate[0] - defender->locate[0]) > 3){
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
            return 0;
        default:
            return -1;
    }
}

int handle_kaguya_skills(sPlayer* attacker, sPlayer* defender, const Card* skill_card, uint8_t level) {
    return 0;
}

int handle_matchgirl_skills(sPlayer* attacker, sPlayer* defender, const Card* skill_card, uint8_t level) {
    return 0;
}