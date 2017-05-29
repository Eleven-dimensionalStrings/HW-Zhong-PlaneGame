#include "cPlaneGame.h"
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
	initgraph(600, 800, 0);
	HWND draw = GetHWnd();
	setbkcolor(WHITE);
	cleardevice();
	IMAGE MAP(600, 800);
	for (int i = 0; i < 5; ++i)
	{
		enemy * e = new enemy(RanHp(en), RanAtk(en), 0, 5, ENEMY * RanPos(en), 0, 60, -1, 1, 0, 0);
		OperateSystem.CreateEnemy(*e);
	}
	system("pause");
	setfillcolor(LIGHTCYAN);
	while (!GetAsyncKeyState(VK_ESCAPE))
	{
		SetWorkingImage(&MAP);
		setbkcolor(WHITE);
		cleardevice();
		PlayerPlane.MovePlayer(PlayerPlane.PlayerOperate());
		OperateSystem.MoveEnemy();
		OperateSystem.EnemyFire();
		OperateSystem.MoveEnemyBullet();
		OperateSystem.CheckCrash(PlayerPlane.GetPos());
		OperateSystem.CheckDisapear();
		if (PlayerPlane.ifFire())
			OperateSystem.CreatePlayerBullet(PlayerPlane.Fire());
		OperateSystem.MovePlayerBullet();
		OperateSystem.DisappearPlayerBullet();
		OperateSystem.PlayerAttackEnemy();
		OperateSystem.DisappearEnemyBullet();
		OperateSystem.EnemyAttackPlayer(PlayerPlane);
		OperateSystem.CheckKill();
		OperateSystem.DrawToScreen(PlayerPlane.GetPos());
		SetWorkingImage(0);
		putimage(0, 0, &MAP);
		Sleep(15);
	}
	closegraph();
}