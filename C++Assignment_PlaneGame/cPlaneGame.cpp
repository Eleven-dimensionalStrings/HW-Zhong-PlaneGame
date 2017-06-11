#include "cPlaneGame.h"
#include<cassert>
using namespace std;
std::default_random_engine en(int(time(0)));
std::uniform_real_distribution<double>Ranf(0, 0.2);
std::uniform_int_distribution<int>RanPos(0, 24);
std::uniform_int_distribution<int>RanDirx(-2, 2);
std::uniform_int_distribution<int>RanDiry(0, 2);
std::uniform_int_distribution<int>RanTime(60, 180);
std::uniform_int_distribution<int>RanKind(0, 100);
const int EqPr[4][3]{ 60,120,500,50,100,450,30,60,200,45,150,600 };
player::player(int thp, int tenergy, int tr, int tatk, int tdef,
	int tcoin, int tx, int ty) :infinite_level(1),
	hp(thp), energy(tenergy), rate(tr), count(0), atk(tatk), def(tdef), coin(tcoin), skill_flag(0),
	x(tx), y(ty), level(1), steel_hp_persent(5), steel_sp_persent(3), eq{ 0,0,0,0,0,0 }
{
}

player::player(string data)
{
	if (data.size() == sizeof(player))
	{
		for (int i = 0; i < sizeof(player); ++i)
		{
			reinterpret_cast<char*>(this)[i] = data[i];
		}
	}
	else
		throw string("This string's length is incorrect to initialize a class 'player'\n");
}

void player::beAttacked(int a)
{
	if (!skill_flag)
		hp -= (a > GetDef() ? a - GetDef() : 1);
}

void player::beCrash(int a_hp)
{
	hp -= a_hp / GetAtk();
}

pair<int, int> player::PlayerOperate(void)
{
	skill_flag = 0;
	int  ud = 0, lr = 0;
	ud = bool(GetAsyncKeyState(VK_S)) - bool(GetAsyncKeyState(VK_W));
	lr = bool(GetAsyncKeyState(VK_D)) - bool(GetAsyncKeyState(VK_A));
	if (GetAsyncKeyState(VK_K) && energy > 0)
	{
		--energy;
		skill_flag = 1;
	}
	if (GetAsyncKeyState(0x50))
		_getch();
	if (ud && lr)
		return pair<int, int>(ud * 2, lr * 2);
	else
		return pair<int, int>(ud * 3, lr * 3);
	//直走时速度为每帧3像素，斜走为2像素
}

void player::MovePlayer(pair<int, int>dir)
{
	if (y + dir.first >= 0 && y + dir.first < 550)
		y += dir.first;
	if (x + dir.second >= 0 && x + dir.second < 550)
		x += dir.second;
}

std::pair<int, int> player::GetPos(void) const
{
	return std::pair<int, int>(x, y);
}

int player::GetDef(void) const
{
	int d = def;
	for (int i = 0; i < 6; ++i)
	{
		if (eq[i] == 21)
			d += 5;
		if (eq[i] == 22)
			d += 10;
		if (eq[i] == 23)
			d += 30;
	}
	return d;
}

bool player::ifFire(void)
{
	if (GetAsyncKeyState(VK_J))
		++count;
	if (count >= GetFireRate() && energy > 0)
		return true;
	return false;
}

std::vector<string> player::GetData(void) const
{
	vector<string>data;
	int digit = 1000000000, ihp = 0, ienergy = 0, uhp = hp, uenergy = energy, ucoin = coin, icoin = 0,
		uatk = GetAtk(), iatk = 0, udef = GetDef(), idef = 0, usteelhp = GetSteelHpPersent(),
		isteelhp = 0, usteelsp = GetSteelSpPersent(), isteelsp = 0, ulv = level, ilv = 0, uinflv = infinite_level, iinflv = 0;
	string shp = "", senergy = "", scoin = "", satk = "", sdef = "", ssteelhp = "", ssteelsp = "", slv = "", sinflv = "";
	for (int i = 0; i < 10; ++i)
	{
		if (uhp / digit)
			++ihp;
		if (uenergy / digit)
			++ienergy;
		if (ucoin / digit)
			++icoin;
		if (uatk / digit)
			++iatk;
		if (udef / digit)
			++idef;
		if (usteelhp / digit)
			++isteelhp;
		if (usteelsp / digit)
			++isteelsp;
		if (ulv / digit)
			++ilv;
		if (uinflv / digit)
			++iinflv;
		if (ihp)
		{
			shp += char((uhp / digit) + 48);
			uhp -= (uhp / digit)*digit;
		}
		if (ienergy)
		{
			senergy += char((uenergy / digit) + 48);
			uenergy -= (uenergy / digit)*digit;
		}
		if (icoin)
		{
			scoin += char((ucoin / digit) + 48);
			ucoin -= (ucoin / digit)*digit;
		}
		if (iatk)
		{
			satk += char((uatk / digit) + 48);
			uatk -= (uatk / digit)*digit;
		}
		if (idef)
		{
			sdef += char((udef / digit) + 48);
			udef -= (udef / digit)*digit;
		}
		if (isteelhp)
		{
			ssteelhp += char((usteelhp / digit) + 48);
			usteelhp -= (usteelhp / digit)*digit;
		}
		if (isteelsp)
		{
			ssteelsp += char((usteelsp / digit) + 48);
			usteelsp -= (usteelsp / digit)*digit;
		}
		if (ilv)
		{
			slv += char((ulv / digit) + 48);
			ulv -= (ulv / digit)*digit;
		}
		if (iinflv)
		{
			sinflv += char((uinflv / digit) + 48);
			uinflv -= (uinflv / digit)*digit;
		}
		digit /= 10;
	}
	shp += '\0';
	senergy += '\0';
	data.push_back(shp);
	data.push_back(senergy);
	data.push_back(scoin);
	data.push_back(satk);
	data.push_back(sdef);
	data.push_back(ssteelhp);
	data.push_back(ssteelsp);
	data.push_back(slv);
	data.push_back(sinflv);
	return data;
}

void player::GainCoin(int c)
{
	coin += c;
}

std::string player::ReturnInformation(void) const
{
	return reinterpret_cast<const char*>(this);
}

bool player::ifBeKilled(void) const
{
	if (hp <= 0)
		return 1;
	return false;
}

