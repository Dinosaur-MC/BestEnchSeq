#ifndef BASICDATA_H
#define BASICDATA_H


#include <QDebug>


//## 程序关键字
#define VERSION_ID 12204120 //NyymmXXx; N:number of century, yy:the two last number of recent year, mm:recent month, XX:version number, x:small number
#define FILEVERSION 4   //Used to control the file version

#define FILE_CONFIG "config.ini"
#define FILE_WEAPONTABLE "WeaponTable.csv"
#define FILE_ENCHTABLE "EnchantmentTable.csv"
#define DIR_EXPORT "./exports/"

#define CONFIG_NUM 11 //Count of config items
#define MINIMUM_EP_ITEM_C 7 //Count of config items

#define FILEHEAD "\xef\xbb\xbf\x0d\x0a"
#define TEXT_NOTE_SYMBOL '#'
#define ID_ECB "附魔书\nEnchanted Book"
#define ICON_ECB QIcon(":/icon/res/enchanted_book.png")

#define UPDATE_JSON "https://raw.githubusercontent.com/Dinosaur-MC/BestEnchSeq/master/update.json"

#define RESTART_CODE 19491001   //The restart code, a program restarting signal


//## 程序基本信息
#define PROGRAM_NAME_CN "最佳附魔顺序计算器"
#define PROGRAM_NAME_EN "The Best Enchanting Sequence"
#define VERSION "4.0_Beta"


//## 其他信息
#define AUTHOR "Dinosaur_MC(Dinosaur-MC)"
#define WEBSITE "https://github.com/Dinosaur-MC/BestEnchSeq"
#define HELP_PAGE "https://www.bilibili.com/video/BV11T4y1D7c2"
#define STATEMENT "本程序为Github开源项目，完全永久免费使用！只推荐通过官方渠道获取本程序。\n任何对原程序进行收费售卖行为均为诈骗，谨防上当受骗！谨慎使用来路不明的非官方发布版本，谨防病毒入侵！"
#define STATEMENT_EN "This program is Github open source project, completely free to use forever! Access to this program is only recommended through official channels. \nAny selling of the original program for a fee is fraud, beware of being cheated! Be careful to use unofficial releases from unknown sources to guard against virus attacks!"


//## 内置加载数据
#include "basicvariable.h"



void loadInternalData();

#endif // BASICDATA_H
