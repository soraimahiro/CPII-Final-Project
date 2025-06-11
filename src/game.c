#include "game.h"
#include "card.h" // 加入 card.h 來取得 shuffle 函數宣告

extern sGame game;

void game_init() {
    // 初始化基本供應牌庫
    for (int type = 0; type < 4; type++) {
        for (int level = 0; level < 3; level++) {
            game.basicBuyDeck[type][level] = initVector();
            
            int cardId;
            if (type == 0) { // 攻擊牌
                cardId = 1 + level;
                for (int i = 0; i < 18; i++) {
                    pushbackVector(&game.basicBuyDeck[type][level], cardId);
                }
            } else if (type == 1) { // 防禦牌
                cardId = 4 + level;
                for (int i = 0; i < 18; i++) {
                    pushbackVector(&game.basicBuyDeck[type][level], cardId);
                }
            } else if (type == 2) { // 移動牌
                cardId = 7 + level;
                for (int i = 0; i < 18; i++) {
                    pushbackVector(&game.basicBuyDeck[type][level], cardId);
                }
            } else { // 通用牌
                if (level == 0) {
                    for (int i = 0; i < 18; i++) {
                        pushbackVector(&game.basicBuyDeck[3][0], 10);
                    }
                }
            }
        }
    }

    // 初始化戰鬥相關變數
    game.nowATK = 0;
    game.nowDEF = 0;
    game.nowMOV = 0;
    game.nowUsingCardID = -1;
    game.totalDamage = 0;
    game.nowShowingCards = initVector();
    
    // 初始化玩家
    int playerCount = TOTAL_PLAYER;
        
    // 初始化觸手 但位置我不太確定欸by beauty
    game.tentacle_TOKEN_locate = initVector();
    
    for (int i = 0; i < playerCount; i++) {
        // 設定隊伍
        if (game.playerMode == 0) { // 1v1模式
            game.players[i].team = (i == 0) ? 1 : 2;
        }
        
        // 設定初始位置
        if (game.playerMode == 0) { // 1v1模式
            game.players[i].locate[0] = (i == 0) ? 4 : 6; // 軌道兩端
            game.players[i].locate[1] = 0; // 只有一條軌道
        }
        
        // 初始化角色
        init_character(&game.players[i]);
        switch (game.players[i].character) {
            case CHARACTER_REDHOOD:
                init_red_hood(&game.players[i]);
                break;
            case CHARACTER_SNOWWHITE:
                init_snow_white(&game.players[i]);
                break;
            case CHARACTER_SLEEPINGBEAUTY:
                init_sleeping_beauty(&game.players[i]);
                break;
            case CHARACTER_ALICE:
                init_alice(&game.players[i]);
                break;
            case CHARACTER_MULAN:
                init_mulan(&game.players[i]);
                break;
            case CHARACTER_KAGUYA:
                init_kaguya(&game.players[i]);
                break;
            case CHARACTER_MERMAID:
                init_mermaid(&game.players[i]);
                pushbackVector(&game.tentacle_TOKEN_locate, game.players[i].locate[0]);
                break;
            case CHARACTER_MATCHGIRL:
                init_match_girl(&game.players[i]);
                break;
            case CHARACTER_DOROTHY:
                init_dorothy(&game.players[i]);
                break;
            case CHARACTER_SCHEHERAZADE:
                init_scheherazade(&game.players[i]);
                break;
        }
        
        // 初始化牌庫
        
        // 添加基本牌到牌庫
        for (int j = 0; j < 3; j++) {
            // 從基本供應牌庫取出 LV1攻擊牌
            if (game.basicBuyDeck[0][0].SIZE > 0) {
                int card;
                if (getVectorTop(&game.basicBuyDeck[0][0], &card)) {
                    pushbackVector(&game.players[i].deck, card);
                    popbackVector(&game.basicBuyDeck[0][0]);
                }
            }
            
            // 從基本供應牌庫取出 LV1防禦牌
            if (game.basicBuyDeck[1][0].SIZE > 0) {
                int card;
                if (getVectorTop(&game.basicBuyDeck[1][0], &card)) {
                    pushbackVector(&game.players[i].deck, card);
                    popbackVector(&game.basicBuyDeck[1][0]);
                }
            }
            
            // 從基本供應牌庫取出 LV1移動牌
            if (game.basicBuyDeck[2][0].SIZE > 0) {
                int card;
                if (getVectorTop(&game.basicBuyDeck[2][0], &card)) {
                    pushbackVector(&game.players[i].deck, card);
                    popbackVector(&game.basicBuyDeck[2][0]);
                }
            }
        }
        
        
        // 把自己的技能牌放一張到牌庫
        if (game.players[i].attackSkill.SIZE > 0) {
            int skillCard;
            if (getVectorTop(&game.players[i].attackSkill, &skillCard)) {
                pushbackVector(&game.players[i].deck, skillCard);
                popbackVector(&game.players[i].attackSkill);
            }
        }
        
        if (game.players[i].defenseSkill.SIZE > 0) {
            int skillCard;
            if (getVectorTop(&game.players[i].defenseSkill, &skillCard)) {
                pushbackVector(&game.players[i].deck, skillCard);
                popbackVector(&game.players[i].defenseSkill);
            }
        }
        
        if (game.players[i].moveSkill.SIZE > 0) {
            int skillCard;
            if (getVectorTop(&game.players[i].moveSkill, &skillCard)) {
                pushbackVector(&game.players[i].deck, skillCard);
                popbackVector(&game.players[i].moveSkill);
            }
        }
        
        
        // 抽初始手牌
        shuffle(&game.players[i].deck);
        
        
        if (i == 0) {
            // 先攻玩家抽4張
            for (int j = 0; j < 4; j++) {
                int card;
                if (getVectorTop(&game.players[i].deck, &card)) {
                    pushbackVector(&game.players[i].hand, card);
                    popbackVector(&game.players[i].deck);
                } else {
                    fprintf(stderr, "(牌庫空) ");
                    break;
                }
            }
        } else {
            // 後攻玩家抽6張
            for (int j = 0; j < 6; j++) {
                int card;
                if (getVectorTop(&game.players[i].deck, &card)) {
                    pushbackVector(&game.players[i].hand, card);
                    popbackVector(&game.players[i].deck);
                } else {
                    fprintf(stderr, "(牌庫空) ");
                    break;
                }
            }
        }       
    }
    
    game.now_turn_player_id = 0; // 玩家0先攻
    game.status = CHOOSE_MOVE; // 進入行動階段

    // print初始化相關資料
    DEBUG_PRINT("遊戲模式: %s\n", game.playerMode == 0 ? "1v1" : "2v2");
    DEBUG_PRINT("玩家數量: %d\n", playerCount);
    for (int i = 0; i < playerCount; i++) {
        DEBUG_PRINT("玩家%d: %s角色 (隊伍%d) 位置[%d,%d] 手牌%d張 %s\n", 
               i + 1, 
               game.players[i].character == CHARACTER_REDHOOD ? "小紅帽" : 
               game.players[i].character == CHARACTER_SNOWWHITE ? "白雪公主" :
               game.players[i].character == CHARACTER_SLEEPINGBEAUTY ? "睡美人" :
               game.players[i].character == CHARACTER_ALICE ? "愛麗絲" :
               game.players[i].character == CHARACTER_MULAN ? "花木蘭" :
               game.players[i].character == CHARACTER_KAGUYA ? "輝夜姬" :
               game.players[i].character == CHARACTER_MERMAID ? "美人魚" :
               game.players[i].character == CHARACTER_MATCHGIRL ? "火柴女孩" :
               game.players[i].character == CHARACTER_DOROTHY ? "桃樂絲" :
               game.players[i].character == CHARACTER_SCHEHERAZADE ? "山魯佐德" : "未知",
               game.players[i].team,
               game.players[i].locate[0],
               game.players[i].locate[1],
               game.players[i].hand.SIZE,
               game.players[i].isBOT ? "(機器人)" : "(人類)");
        DEBUG_PRINT_VEC(&game.players[i].hand, "玩家%d (%s) 初始手牌", i + 1, 
               game.players[i].character == CHARACTER_REDHOOD ? "小紅帽" : 
               game.players[i].character == CHARACTER_SNOWWHITE ? "白雪公主" :
               game.players[i].character == CHARACTER_SLEEPINGBEAUTY ? "睡美人" :
               game.players[i].character == CHARACTER_ALICE ? "愛麗絲" :
               game.players[i].character == CHARACTER_MULAN ? "花木蘭" :
               game.players[i].character == CHARACTER_KAGUYA ? "輝夜姬" :
               game.players[i].character == CHARACTER_MERMAID ? "美人魚" :
               game.players[i].character == CHARACTER_MATCHGIRL ? "火柴女孩" :
               game.players[i].character == CHARACTER_DOROTHY ? "桃樂絲" :
               game.players[i].character == CHARACTER_SCHEHERAZADE ? "山魯佐德" : "未知");
    }
}