void player::BuyEquipment(void)
{
	MOUSEMSG click;
	int choose = 1, bag = 0, ex = 0;
	IMAGE screen{ 600,800 };
	IMAGE eq01, eq02, eq03, eq11, eq12, eq13, eq21, eq22, eq23, eq31, eq32, eq33, buy, sell, exit;
	loadimage(&eq02, "2.bmp", 85, 64, 0);
	loadimage(&eq01, "1.bmp", 85, 64, 0);
	loadimage(&eq03, "3.bmp", 85, 64, 0);
	loadimage(&eq11, "11.bmp", 85, 64, 0);
	loadimage(&eq12, "12.bmp", 85, 64, 0);
	loadimage(&eq13, "13.bmp", 85, 64, 0);
	loadimage(&eq21, "21.bmp", 85, 64, 0);
	loadimage(&eq22, "22.bmp", 85, 64, 0);
	loadimage(&eq23, "23.bmp", 85, 64, 0);
	loadimage(&eq31, "31.bmp", 85, 64, 0);
	loadimage(&eq32, "32.bmp", 85, 64, 0);
	loadimage(&eq33, "33.bmp", 85, 64, 0);
	loadimage(&buy, "buy.bmp", 85, 64, 0);
	loadimage(&sell, "sell.bmp", 85, 64, 0);
	loadimage(&exit, "exit.bmp", 85, 64, 0);
	while (!ex)
	{
		SetWorkingImage(&screen);
		setbkcolor(0xF6FFFF);
		cleardevice();
		click = GetMouseMsg();
		if (click.mkLButton)
		{
			for (int x = 50; x <= 250; x += 100)
			{
				for (int y = 50; y <= 410; y += 120)
				{
					if (click.x >= x && click.x < x + 85 && click.y >= y && click.y < y + 64)
					{
						choose = (x - 50) / 100 + ((10 * (y - 50)) / 120) + 1;
					}
				}
			}
			for (int i = 1; i <= 6; ++i)
			{
				if (click.x >= i * 100 - 90 && click.x < i * 100 - 5 && click.y >= 550 && click.y < 614)
					bag = i - 1;
			}
			if (click.x >= 420 && click.x < 505 && click.y >= 680 && click.y < 744)
				ex = 1;
			if (click.x >= 60 && click.x < 145 && click.y >= 680 && click.y < 744)
			{
				if (eq[bag] == 0 && choose % 10 == 1)
				{
					if (coin >= EqPr[choose / 10][(choose % 10) - 1])
					{
						coin -= EqPr[choose / 10][(choose % 10) - 1];
						eq[bag] = choose;
					}
				}
				if (eq[bag] + 1 == choose && coin >= EqPr[choose / 10][(choose % 10) - 1])
				{
					coin -= EqPr[choose / 10][(choose % 10) - 1];
					eq[bag] = choose;
				}
			}
			if (click.x >= 240 && click.x < 325 && click.y >= 680 && click.y < 744 && eq[bag] != 0)
			{
				coin += EqPr[eq[bag] / 10][(eq[bag] % 10) - 1];
				eq[bag] = 0;
			}
		}
		putimage(50, 50, &eq01);
		putimage(150, 50, &eq02);
		putimage(250, 50, &eq03);
		putimage(50, 170, &eq11);
		putimage(150, 170, &eq12);
		putimage(250, 170, &eq13);
		putimage(50, 290, &eq21);
		putimage(150, 290, &eq22);
		putimage(250, 290, &eq23);
		putimage(50, 410, &eq31);
		putimage(150, 410, &eq32);
		putimage(250, 410, &eq33);
		putimage(60, 680, &buy);
		putimage(240, 680, &sell);
		putimage(420, 680, &exit);
		setlinecolor(BLACK);
		rectangle(((choose % 10) - 1) * 100 + 45, (choose / 10) * 120 + 45, ((choose % 10) - 1) * 100 + 140, (choose / 10) * 120 + 119);
		rectangle(bag * 100 + 5, 545, bag * 100 + 100, 619);
		setlinecolor(BLUE);
		rectangle(10, 550, 95, 614);
		rectangle(110, 550, 195, 614);
		rectangle(210, 550, 295, 614);
		rectangle(310, 550, 395, 614);
		rectangle(410, 550, 495, 614);
		rectangle(510, 550, 595, 614);
		for (int i = 0; i < 6; ++i)
		{
			if (eq[i] == 1)
			{
				putimage(100 * i + 10, 550, &eq01);
			}
			if (eq[i] == 2)
			{
				putimage(100 * i + 10, 550, &eq02);
			}
			if (eq[i] == 3)
			{
				putimage(100 * i + 10, 550, &eq03);
			}
			if (eq[i] == 11)
			{
				putimage(100 * i + 10, 550, &eq11);
			}
			if (eq[i] == 12)
			{
				putimage(100 * i + 10, 550, &eq12);
			}
			if (eq[i] == 13)
			{
				putimage(100 * i + 10, 550, &eq13);
			}
			if (eq[i] == 21)
			{
				putimage(100 * i + 10, 550, &eq21);
			}
			if (eq[i] == 22)
			{
				putimage(100 * i + 10, 550, &eq22);
			}
			if (eq[i] == 23)
			{
				putimage(100 * i + 10, 550, &eq23);
			}
			if (eq[i] == 31)
			{
				putimage(100 * i + 10, 550, &eq31);
			}
			if (eq[i] == 32)
			{
				putimage(100 * i + 10, 550, &eq32);
			}
			if (eq[i] == 33)
			{
				putimage(100 * i + 10, 550, &eq33);
			}
		}
		vector<string>data(GetData());
		settextcolor(BLACK);
		string scoin = data[2];
		data[5] += "%";
		data[6] += "%";
		outtextxy(400, 325, "HP:");
		outtextxy(400, 350, "MP:");
		outtextxy(400, 375, "COIN:");
		outtextxy(400, 400, "ATK:");
		outtextxy(400, 425, "DEF:");
		outtextxy(400, 450, "S_HP:");
		outtextxy(400, 475, "S_SP:");
		outtextxy(450, 325, &data[0][0]);
		outtextxy(450, 350, &data[1][0]);
		outtextxy(450, 375, &data[2][0]);
		outtextxy(450, 400, &data[3][0]);
		outtextxy(450, 425, &data[4][0]);
		outtextxy(450, 450, &data[5][0]);
		outtextxy(450, 475, &data[6][0]);
		switch (choose)
		{
		case 1:
			outtextxy(350, 50, "                 秘银锤");
			outtextxy(350, 100, "攻击力 + 15");
			outtextxy(350, 150, "攻速 + 0.08");
			outtextxy(350, 200, "售价: 60");
			outtextxy(350, 250, "无购买条件");
			break;
		case 2:
			outtextxy(350, 50, "                 漩涡之锤");
			outtextxy(350, 100, "攻击力 + 25");
			outtextxy(350, 150, "攻速 + 0.13");
			outtextxy(350, 200, "售价: 120");
			outtextxy(350, 250, "选定装备栏为秘银锤方可购买");
			break;
		case 3:
			outtextxy(350, 50, "                 雷神之锤");
			outtextxy(350, 100, "攻击力 + 80");
			outtextxy(350, 150, "攻速 + 0.25");
			outtextxy(350, 200, "售价: 500");
			outtextxy(350, 250, "选定装备栏为漩涡之锤方可购买");
			break;
		case 11:
			outtextxy(350, 50, "                 吸血面具");
			outtextxy(350, 100, "生命吸取 + 0.10");
			outtextxy(350, 150, "售价: 50");
			outtextxy(350, 200, "无购买条件");
			break;
		case 12:
			outtextxy(350, 50, "                 疯狂面具");
			outtextxy(350, 100, "生命吸取 + 0.15");
			outtextxy(350, 150, "售价: 100");
			outtextxy(350, 200, "选定装备栏为吸血面具方可购买");
			break;
		case 13:
			outtextxy(350, 50, "                 撒旦之邪力");
			outtextxy(350, 100, "生命吸取 + 0.3");
			outtextxy(350, 150, "售价: 450");
			outtextxy(350, 200, "选定装备栏为疯狂面具方可购买");
			break;
		case 21:
			outtextxy(350, 50, "                 锁子甲");
			outtextxy(350, 100, "护甲 + 5");
			outtextxy(350, 150, "售价: 30");
			outtextxy(350, 200, "无购买条件");
			break;
		case 22:
			outtextxy(350, 50, "                 刃甲");
			outtextxy(350, 100, "护甲 + 10");
			outtextxy(350, 150, "售价: 60");
			outtextxy(350, 200, "选定装备栏为锁子甲方可购买");
			break;
		case 23:
			outtextxy(350, 50, "                 强袭胸甲");
			outtextxy(350, 100, "护甲 + 30");
			outtextxy(350, 150, "售价: 200");
			outtextxy(350, 200, "选定装备栏为刃甲方可购买");
			break;
		case 31:
			outtextxy(350, 50, "                 欢欣之刃");
			outtextxy(350, 100, "法力吸取 + 0.08");
			outtextxy(350, 150, "售价: 45");
			outtextxy(350, 200, "无购买条件");
			break;
		case 32:
			outtextxy(350, 50, "                 净魂之刃");
			outtextxy(350, 100, "法力吸取 + 0.12");
			outtextxy(350, 150, "售价: 150");
			outtextxy(350, 200, "选定装备栏为欢欣之刃方可购买");
			break;
		case 33:
			outtextxy(350, 50, "                 高级净魂之刃");
			outtextxy(350, 100, "法力吸取 + 0.16");
			outtextxy(350, 150, "售价: 600");
			outtextxy(350, 200, "选定装备栏为净魂之刃方可购买");
			break;
		default:
			break;
		}
		outtextxy(350, 280, "物品出售时获得原价金币");
		outtextxy(350, 300, "但是无法获得前置物品的金币");
		SetWorkingImage(0);
		putimage(0, 0, &screen);
	}
}

