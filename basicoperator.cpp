#include "basicoperator.h"
#include "basicdata.h"

FileOperator::FileOperator(QObject *parent) :
    QObject{parent}
{

}

void FileOperator::saveConfig(Config *)
{

}

void FileOperator::saveWeaponTable(QVector<Weapon> *)
{

}

void FileOperator::saveEnchantmentTable(QVector<Ench> *)
{

}


void FileOperator::loadConfig(Config *)
{

}

void FileOperator::loadWeaponTable(QVector<Weapon> *)
{

}

void FileOperator::loadEnchantmentTable(QVector<Ench> *)
{

}


void FileOperator::saveExport(ListItemWidget_FlowStep *)
{

}



EnchFilter::EnchFilter()
{

}

ItemFilter::ItemFilter()
{

}


void deliverID()    // 分配魔咒和Weapon的数字ID
{
    // 魔咒的数字ID
    int retc = raw_enchantment_table.count();

    for(int i = 0; i < retc; i++)
    {
        EnchPlus tm;
        tm.id = i;
        tm.edition = raw_enchantment_table.at(i).edition;
        tm.poor_max_level = raw_enchantment_table.at(i).poor_max_level;
        tm.max_level = raw_enchantment_table.at(i).max_level;
        tm.multiplier[0] = raw_enchantment_table.at(i).multiplier[0];
        tm.multiplier[1] = raw_enchantment_table.at(i).multiplier[1];
        enchantment_table.append(tm);
    }

    for(int i = 0; i < retc; i++)
    {
        int repc = raw_enchantment_table.at(i).repulsion.count();
        for(int j = 0; j < repc; j++)
        {
            for(int k = 0; k < retc; k++)
            {
                if(raw_enchantment_table.at(i).repulsion.at(j) == raw_enchantment_table.at(k).name)
                {
                    enchantment_table[i].repulsion.append(k);
                    break;
                }
            }
        }
    }


    // Weapon的数字ID
    int wpc = raw_weapon_table.count();

    for(int i = 0; i < wpc; i++)
    {
        Weapon tm;
        tm.id = i;
        weapon_table.append(tm);
    }

    for(int i = 0; i < retc; i++)
    {
        for(int j = 0; j < wpc; j++)
        {
            if(raw_enchantment_table.at(i).suitable.at(j))
            {
                weapon_table[j].suitableE.append(enchantment_table.at(i));
            }
        }
    }
}


bool checkRepulsed(Ench e, Item it) // 检查是否存在魔咒冲突
{
    EnchPlus ep = enchantment_table.at(e.id);
    int ec = ep.repulsion.count();
    int iec = it.ench.count();

    for(int i = 0; i < ec; i++)
    {
        for(int j = 0; j < iec; j++)
        {
            if(ep.repulsion.at(i) == it.ench.at(j).id)
                return true;
        }
    }

    return false;
}

int* preforge(Item a, Item b, MCE mode) // 花费计算
{
    if(mode == MCE::All)    // 排除不支持的mode
        return NULL;

    int *cost = new int[4]; // 0:总花费；1:忽略维修；2:忽略维修和冲突；3:忽略维修、冲突和惩罚
    int a_ec = a.ench.count();  // 目标物品的魔咒数
    int b_ec = b.ench.count();  // 牺牲物品的魔咒数
    int multi = b.isECB;  // 选择乘数


    // 处理冲突的魔咒
    if(a_ec > 0)
    {
        for(int i = 0; i < b_ec; i++)
        {
            if(checkRepulsed(b.ench.at(i), a))
            {
                if(mode == MCE::Java)   // JE中每一项冲突花费+1
                    cost[0] += 1;
                b.ench.remove(i);   // 删除冲突的魔咒
                i--;
            }
        }
    }
    b.ench.squeeze();   // 释放多余内存
    b_ec = b.ench.count();  // 更新魔咒数

    cost[2] = cost[0];  // 暂存冲突花费


    // 计算魔咒等级花费
    for(int i = 0; i < b_ec; i++)
    {
        EnchPlus bep = enchantment_table.at(b.ench.at(i).id);   // 拉取魔咒表对应数据
        if(a_ec > 0)
        {
            int p = a.ench.indexOf(b.ench.at(i));   // 查找是否存在两个相同的可合并魔咒
            if(p < 0)   // 不存在
                cost[0] += bep.multiplier[multi] * b.ench.at(i).lvl;
            else    // 存在
            {
                if(mode == MCE::Java)
                    cost[0] += bep.multiplier[multi] * std::min(forge(a.ench.at(p).lvl, b.ench.at(i).lvl), bep.max_level);
                else
                    cost[0] += bep.multiplier[multi] * (std::min(forge(a.ench.at(p).lvl, b.ench.at(i).lvl), bep.max_level) - a.ench.at(p).lvl);
            }
        }
        else
            cost[0] += bep.multiplier[multi] * b.ench.at(i).lvl;
    }


    // 计算其它项
    cost[3] = cost[0] - cost[2];

    cost[0] += pow(2, a.penalty) + pow(2, b.penalty) -2; // 计算累积惩罚
    cost[2] = cost[0] - cost[2];

    cost[1] = cost[0];
    if(a.durability < 100 && b.durability > 0)  // 计算维修费用
        cost[0] += 2;


    // 计算完成，返回4个花费等级数，用完记得释放
    return cost;
}

int forge(int a, int b) // 等级合并（不计上限）
{
    if(a == b)
        a++;
    else
        a = std::max(a, b);

    return a;
}

Item forge(Item a, Item b)  // 物品合并
{
    Item tm = a;

    int a_ec = a.ench.count();  // 目标物品的魔咒数
    int b_ec = b.ench.count();  // 牺牲物品的魔咒数

    // 处理冲突的魔咒
    if(a_ec > 0)
    {
        for(int i = 0; i < b_ec; i++)
        {
            if(checkRepulsed(b.ench.at(i), a))
            {
                b.ench.remove(i);   // 删除冲突的魔咒
                i--;
            }
        }
    }
    b.ench.squeeze();   // 释放多余内存
    b_ec = b.ench.count();  // 更新魔咒数


    // 合并物品
    for(int i = 0; i < b_ec; i++)
    {
        EnchPlus bep = enchantment_table.at(b.ench.at(i).id);   // 拉取魔咒表对应数据
        if(a_ec > 0)
        {
            int p = a.ench.indexOf(b.ench.at(i));   // 查找是否存在两个相同的可合并魔咒
            if(p < 0)   // 不存在
                tm.ench.append(b.ench.at(i));
            else    // 存在
            {
                Ench e = b.ench.at(i);
                e.lvl = std::min(forge(a.ench.at(p).lvl, b.ench.at(i).lvl), bep.max_level); // 合并等级
                tm.ench.append(e);
            }
        }
        else
            tm.ench.append(b.ench.at(i));
    }

    // 返回合并后的物品
    return tm;
}


