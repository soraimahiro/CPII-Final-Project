#ifndef _ARCHITECTURE_H
#define _ARCHITECTURE_H
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vector.h"

typedef struct _player {
    int8_t team;  // for 2v2 mode
    uint8_t locate[2];
    uint8_t character;
    uint8_t maxlife;
    uint8_t life;
    uint8_t maxdefense;
    uint8_t defense;
    uint8_t energy;
    uint8_t specialGate;
    vector hand;
    vector deck;
    vector usecards;
    vector graveyard;
    vector metamorphosis;
    vector attackSkill;
    vector defenseSkill;
    vector moveSkill;
    vector specialDeck;
    // Little Red Riding Hood 0
    struct {
        int32_t saveCard[3];
    } redHood;

    // Snow White 1
    struct {
        vector remindPosion;
    } snowWhite;

    // sleeping Beauty 2
    struct {
        uint32_t AWAKEN_TOKEN;
        int8_t AWAKEN;  // awaken(1) or sleep(0)
        int8_t dayNightmareDrawRemind;
        int32_t atkRise;
        int32_t atkRiseTime;
        int8_t usedmeta1;
    } sleepingBeauty;

    // alice 3
    struct {
        uint8_t identity;  // 0:none 1:紅心皇后 2:瘋帽子 3:柴郡貓
        int32_t riseBasic;
        int32_t restartTurn;
        int32_t havedrestart;
    } alice;

    // Mulan 4
    struct {
        uint32_t KI_TOKEN;
        uint8_t extraCard;
        uint8_t extraDraw;
    } mulan;

    // kaguya 5
    struct {
        int8_t useDefenseAsATK;
        int8_t useMoveTarget;
    } kaguya;

    // mermaid 6
    struct {
    } mermaid;

    // Match Girl 7
    struct {
        uint32_t remindMatch;
        uint32_t pushedMatch;
    } matchGirl;

    // dorothy 8
    struct {
        uint32_t COMBO_TOKEN;
        int8_t canCombo;
    } dorothy;

    // Scheherazade 9
    struct {
        vector destiny_TOKEN_locate;
        vector destiny_TOKEN_type;  // 1:blue, 2:red
        int8_t selectToken;
    } scheherazade;
} player;
enum state {
    CHOOSE_IDENTITY = 0,
    CHOOSE_TENTACLE_LOCATION,
    CHOOSE_SPECIAL_CARD,
    APPEND_DESTINY_TOKEN,
    SET_TARGET_LOCATE_TO_NEARBY,
    CHOOSE_MOVE,
    BUY_CARD_TYPE,
    REMOVE_HG,
    DROP_H,
    USE_ATK,
    USE_DEF,
    USE_MOV,
    USE_POSION,
    CHOOSE_MOVING_DIR,
    USE_SKILL,
    TRIGGER_COMBO,
    USEBASIC,
    KNOCKBACK,
    MOVE_TARGET,
    PUT_TARGET_POSITION,
    SHUFFLE_POSION_TO_DECK,
    CHOOSE_CARD_BACK,
    SLEEP_ATK_HERTSELF,
    USE_AWAKEN_TOKEN,
    LOST_LIFE_FOR_USESKILL,
    RECYCLE_CARD,
    CHOOSECARDS,
    TAKE_TO_HAND,
    CHANGE_IDENTITY,
    CHOOSE_MOVE_DIS,
    SEND_CARD,
    GET_KI,
    SPEND_KI_FOR_ATK,
    SPEND_KI_FOR_DRAW,
    SPEND_KI_FOR_MOV,
    DROP_ONE_DRAW_ONE,
    PUT_TO_ANOTHER_SIDE,
    CHOOSE_MOVE_NEARBY,
    KEEP_OR_BACK,
    LOST_LIFE_FOR_REMOVECARD,
    KAGUYA_MOVE_TARGET,
    MOVE_TO_TANTACLE,
    CHOOSE_TANTACLE,
    MOVE_TANTACLE,
    DROPCARD_MOVE_TANTACLE,
    SPEND_ENERGY,
    SPEND_LIFE,
    RECYCLE_MATCH,
    DROP_CARD,
    SPEND_COMBO,
    FLIP_TOKEN_TO_RED,
    CHOOSE_TOKEN,
    TOKEN_GOAL,
    GET_ULTRA,
    USE_METAMORPHOSIS,
};
/*
state                         return type  meaning
CHOOSE_IDENTITY               int8_t       1:紅心皇后 2:瘋帽子 3:柴郡貓
CHOOSE_TENTACLE_LOCATION      int32_t      tentacle location
CHOOSE_SPECIAL_CARD           int32_t      special card id
APPEND_DESTINY_TOKEN          int32_t      -1,-2,-3 meaning skill(atk/def/mov), 1~10 meaning basic(atk/def/mov/general)
SET_TARGET_LOCATE_TO_NEARBY   int32_t      set location(1~9)
CHOOSE_MOVE                   int32_t      choose moves
                                           (0:focus, 1-3:use basic cards(1:atk,2:def,3:mov),
                                            4:use a skill card, 5:use a special card, 6:buy a card,
                                            7:metamorphosis, 8:charactor special move,9:drop poison
                                            10:end)
BUY_CARD_TYPE                 int32_t      -1,-2,-3 meaning skill(atk/def/mov), 1~10 meaning basic(atk/def/mov/general)

REMOVE_HG                     int32_t      for remove card, choose hand or graveyard(negtive:graveyard, positive:hand, value:index of card, **1 base**)
DROP_H                        int32_t      for drop card, choose hand(value:index of card, **1 base**)
USE_ATK                       int32_t      for use atk basic card, choose hand(value:index of card, **1 base**, 0 is meaning stop use)
USE_DEF                       int32_t      for use def basic card, choose hand(value:index of card, **1 base**, 0 is meaning stop use)
USE_MOV                       int32_t      for use mov basic card, choose hand(value:index of card, **1 base**, 0 is meaning stop use)
USE_POSION                    int32_t      for use posion card, choose hand(value:index of card, **1 base**)
CHOOSE_MOVING_DIR             int8_t       moving face(0:left, 1:right)
USE_SKILL                     int32_t      for use skill card, choose hand(value:index of card, **1 base**)
TRIGGER_COMBO                 int8_t       0:NO 1:YES(you dont need to implement this if you didn't implement dorothy)
USEBASIC                      int32_t      for use skill card, choose basic card from hand(value:index of card, **1 base**)
KNOCKBACK                     int32_t      knockback enemy distanse(hint:check avilable value on player's last use card)
MOVE_TARGET                   int32_t      move target(return locate after move)

PUT_TARGET_POSITION           int32_t      choose number of posion you want to push to target's deck(hint:check avilable value on player's last use card and skill that you used)
SHUFFLE_POSION_TO_DECK        int8_t       shuffle posion to deck instead of put into graveyard(1:yes, 0:no)
CHOOSE_CARD_BACK              int32_t      shuffle back the target card from graveyard to deck(0:cancel, value:index of card, **1 base**)

SLEEP_ATK_HERTSELF            int32_t      sleeping beauty's skill hert herself(hint:you can check the last card you use to avoid cheating)
USE_AWAKEN_TOKEN                int8_t       use awaken token when using skill(at most 3)
LOST_LIFE_FOR_USESKILL        int8_t       sleeping beauty's metamorphosis, (choose lose 0(not trigger),2,4,6 life)
RECYCLE_CARD                  int32_t      sleeping beauty's metamorphosis, (choose a card from graveyard(0 is not trigger, 1 base))

CHOOSECARDS                   int32_t      choose a card from nowShowingCards(return card id)
TAKE_TO_HAND                  int8_t       get card instead put it from graveyards (return 0 or 1)
CHANGE_IDENTITY               int8_t       change identity or not(check last use card to check which you will change)(return 0:no or 1:yes)
CHOOSE_MOVE_DIS               int8_t       choose the moving distanse(check last use card to confirm limit of move distanse)
SEND_CARD                     int32_t      for sending card to target, choose hand or graveyard(negtive:graveyard, positive:hand, value:index of card, **1 base**, 0 is stop sending)

GET_KI                        int32_t      get ki by drop defense card or general card(return index of card, **1 base**, 0 is meaning not use)
SPEND_KI_FOR_ATK              int32_t      use ki for atk metamorphosis (return the number of KI you want to use)
SPEND_KI_FOR_DRAW             int32_t      use ki for defense skill (return the number of KI you want to use)
SPEND_KI_FOR_MOV              int32_t      use ki for move metamorphosis (return the number of KI you want to use)
DROP_ONE_DRAW_ONE             int32_t      for drop card, choose hand(value:index of card, **1 base**, 0 is not do it)
PUT_TO_ANOTHER_SIDE           int8_t       choose move target to another side or not
CHOOSE_MOVE_NEARBY            int8_t       move to the locate next to target(-1:left, 1:right, 0:no move)

KEEP_OR_BACK                  int8_t       keep the card or drop it(1:keep, 0:drop, the card store in nowusingCardID)
LOST_LIFE_FOR_REMOVECARD      int8_t       kaguya's skill, (1:lost, 0:no)
KAGUYA_MOVE_TARGET                   int8_t       kaguya's move metamorphosis(-n:left, n:right, n is distance)

MOVE_TO_TANTACLE              int32_t      locate you choose(should in tentacle_TOKEN_locate or your original locate)
CHOOSE_TANTACLE               int32_t      choose a tantacle
MOVE_TANTACLE                 int32_t      move a tantacle you choose(locate after move tantacle)
DROPCARD_MOVE_TANTACLE        int32_t      drop a card for moving tantacle(value:index of card, **1 base**)

SPEND_ENERGY                  int32_t      spend energy for rise the atk(0 is meaning not use)
SPEND_LIFE                    int32_t      spend life for draw card(0 is meaning not use)
RECYCLE_MATCH                 int32_t      get back the match from target's graveyard(check the max value you can recycle from target's graveyard and the card you use)
DROP_CARD                     int32_t      choose a card to drop/save(0 is end, 1 base)
SPEND_COMBO                   int32_t      spend combo token for ultra skill(send number of tokens you use)

FLIP_TOKEN_TO_RED             int8_t       -1,-2,-3 meaning skill(atk/def/mov), 1~10 meaning basic(atk/def/mov/general), 0 is not flip
CHOOSE_TOKEN                  int8_t       choose a destiny token for action(index of tokens, -1 is give up)
TOKEN_GOAL                    int8_t       choose location of the token you choose for moving(-1,-2,-3 meaning skill(atk/def/mov), 1~10 meaning basic(atk/def/mov/general))
GET_ULTRA                     int8_t       choose a special card to hand when your life lower than gate first time
USE_METAMORPHOSIS             int32_t      trigger a active metamorphosis(return index of metamorphosis, 0 base)
*/
typedef struct _game {
    player players[4];
    int8_t now_turn_player_id;
    int8_t playerMode;  // 1v1 MODE(0) or 2v2 MODE(1)
    int8_t relicMode;
    // mermaid
    vector tentacle_TOKEN_locate;

    // 1v1 MODE is from 1 to 9
    uint32_t relic[11];
    vector relicDeck;
    vector relicGraveyard;
    vector basicBuyDeck[4][3];  // attack(0) LV1~3 defense(1) LV1~3 move(2) LV1~3 generic(3)
    enum state status;
    // metadata (for using basic card)
    int32_t nowATK;
    int32_t nowDEF;
    int32_t nowMOV;
    int32_t nowUsingCardID;
    vector nowShowingCards;
    int32_t totalDamage;
} game;

#endif