int player::GetSteelHpPersent(void) const
{
	int st = steel_hp_persent;
	for (int i = 0; i < 6; ++i)
	{
		if (eq[i] == 11)
			st += 10;
		if (eq[i] == 12)
			st += 15;
		if (eq[i] == 13)
			st += 30;
	}
	return st;
}

int player::GetSteelSpPersent(void) const
{
	int st = steel_sp_persent;
	for (int i = 0; i < 6; ++i)
	{
		if (eq[i] == 31)
			st += 8;
		if (eq[i] == 32)
			st += 12;
		if (eq[i] == 33)
			st += 16;
	}
	return st;
}

void player::SteelHpAndSp(int dhp)
{
	hp += dhp*GetSteelHpPersent() / 100;
	energy += dhp*GetSteelSpPersent() / 100;
}

int player::GetFireRate(void) const
{
	int r = rate;
	double a = 1;
	for (int i = 0; i < 6; ++i)
	{
		if (eq[i] == 1)
			a += 0.08;
		if (eq[i] == 2)
			a += 0.13;
		if (eq[i] == 3)
			a += 0.25;
	}
	r = int(double(r) / a);
	return r;
}

int player::GetAtk(void) const
{
	int a = atk;
	for (int i = 0; i < 6; ++i)
	{
		if (eq[i] == 1)
			a += 15;
		if (eq[i] == 2)
			a += 25;
		if (eq[i] == 3)
			a += 80;
	}
	return a;
}

int player::GetLevel(int kind) const
{
	return (kind == 1 ? level : infinite_level);
}

void player::PassLevel(int kind)
{
	if (kind == 1)
	{
		coin += level * 100;
		++level;
	}
	else
	{
		++infinite_level;
		coin += infinite_level * 20;
	}
}

player_bullet & player::Fire(void)
{
	--energy;
	count = 0;
	player_bullet * t = new player_bullet(GetAtk(), x + PLAYER / 2, y - 7);
	return *t;
}

bool enemy::GetFlag(int f) const
{
	switch (f)
	{
	case 1:
		return move_list_flag;
	case 2:
		return fire_list_flag;
	default:
		return 0;
	}
	return false;
}

void enemy::SetLength(int tL)
{
	enemy_length = tL;
}

int enemy::GetLength(void) const
{
	return enemy_length;
}

void enemy::SetBulletRadio(int r)
{
	bullet_radio = r;
}

void enemy::Draw(void) const
{
	int l_hp = (hp  * enemy_length) / total_hp;
	setfillcolor(0x00FF00);
	solidrectangle(x, y, x + ENEMY, y + ENEMY);
	setfillcolor(0x0000FF);
	solidrectangle(x, y - 7, x + l_hp, y - 2);
}

std::pair<int, int> enemy::GetFireSpeed(void) const
{
	return std::pair<int, int>(fire_speed_x, fire_speed_y);
}

int enemy::GetBulletRadio(void) const
{
	return bullet_radio;
}

std::pair<int, int> enemy::GetSpeed(void) const
{
	return std::pair<int, int>(move_speed_x, move_speed_y);
}

bool enemy::countSkill(void)
{
	return false;
}

enemy::enemy(int thp, int tatk, int tdef, int tcoin, int tx, int ty,
	int tatkr, int tdir_x, int tdir_y, int tfire_speed_x, int tfire_speed_y, int tfire_count) :
	hp(thp), atk(tatk), def(tdef), coin(tcoin), x(tx), y(ty), fire_rate(tatkr), move_speed_x(tdir_x),
	move_speed_y(tdir_y), fire_speed_x(tfire_speed_x), fire_speed_y(tfire_speed_y),
	fire_cd(tfire_count), total_hp(thp), move_count(0), fire_count(0), enemy_length(ENEMY),
	fire_list_flag(0), move_list_flag(0), move_frame(1), now_frame(1), bullet_radio(BULLET)
{
}

void enemy::move(void)
{
	if (ifLeftRightLimit())
		move_speed_x *= -1;
	++now_frame;
	if (now_frame >= move_frame)
	{
		x += move_speed_x;
		y += move_speed_y;
	}
}

bool enemy::ifBeAttacked(int bx, int by)const
{
	if (bx >= x - BULLET && bx <= x + enemy_length + BULLET && by <= y + enemy_length + BULLET && by >= y - BULLET)
		return 1;
	else
		return 0;
}

int enemy::beAttacked(int patk)
{
	int st = 0;
	st = (patk > def ? patk - def : 1);
	if (st > hp)
		st = hp;
	hp -= (patk > def ? patk - def : 1);
	return st;
}

bool enemy::countFire(void)
{
	++fire_cd;
	if (fire_cd >= fire_rate)
	{
		fire_cd = 0;
		return 1;
	}
	return 0;
}

enemy_bullet & enemy::Fire(void)
{
	enemy_bullet * teb = new enemy_bullet(atk, x + enemy_length / 2, y + 25 + bullet_radio, fire_speed_x, fire_speed_y);
	teb->SetRadio(GetBulletRadio());
	return *teb;
}