void init_character(sPlayer* p){
    p->hand = initVector();
    p->deck = initVector();
    p->usecards = initVector();
    p->graveyard = initVector();
    p->metamorphosis = initVector();
    p->attackSkill = initVector();
    p->defenseSkill = initVector();
    p->moveSkill = initVector();
    p->specialDeck = initVector();
}

void init_red_hood(sPlayer* p) {
    p->maxlife = 30;
    p->life = 30;
    p->defense = 0;
    p->maxdefense = 6;
    p->energy = 0;
    p->specialGate = 15;

    // 攻擊技能牌庫
    pushbackVector(&p->attackSkill, 138); // 板載緩存 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->attackSkill, 13); // 致命狙擊 x3
    pushbackVector(&p->attackSkill, 135); // 過載燃燒 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->attackSkill, 12); // 精準射擊 x2
    pushbackVector(&p->attackSkill, 11); // 快速射擊 x1

    // 防禦技能牌庫
    pushbackVector(&p->defenseSkill, 138); // 板載緩存 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->defenseSkill, 16); // 終極護盾 x3
    pushbackVector(&p->defenseSkill, 136); // 兜帽系統 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->defenseSkill, 15); // 電流護盾 x2
    pushbackVector(&p->defenseSkill, 14); // 能量護盾 x1

    // 移動技能牌庫
    pushbackVector(&p->moveSkill, 138); // 板載緩存 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->moveSkill, 19); // 暴怒噴射 x3
    pushbackVector(&p->moveSkill, 137); // 變異感應 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->moveSkill, 18); // 火力噴射 x2
    pushbackVector(&p->moveSkill, 17); // 彈道噴射 x1

    // 必殺牌
    pushbackVector(&p->specialDeck, 20); // 餓狼吞噬
    pushbackVector(&p->specialDeck, 21); // 系統入侵
    pushbackVector(&p->specialDeck, 22); // 復仇之雨

    p->redHood.saveCard[0] = -1;
    p->redHood.saveCard[1] = -1;
    p->redHood.saveCard[2] = -1;

}

