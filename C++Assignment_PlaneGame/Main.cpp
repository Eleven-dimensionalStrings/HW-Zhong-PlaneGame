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
	initgraph(600, 800, SHOWCONSOLE);
	HWND draw = GetHWnd();
	MoveWindow(draw, 400, 0, 615, 839, NULL);
	setbkcolor(0xF9FFFF);
	cleardevice();
	IMAGE MAP(600, 800);
	settextcolor(0x000000);
	setfillcolor(LIGHTCYAN);
	SetWorkingImage(&MAP);
	setbkcolor(0xF6FFFF);
	cout << clock() << endl;
	OperateSystem.tSet();
	cout << clock() << endl;
	PlayerPlane.BuyEquipment();
	while (!GetAsyncKeyState(VK_ESCAPE))
	{
		SetWorkingImage(&MAP);
		cleardevice();
		PlayerPlane.MovePlayer(PlayerPlane.PlayerOperate());
		if (!OperateSystem.AppearEnemy() || PlayerPlane.ifBeKilled())
			break;
		OperateSystem.MoveEnemy();
		OperateSystem.EnemyFire();
		OperateSystem.EnemyUseSkill(PlayerPlane);
		OperateSystem.MoveEnemyBullet(PlayerPlane);
		PlayerPlane.beCrash(OperateSystem.CheckCrash(PlayerPlane.GetPos()));
		OperateSystem.CheckDisapear();
		if (PlayerPlane.ifFire())
			OperateSystem.CreatePlayerBullet(PlayerPlane.Fire());
		OperateSystem.MovePlayerBullet();
		OperateSystem.DisappearPlayerBullet();
		OperateSystem.PlayerAttackEnemy(PlayerPlane);
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