bool enemy::ifKill(void)const
{
	if (hp <= 0)
		return 1;
	else
		return 0;
}

bool enemy::ifLeftRightLimit(void) const
{
	if ((x > 600 - GetLength() && move_speed_x > 0) || (x < 0 && move_speed_x < 0))
		return 1;
	else
		return 0;
}

bool enemy::ifDownLimit(void) const
{
	if (y >= 600 - GetLength())
		return 1;
	else
		return 0;
}

pair<int, int> enemy::GetPos(void)const
{
	return pair<int, int>(x, y);
}

int enemy::GetHpPercent(void) const
{
	return 100 * hp / total_hp;
}

int enemy::GetAtk(void) const
{
	return atk;
}

int enemy::GetDef(void) const
{
	return def;
}

int enemy::GetHp(void) const
{
	return hp;
}

int enemy::GetCoin(void) const
{
	return coin;
}

bool enemy::CheckCrash(pair<int, int> pos) const
{
	if (x >= pos.first - GetLength() && x <= pos.first + 50 && y >= pos.second - GetLength() && y <= pos.second + 50)
		return 1;
	else
		return 0;
}

void enemy::SetMoveList(std::vector<std::array<int, 3>> tmove_list)
{
	move_list = tmove_list;
	move_step = move_list.begin();
	move_count = 0;
	move_list_flag = 1;
	move_speed_x = (*move_step)[0];
	move_speed_y = (*move_step)[1];
}

void enemy::SetFireList(std::vector<std::array<int, 3>> tfire_list)
{
	fire_list = tfire_list;
	fire_step = fire_list.begin();
	fire_count = 0;
	fire_list_flag = 1;
	fire_speed_x = (*fire_step)[0];
}

void enemy::ChangeFireState(void)
{
	++fire_count;
	if (fire_count >= (*fire_step)[2])
	{
		fire_count = 0;
		++fire_step;
#ifndef DEBUG
		assert(fire_step >= fire_list.begin() && fire_step < fire_list.end());
#endif // !DEBUG
		fire_speed_x = (*fire_step)[0];
		fire_speed_y = (*fire_step)[1];
	}
}

void enemy::ChangeMoveState(void)
{
	++move_count;
	if (move_count >= (*move_step)[2])
	{
		++move_step;
		assert(move_step >= move_list.begin() && move_step < move_list.end());
		move_count = 0;
		move_speed_x = (*move_step)[0];
		move_speed_y = (*move_step)[1];
	}
}

void enemy::UseSkill(player&, operate_system&, int num)
{
}

void enemy::ModifyHp(int a)
{
	hp += a;
}

int enemy::GetSkillKind(int num) const
{
	return 0;
}

int enemy::GetSkillQuantity(void) const
{
	return 0;
}

void enemy::SetFrame(int tframe)
{
	move_frame = tframe;
}

bool enemy::CheckDisapear(void)const
{
	if (x < -5 || y < -100 || x>600 || y>575)
		return 1;
	return 0;
}

operate_system::operate_system() :appear_enemy_count(0)
{
}

void operate_system::CreateEnemy(enemy& tenemy)
{
	EnemyList.push_back(&tenemy);
}

void operate_system::CreateEnemyBullet(enemy_bullet & teb)
{
	EnemyBulletList.push_back(&teb);
}

void operate_system::CreatePlayerBullet(player_bullet & tpb)
{
	PlayerBulletList.push_back(&tpb);
}

void operate_system::MoveEnemy(void)
{
	list<enemy*>::iterator i;
	for (i = EnemyList.begin(); i != EnemyList.end(); ++i)
	{
		if ((**i).GetFlag(1))
			(**i).ChangeMoveState();
		(**i).move();
	}
}

player_bullet::player_bullet(int ta, int tx, int ty) :atk(ta), x(tx), y(ty), speed(4), frame(1), now_frame(1), radio(BULLET)
{
}

std::pair<int, int> player_bullet::GetPos(void) const
{
	return std::pair<int, int>(x, y);
}

int player_bullet::GetAtk(void) const
{
	return atk;
}

void player_bullet::move(void)
{
	++now_frame;
	if (now_frame >= frame)
	{
		now_frame = 0;
		y -= speed;
	}
}

bool player_bullet::CheckDisapear(void)
{
	if (y < 0)
		return 1;
	return 0;
}

std::pair<int, int> player_bullet::GetPos(void)
{
	return std::pair<int, int>(x, y);
}

void player_bullet::SetSpeed(int tspeed, int tframe)
{
	speed = tspeed;
	frame = tframe;
}

int player_bullet::GetRadio(void) const
{
	return radio;
}

void player_bullet::SetRadio(int tR)
{
	radio = tR;
}

void player_bullet::draw(void) const
{
	solidcircle(x, y, radio);
}

enemy_bullet::enemy_bullet(int ta, int tx, int ty, int tspeedx, int tspeedy) :
	atk(ta), x(tx), y(ty), speed_x(tspeedx), speed_y(tspeedy), frame(1), now_frame(1), radio(BULLET)
{
}

bool enemy_bullet::CheckDisapear(void)
{
	if (x < 0 || x>600 || y < 0 || y>600)
		return 1;
	return 0;
}

void enemy_bullet::Move(player&)
{
	++now_frame;
	if (now_frame >= frame)
	{
		x += speed_x;
		y += speed_y;
		now_frame = 0;
	}
}

pair<int, int> enemy_bullet::GetPos(void) const
{
	return pair<int, int>(x, y);
}

int enemy_bullet::GetAtk(void) const
{
	return atk;
}

void enemy_bullet::SetSpeed(int tspeedx, int tspeedy)
{
	speed_x = tspeedx;
	speed_y = tspeedy;
}

int enemy_bullet::GetRadio(void) const
{
	return radio;
}

void enemy_bullet::SetRadio(int tR)
{
	radio = tR;
}

bool enemy_bullet::ifAttack(pair<int, int> pos) const
{
	if (x > pos.first - radio && x<pos.first + PLAYER + radio && y>pos.second - radio && y < pos.second + PLAYER + radio)
		return 1;
	return false;
}

void enemy_bullet::Draw(void) const
{
	solidcircle(x, y, radio);
}

int enemy_bullet::GetX(void) const
{
	return x;
}

int enemy_bullet::GetY(void) const
{
	return y;
}

void enemy_bullet::SetFrame(int tframe)
{
	frame = tframe;
}

int enemy_bullet::GetFrame(void) const
{
	return frame;
}

void enemy_bullet::ModifyNowFrame(int n)
{
	if (n)
		++now_frame;
	else
		now_frame = 0;
}

std::pair<int, int> enemy_bullet::GetSpeed(void) const
{
	return std::pair<int, int>(speed_x, speed_y);
}

void enemy_bullet::ModifyPos(std::pair<int, int> speed)
{
	x += speed_x;
	y += speed_y;
}

int enemy_bullet::GetNowFrame(void) const
{
	return now_frame;
}


