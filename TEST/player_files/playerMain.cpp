#include"teamstyle18-my-1.h"
#include"initialize.h"
#include<iostream>
#include<cstdlib>
#include<vector>
#include<algorithm>
#include<set>
using namespace std;
using namespace infomation;

int dist(Position &a, Position &b) ;
inline bool ifbuilding(Position &a) ;
inline bool iffly(Unit &a) ;
inline int J_threat(Unit &me, Position &p) ;
int getval(Unit &me, Position &a, Position &dest) ;
void amove(Unit &a, Position dest) ;
inline bool ifbuilding(Position &a)//����Ҫ�����ܶ�׿��map�Ǹ�����ĺ���������ȡ��ǰλ���Ƿ��н���
{
	
	if(map.at(a).building==nullptr)
		return 0 ;//�оͷ���1���޾ͷ���0
	return 1;
}
inline int J_threat(Unit &me, Position &p)//��вֵ��������ʱ�趨Ϊ0
{
	return 0 ;
}
int getval(Unit &me, Position &a, Position &dest)//������ֵ��������ʱû����в������ֱ�Ӽǳ���Ŀ�ĵؾ���
{
	return dist(a, dest) + J_threat(me, a) ;
}
inline bool iffly(Unit &a)//�����Ƿ��Ƿ�����
{
	return (a.unit_type == AIRCRAFT) ;
}
int dist(Position &a, Position &b)//����
{
	return (abs(a.x - b.x) + abs(a.y -b.y)) ;
}
void amove(Unit &a, Position dest)//�ƶ���������destΪ��ǰĿ�ĵ�
{
	Position tdest ;
	int maxd = (int)a.max_speed_now ;
	int n = 2 * maxd * maxd + 2 * maxd +1 ;//���������
	int tval, val ;//ĳ��λ����ֵ����ǰ��С��ֵ
	Position *maydest = new Position[n] ;
	int j = 0 ;
	for (int x = a.position.x - maxd; x <= a.position.x; x++)//������벿��ͼ�ε�λ��
	{
		for (int y = a.position.y + a.position.x - x - maxd;  y <= a.position.y - (a.position.x - x - maxd); y++)
		{
			maydest[j].x = x ; 
			maydest[j].y = y ;
			j++ ;
		}
	}
	for (int x = a.position.x + 1; x <= a.position.x + maxd; x++)//�Ұ벿��ͼ��
	{
		for (int y = a.position.y - (a.position.x - x + maxd); y <= a.position.y + (a.position.x - x + maxd); y++)
		{
			maydest[j].x = x ; 
			maydest[j].y = y ;
			j++ ;
		}
	}
	tdest = a.position ;
	val = getval(a, tdest, dest) ;//��ʼ����ֵΪԭλ������һ������ֵ���͵�
	if (iffly(a))//�ɻ�����
	{
		for (int i = 0; i < n; i++)
		{
			tval = getval(a, maydest[i], dest) ;
			if (tval < val)
			{
				tdest = maydest[i] ;
				val = tval ;
			}
		}
	}
	else
	{
		for (int i = 0; i < n; i++)
		{
			if (ifbuilding(maydest[i]))
			{
				continue ;
			}
			else
			{
				tval = getval(a, maydest[i], dest) ;
				if (tval < val)
				{
					tdest = maydest[i] ;
					val = tval ;
				}
			}
		}
	}
	Move(a.unit_id, tdest) ;//�������ú����ƶ�����ǰĿ�ĵ�
}

int TEAMID;
int NOW_TURN;
int UNITSIZE;
int OTHERTEAMID;
Unit *ALLUNIT,*MYHOME,*OTHERHOME;

inline int Distance(Position x,Position y){
	return (abs(x.x-y.x)+abs(x.y-y.y));
}

Unit* getOtherHome(){
	for(int i=0;i<UNITSIZE;i++){
		Unit* t=ALLUNIT+i;
		if(t->unit_id==OTHERTEAMID)
			return t;
	}
}

Unit* getMyHome(){
	for(int i=0;i<UNITSIZE;i++){
		Unit* t=ALLUNIT+i;
		if(t->unit_id==TEAMID)
			return t;
	}
}

