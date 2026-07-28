# GAS_Project 资产目录结构

```
Content/
├── GAS/                              # 🔴 GAS 核心资产
│   ├── Abilities/                    # GameplayAbility 蓝图
│   │   ├── GA_Attack_Light           # 轻攻击
│   │   ├── GA_Attack_Heavy           # 重攻击
│   │   ├── GA_Dash                   # 闪避
│   │   ├── GA_JumpAttack             # 跳跃攻击
│   │   └── GA_Ultimate               # 大招
│   │
│   ├── Effects/                      # GameplayEffect 蓝图
│   │   ├── Init/                     # 初始化类 (Infinite, 设置基础属性)
│   │   │   └── GE_InitStats          # 初始化 Health/Mana/Stamina -> Max
│   │   ├── Damage/                   # 伤害类 (Instant, 造成伤害)
│   │   │   ├── GE_Damage_Base        # 基础伤害模板
│   │   │   └── GE_Damage_LightAttack # 轻攻击伤害实例
│   │   ├── Heal/                     # 治疗类
│   │   │   └── GE_Heal_Small
│   │   ├── Buff/                     # Buff 类 (HasDuration)
│   │   │   ├── GE_Buff_AttackUp
│   │   │   └── GE_Buff_SpeedUp
│   │   └── Debuff/                   # Debuff 类
│   │       ├── GE_Debuff_Stun
│   │       └── GE_Debuff_Slow
│   │
│   ├── Cues/                         # GameplayCue 蓝图 (VFX/SFX/动画通知)
│   │   ├── GC_MeleeImpact            # 近战命中特效
│   │   ├── GC_Heal                   # 治疗特效
│   │   └── GC_Death                  # 死亡特效
│   │
│   └── Tasks/                        # AbilityTask 蓝图 (如有自定义)
│       └── AT_WaitInputPressed       # 等待输入按下
│
├── Characters/                       # 🟡 角色资产
│   ├── Hero/
│   │   ├── BP_HeroCharacter          # 玩家角色蓝图 (继承 AGAS_ProjectCharacter)
│   │   ├── ABP_Hero                  # 动画蓝图
│   │   └── Materials/
│   └── Enemies/
│       ├── BP_Enemy_Base             # 敌人基类
│       ├── BP_Enemy_Melee            # 近战敌人
│       ├── BP_Enemy_Ranged           # 远程敌人
│       ├── AI/
│       │   ├── BT_Enemy              # BehaviorTree
│       │   └── BB_Enemy              # Blackboard
│       └── ABP_Enemy                 # 敌人动画蓝图
│
├── Input/                            # 🟢 输入资产
│   ├── Actions/                      # InputAction
│   │   ├── IA_Attack
│   │   ├── IA_HeavyAttack
│   │   ├── IA_Dash
│   │   └── IA_Ultimate
│   └── IMC_GAS                       # InputMappingContext (GAS 专用)
│
├── UI/                               # 🔵 UI 资产
│   ├── WBP_HUD                       # 主 HUD
│   ├── WBP_HealthBar                 # 血条
│   ├── WBP_ManaBar                   # 蓝条
│   ├── WBP_StaminaBar                # 耐力条
│   ├── WBP_SkillIcon                 # 技能图标
│   └── WBP_SkillBar                  # 技能栏
│
├── Data/                             # 🟣 数据资产
│   ├── DT_HeroStats                  # DataTable: 属性配置表
│   ├── DT_EnemyStats                 # DataTable: 敌人属性表
│   ├── DA_AbilityConfig              # DataAsset: 技能参数配置
│   └── DA_HeroConfig                 # DataAsset: 英雄全局配置
│
├── Animation/                        # 🟠 动画资产
│   ├── Montages/
│   │   ├── AM_Attack_Light           # 轻攻击蒙太奇
│   │   ├── AM_Attack_Heavy           # 重攻击蒙太奇
│   │   ├── AM_Dash                   # 闪避蒙太奇
│   │   └── AM_HitReaction            # 受击蒙太奇
│   └── Notifies/
│       ├── AN_DoAttackTrace          # AnimNotify: 攻击检测窗口
│       └── AN_EndAbility             # AnimNotify: 结束技能
│
├── Maps/                             # 🗺️ 关卡
│   ├── Lvl_TestArena                 # 测试竞技场
│   └── Lvl_Main                      # 主关卡
│
└── ThirdPersonCPP/                   # 📦 保持模板原有内容不动
    ├── Blueprints/
    ├── Maps/
    └── ...
```

## GameplayTags 路径规划

在 `Config/DefaultGameplayTags.ini` 中预设：

```
Ability.Attack.Light      — 轻攻击输入
Ability.Attack.Heavy      — 重攻击输入
Ability.Dash              — 闪避输入
Ability.Ultimate          — 大招输入
Ability.Jump              — 跳跃攻击输入

State.Dead                — 死亡
State.Stunned             — 眩晕
State.Invulnerable        — 无敌
State.Sprinting           — 冲刺中
State.Blocking            — 格挡中

Cooldown.Attack.Light     — 轻攻击冷却
Cooldown.Attack.Heavy     — 重攻击冷却
Cooldown.Dash             — 闪避冷却

Event.Damage.Received     — 受到伤害事件
Event.Damage.Dealt        — 造成伤害事件
Event.Heal.Received       — 受到治疗事件
Event.Death               — 死亡事件
```

## 命名规范

| 前缀 | 类型 | 示例 |
|------|------|------|
| `GA_` | GameplayAbility | `GA_Attack_Light` |
| `GE_` | GameplayEffect | `GE_Damage_Base` |
| `GC_` | GameplayCue | `GC_MeleeImpact` |
| `BP_` | Blueprint Actor | `BP_HeroCharacter` |
| `WBP_` | Widget Blueprint | `WBP_HUD` |
| `AM_` | AnimMontage | `AM_Attack_Light` |
| `AN_` | AnimNotify | `AN_EndAbility` |
| `DT_` | DataTable | `DT_HeroStats` |
| `DA_` | DataAsset | `DA_AbilityConfig` |
| `ABP_` | AnimBlueprint | `ABP_Hero` |
| `IA_` | InputAction | `IA_Attack` |
| `IMC_` | InputMappingContext | `IMC_GAS` |
| `Lvl_` | Level/Map | `Lvl_TestArena` |