void operate_system::DrawToScreen(pair<int, int>plp, vector<string>player_data)
{
	//setlinecolor()
	for (list<enemy*>::iterator i = EnemyList.begin(); i != EnemyList.end(); ++i)
	{
		(**i).Draw();
	}
	setfillcolor(LIGHTBLUE);
	fillrectangle(plp.first, plp.second, plp.first + PLAYER, plp.second + PLAYER);
	setfillcolor(LIGHTCYAN);
	pair<int, int>pos;
	for (list<player_bullet*>::iterator i = PlayerBulletList.begin(); i != PlayerBulletList.end(); ++i)
	{
		(**i).draw();
	}
	list<enemy_bullet*>::iterator i;
	setfillcolor(0xAA00AA);
	for (i = EnemyBulletList.begin(); i != EnemyBulletList.end(); ++i)
	{
		(**i).Draw();
	}
	settextcolor(BLACK);
	outtextxy(0, 620, "HP:");
	outtextxy(0, 645, "MP:");
	outtextxy(0, 670, "COIN:");
	outtextxy(0, 695, "ATK:");
	outtextxy(0, 720, "DEF:");
	outtextxy(0, 745, "S_HP:");
	outtextxy(0, 770, "S_SP:");
	outtextxy(300, 650, "LEVEL:");
	outtextxy(300, 700, "INFINITE LEVEL:");
	outtextxy(50, 620, &player_data[0][0]);
	outtextxy(50, 645, &player_data[1][0]);
	outtextxy(50, 670, &player_data[2][0]);
	outtextxy(50, 695, &player_data[3][0]);
	outtextxy(50, 720, &player_data[4][0]);
	outtextxy(50, 745, &player_data[5][0]);
	outtextxy(50, 770, &player_data[6][0]);
	outtextxy(450, 650, &player_data[7][0]);
	outtextxy(450, 700, &player_data[8][0]);
}

int operate_system::CheckCrash(std::pair<int, int>pos)
{
	list<enemy*>::iterator i;
	int px = pos.first, py = pos.second, ex = 0, ey = 0, mhp = 0;
	for (i = EnemyList.begin(); i != EnemyList.end();)
	{
		if ((**i).CheckCrash(pos))
		{
			mhp += (**i).GetAtk()*(**i).GetHp();
			delete *i;
			EnemyList.erase(i++);
		}
		else
			++i;
	}
	return mhp;
}

void operate_system::CheckDisapear(void)
{
	list<enemy*>::iterator i;
	for (i = EnemyList.begin(); i != EnemyList.end();)
	{
		if ((**i).CheckDisapear())
		{
			delete *i;
			EnemyList.erase(i++);
		}
		else
			++i;
	}
}

int operate_system::CheckKill(void)
{
	list<enemy*>::iterator i;
	int coin = 0;
	for (i = EnemyList.begin(); i != EnemyList.end();)
	{
		if ((**i).ifKill())
		{
			coin += (**i).GetCoin();
			delete *i;
			EnemyList.erase(i++);
		}
		else
			++i;
	}
	return coin;
}

void operate_system::PlayerAttackEnemy(player& plane)
{
	list<enemy*>::iterator i;
	list<player_bullet*>::iterator j;
	for (i = EnemyList.begin(); i != EnemyList.end(); ++i)
	{
		for (j = PlayerBulletList.begin(); j != PlayerBulletList.end(); )
		{
			if ((**i).ifBeAttacked((**j).GetPos().first, (**j).GetPos().second))
			{
				plane.SteelHpAndSp((**i).beAttacked((**j).GetAtk()));
				delete *j;
				PlayerBulletList.erase(j++);
			}
			else
				++j;
		}
	}
}

void operate_system::MovePlayerBullet(void)
{
	list<player_bullet*>::iterator i;
	for (i = PlayerBulletList.begin(); i != PlayerBulletList.end(); ++i)
	{
		(**i).move();
	}
}

void operate_system::DisappearPlayerBullet(void)
{
	list<player_bullet*>::iterator i;
	for (i = PlayerBulletList.begin(); i != PlayerBulletList.end(); )
	{
		if ((**i).GetPos().second < 0)
		{
			delete *i;
			PlayerBulletList.erase(i++);
		}
		else
			++i;
	}
}

void operate_system::MoveEnemyBullet(player& pl)
{
	list<enemy_bullet*>::iterator i;
	for (i = EnemyBulletList.begin(); i != EnemyBulletList.end(); ++i)
	{
		(**i).Move(pl);
	}
}

void operate_system::EnemyFire(void)
{
	list<enemy*>::iterator i;
	for (i = EnemyList.begin(); i != EnemyList.end(); ++i)
	{
		if ((**i).countFire())
		{
			if ((**i).GetFlag(2))
			{
				(**i).ChangeFireState();
			}
			CreateEnemyBullet((**i).Fire());
		}
	}
}

void operate_system::EnemyAttackPlayer(player& plane)
{
	list<enemy_bullet*>::iterator i;
	for (i = EnemyBulletList.begin(); i != EnemyBulletList.end();)
	{
		if ((*i)->ifAttack(plane.GetPos()))
		{
			plane.beAttacked((**i).GetAtk());
			delete *i;
			EnemyBulletList.erase(i++);
		}
		else
			++i;
	}
}

void operate_system::DisappearEnemyBullet(void)
{
	list<enemy_bullet*>::iterator i;
	for (i = EnemyBulletList.begin(); i != EnemyBulletList.end();)
	{
		if ((**i).CheckDisapear())
		{
			delete *i;
			EnemyBulletList.erase(i++);
		}
		else
			++i;
	}
}

void operate_system::SetAppearEnemyList(std::vector<std::array<int, 2>> tEnemyAppearList,
	std::vector<std::vector<std::array<int, 3>>>tAppearEnemyMoveList,
	std::vector<std::vector<std::array<int, 3>>>tAppearEnemyFireList,
	std::vector<std::array<int, 11>>tAppearEnemyAttribution)
{
	appear_enemy_list = tEnemyAppearList;
	appear_enemy_step = appear_enemy_list.begin();
	appear_enemy_move_list = tAppearEnemyMoveList;
	appear_enemy_move_inf = appear_enemy_move_list.begin();
	appear_enemy_fire_list = tAppearEnemyFireList;
	appear_enemy_fire_inf = appear_enemy_move_list.begin();
	appear_enemy_attribution = tAppearEnemyAttribution;
	appear_enemy_attribution_inf = appear_enemy_attribution.begin();
	appear_enemy_count = 0;
}

