#pragma once
#ifndef _cPlaneGame_H_
#define _cPlaneGame_H_
#include <string>
#include <fstream>
#include <list>
#include <vector>
#include <array>
#include <random>
#include <ctime>
#include <Windows.h>
#include <easyx.h>
#include <graphics.h>
#define VK_W 0x57
#define VK_S 0x53
#define VK_A 0x41
#define VK_D 0x44
#define VK_J 0x4A
#define VK_K 0x4B
#define VK_L 0x4C
#define ENEMY 25
#define PLAYER 50
#define BULLET 4
class player_bullet
{
private:
	int atk;
	int x;
	int y;
public:
	player_bullet(int ta, int tx, int ty);
	std::pair<int, int> GetPos(void)const;
	int GetAtk(void)const;
	void move(void);
	bool CheckDisapear(void);
	std::pair<int, int> GetPos(void);
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
	int skill1_n;//玩家第一个技能的代号，用于确定该技能是什么
	int skill1_cd;//玩家第一个技能的冷却时间
public:
	player(int thp, int tenergy, int tr, int tc, int tatk, int tdef, int tcoin, int tx, int ty, int tskill1_n, int tskill1_cd);
	bool ifBeAttacked(std::pair<int, int>pos)const;
	void beAttacked(int a);
	std::pair<int, int> PlayerOperate(void)const;
	void MovePlayer(std::pair<int, int>);
	std::pair<int, int> GetPos(void)const;
	int GetDef(void)const;
	player_bullet& Fire(void);
	bool ifFire(void);
};
class enemy_bullet
{
private:
	int atk;
	int x;//坐标以窗口左上角为（0，0），x为横坐标，y为纵坐标
	int y;
	int dir;//控制是否斜走，-1为左斜，1为右斜
public:
	enemy_bullet(int ta, int tx, int ty, int td);
	bool CheckDisapear(void);
	void Move(void);
	std::pair<int, int> GetPos(void)const;
	int GetAtk(void)const;
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
	int move_dir_x = 0;//敌人x轴上移动速度和方向
	int move_dir_y = 0;//敌人y轴上移动速度和方向
	int fire_dir = 0;//随机生成开火是否斜向
	int fire_count = 0;//每帧加1，达到rate则开火并重置
public:
	enemy(int thp, int tatk, int tdef, int tcoin, int tx, int ty, int tatkr, int tdir_x, int tdir_y, int tfire_dir, int tfire_count);
	void move(void);
	bool ifBeAttacked(int bx, int by)const;
	void beAttacked(int);
	virtual bool countFire(void);
	virtual enemy_bullet& Fire(void);
	bool ifKill(void)const;
	bool ifLeftRightLimit(void)const;
	bool ifDownLimit(void)const;
	bool ifCrashPlayer(int px, int py)const;
	std::pair<int, int> GetPos(void)const;
	int GetHpPercent(void)const;
	int GetAtk(void)const;
	int GetHp(void)const;
	int GetCoin(void)const;
	bool CheckDisapear(void)const;
	bool CheckCrash(std::pair<int, int>pos)const;
};
class boss : public enemy
{
private:

public:

};
class normal_enemy : public enemy
{
private:

public:

};
class operate_system
{
private:
	std::list<enemy*>EnemyList;
	std::list<enemy_bullet*>EnemyBulletList;
	std::list<player_bullet*>PlayerBulletList;
	std::vector<std::array<int, 2>>EnemyAppearList;//用于出现之后enemy
public:
	operate_system() {}
	void CreateEnemy(enemy& tenemy);
	void CreateEnemyBullet(enemy_bullet& teb);
	void CreatePlayerBullet(player_bullet& tpb);
	void MoveEnemy(void);
	void DrawToScreen(std::pair<int, int>);
	int CheckCrash(std::pair<int, int>);//返回碰撞enemy的hp*atk
	void CheckDisapear(void);
	int CheckKill(void);//返回获得金币总数
	void PlayerAttackEnemy(void);
	void MovePlayerBullet(void);
	void DisappearPlayerBullet(void);
	void MoveEnemyBullet(void);
	void EnemyFire(void);
	void EnemyAttackPlayer(player&);
	void DisappearEnemyBullet(void);
	
};
#endif // !_cPlaneGame_H_