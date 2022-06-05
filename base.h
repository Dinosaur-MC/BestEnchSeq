#ifndef BASE_H
#define BASE_H

#define PROGRAM_NAME_CN "最佳附魔顺序计算器"
#define PROGRAM_NAME_EN "The Best Enchanting Sequence"
#define VERSION "3.0_Beta"
#define VERSION_ID 12204120 //NyymmXXx; N:number of year, yy:the two last number of recent year, mm:recent month, XX:version number, x:small number

#define AUTHOR "Dinosaur_MC(Dinosaur-MC)"
#define WEBSITE "https://github.com/Dinosaur-MC/BestEnchSeq"
#define HELP_PAGE "https://www.bilibili.com/video/BV11T4y1D7c2"
#define STATEMENT "本程序为Github开源项目，完全永久免费使用！只推荐通过官方渠道获取本程序。\n任何对原程序进行收费售卖行为均为诈骗，谨防上当受骗！谨慎使用来路不明的非官方发布版本，谨防病毒入侵！"
#define STATEMENT_EN "This program is Github open source project, completely free to use forever! Access to this program is only recommended through official channels. \nAny selling of the original program for a fee is fraud, beware of being cheated! Be careful to use unofficial releases from unknown sources to guard against virus attacks!"

// 引入基础库
#include "base/Ench.h"
#include "base/EnchSet.h"
#include "base/FlowStep.h"
#include "base/Item.h"
#include "base/ItemPool.h"
#include "base/Summary.h"
#include "base/WeaponTable.h"
#include "base/EnchTable.h"

#endif // BASE_H