void init_snow_white(sPlayer* p) {
    p->maxlife = 34;
    p->life = 34;
    p->defense = 0;
    p->maxdefense = 6;
    p->energy = 0;
    p->specialGate = 17;

    // 攻擊技能牌庫
    pushbackVector(&p->attackSkill, 142); // 至純之毒 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->attackSkill, 25); // 水晶風暴 x3
    pushbackVector(&p->attackSkill, 139); // 水晶之棺 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->attackSkill, 24); // 水晶漩渦 x2
    pushbackVector(&p->attackSkill, 23); // 水晶碎片 x1

    // 防禦技能牌庫
    pushbackVector(&p->defenseSkill, 142); // 至純之毒 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->defenseSkill, 28); // 玷污的狂歡 x3
    pushbackVector(&p->defenseSkill, 140); // 墮落之劫 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->defenseSkill, 27); // 玷污的盛筵 x2
    pushbackVector(&p->defenseSkill, 26); // 玷污的恩惠 x1

    // 移動技能牌庫
    pushbackVector(&p->moveSkill, 142); // 至純之毒 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->moveSkill, 31); // 破碎的命運 x3
    pushbackVector(&p->moveSkill, 141); // 劇毒之蝕 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->moveSkill, 30); // 破碎的現實 x2
    pushbackVector(&p->moveSkill, 29); // 破碎的幻想 x1

    // 必殺牌
    pushbackVector(&p->specialDeck, 32); // 七蛇之怒
    pushbackVector(&p->specialDeck, 33); // 魔鏡之雨
    pushbackVector(&p->specialDeck, 34); // 醞釀之災

    p->snowWhite.remindPosion = initVector();
}

