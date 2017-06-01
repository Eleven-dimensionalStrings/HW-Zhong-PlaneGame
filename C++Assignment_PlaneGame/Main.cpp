#include "cPlaneGame.h"
#include <iostream>
using namespace std;
int main()
{
	SetPriorityClass(GetCurrentProcess(), 0x00000040);
	operate_system OperateSystem;
	player PlayerPlane(1000, 1000, 10, 0, 30, 5, 0, 275, 549, 0, 0);
	default_random_engine en(time(0));
	uniform_int_distribution<int>RanPos(0, 24);
	uniform_int_distribution<int>RanAtk(10, 20);
	uniform_int_distribution<int>RanHp(100, 150);
	uniform_int_distribution<int>RanDir(-2, 2);
	for (int j = 0; j < 10; ++j)
	{
		enemy * e = new enemy(RanHp(en), RanAtk(en), 0, 5, ENEMY * RanPos(en), 0, 60, -1, 1, RanDir(en) / 2, RanDir(en) / 2 + 2, 0);
		vector<array<int, 3>>a;
		array<int, 3>b{ 0,2,20 };
		a.push_back(b);
		for (int i = 0; i < 100; ++i)
		{
			b[0] = RanDir(en);
			b[1] = RanDir(en);
			b[2] = RanHp(en);
			a.push_back(b);
		}
		e->SetMoveList(a);
		OperateSystem.CreateEnemy(*e);
	}
	initgraph(600, 800, 0);
	HWND draw = GetHWnd();
	MoveWindow(draw, 400, 0, 615, 839, NULL);
	setbkcolor(0xF9FFFF);
	cleardevice();
	IMAGE MAP(600, 800);
	settextcolor(0x000000);
	system("pause");
	setfillcolor(LIGHTCYAN);
	SetWorkingImage(&MAP);
	setbkcolor(0xF9FFFF);
	while (!GetAsyncKeyState(VK_ESCAPE))
	{
		SetWorkingImage(&MAP);
		//setbkcolor(0xF9FFFF);
		cleardevice();
		PlayerPlane.MovePlayer(PlayerPlane.PlayerOperate());
		OperateSystem.MoveEnemy();
		OperateSystem.EnemyFire();
		OperateSystem.MoveEnemyBullet();
		PlayerPlane.beCrash(OperateSystem.CheckCrash(PlayerPlane.GetPos()));
		OperateSystem.CheckDisapear();
		if (PlayerPlane.ifFire())
			OperateSystem.CreatePlayerBullet(PlayerPlane.Fire());
		OperateSystem.MovePlayerBullet();
		OperateSystem.DisappearPlayerBullet();
		OperateSystem.PlayerAttackEnemy();
		OperateSystem.DisappearEnemyBullet();
		OperateSystem.EnemyAttackPlayer(PlayerPlane);
		OperateSystem.CheckKill();
		OperateSystem.DrawToScreen(PlayerPlane.GetPos(), PlayerPlane.GetData());
		SetWorkingImage(0);
		putimage(0, 0, &MAP);
		Sleep(15);
	}
	closegraph();
}