#include "basicdata.h"

void loadInternalData(QVector<Option> *opt)
{
    // 程序设置项键名  （勿随意调换顺序）
    opt->append({"config_version", ValueType::Int});
    opt->append({"default_edition", ValueType::Int});
    opt->append({"default_itemconfig", ValueType::Int});
    opt->append({"default_algorithm", ValueType::Int});
    opt->append({"export_path", ValueType::String});
    opt->append({"constant", ValueType::Bool});
    opt->append({"auto_check_update", ValueType::Bool});
    opt->append({"enable_custom_weapon", ValueType::Bool});
    opt->append({"enable_custom_enchantment", ValueType::Bool});
    opt->append({"enable_reszie_window", ValueType::Bool});
    opt->append({"deverloper_mode", ValueType::Bool});
}

void loadInternalData(Config *config)
{
    // 程序默认设置值
    config->config_version = VERSION_ID;
    config->default_edition = MCE::Java;
    config->default_itemconfig = ICM::AllLevelEBook;
    config->default_algorithm = ALGM::GlobalAverage;
    config->export_path = "";
    config->constant = false;
    config->auto_check_update = true;
    config->enable_custom_we = false;
    config->enable_custom_en = false;
    config->enable_reszie_window = false;
    config->deverloper_mode = false;
}

void loadInternalData(QVector<raw_Weapon> *rwps)
{
    // 内置Weapon数据表
    rwps->append({"剑\nSword", QIcon("res/netherite_sword->png")});
    rwps->append({"镐\nPickaxe", QIcon("res/netherite_pickaxe->png")});
    rwps->append({"斧\nAxe",QIcon("res/netherite_axe->png")});
    rwps->append({"铲\nShovel", QIcon("res/netherite_shovel->png")});
    rwps->append({"锄\nHoe", QIcon("res/netherite_hoe->png")});
    rwps->append({"头盔\nHelmet", QIcon("res/netherite_helmet->png")});
    rwps->append({"胸甲\nChestplate", QIcon("res/netherite_chestplate->png")});
    rwps->append({"护腿\nLeggings", QIcon("res/netherite_leggings->png")});
    rwps->append({"靴\nBoots", QIcon("res/netherite_boots->png")});
    rwps->append({"弓\nBow", QIcon("res/bow->png")});
    rwps->append({"弩\nCrossbow", QIcon("res/crossbow_standby->png")});
    rwps->append({"三叉戟\nTrident", QIcon("res/trident->png")});
    rwps->append({"钓鱼竿\nFishing Rod", QIcon("res/fishing_rod->png")});
}