void init_sleeping_beauty(sPlayer* p) {
    p->maxlife = 42;
    p->life = 42;
    p->defense = 0;
    p->maxdefense = 6;
    p->energy = 0;
    p->specialGate = 21;

    // 攻擊技能牌庫
    pushbackVector(&p->attackSkill, 146); // 強制治療 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->attackSkill, 37); // 心靈狂怒 x3
    pushbackVector(&p->attackSkill, 143); // 放血療法 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->attackSkill, 36); // 心靈之怒 x2
    pushbackVector(&p->attackSkill, 35); // 心靈震顫 x1

    // 防禦技能牌庫
    pushbackVector(&p->defenseSkill, 146); // 強制治療 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->defenseSkill, 40); // 爆裂之魂 x3
    pushbackVector(&p->defenseSkill, 144); // 血祭之禮 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->defenseSkill, 39); // 爆裂之骨 x2
    pushbackVector(&p->defenseSkill, 38); // 爆裂之鎖 x1

    // 移動技能牌庫
    pushbackVector(&p->moveSkill, 146); // 強制治療 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->moveSkill, 43); // 黑暗絞殺 x3
    pushbackVector(&p->moveSkill, 145); // 精神屏障 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->moveSkill, 42); // 黑暗糾纏 x2
    pushbackVector(&p->moveSkill, 41); // 黑暗碰觸 x1

    // 必殺牌
    pushbackVector(&p->specialDeck, 44); // 喚醒沉睡
    pushbackVector(&p->specialDeck, 45); // 白日夢魘
    pushbackVector(&p->specialDeck, 46); // 血脈重鑄

    p->sleepingBeauty.AWAKEN_TOKEN = 0;
    p->sleepingBeauty.AWAKEN = 0;
    p->sleepingBeauty.dayNightmareDrawRemind = 0;
    p->sleepingBeauty.atkRise = 0;
    p->sleepingBeauty.atkRiseTime = 0;
    p->sleepingBeauty.usedmeta1 = 0;
}

void init_alice(sPlayer* p) {
    p->maxlife = 32;
    p->life = 32;
    p->defense = 0;
    p->maxdefense = 6;
    p->energy = 0;
    p->specialGate = 16;

    // 攻擊技能牌庫
    pushbackVector(&p->attackSkill, 150); // 開始我的表演 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->attackSkill, 49); // 操控牌局 x3
    pushbackVector(&p->attackSkill, 147); // 砍掉她的頭 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->attackSkill, 48); // 扭轉牌局 x2
    pushbackVector(&p->attackSkill, 47); // 開啟牌局 x1

    // 防禦技能牌庫
    pushbackVector(&p->defenseSkill, 150); // 開始我的表演 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->defenseSkill, 52); // 帽子戲法 x3
    pushbackVector(&p->defenseSkill, 148); // 仙境降臨 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->defenseSkill, 51); // 精神幻術 x2
    pushbackVector(&p->defenseSkill, 50); // 魔力技巧 x1

    // 移動技能牌庫
    pushbackVector(&p->moveSkill, 150); // 開始我的表演 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->moveSkill, 55); // 詭異的詭異 x3
    pushbackVector(&p->moveSkill, 149); // 我們全是瘋子 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->moveSkill, 54); // 詭異的隱蔽 x2
    pushbackVector(&p->moveSkill, 53); // 詭異的敏捷 x1

    // 必殺牌
    pushbackVector(&p->specialDeck, 56); // 無休止的派對
    pushbackVector(&p->specialDeck, 57); // 精彩的奇妙日
    pushbackVector(&p->specialDeck, 58); // 遊戲盡在掌控

    p->alice.identity = 0;
    p->alice.riseBasic = 0;
    p->alice.restartTurn = 0;
    p->alice.havedrestart = 0;
}

void init_mulan(sPlayer* p) {
    p->maxlife = 34;
    p->life = 34;
    p->defense = 0;
    p->maxdefense = 3;
    p->energy = 0;
    p->specialGate = 17;

    // 攻擊技能牌庫
    pushbackVector(&p->attackSkill, 154); // 暴風前夕 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->attackSkill, 61); // 堅不可摧 x3
    pushbackVector(&p->attackSkill, 151); // 氣慣全身 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->attackSkill, 60); // 勢不可擋 x2
    pushbackVector(&p->attackSkill, 59); // 不容小覷 x1

    // 防禦技能牌庫
    pushbackVector(&p->defenseSkill, 154); // 暴風前夕 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->defenseSkill, 64); // 以弱勝強 x3
    pushbackVector(&p->defenseSkill, 152); // 主宰命運 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->defenseSkill, 63); // 以柔克剛 x2
    pushbackVector(&p->defenseSkill, 62); // 以靜制動 x1

    // 移動技能牌庫
    pushbackVector(&p->moveSkill, 154); // 暴風前夕 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->moveSkill, 67); // 絕不饒恕 x3
    pushbackVector(&p->moveSkill, 153); // 長驅直入 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->moveSkill, 66); // 毫不留情 x2
    pushbackVector(&p->moveSkill, 65); // 永不退縮 x1

    // 必殺牌
    pushbackVector(&p->specialDeck, 68); // 氣沖雲霄
    pushbackVector(&p->specialDeck, 69); // 直面混沌
    pushbackVector(&p->specialDeck, 70); // 雷霆一擊

    p->mulan.KI_TOKEN = 0;
    p->mulan.extraCard = 0;
    p->mulan.extraDraw = 0;
}

