#pragma once
/*
UNIT_TYPE, 单位类型：基地，步兵，坦克，建筑，飞机
ORIGIN_MAX_HEALTH, 生命上限
ORIGIN_MAX_SPEED, 最大速度
ORIGIN_SHOT_RANGE, 射程
ORIGIN_DEFENSE, 防御
ORIGIN_ATTACK, 攻击
SKILL_CD_1, 技能1CD
SKILL_CD_2, 技能2CD
MAX_ACCOUNT, 最大数量
PEOPLE_COST, 人口消耗
MONEY_COST, 金钱消耗
TECH_COST, 科技消耗

（1）技能指令结算（除小鲜肉占领技能）
（2）单位死亡判定
（3) 判断主基地是否被摧毁，若被摧毁则进行相关胜负判定
（4）移动指令结算
（5）兵种获取指令结算
（6）小鲜肉占领建筑
（7）回合达到上限时进行相关胜负判定
*/
#include<vector>
extern int NOW_TURN;
extern int TEAMID;
using namespace std;
#include"teamstyle18-my-1.h"

/*
本文件所定义接口：

对象 MY,EN：
包含己方与敌人的所有资源与单位vector<Unit*>，不同的单位存在不同的vector中，也有整体的，
包含所有单位的vector:battle_unit,resourse_building,production_building,meat,还有英雄单位
的指针

对象 NE：
中立建筑的vector<Unit*>

对象 teamid

函数void initialize(void)：
初始化或者刷新所有信息

对象map：
其内将地图上的每一个节点封装为一个pos_info类的实例，实际上以一个pos_info pos[10000]的形式储存，
但在调用时最好使用pos_info&at(Position&)函数，因为其有安全防护机制。
	{
	对象pos_info：
	包含位于其上的敌我单位，建筑，以及经过计算的威胁值，敌我战力值
	}
*/
static bool teamid;

namespace infomation
{
	class player
	{
	public:
		void flush()
		{
			base = nullptr;

			superman = nullptr;
			nuke_tank = nullptr;
			eagle = nullptr;

			resourse.money = 0;
			resourse.remain_people = 0;
			resourse.tech = 0;

			battle_unit.clear();
			resourse_building.clear();
			production_building.clear();
			meat.clear();

			hacker.clear();
			battle_tank.clear();
			bolt_tank.clear();
			uav.clear();

			hack_lab.clear();
			bid_lab.clear();
			car_lab.clear();
			elec_lab.clear();
			radiation_lab.clear();
			uav_lab.clear();
			aircraft_lab.clear();
			build_lab.clear();
			finance_lab.clear();
			material_lab.clear();
			nano_lab.clear();

			teach_building.clear();
			bank.clear();
		}
		player()
		{
			Unit* base = nullptr;

			Unit* superman = nullptr;
			Unit* nuke_tank = nullptr;
			Unit* eagle = nullptr;

			resourse.money = 0;
			resourse.remain_people = 0;
			resourse.tech = 0;
		}
		struct
		{
			int money;
			int remain_people;
			int tech;
		}resourse;
		vector<Unit*> battle_unit;
		vector<Unit*> resourse_building;
		vector<Unit*> production_building;
		vector<Unit*> meat;
		Unit* base;

		Unit* superman;
		Unit* nuke_tank;
		Unit* eagle;

		vector<Unit*> hacker;
		vector<Unit*> battle_tank;
		vector<Unit*> bolt_tank;
		vector<Unit*> uav;

		vector<Unit*> hack_lab;
		vector<Unit*> bid_lab;
		vector<Unit*> car_lab;;
		vector<Unit*> elec_lab;
		vector<Unit*> radiation_lab;
		vector<Unit*> uav_lab;
		vector<Unit*> aircraft_lab;
		vector<Unit*> build_lab;
		vector<Unit*> finance_lab;
		vector<Unit*> material_lab;
		vector<Unit*> nano_lab;

		vector<Unit*> teach_building;
		vector<Unit*> bank;
	}MY,EN;