void loadInternalData(QVector<raw_EnchPlus> *reps)
{
    // 内置魔咒数据表
    reps->append({"水下速掘-aqua_affinity", MCE::All, 1, 1, {4, 2}, {""}, {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0}});
    reps->append({"节肢杀手-bane_of_arthropods", MCE::All, 5, 4, {2, 1}, {"锋利-sharpness", "亡灵杀手-smite"}, {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}});
    reps->append({"爆炸保护-blast_protection", MCE::All, 4, 4, {4, 2}, {"火焰保护-fire_protection", "弹射物保护-projectile_protection", "保护-protection"}, {0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0}});
    reps->append({"引雷-channeling", MCE::All, 1, 1, {8, 4}, {"激流-riptide"}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}});
    reps->append({"绑定诅咒-binding_curse", MCE::All, 1, 0, {8, 4}, {""}, {0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0}});
    reps->append({"消失诅咒-vanishing_curse", MCE::All, 1, 0, {8, 4}, {""}, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}});
    reps->append({"深海探索者-depth_strider", MCE::All, 3, 3, {4, 2}, {"冰霜行者-frost_walker"}, {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0}});
    reps->append({"效率-efficiency", MCE::All, 5, 4, {1, 1}, {""}, {0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}});
    reps->append({"摔落保护-feather_falling", MCE::All, 4, 4, {2, 1}, {""}, {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0}});
    reps->append({"火焰附加-fire_aspect", MCE::All, 2, 2, {4, 2}, {""}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}});
    reps->append({"火焰保护-fire_protection", MCE::All, 4, 4, {2, 1}, {"爆炸保护-blast_protection", "弹射物保护-projectile_protection", "保护-protection"}, {0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0}});
    reps->append({"火矢-flame", MCE::All, 1, 1, {4, 2}, {""}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0}});
    reps->append({"时运-fortune", MCE::All, 3, 3, {4, 2}, {"精准采集-silk_touch"}, {0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}});
    reps->append({"冰霜行者-frost_walker", MCE::All, 2, 0, {4, 2}, {"深海探索者-depth_strider"}, {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0}});
    reps->append({"穿刺-impaling", MCE::Java, 5, 5, {4, 2}, {""}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}});
    reps->append({"穿刺-impaling", MCE::Bedrock, 5, 5, {2, 1}, {""}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}});
    reps->append({"无限-infinity", MCE::All, 1, 1, {8, 4}, {"经验修补-mending"}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0}});
    reps->append({"击退-knockback", MCE::All, 2, 2, {2, 1}, {""}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}});
    reps->append({"抢夺-looting", MCE::All, 3, 3, {4, 2}, {""}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}});
    reps->append({"忠诚-loyalty", MCE::All, 3, 3, {1, 1}, {"激流-riptide"}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}});
    reps->append({"海之眷顾-luck_of_the_sea", MCE::All, 3, 3, {4, 2}, {""}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}});
    reps->append({"饵钓-lure", MCE::All, 3, 3, {4, 2}, {""}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}});
    reps->append({"经验修补-mending", MCE::All, 1, 0, {4, 2}, {"无限-infinity"}, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}});
    reps->append({"多重射击-multishot", MCE::All, 1, 1, {4, 2}, {"穿透-piercing"}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0}});
    reps->append({"穿透-piercing", MCE::All, 4, 4, {1, 1}, {"多重射击-multishot"}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0}});
    reps->append({"力量-power", MCE::All, 5, 4, {1, 1}, {""}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0}});
    reps->append({"弹射物保护-projectile_protection", MCE::All, 4, 4, {2, 1}, {"爆炸保护-blast_protection", "火焰保护-fire_protection", "保护-protection"}, {0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0}});
    reps->append({"保护-protection", MCE::All, 4, 4, {1, 1}, {"爆炸保护-blast_protection", "火焰保护-fire_protection", "弹射物保护-projectile_protection"}, {0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0}});
    reps->append({"冲击-punch", MCE::All, 2, 2, {4, 2}, {""}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0}});
    reps->append({"快速装填-quick_charge", MCE::All, 3, 3, {2, 1}, {""}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0}});
    reps->append({"水下呼吸-respiration", MCE::All, 3, 3, {4, 2}, {""}, {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0}});
    reps->append({"激流-riptide", MCE::All, 3, 3, {4, 2}, {"引雷-channeling", "忠诚-loyalty"}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}});
    reps->append({"锋利-sharpness", MCE::All, 5, 4, {1, 1}, {"节肢杀手-bane_of_arthropods", "亡灵杀手-smite"}, {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}});
    reps->append({"精准采集-silk_touch", MCE::All, 1, 1, {8, 4}, {"时运-fortune"}, {0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}});
    reps->append({"亡灵杀手-smite", MCE::All, 5, 4, {2, 1}, {"节肢杀手-bane_of_arthropods", "锋利-sharpness"}, {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}});
    reps->append({"灵魂疾行-soul_speed", MCE::All, 3, 0, {8, 4}, {""}, {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0}});
    reps->append({"横扫之刃-sweeping", MCE::Java, 3, 3, {4, 2}, {""}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}});
    reps->append({"荆棘-thorns", MCE::All, 3, 3, {8, 4}, {""}, {0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0}});
    reps->append({"耐久-unbreaking", MCE::All, 3, 3, {2, 1}, {""}, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}});
    reps->append({"迅捷潜行_swift_sneak", MCE::All, 3, 0, {8, 4}, {""}, {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0}});
}