void init_kaguya(sPlayer* p) {
    p->maxlife = 32;
    p->life = 32;
    p->defense = 0;
    p->maxdefense = 6;
    p->energy = 0;
    p->specialGate = 16;

    // 攻擊技能牌庫
    pushbackVector(&p->attackSkill, 158); // 月下沉思 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->attackSkill, 73); // 領悟的化身 x3
    pushbackVector(&p->attackSkill, 155); // 懲戒時刻 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->attackSkill, 72); // 領悟的榮耀 x2
    pushbackVector(&p->attackSkill, 71); // 領悟的光芒 x1

    // 防禦技能牌庫
    pushbackVector(&p->defenseSkill, 158); // 月下沉思 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->defenseSkill, 76); // 神性的召換 x3
    pushbackVector(&p->defenseSkill, 156); // 血色月光 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->defenseSkill, 75); // 久遠的回響 x2
    pushbackVector(&p->defenseSkill, 74); // 困惑的回聲 x1

    // 移動技能牌庫
    pushbackVector(&p->moveSkill, 158); // 月下沉思 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->moveSkill, 79); // 痛徹的淨化 x3
    pushbackVector(&p->moveSkill, 157); // 靈性本能 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->moveSkill, 78); // 頓悟的決心 x2
    pushbackVector(&p->moveSkill, 77); // 專注的自省 x1

    // 必殺牌
    pushbackVector(&p->specialDeck, 80); // 炙熱的竹刀
    pushbackVector(&p->specialDeck, 81); // 注定的審判
    pushbackVector(&p->specialDeck, 82); // 躁動的血性

    p->kaguya.useDefenseAsATK = 0;
    p->kaguya.useMoveTarget = 0;
}

void init_mermaid(sPlayer* p) {
    p->maxlife = 36;
    p->life = 36;
    p->defense = 0;
    p->maxdefense = 3;
    p->energy = 0;
    p->specialGate = 18;

    // 攻擊技能牌庫
    pushbackVector(&p->attackSkill, 162); // 暗潮湧動 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->attackSkill, 85); // 海妖的尖嘯 x3
    pushbackVector(&p->attackSkill, 159); // 暴風之蝕 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->attackSkill, 84); // 海妖的歌聲 x2
    pushbackVector(&p->attackSkill, 83); // 海妖的召喚 x1

    // 防禦技能牌庫
    pushbackVector(&p->defenseSkill, 162); // 暗潮湧動 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->defenseSkill, 88); // 復仇之怒 x3
    pushbackVector(&p->defenseSkill, 160); // 神秘共鳴 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->defenseSkill, 87); // 噴薄之怒 x2
    pushbackVector(&p->defenseSkill, 86); // 洶湧之怒 x1

    // 移動技能牌庫
    pushbackVector(&p->moveSkill, 162); // 暗潮湧動 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->moveSkill, 91); // 深淵的征服 x3
    pushbackVector(&p->moveSkill, 161); // 海的女兒 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->moveSkill, 90); // 深淵的入侵 x2
    pushbackVector(&p->moveSkill, 89); // 深淵的蠶食 x1

    // 必殺牌
    pushbackVector(&p->specialDeck, 92); // 人魚復興
    pushbackVector(&p->specialDeck, 93); // 遠古甦醒
    pushbackVector(&p->specialDeck, 94); // 淨化之潮

}