bool operate_system::AppearEnemy(int level, int kind)
{
	if (appear_enemy_step == appear_enemy_list.end())
		return 0;
	++appear_enemy_count;
	kind = (kind == 1 ? 2 : 1);
	int n1 = 0, n2 = 0, n3 = 0, n4 = 0;
	n1 += (kind*level <= 20 ? kind*level : 20);
	n2 += 2 * n1;
	n3 += 3 * n1;
	n4 = n1 * 2 + n3;
	if (appear_enemy_count >= (*appear_enemy_step)[1])
	{
		appear_enemy_count = 0;
		if ((*appear_enemy_step)[0] <= n1)
		{
			if ((*appear_enemy_attribution_inf)[0] != -1)
			{
				enemy* e = new normal_1{ (*appear_enemy_attribution_inf)[0],(*appear_enemy_attribution_inf)[1],
					   (*appear_enemy_attribution_inf)[2], (*appear_enemy_attribution_inf)[3], (*appear_enemy_attribution_inf)[4],
					   (*appear_enemy_attribution_inf)[5], (*appear_enemy_attribution_inf)[6], (*appear_enemy_attribution_inf)[7],
					   (*appear_enemy_attribution_inf)[8], (*appear_enemy_attribution_inf)[9],(*appear_enemy_attribution_inf)[10],0 };
				(*e).SetMoveList(*appear_enemy_move_inf);
				(*e).SetFireList(*appear_enemy_fire_inf);
				(*e).SetBulletRadio(3);
				(*e).SetLength(25);
				this->CreateEnemy(*e);
			}
			else
			{
				enemy* e = new enemy{ (*appear_enemy_attribution_inf)[0],(*appear_enemy_attribution_inf)[1],
					(*appear_enemy_attribution_inf)[2], (*appear_enemy_attribution_inf)[3], (*appear_enemy_attribution_inf)[4],
					(*appear_enemy_attribution_inf)[5], (*appear_enemy_attribution_inf)[6], (*appear_enemy_attribution_inf)[7],
					(*appear_enemy_attribution_inf)[8], (*appear_enemy_attribution_inf)[9],(*appear_enemy_attribution_inf)[10],0 }; (*e).SetMoveList(*appear_enemy_move_inf);
				(*e).SetFireList(*appear_enemy_fire_inf);
				(*e).SetBulletRadio(3);
				(*e).SetLength(25);
				this->CreateEnemy(*e);
			}
		}
		else if ((*appear_enemy_step)[0] <= n2)
		{
			if ((*appear_enemy_attribution_inf)[0] != -1)
			{
				enemy* e = new normal_2{ (*appear_enemy_attribution_inf)[0],(*appear_enemy_attribution_inf)[1],
					(*appear_enemy_attribution_inf)[2], (*appear_enemy_attribution_inf)[3], (*appear_enemy_attribution_inf)[4],
					(*appear_enemy_attribution_inf)[5], (*appear_enemy_attribution_inf)[6], (*appear_enemy_attribution_inf)[7],
					(*appear_enemy_attribution_inf)[8], (*appear_enemy_attribution_inf)[9],(*appear_enemy_attribution_inf)[10],0 }; (*e).SetMoveList(*appear_enemy_move_inf);
				(*e).SetFireList(*appear_enemy_fire_inf);
				(*e).SetBulletRadio(3);
				(*e).SetLength(25);
				this->CreateEnemy(*e);
			}
			else
			{
				enemy* e = new enemy{ (*appear_enemy_attribution_inf)[0],(*appear_enemy_attribution_inf)[1],
					(*appear_enemy_attribution_inf)[2], (*appear_enemy_attribution_inf)[3], (*appear_enemy_attribution_inf)[4],
					(*appear_enemy_attribution_inf)[5], (*appear_enemy_attribution_inf)[6], (*appear_enemy_attribution_inf)[7],
					(*appear_enemy_attribution_inf)[8], (*appear_enemy_attribution_inf)[9],(*appear_enemy_attribution_inf)[10],0 }; (*e).SetMoveList(*appear_enemy_move_inf);
				(*e).SetFireList(*appear_enemy_fire_inf);
				(*e).SetBulletRadio(3);
				(*e).SetLength(25);
				this->CreateEnemy(*e);
			}
		}
		else if ((*appear_enemy_step)[0] <= n3)
		{
			if ((*appear_enemy_attribution_inf)[0] != -1)
			{
				enemy* e = new normal_3{ (*appear_enemy_attribution_inf)[0],(*appear_enemy_attribution_inf)[1],
					(*appear_enemy_attribution_inf)[2], (*appear_enemy_attribution_inf)[3], (*appear_enemy_attribution_inf)[4],
					(*appear_enemy_attribution_inf)[5], (*appear_enemy_attribution_inf)[6], (*appear_enemy_attribution_inf)[7],
					(*appear_enemy_attribution_inf)[8], (*appear_enemy_attribution_inf)[9],(*appear_enemy_attribution_inf)[10],0 }; (*e).SetMoveList(*appear_enemy_move_inf);
				(*e).SetFireList(*appear_enemy_fire_inf);
				(*e).SetBulletRadio(3);
				(*e).SetLength(25);
				this->CreateEnemy(*e);
			}
			else
			{
				enemy* e = new enemy{ (*appear_enemy_attribution_inf)[0],(*appear_enemy_attribution_inf)[1],
					(*appear_enemy_attribution_inf)[2], (*appear_enemy_attribution_inf)[3], (*appear_enemy_attribution_inf)[4],
					(*appear_enemy_attribution_inf)[5], (*appear_enemy_attribution_inf)[6], (*appear_enemy_attribution_inf)[7],
					(*appear_enemy_attribution_inf)[8], (*appear_enemy_attribution_inf)[9],(*appear_enemy_attribution_inf)[10],0 }; (*e).SetMoveList(*appear_enemy_move_inf);
				(*e).SetFireList(*appear_enemy_fire_inf);
				(*e).SetBulletRadio(3);
				(*e).SetLength(25);
				this->CreateEnemy(*e);
			}
		}
		else if ((*appear_enemy_step)[0] <= n4)
		{
			if ((*appear_enemy_attribution_inf)[0] != -1)
			{
				enemy* e = new normal_4{ (*appear_enemy_attribution_inf)[0],(*appear_enemy_attribution_inf)[1],
					(*appear_enemy_attribution_inf)[2], (*appear_enemy_attribution_inf)[3], (*appear_enemy_attribution_inf)[4],
					(*appear_enemy_attribution_inf)[5], (*appear_enemy_attribution_inf)[6], (*appear_enemy_attribution_inf)[7],
					(*appear_enemy_attribution_inf)[8], (*appear_enemy_attribution_inf)[9],(*appear_enemy_attribution_inf)[10],0 }; (*e).SetMoveList(*appear_enemy_move_inf);
				(*e).SetFireList(*appear_enemy_fire_inf);
				(*e).SetBulletRadio(3);
				(*e).SetLength(25);
				this->CreateEnemy(*e);
			}
			else
			{
				enemy* e = new enemy{ (*appear_enemy_attribution_inf)[0],(*appear_enemy_attribution_inf)[1],
					(*appear_enemy_attribution_inf)[2], (*appear_enemy_attribution_inf)[3], (*appear_enemy_attribution_inf)[4],
					(*appear_enemy_attribution_inf)[5], (*appear_enemy_attribution_inf)[6], (*appear_enemy_attribution_inf)[7],
					(*appear_enemy_attribution_inf)[8], (*appear_enemy_attribution_inf)[9],(*appear_enemy_attribution_inf)[10],0 }; (*e).SetMoveList(*appear_enemy_move_inf);
				(*e).SetFireList(*appear_enemy_fire_inf);
				(*e).SetBulletRadio(3);
				(*e).SetLength(25);
				this->CreateEnemy(*e);
			}
		}
		else
		{
			if ((*appear_enemy_attribution_inf)[0] != -1)
			{
				enemy* e = new enemy{ (*appear_enemy_attribution_inf)[0],(*appear_enemy_attribution_inf)[1],
					(*appear_enemy_attribution_inf)[2], (*appear_enemy_attribution_inf)[3], (*appear_enemy_attribution_inf)[4],
					(*appear_enemy_attribution_inf)[5], (*appear_enemy_attribution_inf)[6], (*appear_enemy_attribution_inf)[7],
					(*appear_enemy_attribution_inf)[8], (*appear_enemy_attribution_inf)[9],(*appear_enemy_attribution_inf)[10],0 }; (*e).SetMoveList(*appear_enemy_move_inf);
				(*e).SetFireList(*appear_enemy_fire_inf);
				(*e).SetBulletRadio(3);
				(*e).SetLength(25);
				this->CreateEnemy(*e);
			}
			else
			{
				enemy* e = new enemy{ (*appear_enemy_attribution_inf)[0],(*appear_enemy_attribution_inf)[1],
					(*appear_enemy_attribution_inf)[2], (*appear_enemy_attribution_inf)[3], (*appear_enemy_attribution_inf)[4],
					(*appear_enemy_attribution_inf)[5], (*appear_enemy_attribution_inf)[6], (*appear_enemy_attribution_inf)[7],
					(*appear_enemy_attribution_inf)[8], (*appear_enemy_attribution_inf)[9],(*appear_enemy_attribution_inf)[10],0 }; (*e).SetMoveList(*appear_enemy_move_inf);
				(*e).SetFireList(*appear_enemy_fire_inf);
				(*e).SetBulletRadio(3);
				(*e).SetLength(25);
				this->CreateEnemy(*e);
			}
		}
		if ((appear_enemy_step) != appear_enemy_list.end())
		{
			++appear_enemy_step;
			++appear_enemy_fire_inf;
			++appear_enemy_attribution_inf;
			++appear_enemy_move_inf;
		}
		else
		{
			return 0;
		}
	}
	return 1;
}

