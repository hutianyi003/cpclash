#pragma once
#include<string>
#include<iostream>
#include<vector>
#include <queue>
#include "communicate.h"
using namespace std;
using std::string;
using std::vector;
//using std::mutex;
//若干枚举型变量

enum UnitType //type_name
{
	BASE,	     //主基地
	INFANTRY,	 //步兵
	VEHICLE,	 //坦克
	AIRCRAFT,	 //飞机
	BUILDING   	 //建筑
};

enum BuffType
{
	HEALTH,	     //最大生命值BUFF
	ATTACK,		 //攻击BUFF
	SPEED,		 //速度BUFF，基地该项对应科技BUFF
	DEFENSE,	 //防御BUFF
	SHOT_RANGE, 	 //射程UFF
	PRODUCE		//生产BUFF，基地该项对应经济BUFF
};

enum attack_type
{
	FORT, //城甲
	UNARMORED, //无甲
	LIGHT, //轻甲
	MEDIUM, //中甲
	HEAVY //重甲
};

enum defense_type
{
	MACHINEGUN, //机枪
	ELEC,  //电磁
	ARTILLERY, //火炮
	PENETRATING, //穿甲
	EXPLOSION, //高爆
};

enum TypeName
{
	__BASE, 
	
	MEAT, HACKER, SUPERMAN,
	
	BATTLE_TANK, BOLT_TANK, NUKE_TANK, 
	
	UAV, EAGLE, 
	
	HACK_LAB, BID_LAB, CAR_LAB, ELEC_LAB, RADIATION_LAB, 
	UAV_LAB, AIRCRAFT_LAB, BUILD_LAB, FINANCE_LAB, MATERIAL_LAB,
	NANO_LAB, TEACH_BUILDING, BANK,
	Type_num
};


enum attribute
{
	UNIT_TYPE,
	ORIGIN_MAX_HEALTH,
	ORIGIN_MAX_SPEED,
	ORIGIN_SHOT_RANGE,
	ORIGIN_DEFENSE,
	DEFENSE_TYPE,
	ORIGIN_ATTACK,
	ATTACK_TYPE,
	SKILL_CD_1,
	SKILL_CD_2,
	MAX_ACCOUNT,
	PEOPLE_COST,
	MONEY_COST,
	TECH_COST,
	attribute_num
};

//伤害百分比[攻击类型][护甲类型]
const float damage_percentage[5][5]={
	0.25,2.00,1.00,0.75,0.50,
	0.75,0.00,1.50,1.00,0.50,
	1.00,0.50,1.00,1.25,0.75,
	1.00,0.25,0.75,0.75,1.50,
	1.50,0.75,0.50,0.50,1.25
};

