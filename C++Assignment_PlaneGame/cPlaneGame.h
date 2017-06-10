#pragma once
#ifndef _cPlaneGame_H_
#define _cPlaneGame_H_
#include <string>
#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <array>
#include <random>
#include <ctime>
#include <Windows.h>
#include <easyx.h>
#include <graphics.h>
#include <conio.h>
#define VK_W 0x57
#define VK_S 0x53
#define VK_A 0x41
#define VK_D 0x44
#define VK_J 0x4A
#define VK_K 0x4B
#define ENEMY 25
#define PLAYER 50
#define BULLET 3
#define LASERLENGTH 5
class player;
class operate_system;
class player_bullet
{
private:
	int atk;
	int x;
	int y;
	int speed;//每过frame帧后移动speed像素
	int frame;
	int now_frame;
	int radio;
public:
	player_bullet(int ta, int tx, int ty);
	std::pair<int, int> GetPos(void)const;
	int GetAtk(void)const;
	void move(void);
	bool CheckDisapear(void);
	std::pair<int, int> GetPos(void);
	void SetSpeed(int tspeed, int tframe);
	int GetRadio(void)const;
	void SetRadio(int tR);
	void draw(void)const;
};
class player
{
private:
	int hp;
	int energy;//玩家能量，开火和使用技能需要消耗
	int rate, count;
	int atk, def;
	int coin;//玩家金币，可购买属性和装备
	int x, y;//坐标以窗口左上角为（0，0），x为横坐标，y为纵坐标
	int level;
	int steel_hp_persent;
	int eq1, eq2, eq3;
public:
	player(int thp, int tenergy, int tr, int tc, int tatk, int tdef, int tcoin, int tx, int ty, int tskill1_n, int tskill1_cd);
	player(std::string data);
	void beAttacked(int a);
	void beCrash(int a_hp);
	std::pair<int, int> PlayerOperate(void)const;
	void MovePlayer(std::pair<int, int>);
	std::pair<int, int> GetPos(void)const;
	int GetDef(void)const;
	player_bullet& Fire(void);
	bool ifFire(void);
	std::vector<std::string> GetData(void)const;
	void GainCoin(int c);
	std::string ReturnInformation(void)const;
	bool ifBeKilled(void)const;
	void BuyEquipment(void);
	int GetSteelHpPersent(void)const;
	void SteelHp(int dhp);
	int GetFireMinus(void)const;
	int GetExAtk(void)const;
	int GetExDef(void)const;
};
class enemy_bullet
{
private:
	int atk;
	int x;//坐标以窗口左上角为（0，0），x为横坐标，y为纵坐标
	int y;
	int speed_x;
	int speed_y;
	int frame;
	int now_frame;
	int radio;
public:
	enemy_bullet(int ta, int tx, int ty, int tspeedx, int tspeedy);
	virtual bool CheckDisapear(void);
	virtual void Move(player&);
	std::pair<int, int> GetPos(void)const;
	int GetAtk(void)const;
	void SetSpeed(int tspeedx,int tspeedy);
	int GetRadio(void)const;
	void SetRadio(int tR);
	virtual bool ifAttack(std::pair<int, int>pos)const;
	virtual void Draw(void)const;
	int GetX(void)const;
	int GetY(void)const;
	void SetFrame(int tframe);
	int GetFrame(void)const;
	void ModifyNowFrame(int);
	std::pair<int, int> GetSpeed(void)const;
	void ModifyPos(std::pair<int, int>speed);
	int GetNowFrame(void)const;
};
class laser : public enemy_bullet
{
public:
	laser(int ta, int tx, int ty, int tspeedx, int tspeedy);
	bool CheckDisapear(void);
	bool ifAttack(std::pair<int, int>pos)const;
	void Draw(void)const;
};
class track_bullet : public enemy_bullet
{
private:
	int change_num, now_num;
public:
	track_bullet(int ta, int tx, int ty, int tspeedx, int tspeedy);
	void ChangeDirection(std::pair<int,int>pos);
	void Move(player&);
};
class enemy
{
private:
	int total_hp;//满血血量
	int hp = 0;//敌人血量
	int atk = 0;//敌人攻击力
	int def = 0;//敌人防御力，受到攻击时伤害减防御力，若小于等于0则造成1点伤害
	int coin = 0;//杀死该敌人获得金币
	int x = 0;//坐标以窗口左上角为（0，0），x为横坐标，y为纵坐标
	int y = 0;
	int fire_rate = 0;//该敌人开火的速度，每rate帧开火一次
	int move_speed_x = 0;//敌人x轴上移动速度和方向
	int move_speed_y = 0;//敌人y轴上移动速度和方向
	int fire_speed_x = 0, fire_speed_y;
	int move_frame, now_frame;
	int fire_cd = 0;//每帧加1，达到rate则开火并重置
	int enemy_length;
	std::vector<std::array<int, 3>>move_list;
	std::vector<std::array<int, 3>>::iterator move_step;
	std::vector<std::array<int, 3>>fire_list;
	std::vector<std::array<int, 3>>::iterator fire_step;
	int move_count, fire_count, move_list_flag, fire_list_flag;
	int bullet_radio;
public:
	enemy(int thp, int tatk, int tdef, int tcoin, int tx, int ty,
		int tatkr, int tdir_x, int tdir_y, int tfire_speed_x, int tfire_speed_y, int tfire_count);
	virtual void move(void);
	virtual bool ifBeAttacked(int bx, int by)const;
	virtual int beAttacked(int);
	virtual bool countFire(void);
	virtual enemy_bullet& Fire(void);
	bool ifKill(void)const;
	bool ifLeftRightLimit(void)const;
	bool ifDownLimit(void)const;
	std::pair<int, int> GetPos(void)const;
	int GetHpPercent(void)const;
	int GetAtk(void)const;
	int GetDef(void)const;
	int GetHp(void)const;
	int GetCoin(void)const;
	bool CheckDisapear(void)const;
	bool CheckCrash(std::pair<int, int>pos)const;
	virtual void SetMoveList(std::vector<std::array<int, 3>>tmove_list);
	virtual void SetFireList(std::vector<std::array<int, 3>>tfire_list);
	virtual void ChangeFireState(void);
	virtual void ChangeMoveState(void);
	virtual void UseSkill(player&,operate_system&,int num);
	void SetFrame(int tframe);
	bool GetFlag(int)const;
	void SetLength(int tL);
	int GetLength(void)const;
	void SetBulletRadio(int r);
	virtual void Draw(void)const;
	std::pair<int, int> GetFireSpeed(void)const;
	int GetBulletRadio(void)const;
	std::pair<int, int>GetSpeed(void)const;
	virtual bool countSkill(void);
	void ModifyHp(int a);
	virtual int GetSkillKind(int num)const;
	virtual int GetSkillQuantity(void)const;
};
class normal_1 : public enemy
{
private:
	int skill_cd, skill_count, skill_time, skill_flag, skill_kind, skill_quantity;
public:
	normal_1(int thp, int tatk, int tdef, int tcoin, int tx, int ty,
		int tatkr, int tdir_x, int tdir_y, int tfire_speed_x, int tfire_speed_y, int tfire_count);
	bool countFire(void);
	enemy_bullet& Fire(void);
	void SetSkill(int tcd, int ttime);
	bool countSkill(void);//flag为技能使用状态，1为正在使用。返回1则需要切换状态。
	void UseSkill(player&, operate_system&,int num);
	int beAttacked(int a);
	int GetSkillKind(int num)const;
	void Draw(void)const;
	int GetSkillQuantity(void)const;
};
class normal_2 : public enemy
{
private:
	int skill_cd, skill_count, skill_time, skill_flag, skill_kind, skill_quantity;
public:
	normal_2(int thp, int tatk, int tdef, int tcoin, int tx, int ty,
		int tatkr, int tdir_x, int tdir_y, int tfire_speed_x, int tfire_speed_y, int tfire_count);
	enemy_bullet& Fire(void);
	void SetSkill(int tcd, int ttime);
	bool countSkill(void);//flag为技能使用状态，1为正在使用。返回1则需要切换状态。
	void UseSkill(player&, operate_system&, int num);
	int beAttacked(int a);
	int GetSkillKind(int num)const;
	void Draw(void)const;
	int GetSkillQuantity(void)const;
};
class normal_3 :public enemy
{
private:

public:
	normal_3(int thp, int tatk, int tdef, int tcoin, int tx, int ty,
		int tatkr, int tdir_x, int tdir_y, int tfire_speed_x, int tfire_speed_y, int tfire_count);
	enemy_bullet& Fire(void);
};
class normal_4 : public enemy
{
private:
	int skill_cd, skill_count, skill_time, skill_flag, skill_kind, skill_quantity;
public:
	normal_4(int thp, int tatk, int tdef, int tcoin, int tx, int ty,
		int tatkr, int tdir_x, int tdir_y, int tfire_speed_x, int tfire_speed_y, int tfire_count);
	void SetSkill(int tcd, int ttime);
	bool countSkill(void);//flag为技能使用状态，1为正在使用。返回1则需要切换状态。
	void UseSkill(player&, operate_system&, int num);
	int GetSkillKind(int num)const;
	int GetSkillQuantity(void)const;
};
class operate_system
{
private:
	std::list<enemy*>EnemyList;
	std::list<enemy_bullet*>EnemyBulletList;
	std::list<player_bullet*>PlayerBulletList;
	std::vector<std::array<int, 2>>appear_enemy_list;//用于出现之后enemy
	int appear_enemy_count;
	std::vector<std::array<int, 2>>::iterator appear_enemy_step;
	std::vector<std::vector<std::array<int, 3>>>appear_enemy_move_list;
	std::vector<std::vector<std::array<int, 3>>>::iterator appear_enemy_move_inf;
	std::vector<std::vector<std::array<int, 3>>>appear_enemy_fire_list;
	std::vector<std::vector<std::array<int, 3>>>::iterator appear_enemy_fire_inf;
	std::vector<std::array<int, 11>>appear_enemy_attribution;
	std::vector<std::array<int, 11>>::iterator appear_enemy_attribution_inf;
public:
	operate_system();
	void CreateEnemy(enemy& tenemy);
	void CreateEnemyBullet(enemy_bullet& teb);
	void CreatePlayerBullet(player_bullet& tpb);
	void MoveEnemy(void);
	void DrawToScreen(std::pair<int, int>, std::vector<std::string>player_data);
	int CheckCrash(std::pair<int, int>);//返回碰撞enemy的hp*atk
	void CheckDisapear(void);
	int CheckKill(void);//返回获得金币总数
	void PlayerAttackEnemy(player& plane);
	void MovePlayerBullet(void);
	void DisappearPlayerBullet(void);
	void MoveEnemyBullet(player&);
	void EnemyFire(void);
	void EnemyAttackPlayer(player& plane);
	void DisappearEnemyBullet(void);
	void SetAppearEnemyList(std::vector<std::array<int, 2>>tEnemyAppearList, 
		std::vector<std::vector<std::array<int, 3>>>tAppearEnemyMoveList,
		std::vector<std::vector<std::array<int, 3>>>tAppearEnemyFireList,
		std::vector<std::array<int, 11>>tAppearEnemyAttribution);
	bool AppearEnemy(void);
	void tSet(void);
	void EnemyUseSkill(player&);
	 
};
#endif // !_cPlaneGame_H_