void operate_system::tSet(int level, int kind)
{
	vector<array<int, 2>>s;
	vector<vector<array<int, 3>>>f;
	vector<array<int, 11>>d;
	vector<vector<array<int, 3>>>m;
	int num = 50;
	if (kind == 1)
		num = level * 30;
	for (int i = 0; i < num; ++i)
	{
		array<int, 2>sa;
		sa[0] = RanKind(en);
		sa[1] = RanTime(en);
		s.push_back(sa);
		vector<array<int, 3>>ma;
		for (int i = 0; i < 10; ++i)
		{
			array<int, 3>maa;
			maa[0] = RanDiry(en);
			maa[1] = RanDiry(en);
			maa[2] = RanTime(en);
			ma.push_back(maa);
		}
		ma.push_back({ 0,1,60000 });
		m.push_back(ma);
		d.push_back(array<int, 11>{ 100 * level + int(100 * Ranf(en) * level), 15 * level + int(15 * Ranf(en) * level),
			0, 5 * level + int(5 * Ranf(en) * level),
			25 * RanPos(en), 0, 60, RanDirx(en), RanDiry(en), 0, 3});
		vector<array<int, 3>>fa;
		for (int i = 0; i < 10; ++i)
		{
			array<int, 3>faa;
			faa[0] = RanDirx(en);
			faa[1] = 3;
			faa[2] = 180;
			fa.push_back(faa);
		}
		fa.push_back({ 0,3,60000 });
		f.push_back(fa);
	}
	this->SetAppearEnemyList(s, m, f, d);
}

void operate_system::EnemyUseSkill(player& plane)
{
	list<enemy*>::iterator i;
	for (i = EnemyList.begin(); i != EnemyList.end(); ++i)
	{
		if ((**i).countSkill())
		{
			for (int j = 1; j <= 3; ++j)
			{
				if ((**i).GetSkillKind(j) == 1)
					(**i).UseSkill(plane, *this, j);
			}
		}
	}
}

void operate_system::clear(void)
{
	EnemyList.clear();
	EnemyBulletList.clear();
	PlayerBulletList.clear();
}

bool operate_system::empty(void) const
{
	if (EnemyList.empty() && EnemyBulletList.empty() && PlayerBulletList.empty())
		return 1;
	return false;
}

normal_1::normal_1(int thp, int tatk, int tdef, int tcoin, int tx, int ty, int tatkr, int tdir_x, int tdir_y,
	int tfire_speed_x, int tfire_speed_y, int tfire_count) :enemy(thp, tatk, tdef, tcoin, tx, ty, tatkr, tdir_x, tdir_y,
		tfire_speed_x, tfire_speed_y, tfire_count), skill_kind(1), skill_cd(120), skill_count(0), skill_flag(0), skill_time(60),
	skill_quantity(1)
{

}

bool normal_1::countFire(void)
{
	if (GetSpeed().first != 0)
		return 0;
	return 1;
}

enemy_bullet & normal_1::Fire(void)
{
	laser* b = new laser(GetAtk(), GetPos().first + (GetLength() - LASERLENGTH) / 2,
		GetPos().second + GetLength(), GetFireSpeed().first, GetFireSpeed().second);
	b->SetRadio(GetBulletRadio());
	b->SetSpeed(0, GetBulletRadio());
	return *b;
}

void normal_1::SetSkill(int tcd, int ttime)
{
	skill_cd = tcd;
	skill_time = ttime;
}

bool normal_1::countSkill(void)
{
	++skill_count;
	if (skill_flag == 1 && skill_count >= skill_time)
	{
		skill_count = 0;
		return 1;
	}
	if (skill_flag == 0 && skill_count >= skill_cd)
	{
		skill_count = 0;
		return 1;
	}
	return 0;
}

void normal_1::UseSkill(player&, operate_system&, int num)
{
	if (num == 1)
		skill_flag = (skill_flag ? 0 : 1);
}

int normal_1::beAttacked(int a)
{
	if (skill_flag == 0)
	{
		if (a > GetDef())
		{
			ModifyHp(GetDef() - a);
			return a - GetDef();
		}
		else
		{
			return 1;
			ModifyHp(-1);
		}
	}
	return 0;
}

int normal_1::GetSkillKind(int num) const
{
	if (num == 1 && skill_count == 0)
		return 1;
	else
		return 0;
}

void normal_1::Draw(void) const
{
	setlinecolor(0x00FF00);
	setfillcolor(0x00FF00);
	if (skill_flag == 1)
	{
		circle(GetPos().first + GetLength() / 2, GetPos().second + GetLength() / 2, int(GetLength() / 1.414));
	}
	int l_hp = GetLength()*GetHpPercent() / 100;
	solidrectangle(GetPos().first, GetPos().second, GetPos().first + GetLength(), GetPos().second + GetLength());
	setfillcolor(0x0000FF);
	solidrectangle(GetPos().first, GetPos().second - 7, GetPos().first + l_hp, GetPos().second - 2);
}

int normal_1::GetSkillQuantity(void) const
{
	return skill_quantity;
}