void init_match_girl(sPlayer* p) {
    p->maxlife = 36;
    p->life = 36;
    p->defense = 0;
    p->maxdefense = 6;
    p->energy = 0;
    p->specialGate = 18;

    // 攻擊技能牌庫
    pushbackVector(&p->attackSkill, 166); // 火焰的捉弄 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->attackSkill, 97); // 無厭的奢望 x3
    pushbackVector(&p->attackSkill, 163); // 痛苦的儀式 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->attackSkill, 96); // 隱密的期望 x2
    pushbackVector(&p->attackSkill, 95); // 虛幻的願望 x1

    // 防禦技能牌庫
    pushbackVector(&p->defenseSkill, 167); // 欲望的捉弄 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->defenseSkill, 100); // 惡魔的契約 x3
    pushbackVector(&p->defenseSkill, 164); // 放縱的渴望 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->defenseSkill, 99); // 惡魔的賭注 x2
    pushbackVector(&p->defenseSkill, 98); // 惡魔的祭品 x1

    // 移動技能牌庫
    pushbackVector(&p->moveSkill, 168); // 命運的捉弄 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->moveSkill, 103); // 殘破的靈魂 x3
    pushbackVector(&p->moveSkill, 165); // 魔鬼的凝視 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->moveSkill, 102); // 虧欠的靈魂 x2
    pushbackVector(&p->moveSkill, 101); // 失重的靈魂 x1

    // 必殺牌
    pushbackVector(&p->specialDeck, 104); // 地獄烈焰
    pushbackVector(&p->specialDeck, 105); // 厄運降臨
    pushbackVector(&p->specialDeck, 106); // 貪婪詛咒

    p->matchGirl.remindMatch = 12;
    p->matchGirl.pushedMatch = 0;
}

void init_dorothy(sPlayer* p) {
    p->maxlife = 40;
    p->life = 40;
    p->defense = 0;
    p->maxdefense = 6;
    p->energy = 0;
    p->specialGate = 20;

    // 攻擊技能牌庫
    pushbackVector(&p->attackSkill, 172); // 無所遁形 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->attackSkill, 109); // 目標清除 x3
    pushbackVector(&p->attackSkill, 169); // 殺戮指令 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->attackSkill, 108); // 目標鎖定 x2
    pushbackVector(&p->attackSkill, 107); // 目標確認 x1

    // 防禦技能牌庫
    pushbackVector(&p->defenseSkill, 172); // 無所遁形 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->defenseSkill, 112); // 讀取完畢 x3
    pushbackVector(&p->defenseSkill, 170); // 超越機器 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->defenseSkill, 111); // 深度搜索 x2
    pushbackVector(&p->defenseSkill, 110); // 思想刺探 x1

    // 移動技能牌庫
    pushbackVector(&p->moveSkill, 172); // 無所遁形 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->moveSkill, 115); // 使命終結 x3
    pushbackVector(&p->moveSkill, 171); // 獲准極刑 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->moveSkill, 114); // 進入視野 x2
    pushbackVector(&p->moveSkill, 113); // 發現敵蹤 x1

    // 必殺牌
    pushbackVector(&p->specialDeck, 116); // 獅子
    pushbackVector(&p->specialDeck, 117); // 鐵皮人
    pushbackVector(&p->specialDeck, 118); // 稻草人

    p->dorothy.COMBO_TOKEN = 0;
    p->dorothy.canCombo = 0;
}

void init_scheherazade(sPlayer* p) {
    p->maxlife = 36;
    p->life = 36;
    p->defense = 0;
    p->maxdefense = 6;
    p->energy = 0;
    p->specialGate = 18;

    // 攻擊技能牌庫
    pushbackVector(&p->attackSkill, 176); // 童話編織者 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->attackSkill, 121); // 扼殺存在 x3
    pushbackVector(&p->attackSkill, 173); // 命運之手 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->attackSkill, 120); // 銷毀記憶 x2
    pushbackVector(&p->attackSkill, 119); // 消除夢境 x1

    // 防禦技能牌庫
    pushbackVector(&p->defenseSkill, 176); // 童話編織者 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->defenseSkill, 124); // 消融之網 x3
    pushbackVector(&p->defenseSkill, 174); // 改寫欲望 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->defenseSkill, 123); // 沉迷之網 x2
    pushbackVector(&p->defenseSkill, 122); // 浸沒之網 x1

    // 移動技能牌庫
    pushbackVector(&p->moveSkill, 176); // 童話編織者 x1
    for (int i = 0; i < 3; i++) pushbackVector(&p->moveSkill, 127); // 支配之腦 x3
    pushbackVector(&p->moveSkill, 175); // 重組思想 x1
    for (int i = 0; i < 2; i++) pushbackVector(&p->moveSkill, 126); // 操縱之手 x2
    pushbackVector(&p->moveSkill, 125); // 監視之眼 x1

    // 必殺牌
    pushbackVector(&p->specialDeck, 128); // 系統刪除
    pushbackVector(&p->specialDeck, 129); // 無法自拔
    pushbackVector(&p->specialDeck, 130); // 切斷通路

    p->scheherazade.destiny_TOKEN_locate = initVector();
    p->scheherazade.destiny_TOKEN_type = initVector();
    p->scheherazade.selectToken = 0;
}