	class map_info
	{
	public:
		friend void initialize();
		map_info()
		{
			pos = new pos_info[10000];
			null_pos = new pos_info;
			for (int iterY = 0; iterY != 100; ++iterY)
			{
				for (int iterX = 0; iterX != 100; ++iterX)
				{
					pos->pos.x = iterX;
					pos->pos.y = iterY;
				}
			}
		}
		class pos_info
		{
		public:
			pos_info()
			{
				Unit* building = nullptr;
				int threat_val = 0;
				int MY_force_val = 0;
				int EN_force_val = 0;
			}
			Position pos;
			vector<Unit*> my_unit;
			vector<Unit*> en_unit;
			Unit* building;
			float threat_val;
			float MY_force_val;
			float EN_force_val;
		};
		pos_info&at(Position&pos)
		{
			if ((pos.x >= 0) && (pos.x < 100) && (pos.y >= 0) && (pos.y < 100))
			{
				return this->pos[pos.y * 100 + pos.x];
			}
			else
			{
				return*null_pos;
			}
		}
	private:
		void add_element(Unit*input)
		{
			pos_info temp = at(input->position);
			if ((input->unit_type = AIRCRAFT) || (input->unit_type = VEHICLE) || (input->unit_type = INFANTRY))
			{
				if (input->flag = teamid)
				{
					temp.my_unit.push_back(input);
				}
				else
				{
					temp.en_unit.push_back(input);
				}
			}
			else
			{
				temp.building = input;
			}
		}
		//构造单例模式
		map_info&operator=(map_info&) {}
		//构造单例模式

		//构造单例模式
		map_info(map_info&) {}
		pos_info*pos;
		//安全空位置
		pos_info*null_pos;
		//地图刷新函数
		void flush()
		{
			for (int iter = 0; iter != 10000; ++iter)
			{
				this->pos[iter].my_unit.clear();
				this->pos[iter].en_unit.clear();
				this->pos->threat_val = 0;
				this->pos->EN_force_val = 0;
				this->pos->MY_force_val = 0;
			}
		}
		//地图卷积函数
		void in_range(float range, void func(Position&, Unit*), Position&pos, Unit*unit)//对点进行在range范围内调用func函数进行卷积，用于生成势力图
		{
			if (range >= 1)
			{
				for (int y = -(int)range + 1; y != (int)range; ++y)
				{
					for (int x = -(int)range + 1; x != (int)range; ++x)
					{
						if (abs(x) + abs(y) <= (int)range)
						{
							func(Position(pos.x + x, pos.y + y), unit);
						}
					}
				}
			}
			else
			{
				return;
			}
		}
	}static map;