void Product(){
	produce(TEAMID);
	for(vector<Unit*>::iterator i=MY.uav_lab.begin();i!=MY.uav_lab.end();i++)
		produce((*i)->unit_id);
	for(vector<Unit*>::iterator i=MY.car_lab.begin();i!=MY.car_lab.end();i++)
		produce((*i)->unit_id);
}

bool build_cmp(const Unit* a,const Unit* b){
	return (Distance(a->position,MYHOME->position)<Distance(b->position,MYHOME->position));
}

set<Unit*> use_meat;

void Capture(){
	vector<Unit*> target_build;
	for(vector<Unit*>::iterator i=EN.uav_lab.begin();i!=EN.uav_lab.end();i++)
		target_build.push_back(*i);
	for(vector<Unit*>::iterator i=NE.begin();i!=NE.end();i++)
		if((*i)->type_name==UAV_LAB)
			target_build.push_back(*i);
	for(vector<Unit*>::iterator i=EN.car_lab.begin();i!=EN.car_lab.end();i++)
		target_build.push_back(*i);
	for(vector<Unit*>::iterator i=NE.begin();i!=NE.end();i++)
		if((*i)->type_name==CAR_LAB)
			target_build.push_back(*i);
	for(vector<Unit*>::iterator i=EN.resourse_building.begin();i!=EN.resourse_building.end();i++)
		target_build.push_back(*i);
	for(vector<Unit*>::iterator i=NE.begin();i!=NE.end();i++)
		if((*i)->type_name==BANK||(*i)->type_name==TEACH_BUILDING)
			target_build.push_back(*i);

	sort(target_build.begin(),target_build.end(),build_cmp);
	for(vector<Unit*>::iterator i=MY.meat.begin();i!=MY.meat.end();i++){
		Unit* now=*i;
		int j=(int)(i-MY.meat.begin());
//		for(vector<Unit*>::iterator j=target_build.begin();j!=target_build.end();j++)
		if(j<target_build.size()){
			if(Distance(now->position,target_build[j]->position)==1)
				capture(now->unit_id,(target_build[j])->unit_id);
			else 
				amove(*now,target_build[j]->position);
		}
		else
			break;
	}
	return;
}			
	
void MoveAll(){
	for(vector<Unit*>::iterator i=MY.battle_unit.begin();i!=MY.battle_unit.end();i++)
		amove((**i),OTHERHOME->position);
}

void putskill(Unit* u,Unit* v){
	skill_1(u->unit_id,v->unit_id,v->position);
	skill_2(u->unit_id,v->unit_id,v->position);
}

void Attack(){
	using namespace infomation;
	for(vector<Unit*>::iterator i=MY.battle_unit.begin();i!=MY.battle_unit.end();i++){
		if(static_cast<float>(Distance((*i)->position,OTHERHOME->position))<=(*i)->shot_range_now)
			putskill(*i,OTHERHOME);
		for(vector<Unit*>::iterator j=EN.battle_unit.begin();j!=EN.battle_unit.end();j++)
			if(static_cast<float>(Distance((*i)->position,(*j)->position))<=(*i)->shot_range_now)
				putskill(*i,*j);
	}
	return;
}

void player_main(void)
{
	/*
	NOW_TURN++;
	initialize();
	TEAMID=static_cast<int>(getTeamId());
	produce(MY.base->unit_id);
	cout<<MY.meat.size()<<endl;
	return;

	*/
	teamid = getTeamId();
	NOW_TURN++;
	UNITSIZE=getUnitSize();
	ALLUNIT=getUnit();
	if(NOW_TURN==1){
		TEAMID=static_cast<int>(getTeamId());
		OTHERTEAMID=1-TEAMID;
		OTHERHOME=getOtherHome();
		MYHOME=getMyHome();
	}
	initialize();
//	ALLRESOURCE=getResourse();
//	ALLBUFF=getBuff();

	// get data
	
	Product();
	//product
	Capture();
	Attack();
	MoveAll();
	//action
	
	return;
}

