#include "cPlaneGame.h"
#include <iostream>
using namespace std;
int main()
{
	SetPriorityClass(GetCurrentProcess(), 0x00000040);
	operate_system OperateSystem;
	player PlayerPlane(1000, 1000, 10, 0, 30, 5, 0, 275, 549, 0, 0);
	uniform_int_distribution<int>RanAtk(10, 20);
	uniform_int_distribution<int>RanHp(100, 150);
	/*for (int j = 0; j < 10; ++j)
	{
		enemy * e = new enemy(RanHp(en), RanAtk(en), 0, 5, ENEMY * RanPos(en), 0, 60, -1, 1, RanDirx(en) / 2, RanDiry(en) / 2 + 2, 0);
		vector<array<int, 3>>a;
		array<int, 3>b{ 0,2,20 };
		a.push_back(b);
		for (int i = 0; i < 100; ++i)
		{
			b[0] = RanDirx(en);
			b[1] = RanDiry(en);
			b[2] = RanHp(en);
			a.push_back(b);
		}
		e->SetMoveList(a);
		OperateSystem.CreateEnemy(*e);
	}*/

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
	cout << clock() << endl;
	OperateSystem.tSet();
	cout << clock() << endl;
	while (!GetAsyncKeyState(VK_ESCAPE))
	{
		SetWorkingImage(&MAP);
		cleardevice();
		PlayerPlane.MovePlayer(PlayerPlane.PlayerOperate());
		if (!OperateSystem.AppearEnemy() || PlayerPlane.ifBeKilled())
			break;
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
		PlayerPlane.GainCoin(OperateSystem.CheckKill());
		OperateSystem.DrawToScreen(PlayerPlane.GetPos(), PlayerPlane.GetData());
		SetWorkingImage(0);
		putimage(0, 0, &MAP);
		Sleep(15);
	}
	closegraph();
	char a[100]{ 48,49,50,51,52,53,54,55,56,57,58 };
	string b = a;
	cout << b << endl;
	cout << PlayerPlane.GetData()[0] << endl;
	system("pause");
}