	vector<Unit*> NE;
	void initialize()
	{
		map.flush();
		MY.flush();
		EN.flush();
		NE.clear();

		if (teamid == 0)
		{
			MY.resourse.money = getResourse().money_1;
			MY.resourse.remain_people = getResourse().remain_people_1;
			MY.resourse.tech = getResourse().tech_1;
			EN.resourse.money = getResourse().money_2;
			EN.resourse.remain_people = getResourse().remain_people_2;
			EN.resourse.tech = getResourse().tech_2;
		}
		else
		{
			MY.resourse.money = getResourse().money_2;
			MY.resourse.remain_people = getResourse().remain_people_2;
			MY.resourse.tech = getResourse().tech_2;
			EN.resourse.money = getResourse().money_1;
			EN.resourse.remain_people = getResourse().remain_people_1;
			EN.resourse.tech = getResourse().tech_1;
			
		}

		Unit*start = getUnit();

		for (int iter = 0; iter != getUnitSize(); ++iter)
		{
			if (start[iter].flag == (int)teamid)
			{
				if (start[iter].type_name == (int)__BASE)
				{
					MY.base = start + iter;
				}
				else if (start[iter].type_name == (int)MEAT)
				{
					MY.meat.push_back(start + iter);
				}
				else if (start[iter].type_name == (int)HACKER)
				{
					MY.hacker.push_back(start + iter);
					MY.battle_unit.push_back(start + iter);
					
				}
				else if (start[iter].type_name == (int)SUPERMAN)
				{
					MY.superman = start + iter;
					MY.battle_unit.push_back(start + iter);
				}
				else if (start[iter].type_name == (int)BATTLE_TANK)
				{
					MY.battle_tank.push_back(start + iter);
					MY.battle_unit.push_back(start + iter);
				}
				else if (start[iter].type_name == (int)BOLT_TANK)
				{
					MY.bolt_tank.push_back(start + iter);
					MY.battle_unit.push_back(start + iter);
				}
				else if (start[iter].type_name == (int)NUKE_TANK)
				{
					MY.nuke_tank = start + iter;
					MY.battle_unit.push_back(start + iter);
				}
				else if (start[iter].type_name == (int)UAV)
				{
					MY.uav.push_back(start + iter);
					MY.battle_unit.push_back(start + iter);
				}
				else if (start[iter].type_name == (int)EAGLE)
				{
					MY.eagle = start + iter;
					MY.battle_unit.push_back(start + iter);
				}
				else if (start[iter].type_name == (int)HACK_LAB)
				{
					MY.hack_lab.push_back(start + iter);
					MY.production_building.push_back(start + iter);
				}
				else if (start[iter].type_name == (int)BID_LAB)
				{
					MY.bid_lab.push_back(start + iter);
					MY.production_building.push_back(start + iter);
				}
				else if (start[iter].type_name == (int)CAR_LAB)
				{
					MY.car_lab.push_back(start + iter);
					MY.production_building.push_back(start + iter);
				}
				else if (start[iter].type_name == (int)ELEC_LAB)
				{
					MY.elec_lab.push_back(start + iter);
					MY.production_building.push_back(start + iter);
				}
				else if (start[iter].type_name == (int)RADIATION_LAB)
				{
					MY.radiation_lab.push_back(start + iter);
					MY.production_building.push_back(start + iter);
				}
				else if (start[iter].type_name == (int)UAV_LAB)
				{
					MY.uav_lab.push_back(start + iter);
					MY.production_building.push_back(start + iter);
				}
				else if (start[iter].type_name == (int)AIRCRAFT_LAB)
				{
					MY.aircraft_lab.push_back(start + iter);
					MY.production_building.push_back(start + iter);
				}
				else if (start[iter].type_name == (int)BUILD_LAB)
				{
					MY.build_lab.push_back(start + iter);
					MY.production_building.push_back(start + iter);
				}
				else if (start[iter].type_name == (int)FINANCE_LAB)
				{
					MY.finance_lab.push_back(start + iter);
					MY.production_building.push_back(start + iter);
				}
				else if (start[iter].type_name == (int)MATERIAL_LAB)
				{
					MY.material_lab.push_back(start + iter);
					MY.production_building.push_back(start + iter);
				}
				else if (start[iter].type_name == (int)NANO_LAB)
				{
					MY.nano_lab.push_back(start + iter);
					MY.production_building.push_back(start + iter);
				}
				else if (start[iter].type_name == (int)TEACH_BUILDING)
				{
					MY.teach_building.push_back(start + iter);
					MY.resourse_building.push_back(start + iter);
				}
				else if (start[iter].type_name == (int)BANK)
				{
					MY.bank.push_back(start + iter);
					MY.resourse_building.push_back(start + iter);
				}
			}
			else if (start[iter].flag == (1-((int)teamid)))
			{
				if (start[iter].type_name == (int)__BASE)
				{
					EN.base = start + iter;
				}
				else if (start[iter].type_name == (int)MEAT)
				{
					EN.meat.push_back(start + iter);
				}
				else if (start[iter].type_name == (int)HACKER)
				{
					EN.hacker.push_back(start + iter);
					EN.battle_unit.push_back(start + iter);
				}
				else if (start[iter].type_name == (int)SUPERMAN)
				{
					EN.superman = start + iter;
					EN.battle_unit.push_back(start + iter);
				}
				else if (start[iter].type_name == (int)BATTLE_TANK)
				{
					EN.battle_tank.push_back(start + iter);
					EN.battle_unit.push_back(start + iter);
				}
				else if (start[iter].type_name == (int)BOLT_TANK)
				{
					EN.bolt_tank.push_back(start + iter);
					EN.battle_unit.push_back(start + iter);
				}
				else if (start[iter].type_name == (int)NUKE_TANK)
				{
					EN.nuke_tank = start + iter;
					EN.battle_unit.push_back(start + iter);
				}
				else if (start[iter].type_name == (int)UAV)
				{
					EN.uav.push_back(start + iter);
					EN.battle_unit.push_back(start + iter);
				}
				else if (start[iter].type_name == (int)EAGLE)
				{
					EN.eagle = start + iter;
					EN.battle_unit.push_back(start + iter);
				}
				else if (start[iter].type_name == (int)HACK_LAB)
				{
					EN.hack_lab.push_back(start + iter);
					EN.production_building.push_back(start + iter);
				}
				else if (start[iter].type_name == (int)BID_LAB)
				{
					EN.bid_lab.push_back(start + iter);
					EN.production_building.push_back(start + iter);
				}
				else if (start[iter].type_name == (int)CAR_LAB)
				{
					EN.car_lab.push_back(start + iter);
					EN.production_building.push_back(start + iter);
				}
				else if (start[iter].type_name == (int)ELEC_LAB)
				{
					EN.elec_lab.push_back(start + iter);
					EN.production_building.push_back(start + iter);
				}
				else if (start[iter].type_name == (int)RADIATION_LAB)
				{
					EN.radiation_lab.push_back(start + iter);
					EN.production_building.push_back(start + iter);
				}
				else if (start[iter].type_name == (int)UAV_LAB)
				{
					EN.uav_lab.push_back(start + iter);
					EN.production_building.push_back(start + iter);
				}
				else if (start[iter].type_name == (int)AIRCRAFT_LAB)
				{
					EN.aircraft_lab.push_back(start + iter);
					EN.production_building.push_back(start + iter);
				}
				else if (start[iter].type_name == (int)BUILD_LAB)
				{
					EN.build_lab.push_back(start + iter);
					EN.production_building.push_back(start + iter);
				}
				else if (start[iter].type_name == (int)FINANCE_LAB)
				{
					EN.finance_lab.push_back(start + iter);
					EN.production_building.push_back(start + iter);
				}
				else if (start[iter].type_name == (int)MATERIAL_LAB)
				{
					EN.material_lab.push_back(start + iter);
					EN.production_building.push_back(start + iter);
				}
				else if (start[iter].type_name == (int)NANO_LAB)
				{
					EN.nano_lab.push_back(start + iter);
					EN.production_building.push_back(start + iter);
				}
				else if (start[iter].type_name == (int)TEACH_BUILDING)
				{
					EN.teach_building.push_back(start + iter);
					EN.resourse_building.push_back(start + iter);
				}
				else if (start[iter].type_name == (int)BANK)
				{
					EN.bank.push_back(start + iter);
					EN.resourse_building.push_back(start + iter);
				}
			}
			else if (start[iter].flag == -1)
			{
				NE.push_back(start + iter);
			}
			map.add_element(start+iter);
		}
		void threat(Position&pos, Unit*unit);
		void EN_force(Position&pos, Unit*unit);
		void MY_force(Position&pos, Unit*unit);
		for (vector<Unit*>::iterator iter = EN.battle_unit.begin(); iter != EN.battle_unit.end();++iter)
		{
			map.in_range((*iter)->shot_range_now, threat, (*iter)->position, (*iter));
		}
		for (vector<Unit*>::iterator iter = EN.battle_unit.begin(); iter != EN.battle_unit.end(); ++iter)
		{
			map.in_range((*iter)->shot_range_now, EN_force, (*iter)->position, (*iter));
		}
		for (vector<Unit*>::iterator iter = MY.battle_unit.begin(); iter != MY.battle_unit.end(); ++iter)
		{
			if ((origin_attribute[(*iter)->type_name][SKILL_CD_1] + (*iter)->skill_last_release_time1) <= NOW_TURN)
			{
				map.in_range((*iter)->shot_range_now, MY_force, (*iter)->position, (*iter));
			}
		}
	}

};

//威胁值计算
void threat(Position&pos, Unit*unit)
{
	if ((origin_attribute[unit->type_name][SKILL_CD_1] + unit->skill_last_release_time1) <= NOW_TURN)
	{
		infomation::map.at(pos).threat_val += unit->attack_now;
	}
	return;
}
//敌方战力
void EN_force(Position&pos, Unit*unit)
{
	infomation::map.at(pos).EN_force_val += unit->attack_now*unit->health_now;
	return;
}
//我方战力
void MY_force(Position&pos, Unit*unit)
{
	infomation::map.at(pos).MY_force_val -= unit->attack_now*unit->health_now;
	return;
}