//用来给unit初始化
const int origin_attribute[Type_num][attribute_num] =
{

{BASE,      7500   ,  0,   12,  50,   0,  12,   2,    1,   -1,   -1,   0,   0,     0	    },
{INFANTRY,	100    ,  3,   1,   10,   1,  0,    -1,   -1,  -1,   -1,   1,   100,   0	    },
{INFANTRY,	150    ,  3,   18,  20,   1,  0,    -1,   1,   -1,   -1,   2,   600,  300		},
{INFANTRY,	500    ,  4,   10,  150,  3,  15,   1,   1,   50,   1,     5,   2000,  1500		},
{VEHICLE,	600    ,  7,   12,  400,  4,  150,  2,   10,  -1,   -1,    4,   1500,  600		},
{VEHICLE,	500    ,  6,   14,  100,  2,  200,  1,   10,  -1,   -1,    3,   1000,  400		},
{VEHICLE,	800    ,  5,   20,  200,  4,  300,  3,   10,  70,   1,     10,  4000,  2500		},
{AIRCRAFT,	300    ,  12,  10,  50,   2,  5,    0,   1,   -1,   -1,    2,   300,   450		},
{AIRCRAFT,	600    ,  15,  16,  300,  3,  200,  4,   20,  50,   1,     8,   1500,  3000		},
{BUILDING,	INT_MAX,  0,   0,   0,    0,  0,   -1,   -1,  -1,   0,     0,   0,     0	    },
{BUILDING,	INT_MAX,  0,   0,   0,    0,  0,   -1,   -1,  -1,   0,     0,   0,     0	    },
{BUILDING,	INT_MAX,  0,   0,   0,    0,  0,   -1,   -1,  -1,   0,     0,   0,     0	    },
{BUILDING,	INT_MAX,  0,   0,   0,    0,  0,   -1,   -1,  -1,   0,     0,   0,     0	    },
{BUILDING,	INT_MAX,  0,   0,   0,    0,  0,   -1,   -1,  -1,   0,     0,   0,     0	    },
{BUILDING,	INT_MAX,  0,   0,   0,    0,  0,   -1,   -1,  -1,   0,     0,   0,     0	    },
{BUILDING,	INT_MAX,  0,   0,   0,    0,  0,   -1,   -1,  -1,   0,     0,   0,     0	    },
{BUILDING,	INT_MAX,  0,   0,   0,    0,  0,   -1,   -1,  -1,   0,     0,   0,     0	    },
{BUILDING,	INT_MAX,  0,   0,   0,    0,  0,   -1,   -1,  -1,   0,     0,   0,     0	    },
{BUILDING,	INT_MAX,  0,   0,   0,    0,  0,   -1,   -1,  -1,   0,     0,   0,     0	    },
{BUILDING,	INT_MAX,  0,   0,   0,    0,  0,   -1,   -1,  -1,   0,     0,   0,     0	    },
{BUILDING,	INT_MAX,  0,   0,   0,    0,  0,   -1,   -1,  -1,   0,     0,   0,     0	    },
{BUILDING,	INT_MAX,  0,   0,   0,    0,  0,   -1,   -1,  -1,   0,     0,   0,     0	    }

};

//定义的若干结构体
//resourse
struct resourse
{
	int money_1;
	int remain_people_1;
	int tech_1;
	
	int money_2;
	int remain_people_2;
	int tech_2;
};
struct Position
{
	int x;
	int y;
	Position(int xx = -1, int yy = -1) :x(xx),y(yy){};
};
const Position none_position = Position(-1,-1);
struct BuildingHandle
{
	int id;
	int flag;
	TypeName type;
	Position pos;
	BuildingHandle(int xx = -1, int yy = -1,TypeName t= Type_num,int x=0,int y=0) :id(xx), flag(yy), type(t),pos(x,y){};
};



struct Unit
{
	TypeName type_name;
	UnitType unit_type;
	int attack_mode;			// 攻击模式，例如可对空，可对坦克，可对步兵之类的
	float attack_now;					// 当前攻击
	float defense_now;				// 当前防御
	int disable_since ;			// 被瘫痪的时间点，用于判断瘫痪时间
	int flag ;					// 所属阵营
	int hacked_point;				// 被黑的点数
	int healing_rate ;		// 治疗 / 维修速率	
	float health_now;					// 当前生命值		
	int is_disable;		// 是否被瘫痪
	float max_health_now;				// 当前HP上限
	float max_speed_now;				// 当前最大速度
	Position position;				// 单位位置
	float shot_range_now;				// 当前射程
	int skill_last_release_time1;// 上次技能1释放时间
	int skill_last_release_time2;// 上次技能2释放时间
	int unit_id;				// 单位id
	Unit();	
	Unit(int unit_id, int flag, TypeName type_name, Position pos);  
	void Print();
};

class Instr	
{
public:
	int instruction_type;				//1表示skil1,2表示skil2,3表示produce,4表示Move,5表示capture
	int the_unit_id;
	int target_id_building_id;
	Position pos1;					
	Position pos2;
	Instr(int instru_type=-1,int u_id=-1,int tar_build_id=-1,Position tpos1=none_position,Position tpos2=none_position);
};





//选手调用函数的声明
void skill_1(int unit_id,int target_id=-1,Position tpos1=none_position,Position tpos2=none_position) ;
void skill_2(int unit_id,int target_id=-1,Position tpos1=none_position,Position tpos2=none_position) ;
void produce(int building_id);
void Move(int unit_id, Position pos);
void capture(int unit_id, int building_id);



Unit * getUnit(void);
int getUnitSize(void);
float * getBuff(void);
resourse getResourse(void);
bool getTeamId(void);
void NewData(void);
