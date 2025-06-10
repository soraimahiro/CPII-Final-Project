#include "card.h"

static const Card allCards[CARD_TOTAL_COUNT] = {
    // 基本牌 (ID 1~10)
    { CARD_BASIC_ATK1,    "LV1攻擊牌", TYPE_ATTACK,  1, 0, 1, 1, 0, "攻擊力1", 0, 0 },
    { CARD_BASIC_ATK2,    "LV2攻擊牌", TYPE_ATTACK,  2, 0, 1, 2, 0, "攻擊力2", 0, 0 },
    { CARD_BASIC_ATK3,    "LV3攻擊牌", TYPE_ATTACK,  3, 0, 1, 3, 0, "攻擊力3", 0, 0 },
    { CARD_BASIC_DEF1,    "LV1防禦牌", TYPE_DEFENSE, 1, 0, 0, 0, 1, "防禦力1", 0, 0 },
    { CARD_BASIC_DEF2,    "LV2防禦牌", TYPE_DEFENSE, 2, 0, 0, 0, 2, "防禦力2", 0, 0 },
    { CARD_BASIC_DEF3,    "LV3防禦牌", TYPE_DEFENSE, 3, 0, 0, 0, 3, "防禦力3", 0, 0 },
    { CARD_BASIC_MOVE1,   "LV1移動牌", TYPE_MOVE,    1, 0, 0, 0, 0, "移動力1", 0, 0 },
    { CARD_BASIC_MOVE2,   "LV2移動牌", TYPE_MOVE,    2, 0, 0, 0, 0, "移動力2", 0, 0 },
    { CARD_BASIC_MOVE3,   "LV3移動牌", TYPE_MOVE,    3, 0, 0, 0, 0, "移動力3", 0, 0 },
    { CARD_BASIC_COMMON,  "通用牌",    TYPE_BASIC,   1, 0, 0, 0, 0, "可當作LV1基本牌使用", 0, 0 },
    
    // 小紅帽 (ID 11~22)
    { CARD_REDHOOD_ATK1_QUICK_SHOT, "快速射擊", TYPE_ATTACK, 1, 0, 1, 1, 0, "射程1 傷害1+O", 0, 0 },
    { CARD_REDHOOD_ATK2_PRECISION_SHOT, "精準射擊", TYPE_ATTACK, 2, 2, 2, 2, 0, "射程2 傷害2+O", 0, 0 },
    { CARD_REDHOOD_ATK3_FATAL_SHOT, "致命狙擊", TYPE_ATTACK, 3, 4, 3, 3, 0, "射程3 傷害3+O", 0, 0 },
    { CARD_REDHOOD_DEF1_ENERGY_SHIELD,    "能量護盾", TYPE_DEFENSE, 1, 0, 1, 1, 1, "若防禦>0，射程1對手受2傷害", 0, 0 },
    { CARD_REDHOOD_DEF2_CURRENT_SHIELD,    "電流護盾", TYPE_DEFENSE, 2, 2, 2, 2, 2, "若防禦>0，射程2對手受4傷害", 0, 0 },
    { CARD_REDHOOD_DEF3_ULTIMATE_SHIELD,    "終極護盾", TYPE_DEFENSE, 3, 4, 3, 3, 3, "若防禦>0，射程3對手受6傷害", 0, 0 },
    { CARD_REDHOOD_MOVE1_JET,    "彈道噴射", TYPE_MOVE,    1, 0, 1, 1, 0, "射程1 傷害1, 將對手擊退至多O格", 0, 0 },
    { CARD_REDHOOD_MOVE2_FIRE_JET,    "火力噴射", TYPE_MOVE,    2, 2, 2, 2, 0, "購買費用2, 射程2 傷害2, 將對手擊退至多O格", 0, 0 },
    { CARD_REDHOOD_MOVE3_RAGE_JET,    "暴怒噴射", TYPE_MOVE,    3, 4, 3, 3, 0, "購買費用4, 射程3 傷害3, 將對手擊退至多O格", 0, 0 },
    { CARD_REDHOOD_SPECIAL1_WOLF_HUNGER,    "餓狼吞噬", TYPE_SPECIAL, 0, 0, 0, 0, 0, "將任意技能供應牌庫頂部兩張加入手牌", 0, 0 },
    { CARD_REDHOOD_SPECIAL2_SYSTEM_HACK,    "系統入侵", TYPE_SPECIAL, 0, 0, 0, 0, 0, "重複本回合內一個行動或技能效果", 0, 0 },
    { CARD_REDHOOD_SPECIAL3_RAIN_OF_VENGEANCE,    "復仇之雨", TYPE_ATTACK,  3, 4, 3, 3, 0, "射程3 傷害3, 將對手擊退3格, 捨三張手牌", 0, 0 },
    // 白雪公主 (ID 23~34, 131~133)
    { CARD_SNOWWHITE_ATK1_CRYSTAL_SHARD, "水晶碎片", TYPE_ATTACK,  1, 0, 1, 1, 0, "射程1 傷害1+O, 對手牌庫頂1棄牌", 0, 0 },
    { CARD_SNOWWHITE_ATK2_CRYSTAL_VORTEX,    "水晶漩渦", TYPE_ATTACK,  2, 2, 1, 2, 0, "射程1 傷害2+O, 對手牌庫頂2棄牌", 0, 0 },
    { CARD_SNOWWHITE_ATK3_CRYSTAL_STORM,      "水晶風暴", TYPE_ATTACK,  3, 4, 1, 3, 0, "射程1 傷害3+O, 對手牌庫頂3棄牌", 0, 0 },
    { CARD_SNOWWHITE_DEF1_TAINTED_GRACE,    "玷污的恩惠",  TYPE_DEFENSE, 1, 0, 1, 1, 1, "射程1 傷害1, 中毒牌庫頂O棄牌", 0, 0 },
    { CARD_SNOWWHITE_DEF2_TAINTED_FEAST, "玷污的盛筵",  TYPE_DEFENSE, 2, 2, 1, 2, 1, "射程1 傷害2, 中毒牌庫頂O棄牌", 0, 0 },
    { CARD_SNOWWHITE_DEF3_TAINTED_CARNIVAL, "玷污的狂歡", TYPE_DEFENSE, 3, 4, 1, 3, 1, "射程1 傷害3, 中毒牌庫頂O棄牌", 0, 0 },
    { CARD_SNOWWHITE_MOVE1_BROKEN_FANTASY, "破碎的幻想", TYPE_MOVE,    1, 0, 0, 1, 0, "射程O 傷害1, 移至與對手相鄰", 0, 0 },
    { CARD_SNOWWHITE_MOVE2_BROKEN_REALITY, "破碎的現實", TYPE_MOVE,    2, 2, 1, 2, 0, "購買費用2, 射程1+O, 傷害2, 移至與對手相鄰", 0, 0 },
    { CARD_SNOWWHITE_MOVE3_BROKEN_FATE, "破碎的命運",  TYPE_MOVE,    3, 4, 2, 3, 0, "購買費用4, 射程2+O, 傷害3, 移至與對手相鄰", 0, 0 },
    { CARD_SNOWWHITE_SPECIAL1_SEVEN_SNAKES_RAGE,    "七蛇之怒",    TYPE_SPECIAL, 0, 0, 1, 0, 0, "射程1, 對手失去X點生命 (X=中毒牌數)", 0, 0 },
    { CARD_SNOWWHITE_SPECIAL2_MAGIC_MIRROR_RAIN,    "魔鏡之雨",    TYPE_SPECIAL, 0, 0, 1, 3, 0, "射程1, 傷害3, 捨全手牌後抽4", 0, 0 },
    { CARD_SNOWWHITE_SPECIAL3_BREWED_DISASTER, "醞釀之災",   TYPE_SPECIAL, 0, 0, 3, 3, 0, "射程3, 傷害3, 可選中毒棄牌洗入牌庫", 0, 0 },
    { CARD_SNOWWHITE_STATUS_POISON1, "中毒1",     TYPE_SPECIAL, 1, 0, 0, 0, 0, "中毒牌 Lv1", 0, 0 },
    { CARD_SNOWWHITE_STATUS_POISON2, "中毒2",     TYPE_SPECIAL, 2, 0, 0, 0, 0, "中毒牌 Lv2", 0, 0 },
    { CARD_SNOWWHITE_STATUS_POISON3, "中毒3",     TYPE_SPECIAL, 3, 0, 0, 0, 0, "中毒牌 Lv3", 0, 0 },

    // 睡美人 (ID 35~46)
    { CARD_SLEEPINGBEAUTY_ATK1_MIND_SHOCK,   "心靈震顫",   TYPE_ATTACK,  1, 0, 1, 1, 0, "射程1 傷害1+O, 可對自己造成最多1傷害", 0, 0 },
    { CARD_SLEEPINGBEAUTY_ATK2_MIND_RAGE,    "心靈之怒",   TYPE_ATTACK,  2, 2, 1, 2, 0, "射程1 傷害2×O, 可對自己造成最多2傷害", 0, 0 },
    { CARD_SLEEPINGBEAUTY_ATK3_MIND_FURY,    "心靈狂怒",   TYPE_ATTACK,  3, 4, 1, 3, 0, "射程1 傷害3×O, 可對自己造成最多3傷害", 0, 0 },
    { CARD_SLEEPINGBEAUTY_DEF1_BURST_CHAIN,  "爆裂之鎖",   TYPE_DEFENSE, 1, 0, 0, 0, 1, "防禦1+O, 提升之後攻擊加1", 0, 0 },
    { CARD_SLEEPINGBEAUTY_DEF2_BURST_BONE,   "爆裂之骨",   TYPE_DEFENSE, 2, 2, 0, 0, 2, "防禦2+O, 提升之後攻擊加2", 0, 0 },
    { CARD_SLEEPINGBEAUTY_DEF3_BURST_SOUL,   "爆裂之魂",   TYPE_DEFENSE, 3, 4, 0, 0, 3, "防禦3+O, 提升之後攻擊加3", 0, 0 },
    { CARD_SLEEPINGBEAUTY_MOVE1_DARK_TOUCH,  "黑暗碰觸",   TYPE_MOVE,    1, 0, 2, 0, 0, "射程2, 傷害O+⬖, 將對手拉向你", 0, 0 },
    { CARD_SLEEPINGBEAUTY_MOVE2_DARK_ENTANGLE,"黑暗糾纏",  TYPE_MOVE,    2, 2, 3, 0, 0, "購買費用2, 射程3, 傷害O+⬖, 將對手拉向你", 0, 0 },
    { CARD_SLEEPINGBEAUTY_MOVE3_DARK_STRANGLE,"黑暗絞殺",  TYPE_MOVE,    3, 4, 4, 0, 0, "購買費用4, 射程4, 傷害O+⬖, 將對手拉向你", 0, 0 },
    { CARD_SLEEPINGBEAUTY_SPECIAL1_AWAKEN_SLEEP, "喚醒沉睡", TYPE_SPECIAL, 0, 0, 0, 0, 0, "獲得3覺醒TOKEN，若沉睡則直接覺醒", 0, 0 },
    { CARD_SLEEPINGBEAUTY_SPECIAL2_DAYMARE,      "白日夢魘", TYPE_SPECIAL, 0, 0, 0, 0, 0, "持續至下回合，每受傷抽等量牌", 0, 0 },
    { CARD_SLEEPINGBEAUTY_SPECIAL3_BLOOD_RECAST, "血脈重鑄", TYPE_SPECIAL, 0, 0, 0, 0, 0, "花費覺醒TOKEN，每1恢復1生命", 0, 0 },

    // 愛麗絲 (ID 47~58)
    { CARD_ALICE_ATK1_START_GAME,           "開啟牌局",   TYPE_ATTACK,  1, 0, 1, 1, 0, "射程1 傷害1, 對手展示3張基本牌", 0, 0 },
    { CARD_ALICE_ATK2_REVERSE_GAME,         "扭轉牌局",   TYPE_ATTACK,  2, 2, 2, 2, 0, "購買費用2, 射程2 傷害2, 對手展示4張牌", 0, 0 },
    { CARD_ALICE_ATK3_CONTROL_GAME,         "操控牌局",   TYPE_ATTACK,  3, 4, 3, 3, 0, "購買費用4, 射程3 傷害3, 對手展示5張牌", 0, 0 },
    { CARD_ALICE_DEF1_MAGIC_TECHNIQUE,      "魔力技巧",   TYPE_DEFENSE, 1, 0, 0, 0, 1, "防禦1, 移除一張基本牌後變瘋帽子", 0, 0 },
    { CARD_ALICE_DEF2_MENTAL_ILLUSION,      "精神幻術",   TYPE_DEFENSE, 2, 2, 0, 0, 2, "購買費用2, 防禦2, 變瘋帽子", 0, 0 },
    { CARD_ALICE_DEF3_HAT_TRICK,            "帽子戲法",   TYPE_DEFENSE, 3, 4, 0, 0, 3, "購買費用4, 防禦3, 變瘋帽子", 0, 0 },
    { CARD_ALICE_MOVE1_WEIRD_AGILITY,       "詭異的敏捷", TYPE_MOVE,    1, 0, 1, 1, 0, "向一方向移動，穿過對手抽牌，同時變柴郡貓", 0, 0 },
    { CARD_ALICE_MOVE2_WEIRD_STEALTH,       "詭異的隱蔽", TYPE_MOVE,    2, 2, 2, 2, 0, "購買費用2, 向一方向移動+抽牌，變柴郡貓", 0, 0 },
    { CARD_ALICE_MOVE3_WEIRD_WEIRDNESS,     "詭異的詭異", TYPE_MOVE,    3, 4, 3, 3, 0, "購買費用4, 向一方向移動+抽牌，變柴郡貓", 0, 0 },
    { CARD_ALICE_SPECIAL1_ENDLESS_PARTY,    "永無止盡的派對", TYPE_SPECIAL, 0, 0, 0, 0, 0, "回合結束後抽取手牌數量等同牌數", 0, 0 },
    { CARD_ALICE_SPECIAL2_WONDERFUL_DAY,    "美妙的一天", TYPE_SPECIAL, 0, 0, 0, 0, 0, "本回合攻擊、防禦、移動牌數值+1", 0, 0 },
    { CARD_ALICE_SPECIAL3_GAME_IN_CONTROL,  "操控全局",   TYPE_SPECIAL, 0, 0, 3, 3, 0, "射程3 傷害3, 將最多5張對手牌洗入牌庫", 0, 0 },
    // 花木蘭 (ID 59~70)
    { CARD_MULAN_ATK1_UNDERESTIMATED, "不容小覷", TYPE_ATTACK, 1, 0, 1, 1, 0, "射程1 傷害1+O, 將對手移至相鄰格", 0, 0 },
    { CARD_MULAN_ATK2_IRRESISTIBLE, "勢不可擋", TYPE_ATTACK, 2, 2, 1, 2, 0, "購買費用2, 射程1 傷害2+O", 0, 0 },
    { CARD_MULAN_ATK3_UNBREAKABLE, "堅不可摧", TYPE_ATTACK, 3, 4, 1, 3, 0, "購買費用4, 射程1 傷害3+O", 0, 0 },
    { CARD_MULAN_DEF1_STILLNESS, "以靜制動", TYPE_DEFENSE, 1, 0, 0, 0, 1, "防禦O, 回合結束時額外抽1牌", 0, 0 },
    { CARD_MULAN_DEF2_SOFTNESS, "以柔克剛", TYPE_DEFENSE, 2, 2, 0, 0, 1, "購買費用2, 防禦O, 回合結束時抽牌", 0, 0 },
    { CARD_MULAN_DEF3_WEAK_OVERCOME_STRONG, "以弱勝強", TYPE_DEFENSE, 3, 4, 0, 0, 1, "購買費用4, 防禦O, 回合結束時抽牌", 0, 0 },
    { CARD_MULAN_MOVE1_NEVER_RETREAT, "永不退縮", TYPE_MOVE, 1, 0, 1, 1, 0, "射程1 傷害1, 將對手擊退並移至相鄰格", 0, 0 },
    { CARD_MULAN_MOVE2_NO_MERCY, "毫不留情", TYPE_MOVE, 2, 2, 1, 2, 0, "購買費用2, 射程1 傷害2, 擊退對手", 0, 0 },
    { CARD_MULAN_MOVE3_NO_FORGIVENESS, "絕不饒恕", TYPE_MOVE, 3, 4, 1, 3, 0, "購買費用4, 射程1 傷害3, 擊退對手", 0, 0 },
    { CARD_MULAN_SPECIAL1_SOARING, "氣衝霄漢", TYPE_SPECIAL, 0, 0, 0, 0, 0, "回合結束時額外抽4張牌", 0, 0 },
    { CARD_MULAN_SPECIAL2_FACE_CHAOS, "面對混沌", TYPE_SPECIAL, 0, 0, 0, 0, 0, "立即移至對手相鄰格並獲3氣", 0, 0 },
    { CARD_MULAN_SPECIAL3_THUNDER_STRIKE, "雷霆一擊", TYPE_SPECIAL, 0, 0, 1, 0, 0, "射程1, 立即花費所有氣獲得傷害 (數值=氣量)", 0, 0 },
    // 輝夜姬 (ID 71~82)
    { CARD_KAGUYA_ATK1_ENLIGHTENED_LIGHT, "領悟的光芒", TYPE_ATTACK, 1, 0, 1, 1, 0, "射程1 傷害1+O, 若防有3則+1", 0, 0 },
    { CARD_KAGUYA_ATK2_ENLIGHTENED_GLORY, "領悟的榮耀", TYPE_ATTACK, 2, 2, 1, 2, 0, "購買費用2, 射程1 傷害2+O, 若防有3則+1", 0, 0 },
    { CARD_KAGUYA_ATK3_ENLIGHTENED_AVATAR, "領悟的化身", TYPE_ATTACK, 3, 4, 1, 3, 0, "購買費用4, 射程1 傷害3+O, 若防有3則+1", 0, 0 },
    { CARD_KAGUYA_DEF1_CONFUSED_ECHO, "困惑的回聲", TYPE_DEFENSE, 1, 0, 1, 0, 1, "防禦1+O, 展示牌庫頂1張", 0, 0 },
    { CARD_KAGUYA_DEF2_ANCIENT_ECHO, "久遠的回響", TYPE_DEFENSE, 2, 2, 2, 0, 2, "購買費用2, 展示牌庫頂2張", 0, 0 },
    { CARD_KAGUYA_DEF3_DIVINE_SUMMON, "神性的召喚", TYPE_DEFENSE, 3, 4, 3, 0, 3, "購買費用4, 展示牌庫頂3張", 0, 0 },
    { CARD_KAGUYA_MOVE1_FOCUSED_INTROSPECTION, "專注的自省", TYPE_MOVE, 1, 0, 0, 1, 0, "射程O 傷害1, 可失1生命換牌", 0, 0 },
    { CARD_KAGUYA_MOVE2_EPIPHANY, "頓悟的決心", TYPE_MOVE, 2, 2, 0, 2, 0, "購買費用2, 射程O 傷害2, 失1生命換牌", 0, 0 },
    { CARD_KAGUYA_MOVE3_PAINFUL_PURIFY, "痛徹的淨化", TYPE_MOVE, 3, 4, 0, 3, 0, "購買費用4, 射程O 傷害3, 失1生命（下回合條件）", 0, 0 },
    { CARD_KAGUYA_SPECIAL1_BLAZING_BAMBOO, "炙熱的竹刀", TYPE_SPECIAL, 0, 0, 0, 0, 6, "持續至下回合開始, 不受傷害", 0, 0 },
    { CARD_KAGUYA_SPECIAL2_FATED_JUDGEMENT, "注定的審判", TYPE_SPECIAL, 0, 0, 0, 0, 6, "下回合開始時, 對手防低則失去差值生命", 0, 0 },
    { CARD_KAGUYA_SPECIAL3_RESTLESS_BLOOD, "躁動的血性", TYPE_SPECIAL, 0, 0, 0, 3, 0, "將對手移至相鄰格並造成3點傷害", 0, 0 },
    // 美人魚 (ID 83~94)
    { CARD_MERMAID_ATK1_SIREN_SUMMON, "海妖的召喚", TYPE_ATTACK, 1, 0, 1, 1, 0, "射程1 傷害1+O, 觸手加成", 0, 0 },
    { CARD_MERMAID_ATK2_SIREN_SONG, "海妖的歌聲", TYPE_ATTACK, 2, 2, 2, 2, 0, "購買費用2, 射程2 傷害2+O, 觸手加成", 0, 0 },
    { CARD_MERMAID_ATK3_SIREN_SCREAM, "海妖的尖嘯", TYPE_ATTACK, 3, 4, 3, 3, 0, "購買費用4, 射程3 傷害3+O, 觸手加成", 0, 0 },
    { CARD_MERMAID_DEF1_SURGING_RAGE, "洶湧之怒", TYPE_DEFENSE, 1, 0, 0, 0, 1, "若對手在觸手格, 對其造成O傷害，且移至觸手格獲防1", 0, 0 },
    { CARD_MERMAID_DEF2_ERUPTING_RAGE, "噴薄之怒", TYPE_DEFENSE, 2, 2, 0, 0, 2, "購買費用2, 觸手效果升級", 0, 0 },
    { CARD_MERMAID_DEF3_REVENGE_RAGE, "復仇之怒", TYPE_DEFENSE, 3, 4, 0, 0, 3, "購買費用4, 觸手效果進階", 0, 0 },
    { CARD_MERMAID_MOVE1_ABYSS_DEVOUR, "深淵的蠶食", TYPE_MOVE, 1, 0, 1, 1, 0, "選擇一觸手, 移動至多O格，如位於觸手格抽1牌", 0, 0 },
    { CARD_MERMAID_MOVE2_ABYSS_INVASION, "深淵的入侵", TYPE_MOVE, 2, 2, 1, 2, 0, "購買費用2, 移動觸手，如位於觸手格抽2牌且對手捨1牌", 0, 0 },
    { CARD_MERMAID_MOVE3_ABYSS_CONQUEST, "深淵的征服", TYPE_MOVE, 3, 4, 1, 3, 0, "購買費用4, 移動觸手，如位於觸手格抽3牌且對手捨2牌", 0, 0 },
    { CARD_MERMAID_SPECIAL1_MERMAID_REVIVAL, "人魚的重生", TYPE_SPECIAL, 0, 0, 1, 0, 0, "生命上限變18，回合結束恢復2生命", 0, 0 },
    { CARD_MERMAID_SPECIAL2_ANCIENT_AWAKEN, "古老的覺醒", TYPE_SPECIAL, 0, 0, 0, 0, 0, "將所有觸手移至你位置；對手傷害減X (X=觸手數)", 0, 0 },
    { CARD_MERMAID_SPECIAL3_PURIFYING_TIDE, "淨化之潮", TYPE_SPECIAL, 0, 0, 0, 0, 0, "觸手進對手格，對手失1生命；進觸手格失X (X=觸手數)", 0, 0 },

    // 火柴女孩 (ID 95~106, 134)
    { CARD_MATCHGIRL_ATK1_ILLUSORY_WISH, "虛幻的願望", TYPE_ATTACK, 1, 0, 1, 1, 0, "射程1 傷害1+O，能量每3加1傷害", 0, 0 },
    { CARD_MATCHGIRL_ATK2_SECRET_HOPE, "隱密的期望", TYPE_ATTACK, 2, 2, 1, 2, 0, "購買費用2, 射程1 傷害2+O，能量每3加1傷害", 0, 0 },
    { CARD_MATCHGIRL_ATK3_INSATIABLE_DESIRE, "無厭的奢望", TYPE_ATTACK, 3, 4, 1, 3, 0, "購買費用4, 射程1 傷害3+O，能量每3加1傷害", 0, 0 },
    { CARD_MATCHGIRL_DEF1_DEMONIC_SACRIFICE, "惡魔的祭品", TYPE_DEFENSE, 1, 0, 0, 0, 1, "防禦1，可失生命抽牌（最多1）", 0, 0 },
    { CARD_MATCHGIRL_DEF2_DEMONIC_BET, "惡魔的賭注", TYPE_DEFENSE, 2, 2, 0, 0, 1, "購買費用2, 防禦1，可失生命抽牌（最多2）", 0, 0 },
    { CARD_MATCHGIRL_DEF3_DEMONIC_PACT, "惡魔的契約", TYPE_DEFENSE, 3, 4, 0, 0, 1, "購買費用4, 防禦1，可失生命抽牌（最多3）", 0, 0 },
    { CARD_MATCHGIRL_MOVE1_WEIGHTLESS_SOUL, "失重的靈魂", TYPE_MOVE, 1, 0, 1, 1, 0, "射程1 傷害1, 從對手棄牌中取回火柴", 0, 0 },
    { CARD_MATCHGIRL_MOVE2_INDEBTED_SOUL, "虧欠的靈魂", TYPE_MOVE, 2, 2, 2, 2, 0, "射程2 傷害2, 取回火柴獲能量+2，恢復1生命", 0, 0 },
    { CARD_MATCHGIRL_MOVE3_BROKEN_SOUL, "殘破的靈魂", TYPE_MOVE, 3, 4, 3, 3, 0, "射程3 傷害3, 每取1火柴獲能量+3，恢復2生命", 0, 0 },
    { CARD_MATCHGIRL_SPECIAL1_HELLFIRE, "地獄火", TYPE_SPECIAL, 0, 0, 1, 0, 0, "射程1，傷害為你能量的一半（向上取整）", 0, 0 },
    { CARD_MATCHGIRL_SPECIAL2_DOOM, "世界末日", TYPE_SPECIAL, 0, 0, 3, 0, 0, "射程3，對手牌庫頂棄6火柴，每火1傷", 0, 0 },
    { CARD_MATCHGIRL_SPECIAL3_GREED_CURSE, "貪婪詛咒", TYPE_SPECIAL, 0, 0, 0, 0, 0, "將最多3張火柴放到對手牌庫頂", 0, 0 },
    { CARD_MATCHGIRL_ITEM_MATCH, "火柴", TYPE_SPECIAL, 0, 0, 0, 0, 0, "火柴牌", 0, 0 },
    // 桃樂絲 (ID 107~118)
    { CARD_DOROTHY_ATK1_TARGET_CONFIRM,   "目標確認",   TYPE_ATTACK, 1, 0, 1, 1, 0, "射程1 傷害1+O, 擊退對手, 邊緣時加傷", 0, 0 },
    { CARD_DOROTHY_ATK2_TARGET_LOCK,      "目標鎖定",   TYPE_ATTACK, 2, 2, 1, 2, 0, "購買費用2, 射程1 傷害2+O, 擊退對手", 0, 0 },
    { CARD_DOROTHY_ATK3_TARGET_CLEAR,     "目標清除",   TYPE_ATTACK, 3, 4, 1, 3, 0, "購買費用4, 射程1 傷害3+O, 擊退對手", 0, 0 },
    { CARD_DOROTHY_DEF1_MIND_PROBE,       "思想刺探",  TYPE_DEFENSE, 1, 0, 6, 0, 0, "射程6 傷害O, 可棄1牌後抽X+1牌", 0, 0 },
    { CARD_DOROTHY_DEF2_DEEP_SEARCH,      "深度搜索",  TYPE_DEFENSE, 2, 2, 7, 0, 0, "購買費用2, 射程7 傷害O, 棄多1牌後抽X+1牌", 0, 0 },
    { CARD_DOROTHY_DEF3_READ_COMPLETE,    "讀取完畢", TYPE_DEFENSE, 3, 4, 8, 0, 0, "購買費用4, 射程8 傷害O, 棄多3牌後抽X+1牌", 0, 0 },
    { CARD_DOROTHY_MOVE1_FIND_ENEMY,      "發現敵蹤", TYPE_MOVE,    1, 0, 1, 0, 0, "射程1+O, 傷害X+1 (X為距離)", 0, 0 },
    { CARD_DOROTHY_MOVE2_ENTER_SIGHT,     "進入視野",  TYPE_MOVE,    2, 2, 2, 0, 0, "購買費用2, 射程2+O, 傷害X+1 (X為距離)", 0, 0 },
    { CARD_DOROTHY_MOVE3_MISSION_END,     "使命終結",  TYPE_MOVE,    3, 4, 3, 0, 0, "購買費用4, 射程3+O, 傷害X+1 (X為距離)", 0, 0 },
    { CARD_DOROTHY_SPECIAL1_LION,         "獅子",       TYPE_SPECIAL, 0, 0, 1, 0, 0, "花費連擊TOKEN, 獲得X傷害及X能量", 0, 0 },
    { CARD_DOROTHY_SPECIAL2_TINMAN,       "鐵皮人",     TYPE_SPECIAL, 0, 0, 0, 0, 0, "花費連擊TOKEN, 恢復X點生命", 0, 0 },
    { CARD_DOROTHY_SPECIAL3_SCARECROW,    "稻草人",     TYPE_SPECIAL, 0, 0, 0, 0, 0, "花費連擊TOKEN, 抽取ceil(X/2)張牌", 0, 0 },

    // 山魯佐德 (ID 119~130)
    { CARD_SCHEHERAZADE_ATK1_ERASE_DREAM,    "消除夢境",  TYPE_ATTACK, 1, 0, 1, 1, 0, "射程1 傷害1+O, 翻轉至多1藍色命運TOKEN", 0, 0 },
    { CARD_SCHEHERAZADE_ATK2_DESTROY_MEMORY, "銷毀記憶", TYPE_ATTACK, 2, 2, 2, 2, 0, "購買費用2, 射程2 傷害2+O, 翻轉至多2藍色命運TOKEN", 0, 0 },
    { CARD_SCHEHERAZADE_ATK3_STRANGLE_EXISTENCE, "扼殺存在", TYPE_ATTACK, 3, 4, 3, 3, 0, "購買費用4, 射程3 傷害3+O, 翻轉至多3藍色命運TOKEN", 0, 0 },
    { CARD_SCHEHERAZADE_DEF1_IMMERSE_NET,      "浸沒之網", TYPE_DEFENSE, 1, 0, 0, 0, 1, "防禦O, 將1枚命運TOKEN移至對手供應牌庫", 0, 0 },
    { CARD_SCHEHERAZADE_DEF2_INDULGE_NET,      "沈迷之網", TYPE_DEFENSE, 2, 2, 0, 0, 1, "購買費用2, 將最多2枚命運TOKEN移至對手牌庫", 0, 0 },
    { CARD_SCHEHERAZADE_DEF3_DISSOLVE_NET,      "消融之網", TYPE_DEFENSE, 3, 4, 0, 0, 1, "購買費用4, 將最多3枚命運TOKEN移至對手牌庫", 0, 0 },
    { CARD_SCHEHERAZADE_MOVE1_WATCHFUL_EYE, "監視之眼", TYPE_MOVE, 1, 0, 1, 1, 0, "射程1 傷害1, 展示對手牌庫頂O張牌", 0, 0 },
    { CARD_SCHEHERAZADE_MOVE2_MANIPULATE_HAND, "操縱之手", TYPE_MOVE, 2, 2, 2, 2, 0, "購買費用2, 射程2 傷害2, 展示對手牌庫頂O張牌", 0, 0 },
    { CARD_SCHEHERAZADE_MOVE3_DOMINATE_BRAIN, "支配之腦", TYPE_MOVE, 3, 4, 3, 3, 0, "購買費用4, 射程3 傷害3, 展示對手牌庫頂O張牌", 0, 0 },
    { CARD_SCHEHERAZADE_SPECIAL1_SYSTEM_DELETE, "系統刪除", TYPE_SPECIAL, 0, 0, 3, 3, 0, "射程3 傷害3, 使對手指定供應牌庫所有牌全棄，傷害=棄牌數", 0, 0 },
    { CARD_SCHEHERAZADE_SPECIAL2_UNABLE_TO_EXTRICATE, "無法自拔", TYPE_SPECIAL, 0, 0, 3, 3, 0, "射程3 傷害3, 選定牌庫，每該牌額外加傷", 0, 0 },
    { CARD_SCHEHERAZADE_SPECIAL3_CUT_OFF_PATH, "切斷通路", TYPE_SPECIAL, 0, 0, 3, 0, 0, "射程3 傷害X, 翻轉最多3枚命運TOKEN (X為翻轉枚數)", 0, 0 },
    // 蛻變牌部分
    // 小紅帽 (ID 135~138)
    { CARD_REDHOOD_METAMORPH1_OVERLOAD_BURN, "過載燃燒", TYPE_METAMORPH, 0, 0, 0, 0, 0, "使用攻擊/移動時，捨1技能牌獲傷害+X (X=牌等級)", 0, 0 },
    { CARD_REDHOOD_METAMORPH2_HOOD_SYSTEM, "兜帽系統", TYPE_METAMORPH, 0, 0, 0, 0, 0, "每當受到傷害時，可捨牌減傷X (X=牌等級)", 0, 0 },
    { CARD_REDHOOD_METAMORPH3_MUTATION_SENSE, "變異感應", TYPE_METAMORPH, 0, 0, 0, 0, 0, "使用移動/攻擊時，可捨牌獲射程+X (X=牌等級)", 0, 0 },
    { CARD_REDHOOD_METAMORPH4_ONBOARD_CACHE, "板載緩存", TYPE_METAMORPH, 0, 0, 0, 0, 0, "行動回合中可存放一張牌，此牌不會棄置", 0, 0 },
    // 白雪公主 (ID 139~142)
    { CARD_SNOWWHITE_METAMORPH1_CRYSTAL_COFFIN, "水晶之棺", TYPE_METAMORPH, 0, 0, 0, 0, 0, "當攻擊造成≥2傷害時，對手中毒牌頂1移棄", 0, 0 },
    { CARD_SNOWWHITE_METAMORPH2_FALLEN_CALAMITY, "墮落之劫", TYPE_METAMORPH, 0, 0, 0, 0, 0, "使用攻/防時，捨一中毒牌可洗入牌庫代替", 0, 0 },
    { CARD_SNOWWHITE_METAMORPH3_TOXIC_EROSION, "劇毒之蝕", TYPE_METAMORPH, 0, 0, 0, 0, 0, "攻擊後，若對手中毒則額外傷害+1", 0, 0 },
    { CARD_SNOWWHITE_METAMORPH4_PURIST_POISON, "至純之毒", TYPE_METAMORPH, 0, 0, 0, 0, 0, "中毒牌進棄牌時，對手額外失1生命，可累積", 0, 0 },
    // 睡美人 (ID 143~146)
    { CARD_SLEEPINGBEAUTY_METAMORPH1_BLOODLETTING, "放血療法", TYPE_METAMORPH, 0, 0, 0, 0, 0, "出基本牌時，可選失2/4/6生命當Lv1/2/3牌", 0, 0 },
    { CARD_SLEEPINGBEAUTY_METAMORPH2_BLOOD_RITUAL, "血祭之禮", TYPE_METAMORPH, 0, 0, 0, 0, 0, "回合限一次，攻擊造成≥2傷害時，從棄牌中回Lv牌", 0, 0 },
    { CARD_SLEEPINGBEAUTY_METAMORPH3_MENTAL_BARRIER, "精神屏障", TYPE_METAMORPH, 0, 0, 0, 0, 0, "使用移動時，可額外獲防禦X (X=技能等級)", 0, 0 },
    { CARD_SLEEPINGBEAUTY_METAMORPH4_FORCED_TREATMENT, "強制治療", TYPE_METAMORPH, 0, 0, 0, 0, 0, "立即恢復5生命，移除此牌", 0, 0 },
    // 愛麗絲 (ID 147~150)
    { CARD_ALICE_METAMORPH1_OFF_WITH_HER_HEAD, "砍掉她的頭", TYPE_METAMORPH, 0, 0, 0, 0, 0, "當身份為紅心皇后時，攻擊可將牌回手代替", 0, 0 },
    { CARD_ALICE_METAMORPH2_WONDERLAND_DESCENT, "仙境降臨", TYPE_METAMORPH, 0, 0, 0, 0, 0, "身份為瘋帽子時，防禦牌轉為回手牌", 0, 0 },
    { CARD_ALICE_METAMORPH3_ALL_MAD, "我們全是瘋子", TYPE_METAMORPH, 0, 0, 0, 0, 0, "身份為柴郡貓時，穿越對手時抽1張牌", 0, 0 },
    { CARD_ALICE_METAMORPH4_SHOWTIME, "開始我的表演", TYPE_METAMORPH, 0, 0, 0, 0, 0, "回合結束時額外抽1牌，可累積", 0, 0 },
    // 花木蘭 (ID 151~154)
    { CARD_MULAN_METAMORPH1_QI_FLOW, "氣慣全身", TYPE_METAMORPH, 0, 0, 0, 0, 0, "攻擊行動時，可花氣獲得傷害+X", 0, 0 },
    { CARD_MULAN_METAMORPH2_DOMINATE_FATE, "主宰命運", TYPE_METAMORPH, 0, 0, 0, 0, 0, "回合結束後，可捨牌換手牌", 0, 0 },
    { CARD_MULAN_METAMORPH3_CHARGE, "長驅直入", TYPE_METAMORPH, 0, 0, 0, 0, 0, "使用移動時，可花氣向前額外移X格", 0, 0 },
    { CARD_MULAN_METAMORPH4_BEFORE_STORM, "暴風前夕", TYPE_METAMORPH, 0, 0, 0, 0, 0, "回合開始時獲1點氣，效果可累積", 0, 0 },
    // 輝夜姬 (ID 155~158)
    { CARD_KAGUYA_METAMORPH1_PENALTY_TIME, "懲戒時刻", TYPE_METAMORPH, 0, 0, 0, 0, 0, "防禦上限+1，每回合限一次，可將防禦牌當攻擊牌用", 0, 0 },
    { CARD_KAGUYA_METAMORPH2_BLOOD_MOON, "血色月光", TYPE_METAMORPH, 0, 0, 0, 0, 0, "防禦上限+1，清理階段每重置3防抽1牌", 0, 0 },
    { CARD_KAGUYA_METAMORPH3_SPIRIT_INSTINCT, "靈性本能", TYPE_METAMORPH, 0, 0, 0, 0, 0, "每回合限一次，若對手防低時可移動其1格", 0, 0 },
    { CARD_KAGUYA_METAMORPH4_MOONLIT_MEDITATION, "月下沉思", TYPE_METAMORPH, 0, 0, 0, 0, 0, "回合結束時獲得2點防禦", 0, 0 },
    // 火柴女孩 (ID 163~168)
    { CARD_MATCHGIRL_METAMORPH1_PAINFUL_RITUAL, "痛苦的儀式", TYPE_METAMORPH, 0, 0, 0, 0, 0, "使用攻擊時，可從對手棄牌中取回火柴，傷害+2", 0, 0 },
    { CARD_MATCHGIRL_METAMORPH2_INDULGENT_DESIRE, "放縱的渴望", TYPE_METAMORPH, 0, 0, 0, 0, 0, "使用防禦時，可從對手棄牌中取回火柴，額外抽1牌", 0, 0 },
    { CARD_MATCHGIRL_METAMORPH3_DEMONIC_GAZE, "魔鬼的凝視", TYPE_METAMORPH, 0, 0, 0, 0, 0, "當生命因傷減少後，可從對手棄牌中取回火柴並移1格", 0, 0 },
    { CARD_MATCHGIRL_METAMORPH4_FLAME_TEASE, "火焰的捉弄", TYPE_METAMORPH, 0, 0, 0, 0, 0, "對手使用火柴牌時，額外獲得1點能量", 0, 0 },
    { CARD_MATCHGIRL_METAMORPH5_DESIRE_TEASE, "欲望的捉弄", TYPE_METAMORPH, 0, 0, 0, 0, 0, "對手使用火柴牌時，額外獲得1點能量", 0, 0 },
    { CARD_MATCHGIRL_METAMORPH6_FATE_TEASE, "命運的捉弄", TYPE_METAMORPH, 0, 0, 0, 0, 0, "對手使用火柴牌時，額外獲得1點能量", 0, 0 },
    // 桃樂絲 (ID 169~172)
    { CARD_DOROTHY_METAMORPH1_KILL_COMMAND, "殺戮指令", TYPE_METAMORPH, 0, 0, 0, 0, 0, "使用攻擊後可連擊，傷害增加", 0, 0 },
    { CARD_DOROTHY_METAMORPH2_BEYOND_MACHINE, "超越機器", TYPE_METAMORPH, 0, 0, 0, 0, 0, "連擊時，若後一技能等級高於前一技能則額外獲連擊TOKEN", 0, 0 },
    { CARD_DOROTHY_METAMORPH3_EXECUTION_GRANTED, "獲准極刑", TYPE_METAMORPH, 0, 0, 0, 0, 0, "使用技能後可捨移動牌，將對手移動至多X格", 0, 0 },
    { CARD_DOROTHY_METAMORPH4_NOWHERE_TO_HIDE, "無所遁形", TYPE_METAMORPH, 0, 0, 0, 0, 0, "回合結束時額外抽1牌，可累積連擊TOKEN", 0, 0 },
    // 山魯佐德 (ID 173~176)
    { CARD_SCHEHERAZADE_METAMORPH1_FATE_HAND, "命運之手", TYPE_METAMORPH, 0, 0, 0, 0, 0, "攻擊造成≥3傷害時，可翻轉1枚藍色命運TOKEN", 0, 0 },
    { CARD_SCHEHERAZADE_METAMORPH2_REWRITE_DESIRE, "改寫欲望", TYPE_METAMORPH, 0, 0, 0, 0, 0, "當對手購牌時，每枚命運TOKEN其失1點生命", 0, 0 },
    { CARD_SCHEHERAZADE_METAMORPH3_REORGANIZE_THOUGHT, "重組思想", TYPE_METAMORPH, 0, 0, 0, 0, 0, "對手回合前展示牌庫頂2張，每枚命運TOKEN失1點生命", 0, 0 },
    { CARD_SCHEHERAZADE_METAMORPH4_FAIRYTALE_WEAVER, "童話編織者", TYPE_METAMORPH, 0, 0, 0, 0, 0, "立即獲得一枚藍色命運TOKEN並放至對手供應牌庫，移除此牌", 0, 0 }
};

const Card* getCardData(CardID id) {
    if (id >= 0 && id < CARD_TOTAL_COUNT) {
        printf("%s\n", allCards[id].name);
        return &allCards[id];
    }
    else return NULL;
}

void shuffle(vector* v) {
    srand(time(NULL));
    if (!v || v->SIZE <= 1) return;

    for (uint32_t i = v->SIZE - 1; i > 0; --i) {
        uint32_t j = rand() % (i + 1);

        int32_t temp = v->array[i];
        v->array[i] = v->array[j];
        v->array[j] = temp;
    }
}