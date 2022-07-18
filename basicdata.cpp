#include "basicdata.h"

void loadInternalData()
{
    // 程序设置项键名
    config_option.append({"default_edition", ValueType::Int});
    config_option.append({"default_algorithm", ValueType::Int});
    config_option.append({"export_path", ValueType::Int});
    config_option.append({"constant", ValueType::String});
    config_option.append({"enable_custom_weapon", ValueType::Int});
    config_option.append({"enable_custom_enchantment", ValueType::Int});
    config_option.append({"auto_check_update", ValueType::Int});
    config_option.append({"enable_reszie_window", ValueType::Int});

    // 程序默认设置值
    current_config.config_version = VERSION_ID;
    current_config.default_edition = MCE::Java;
    current_config.default_itemconfig = ICM::AllLevelEBook;
    current_config.default_algorithm = ALGM::GlobalAverage;
    current_config.export_path = "";
    current_config.constant = false;
    current_config.enableCustomEn = false;
    current_config.enableCustomWe = false;
    current_config.autoCheckUpdate = true;
    current_config.deverloperMode = false;

    // 内置Weapon数据表
    raw_weapon_table.append({"剑\nSword", "res/netherite_sword.png"});
    raw_weapon_table.append({"镐\nPickaxe", "res/netherite_pickaxe.png"});
    raw_weapon_table.append({"斧\nAxe","res/netherite_axe.png"});
    raw_weapon_table.append({"铲\nShovel", "res/netherite_shovel.png"});
    raw_weapon_table.append({"锄\nHoe", "res/netherite_hoe.png"});
    raw_weapon_table.append({"头盔\nHelmet", "res/netherite_helmet.png"});
    raw_weapon_table.append({"胸甲\nChestplate", "res/netherite_chestplate.png"});
    raw_weapon_table.append({"护腿\nLeggings", "res/netherite_leggings.png"});
    raw_weapon_table.append({"靴\nBoots", "res/netherite_boots.png"});
    raw_weapon_table.append({"弓\nBow", "res/bow.png"});
    raw_weapon_table.append({"弩\nCrossbow", "res/crossbow_standby.png"});
    raw_weapon_table.append({"三叉戟\nTrident", "res/trident.png"});
    raw_weapon_table.append({"钓鱼竿\nFishing Rod", "res/fishing_rod.png"});

    // 内置魔咒数据表
    raw_enchantment_table.append({"水下速掘-aqua_affinity", MCE::All, 1, 1, {4, 2}, {""}, {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0}});
    raw_enchantment_table.append({"节肢杀手-bane_of_arthropods", MCE::All, 4, 5, {2, 1}, {"锋利-sharpness", "亡灵杀手-smite"}, {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}});
    raw_enchantment_table.append({"爆炸保护-blast_protection", MCE::All, 4, 4, {4, 2}, {"火焰保护-fire_protection", "弹射物保护-projectile_protection", "保护-protection"}, {0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0}});
    raw_enchantment_table.append({"引雷-channeling", MCE::All, 1, 1, {8, 4}, {"激流-riptide"}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}});
    raw_enchantment_table.append({"绑定诅咒-binding_curse", MCE::All, 0, 1, {8, 4}, {""}, {0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0}});
    raw_enchantment_table.append({"消失诅咒-vanishing_curse", MCE::All, 0, 1, {8, 4}, {""}, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}});
    raw_enchantment_table.append({"深海探索者-depth_strider", MCE::All, 3, 3, {4, 2}, {"冰霜行者-frost_walker"}, {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0}});
    raw_enchantment_table.append({"效率-efficiency", MCE::All, 4, 5, {1, 1}, {""}, {0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}});
    raw_enchantment_table.append({"摔落保护-feather_falling", MCE::All, 4, 4, {2, 1}, {""}, {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0}});
    raw_enchantment_table.append({"火焰附加-fire_aspect", MCE::All, 2, 2, {4, 2}, {""}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}});
    raw_enchantment_table.append({"火焰保护-fire_protection", MCE::All, 4, 4, {2, 1}, {"爆炸保护-blast_protection", "弹射物保护-projectile_protection", "保护-protection"}, {0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0}});
    raw_enchantment_table.append({"火矢-flame", MCE::All, 1, 1, {4, 2}, {""}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0}});
    raw_enchantment_table.append({"时运-fortune", MCE::All, 3, 3, {4, 2}, {"精准采集-silk_touch"}, {0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}});
    raw_enchantment_table.append({"冰霜行者-frost_walker", MCE::All, 0, 2, {4, 2}, {"深海探索者-depth_strider"}, {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0}});
    raw_enchantment_table.append({"穿刺-impaling", MCE::Java, 5, 5, {4, 2}, {""}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}});
    raw_enchantment_table.append({"穿刺-impaling", MCE::Bedrock, 5, 5, {2, 1}, {""}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}});
    raw_enchantment_table.append({"无限-infinity", MCE::All, 1, 1, {8, 4}, {"经验修补-mending"}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0}});
    raw_enchantment_table.append({"击退-knockback", MCE::All, 2, 2, {2, 1}, {""}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}});
    raw_enchantment_table.append({"抢夺-looting", MCE::All, 3, 3, {4, 2}, {""}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}});
    raw_enchantment_table.append({"忠诚-loyalty", MCE::All, 3, 3, {1, 1}, {"激流-riptide"}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}});
    raw_enchantment_table.append({"海之眷顾-luck_of_the_sea", MCE::All, 3, 3, {4, 2}, {""}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}});
    raw_enchantment_table.append({"饵钓-lure", MCE::All, 3, 3, {4, 2}, {""}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}});
    raw_enchantment_table.append({"经验修补-mending", MCE::All, 0, 1, {4, 2}, {"无限-infinity"}, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}});
    raw_enchantment_table.append({"多重射击-multishot", MCE::All, 1, 1, {4, 2}, {"穿透-piercing"}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0}});
    raw_enchantment_table.append({"穿透-piercing", MCE::All, 4, 4, {1, 1}, {"多重射击-multishot"}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0}});
    raw_enchantment_table.append({"力量-power", MCE::All, 4, 5, {1, 1}, {""}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0}});
    raw_enchantment_table.append({"弹射物保护-projectile_protection", MCE::All, 4, 4, {2, 1}, {"爆炸保护-blast_protection", "火焰保护-fire_protection", "保护-protection"}, {0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0}});
    raw_enchantment_table.append({"保护-protection", MCE::All, 4, 4, {1, 1}, {"爆炸保护-blast_protection", "火焰保护-fire_protection", "弹射物保护-projectile_protection"}, {0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0}});
    raw_enchantment_table.append({"冲击-punch", MCE::All, 2, 2, {4, 2}, {""}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0}});
    raw_enchantment_table.append({"快速装填-quick_charge", MCE::All, 3, 3, {2, 1}, {""}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0}});
    raw_enchantment_table.append({"水下呼吸-respiration", MCE::All, 3, 3, {4, 2}, {""}, {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0}});
    raw_enchantment_table.append({"激流-riptide", MCE::All, 3, 3, {4, 2}, {"引雷-channeling", "忠诚-loyalty"}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}});
    raw_enchantment_table.append({"锋利-sharpness", MCE::All, 4, 5, {1, 1}, {"节肢杀手-bane_of_arthropods", "亡灵杀手-smite"}, {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}});
    raw_enchantment_table.append({"精准采集-silk_touch", MCE::All, 1, 1, {8, 4}, {"时运-fortune"}, {0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}});
    raw_enchantment_table.append({"亡灵杀手-smite", MCE::All, 4, 5, {2, 1}, {"节肢杀手-bane_of_arthropods", "锋利-sharpness"}, {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}});
    raw_enchantment_table.append({"灵魂疾行-soul_speed", MCE::All, 0, 3, {8, 4}, {""}, {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0}});
    raw_enchantment_table.append({"横扫之刃-sweeping", MCE::Java, 3, 3, {4, 2}, {""}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}});
    raw_enchantment_table.append({"荆棘-thorns", MCE::All, 3, 3, {8, 4}, {""}, {0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0}});
    raw_enchantment_table.append({"耐久-unbreaking", MCE::All, 3, 3, {2, 1}, {""}, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}});
    raw_enchantment_table.append({"迅捷潜行_swift_sneak", MCE::All, 0, 3, {8, 4}, {""}, {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0}});
}
