#ifndef BUILTIN_DATA_H
#define BUILTIN_DATA_H

// Application Infomation

#define TEXT_PROJECT_NAME "BestEnchSeq"
#define TEXT_PROGRAM_NAME_CN "最佳附魔顺序计算器"
#define TEXT_PROGRAM_NAME_EN "The Best Enchanting Sequence"
#define TEXT_PROGRAM_NAME_ABBR "BESQ"
#define VERSION_NAME "v4.2.0"
#define VERSION_ID 12307020 // NyymmXXx; N:number of century, yy:the two last number of recent year, mm:recent month, XX:capital number, x:sub number

#define TEXT_AUTHOR "Dinosaur_MC(Dinosaur-MC)"
#define LINK_HOME_PAGE "https://github.com/Dinosaur-MC/BestEnchSeq"
#define LINK_HELP_PAGE "https://www.bilibili.com/video/BV11T4y1D7c2"

// License & Announcement

#define LICENSE "GPL-v3.0"
#define STATEMENT "本程序为Github开源项目，完全永久免费使用！只推荐通过官方渠道获取本程序。\n任何对原程序进行收费售卖行为均为诈骗，谨防上当受骗！谨慎使用来路不明的非官方发布版本，谨防病毒入侵！"

// File Data

#define FILE_CONFIG "config.ini"
#define FILE_WEAPONTABLE "WeaponTable.csv"
#define FILE_ENCHTABLE "EnchantmentTable.csv"

#define FILEVERSION 5 // 文件版本

#define TEXT_SIGH_SYMBOL '@' // 特殊标识符
#define TEXT_NOTE_SYMBOL '#' // 注释符
#define FILEHEAD_CSV "\xef\xbb\xbf"

// Resources Links

#define LINK_UPDATE_DATA "https://raw.githubusercontent.com/Dinosaur-MC/BestEnchSeq/master/update_data.json"

// Other

#define ICON_ECB QIcon(":/icon/enchanted_book.png")
#define RESTART_CODE 19491001 // 重启代码

#endif // BUILTIN_DATA_H
