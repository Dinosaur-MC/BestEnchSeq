#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>

#define CONFIG_NUM 8
enum MCE {All=1, Java, Bedrock};  //枚举变量，MC编译版本
enum ICM {AllLevelEBook=1, BasicEBook, AdvanceMode};    //枚举变量，物品配置模式
enum ALGM {GlobalAverage=1, DifficultyFirst, Greedy, Enumeration, SimpleEnumeration};   //枚举变量，算法名

struct Config {
  int config_version;   //配置文件版本，用于检查软件是否已更新
  MCE default_edition;  //默认MC编译版本
  ICM default_itemconfig;   //默认物品配置模式
  ALGM default_algorithm;    //默认算法
  QString export_path;  //默认流程导出路径
  bool constant;    //是否自动导出

  bool enableCustomEn;  //是否启用自定义魔咒
  bool enableCustomWe;  //是否启用自定义武器
  bool autoCheckUpdate; //是否自动检查更新

  bool deverloperMode;
};

#endif // CONFIG_H