laser::laser(int ta, int tx, int ty, int tspeedx, int tspeedy) :enemy_bullet(ta, tx, ty, tspeedx, tspeedy)
{
}

bool laser::CheckDisapear(void)
{
	if (GetX() < 0 || GetX() + LASERLENGTH > 600 || GetY() < 0 || GetY() + GetRadio() > 600)
		return 1;
	return false;
}

bool laser::ifAttack(std::pair<int, int> pos) const
{
	if (GetX() > pos.first - LASERLENGTH && GetX() < pos.first + PLAYER &&
		GetY() > pos.second - GetRadio() && GetY() < pos.second + PLAYER)
		return 1;
	return false;
}

void laser::Draw(void) const
{
	solidrectangle(GetX(), GetY(), GetX() + LASERLENGTH, GetY() + GetRadio());
}

normal_2::normal_2(int thp, int tatk, int tdef, int tcoin, int tx, int ty, int tatkr, int tdir_x,
	int tdir_y, int tfire_speed_x, int tfire_speed_y, int tfire_count) :enemy(thp, tatk, tdef, tcoin, tx, ty, tatkr, tdir_x, tdir_y,
		tfire_speed_x, tfire_speed_y, tfire_count), skill_kind(2), skill_cd(120), skill_count(0), skill_flag(0), skill_time(60),
	skill_quantity(1)
{
}

enemy_bullet & normal_2::Fire(void)
{
	enemy_bullet * teb = new enemy_bullet(GetAtk(), GetPos().first + GetLength() / 2, GetPos().second + 25 + GetBulletRadio(),
		GetFireSpeed().first, GetFireSpeed().second);
	teb->SetRadio(GetBulletRadio());
	return *teb;
}

void normal_2::SetSkill(int tcd, int ttime)
{
	skill_cd = tcd;
	skill_time = ttime;
}

bool normal_2::countSkill(void)
{
	++skill_count;
	if (skill_flag == 1 && skill_count >= skill_time)
	{
		skill_count = 0;
		return 1;
	}
	if (skill_flag == 0 && skill_count >= skill_cd)
	{
		skill_count = 0;
		return 1;
	}
	return 0;
}

void normal_2::UseSkill(player & pl, operate_system & op, int num)
{
	if (num == 2)
	{
		skill_flag = (skill_flag ? 0 : 1);
		if (skill_flag == 1)
		{
			enemy* tene1 = new enemy{ 60,10,0,0,GetPos().first - GetLength(),GetPos().second + GetLength(),60,0,1,0,2,0 };
			enemy* tene2 = new enemy{ 60,10,0,0,GetPos().first + GetLength() * 2,GetPos().second + GetLength(),60,0,1,0,2,0 };
			tene1->SetBulletRadio(3);
			tene2->SetBulletRadio(3);
			op.CreateEnemy(*tene1);
			op.CreateEnemy(*tene2);
		}
	}
}

int normal_2::beAttacked(int a)
{
	if (skill_flag == 0)
	{
		if (a > GetDef())
		{
			ModifyHp(GetDef() - a);
			return a - GetDef();
		}
		else
		{
			return 1;
			ModifyHp(-1);
		}
	}
	return 0;
}

int normal_2::GetSkillKind(int num) const
{
	if (num == 2 && skill_count == 0)
		return 1;
	else
		return 0;
}

void normal_2::Draw(void) const
{
	setfillcolor(0x00FF00);
	int l_hp = GetLength()*GetHpPercent() / 100;
	solidrectangle(GetPos().first, GetPos().second, GetPos().first + GetLength(), GetPos().second + GetLength());
	setfillcolor(0x0000FF);
	solidrectangle(GetPos().first, GetPos().second - 7, GetPos().first + l_hp, GetPos().second - 2);
}

int normal_2::GetSkillQuantity(void) const
{
	return skill_quantity;
}

track_bullet::track_bullet(int ta, int tx, int ty, int tspeedx, int tspeedy) :enemy_bullet(ta, tx, ty, tspeedx, tspeedy),
change_num(300), now_num(0)
{
}

void track_bullet::ChangeDirection(pair<int, int>pos)
{
	++now_num;
	if (now_num <= change_num)
	{
		int xm = GetPos().first - pos.first, ym = GetPos().second - pos.second;
		if (xm > 50 && ym > 50)
			SetSpeed(-2, -2);
		if (xm < -50 && ym < -50)
			SetSpeed(2, 2);
		if (xm > 50 && ym < -50)
			SetSpeed(-2, 2);
		if (xm < -50 && ym > 50)
			SetSpeed(2, -2);
		if (ym < 0 && xm>0 && xm < 50)
			SetSpeed(0, 3);
	}
}

void track_bullet::Move(player& pl)
{
	ChangeDirection(pl.GetPos());
	ModifyNowFrame(1);
	if (GetNowFrame() >= GetFrame())
	{
		ModifyPos(GetSpeed());
		ModifyNowFrame(0);
	}
}

normal_3::normal_3(int thp, int tatk, int tdef, int tcoin, int tx, int ty, int tatkr, int tdir_x, int tdir_y,
	int tfire_speed_x, int tfire_speed_y, int tfire_count) :enemy(thp, tatk, tdef, tcoin, tx, ty, tatkr, tdir_x, tdir_y,
		tfire_speed_x, tfire_speed_y, tfire_count)
{
}

enemy_bullet & normal_3::Fire(void)
{
	enemy_bullet * teb = new track_bullet(GetAtk(), GetPos().first + GetLength() / 2, GetPos().second + 25 + GetBulletRadio(),
		0, 3);
	teb->SetRadio(GetBulletRadio());
	return *teb;
}

normal_4::normal_4(int thp, int tatk, int tdef, int tcoin, int tx, int ty, int tatkr, int tdir_x, int tdir_y,
	int tfire_speed_x, int tfire_speed_y, int tfire_count) : enemy(thp, tatk, tdef, tcoin, tx, ty, tatkr, tdir_x, tdir_y,
		tfire_speed_x, tfire_speed_y, tfire_count), skill_kind(3), skill_cd(120), skill_count(0), skill_flag(0), skill_time(60),
	skill_quantity(1)
{
}

void normal_4::SetSkill(int tcd, int ttime)
{
	skill_cd = tcd;
	skill_time = ttime;
}

bool normal_4::countSkill(void)
{
	++skill_count;
	if (skill_flag == 1 && GetHpPercent() < 100)
	{
		ModifyHp(1);
	}
	if (skill_flag == 1 && skill_count >= skill_time)
	{
		skill_count = 0;
		return 1;
	}
	if (skill_flag == 0 && skill_count >= skill_cd)
	{
		skill_count = 0;
		return 1;
	}
	return 0;
}

void normal_4::UseSkill(player &, operate_system &, int num)
{
	if (num == 3)
		skill_flag = (skill_flag ? 0 : 1);
}

int normal_4::GetSkillKind(int num) const
{
	if (num == 3 && skill_count == 0)
		return 1;
	else
		return 0;
}

int normal_4::GetSkillQuantity(void) const
{
	return skill_quantity;
}
