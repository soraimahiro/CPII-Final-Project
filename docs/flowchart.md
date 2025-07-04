graph TD
    A[遊戲開始] --> B[選擇角色 - 玩家1]
    B --> C[選擇角色 - 玩家2]
    
    C --> D[初始化遊戲狀態]
    D --> E[發牌階段]
    E --> F[設置初始生命值]
    F --> G[決定先手玩家]
    
    G --> H[回合開始]
    H --> I[抽牌階段]
    I --> J[主要階段]
    
    J --> K{玩家行動選擇}
    K -->|出牌| L[選擇要出的牌]
    K -->|使用能力| M[選擇角色能力]
    K -->|結束回合| N[回合結束]
    
    L --> O{牌的類型?}
    O -->|攻擊牌| P[攻擊牌處理]
    O -->|防禦牌| Q[防禦牌處理]
    O -->|特殊牌| R[執行特殊效果]
    O -->|移動牌| S[移動牌處理]
    
    %% 防禦牌詳細處理分支
    Q --> Q1{防禦牌類型?}
    Q1 -->|格擋牌| Q2[格擋牌處理]
    Q1 -->|護甲牌| Q3[護甲牌處理]
    Q1 -->|反制牌| Q4[反制牌處理]
    Q1 -->|治療牌| Q5[治療牌處理]
    Q1 -->|增益牌| Q6[增益牌處理]
    
    %% 格擋牌處理
    Q2 --> Q2A[檢查格擋條件]
    Q2A --> Q2B{是否可以格擋?}
    Q2B -->|否| Q2C[顯示無法格擋訊息]
    Q2B -->|是| Q2D[設置格擋狀態]
    Q2C --> AA[檢查遊戲狀態]
    Q2D --> Q2E{格擋類型?}
    Q2E -->|完全格擋| Q2F[設置完全格擋標記]
    Q2E -->|部分格擋| Q2G[設置格擋減免數值]
    Q2F --> Q2H[記錄格擋持續時間]
    Q2G --> Q2I[記錄格擋減免量]
    Q2H --> Q2J[更新角色防禦狀態]
    Q2I --> Q2J
    Q2J --> Q2K[顯示格擋效果訊息]
    Q2K --> AA
    
    %% 護甲牌處理
    Q3 --> Q3A[檢查護甲條件]
    Q3A --> Q3B{是否可以穿戴護甲?}
    Q3B -->|否| Q3C[顯示無法穿戴訊息]
    Q3B -->|是| Q3D[選擇護甲目標]
    Q3C --> AA
    Q3D --> Q3E{目標類型?}
    Q3E -->|自己| Q3F[為自己穿戴護甲]
    Q3E -->|隊友| Q3G[為隊友穿戴護甲]
    Q3F --> Q3H[計算護甲數值]
    Q3G --> Q3H
    Q3H --> Q3I[檢查護甲疊加規則]
    Q3I --> Q3J{護甲是否可疊加?}
    Q3J -->|可疊加| Q3K[增加護甲值]
    Q3J -->|不可疊加| Q3L[替換現有護甲]
    Q3K --> Q3M[更新護甲狀態]
    Q3L --> Q3M
    Q3M --> Q3N[顯示護甲效果訊息]
    Q3N --> AA
    
    %% 反制牌處理
    Q4 --> Q4A[檢查反制條件]
    Q4A --> Q4B{是否可以反制?}
    Q4B -->|否| Q4C[顯示無法反制訊息]
    Q4B -->|是| Q4D[設置反制狀態]
    Q4C --> AA
    Q4D --> Q4E{反制類型?}
    Q4E -->|攻擊反制| Q4F[設置攻擊反制標記]
    Q4E -->|法術反制| Q4G[設置法術反制標記]
    Q4E -->|全面反制| Q4H[設置全面反制標記]
    Q4F --> Q4I[設置反制傷害數值]
    Q4G --> Q4J[設置反制效果]
    Q4H --> Q4K[設置全面反制效果]
    Q4I --> Q4L[記錄反制持續時間]
    Q4J --> Q4L
    Q4K --> Q4L
    Q4L --> Q4M[更新角色反制狀態]
    Q4M --> Q4N[顯示反制效果訊息]
    Q4N --> AA
    
    %% 治療牌處理
    Q5 --> Q5A[檢查治療條件]
    Q5A --> Q5B{是否可以治療?}
    Q5B -->|否| Q5C[顯示無法治療訊息]
    Q5B -->|是| Q5D[選擇治療目標]
    Q5C --> AA
    Q5D --> Q5E{目標類型?}
    Q5E -->|自己| Q5F[為自己治療]
    Q5E -->|隊友| Q5G[為隊友治療]
    Q5F --> Q5H[計算治療數值]
    Q5G --> Q5H
    Q5H --> Q5I[檢查生命值上限]
    Q5I --> Q5J{治療後是否超過上限?}
    Q5J -->|是| Q5K[設定為最大生命值]
    Q5J -->|否| Q5L[增加計算的治療量]
    Q5K --> Q5M[更新目標生命值]
    Q5L --> Q5M
    Q5M --> Q5N[觸發治療相關效果]
    Q5N --> Q5O[顯示治療效果訊息]
    Q5O --> AA
    
    %% 增益牌處理
    Q6 --> Q6A[檢查增益條件]
    Q6A --> Q6B{是否可以施加增益?}
    Q6B -->|否| Q6C[顯示無法施加訊息]
    Q6B -->|是| Q6D[選擇增益目標]
    Q6C --> AA
    Q6D --> Q6E{增益類型?}
    Q6E -->|攻擊力增益| Q6F[增加攻擊力]
    Q6E -->|防禦力增益| Q6G[增加防禦力]
    Q6E -->|速度增益| Q6H[增加移動速度]
    Q6E -->|特殊增益| Q6I[施加特殊效果]
    Q6F --> Q6J[設置增益持續時間]
    Q6G --> Q6J
    Q6H --> Q6J
    Q6I --> Q6J
    Q6J --> Q6K[更新目標增益狀態]
    Q6K --> Q6L[顯示增益效果訊息]
    Q6L --> AA
    
    %% 移動牌處理分支
    S[移動牌處理] --> S1[檢查移動條件]
    S1 --> S1A[檢查角色是否被束縛]
    S1A --> S1B{角色是否被束縛?}
    S1B -->|是| S1C[檢查束縛類型]
    S1B -->|否| S1D[檢查移動點數]
    
    S1C --> S1E{束縛類型?}
    S1E -->|完全束縛| S1F[顯示完全無法移動訊息]
    S1E -->|部分束縛| S1G[計算束縛限制]
    S1F --> S4[顯示無法移動訊息]
    S1G --> S1H[減少可移動距離]
    S1H --> S1D
    
    S1D --> S1I[檢查移動點數是否足夠]
    S1I --> S1J{移動點數 >= 移動需求?}
    S1J -->|否| S1K[顯示移動點數不足訊息]
    S1J -->|是| S1L[檢查地形限制]
    S1K --> S4
    
    S1L --> S1M[檢查目標位置是否可達]
    S1M --> S2{是否可以移動?}
    S2 -->|是| S3[選擇移動目標位置]
    S2 -->|否| S4
    
    %% 選擇移動目標位置
    S3 --> S3A[顯示可移動範圍]
    S3A --> S3B[計算移動範圍]
    S3B --> S3C{移動類型?}
    S3C -->|直線移動| S3D[計算直線可達位置]
    S3C -->|自由移動| S3E[計算自由移動範圍]
    S3C -->|特殊移動| S3F[計算特殊移動位置]
    
    S3D --> S3G[標記可選位置]
    S3E --> S3G
    S3F --> S3G
    S3G --> S3H[等待玩家選擇]
    S3H --> S3I[驗證選擇的位置]
    S3I --> S3J{選擇位置是否有效?}
    S3J -->|否| S3K[顯示無效位置訊息]
    S3J -->|是| S3L[確認移動目標]
    S3K --> S3H
    S3L --> S5[執行移動]
    
    %% 執行移動
    S5 --> S5A[檢查移動路徑]
    S5A --> S5B{路徑是否暢通?}
    S5B -->|否| S5C[計算迂迴路徑]
    S5B -->|是| S5D[確認直接路徑]
    
    S5C --> S5E{是否有迂迴路徑?}
    S5E -->|否| S5F[顯示路徑阻塞訊息]
    S5E -->|是| S5G[使用迂迴路徑]
    S5F --> S4
    S5G --> S5H[計算移動消耗]
    S5D --> S5H
    
    S5H --> S5I[扣除移動點數]
    S5I --> S5J[執行移動動畫]
    S5J --> S6[更新角色位置]
    
    %% 更新角色位置
    S6 --> S6A[記錄舊位置]
    S6A --> S6B[設定新位置]
    S6B --> S6C[更新位置相關數據]
    S6C --> S6D[檢查位置衝突]
    S6D --> S6E{是否有位置衝突?}
    S6E -->|是| S6F[處理位置衝突]
    S6E -->|否| S6G[確認位置更新]
    
    S6F --> S6H{衝突類型?}
    S6H -->|友方單位| S6I[交換位置]
    S6H -->|敵方單位| S6J[推擠處理]
    S6H -->|障礙物| S6K[阻擋處理]
    
    S6I --> S6L[更新雙方位置]
    S6J --> S6M[計算推擠效果]
    S6K --> S6N[取消移動]
    S6L --> S6G
    S6M --> S6O[執行推擲動作]
    S6N --> S4
    S6O --> S6G
    
    S6G --> S7[觸發移動相關效果]
    
    %% 觸發移動相關效果
    S7 --> S7A[檢查離開位置效果]
    S7A --> S7B[觸發離開效果]
    S7B --> S7C[檢查進入位置效果]
    S7C --> S7D[觸發進入效果]
    S7D --> S7E[檢查移動觸發能力]
    S7E --> S7F{是否有移動觸發能力?}
    S7F -->|是| S7G[觸發移動能力]
    S7F -->|否| S7H[檢查地形效果]
    
    S7G --> S7I{能力類型?}
    S7I -->|攻擊能力| S7J[觸發移動攻擊]
    S7I -->|防禦能力| S7K[觸發移動防禦]
    S7I -->|特殊能力| S7L[觸發特殊效果]
    
    S7J --> S7M[執行移動攻擊效果]
    S7K --> S7N[執行移動防禦效果]
    S7L --> S7O[執行特殊移動效果]
    S7M --> S7H
    S7N --> S7H
    S7O --> S7H
    
    S7H --> S7P[檢查地形特殊效果]
    S7P --> S7Q{地形類型?}
    S7Q -->|普通地形| S7R[無特殊效果]
    S7Q -->|危險地形| S7S[觸發地形傷害]
    S7Q -->|增益地形| S7T[觸發地形增益]
    S7Q -->|特殊地形| S7U[觸發特殊地形效果]
    
    S7R --> S7V[完成移動處理]
    S7S --> S7W[計算地形傷害]
    S7T --> S7X[計算地形增益]
    S7U --> S7Y[處理特殊地形]
    
    S7W --> S7Z[應用地形效果]
    S7X --> S7Z
    S7Y --> S7Z
    S7Z --> S7V
    S7V --> AA[檢查遊戲狀態]
    
    %% 無法移動處理
    S4 --> S4A[記錄無法移動原因]
    S4A --> S4B[顯示錯誤訊息]
    S4B --> S4C[回到選擇階段]
    S4C --> AA
    
    %% 攻擊牌詳細處理分支
    P --> P1[檢查攻擊條件]
    P1 --> P2{是否在攻擊範圍內?}
    P2 -->|否| P3[顯示超出範圍訊息]
    P2 -->|是| P4[選擇攻擊目標]
    P3 --> AA
    
    P4 --> P5{目標類型?}
    P5 -->|單一目標| P6[確認單一目標]
    P5 -->|多重目標| P7[選擇多個目標]
    P5 -->|範圍攻擊| P8[確認範圍內所有目標]
    
    P6 --> P9[檢查目標防禦狀態]
    P7 --> P9
    P8 --> P9
    
    P9 --> P10{目標是否有防禦?}
    P10 -->|有防禦| P11[計算防禦減免]
    P10 -->|無防禦| P12[直接計算傷害]
    
    P11 --> P13[最終傷害 = 攻擊力 - 防禦力]
    P12 --> P14[最終傷害 = 攻擊力]
    
    P13 --> P15{最終傷害 > 0?}
    P14 --> P15
    P15 -->|是| P16[造成傷害]
    P15 -->|否| P17[攻擊無效果]
    
    P16 --> P18[更新目標生命值]
    P17 --> AA
    P18 --> P19[檢查擊殺效果]
    P19 --> P20{目標是否死亡?}
    P20 -->|是| P21[觸發擊殺效果]
    P20 -->|否| P22[觸發受傷效果]
    
    P21 --> P23[移除死亡角色]
    P22 --> AA
    P23 --> P24[觸發死亡相關能力]
    P24 --> AA
    
    R --> T[處理特殊效果]
    T --> AA
    
    M --> U{能力類型?}
    U -->|主動能力| V[執行主動能力]
    U -->|被動能力| W[觸發被動能力]
    
    V --> AA
    W --> AA
    
    AA --> X{遊戲是否結束?}
    X -->|是| Y[判定勝負]
    X -->|否| Z[切換到下一位玩家]
    
    Z --> H
    
    N --> BB[棄牌階段]
    BB --> CC[手牌數量檢查]
    CC --> DD{手牌是否超過上限?}
    DD -->|是| EE[選擇棄牌]
    DD -->|否| Z
    EE --> Z
    
    Y --> FF{勝利條件?}
    FF -->|生命值歸零| GG[對手生命值歸零獲勝]
    FF -->|特殊勝利條件| HH[角色特殊勝利條件達成]
    
    GG --> II[顯示遊戲結果]
    HH --> II
    II --> JJ[記錄遊戲數據]
    JJ --> KK{是否再次遊戲?}
    KK -->|是| B
    KK -->|否| LL[遊戲結束]
    
    %% 特殊遊戲狀態處理
    AA --> MM{特殊狀態檢查}
    MM -->|火柴女孩狀態| NN[處理火柴效果]
    MM -->|白雪公主毒藥| OO[處理毒藥效果]
    MM -->|其他角色特效| PP[處理角色特殊效果]
    MM -->|無特殊狀態| X
    
    NN --> X
    OO --> X
    PP --> X
    
    style A fill:#e1f5fe
    style LL fill:#ffebee
    style Y fill:#fff3e0
    style II fill:#e8f5e8
    style Q fill:#e8f5e8
    style Q2 fill:#f3e5f5
    style Q3 fill:#f3e5f5
    style Q4 fill:#f3e5f5
    style Q5 fill:#f3e5f5
    style Q6 fill:#f3e5f5