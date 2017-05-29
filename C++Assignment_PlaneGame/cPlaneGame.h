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
	int energy;//��������������ʹ�ü�����Ҫ����
	int rate, count;
	int atk, def;
	int coin;//��ҽ�ң��ɹ������Ժ�װ��
	int x, y;//�����Դ������Ͻ�Ϊ��0��0����xΪ�����꣬yΪ������
	int skill1_n;//��ҵ�һ�����ܵĴ��ţ�����ȷ���ü�����ʲô
	int skill1_cd;//��ҵ�һ�����ܵ���ȴʱ��
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
	int x;//�����Դ������Ͻ�Ϊ��0��0����xΪ�����꣬yΪ������
	int y;
	int dir;//�����Ƿ�б�ߣ�-1Ϊ��б��1Ϊ��б
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
	int total_hp;//��ѪѪ��
	int hp = 0;//����Ѫ��
	int atk = 0;//���˹�����
	int def = 0;//���˷��������ܵ�����ʱ�˺�������������С�ڵ���0�����1���˺�
	int coin = 0;//ɱ���õ��˻�ý��
	int x = 0;//�����Դ������Ͻ�Ϊ��0��0����xΪ�����꣬yΪ������
	int y = 0;
	int fire_rate = 0;//�õ��˿�����ٶȣ�ÿrate֡����һ��
	int move_dir_x = 0;//����x�����ƶ��ٶȺͷ���
	int move_dir_y = 0;//����y�����ƶ��ٶȺͷ���
	int fire_dir = 0;//������ɿ����Ƿ�б��
	int fire_count = 0;//ÿ֡��1���ﵽrate�򿪻�����
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
	std::vector<std::array<int, 2>>EnemyAppearList;//���ڳ���֮��enemy
public:
	operate_system() {}
	void CreateEnemy(enemy& tenemy);
	void CreateEnemyBullet(enemy_bullet& teb);
	void CreatePlayerBullet(player_bullet& tpb);
	void MoveEnemy(void);
	void DrawToScreen(std::pair<int, int>);
	int CheckCrash(std::pair<int, int>);//������ײenemy��hp*atk
	void CheckDisapear(void);
	int CheckKill(void);//���ػ�ý������
	void PlayerAttackEnemy(void);
	void MovePlayerBullet(void);
	void DisappearPlayerBullet(void);
	void MoveEnemyBullet(void);
	void EnemyFire(void);
	void EnemyAttackPlayer(player&);
	void DisappearEnemyBullet(void);
	
};
#endif // !_cPlaneGame_H_