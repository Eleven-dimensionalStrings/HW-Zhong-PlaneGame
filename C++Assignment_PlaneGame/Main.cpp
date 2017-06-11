#include "cPlaneGame.h"
#include <iostream>
using namespace std;
int main()
{
	int now_mode = 1;
	SetPriorityClass(GetCurrentProcess(), 0x00000040);
	operate_system OperateSystem;
	player PlayerPlane(1000, 1000, 10, 25, 5, 100, 275, 549);
	initgraph(600, 800, 0);
	HWND draw = GetHWnd();
	MoveWindow(draw, 400, 0, 615, 839, NULL);
	setbkcolor(0xF9FFFF);
	cleardevice();
	IMAGE MAP(600, 800);
	settextcolor(0x000000);
	setfillcolor(LIGHTCYAN);
	SetWorkingImage(&MAP);
	setbkcolor(0xF6FFFF);
	cleardevice();
	IMAGE StartPage(600, 800);
	IMAGE NewGame, Continue, Infinite, HelpPage, Quit, Help;
	loadimage(&HelpPage, "HelpPage.bmp");
	loadimage(&NewGame, "newgame.bmp");
	loadimage(&Continue, "continue.bmp");
	loadimage(&Infinite, "infinitemode.bmp");
	loadimage(&Help, "help.bmp");
	loadimage(&Quit, "quit.bmp");
choose_mode:
	FlushMouseMsgBuffer();
	MOUSEMSG mouse;
	SetWorkingImage(&StartPage);
	setbkcolor(0xF6FFFF);
	cleardevice();
	putimage(300 - NewGame.getwidth() / 2, 50, &NewGame);
	putimage(300 - Continue.getwidth() / 2, 200, &Continue);
	putimage(300 - Infinite.getwidth() / 2, 350, &Infinite);
	putimage(300 - Help.getwidth() / 2, 500, &Help);
	putimage(300 - Quit.getwidth() / 2, 650, &Quit);
	SetWorkingImage(0);
	putimage(0, 0, &StartPage);
	mouse = GetMouseMsg();
	while (!(mouse.x >= 300 - Quit.getwidth() / 2 && mouse.x < 300 + Quit.getwidth() / 2 &&
		mouse.y >= 650 && mouse.y < 650 + Quit.getheight() && mouse.mkLButton))
	{
		mouse = GetMouseMsg();
		if (mouse.x >= 300 - NewGame.getwidth() / 2 && mouse.x < 300 + NewGame.getwidth() / 2 &&
			mouse.y >= 50 && mouse.y < 50 + NewGame.getheight() && mouse.mkLButton)
			goto new_game;
		if (mouse.x >= 300 - Continue.getwidth() / 2 && mouse.x < 300 + Continue.getwidth() / 2 &&
			mouse.y >= 200 && mouse.y < 200 + Continue.getheight() && mouse.mkLButton)
		{
			if (now_mode == 1)
				goto continue_game;
			else
				goto infinite_mode;
		}
		if (mouse.x >= 300 - Infinite.getwidth() / 2 && mouse.x < 300 + Infinite.getwidth() / 2 &&
			mouse.y >= 350 && mouse.y < 350 + Infinite.getheight() && mouse.mkLButton)
			goto infinite_mode;
		if (mouse.x >= 300 - Help.getwidth() / 2 && mouse.x < 300 + Help.getwidth() / 2 &&
			mouse.y >= 500 && mouse.y < 500 + Help.getheight() && mouse.mkLButton)
			goto HelpPage;
	}
	goto quit_game;
new_game:
	OperateSystem.clear();
	PlayerPlane = { 1000, 1000, 10, 20, 5, 100, 275, 549 };
continue_game:
	if (PlayerPlane.GetLevel(1) == 7)
	{
		SetWorkingImage(0);
		setbkcolor(0xF6FFFF);
		cleardevice();
		outtextxy(235, 350, "你已经通关了！");
		outtextxy(220, 400, "去试试无尽模式吧");
		outtextxy(240, 450, "按空格键继续");
		while (!GetAsyncKeyState(VK_SPACE));
		goto choose_mode;
	}
	OperateSystem.tSet(PlayerPlane.GetLevel(1), 1);
	now_mode = 1;
	while (!GetAsyncKeyState(VK_ESCAPE))
	{
		SetWorkingImage(&MAP);
		setbkcolor(0xF6FFFF);
		cleardevice();
		PlayerPlane.MovePlayer(PlayerPlane.PlayerOperate());
		if (!OperateSystem.AppearEnemy(PlayerPlane.GetLevel(1),1) && OperateSystem.empty())
		{
			PlayerPlane.PassLevel(1);
			cleardevice();
			outtextxy(235, 350, "恭喜你通过了这关！");
			outtextxy(220, 400, "按J键继续，按K键退出");
			SetWorkingImage(0);
			putimage(0, 0, &MAP);
			while (1)
			{
				if (GetAsyncKeyState(VK_J))
					goto continue_game;
				else if (GetAsyncKeyState(VK_K))
					goto choose_mode;
			}
		}
		if (PlayerPlane.ifBeKilled())
		{
			PlayerPlane = { 1000, 1000, 10, 25, 5, 100, 275, 549 };
		}
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
		if (GetAsyncKeyState(VK_SPACE))
			PlayerPlane.BuyEquipment();
		Sleep(15);
	}
	goto choose_mode;
infinite_mode:
	now_mode = 2;
	OperateSystem.tSet(PlayerPlane.GetLevel(2), 2);
	while (!GetAsyncKeyState(VK_ESCAPE))
	{
		SetWorkingImage(&MAP);
		setbkcolor(0xF6FFFF);
		cleardevice();
		PlayerPlane.MovePlayer(PlayerPlane.PlayerOperate());
		if (!OperateSystem.AppearEnemy(PlayerPlane.GetLevel(2), 2) && OperateSystem.empty())
		{
			PlayerPlane.PassLevel(2);
			cleardevice();
			outtextxy(235, 350, "恭喜你通过了这关！");
			outtextxy(220, 400, "按J键继续，按K键退出");
			SetWorkingImage(0);
			putimage(0, 0, &MAP);
			while (1)
			{
				if (GetAsyncKeyState(VK_J))
					goto infinite_mode;
				else if (GetAsyncKeyState(VK_K))
					goto choose_mode;
			}
		}
		if (PlayerPlane.ifBeKilled())
		{
			PlayerPlane = { 1000, 1000, 10, 25, 5, 100, 275, 549 };
		}
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
		if (GetAsyncKeyState(VK_SPACE))
			PlayerPlane.BuyEquipment();
		Sleep(15);
	}
	goto choose_mode;
HelpPage:
	SetWorkingImage(0);
	setbkcolor(0xF6FFFF);
	cleardevice();
	putimage(0, 0, &HelpPage);
	while (!GetAsyncKeyState(VK_ESCAPE))
	{
	}
	goto choose_mode;
quit_game:
	closegraph();
}