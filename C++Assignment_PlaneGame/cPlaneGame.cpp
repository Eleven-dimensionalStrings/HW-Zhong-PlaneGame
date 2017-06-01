#include "cPlaneGame.h"
using namespace std;

player::player(int thp, int tenergy, int tr, int tc, int tatk, int tdef,
	int tcoin, int tx, int ty, int tskill1_n, int tskill1_cd) :
	hp(thp), energy(tenergy), rate(tr), count(tc), atk(tatk), def(tdef), coin(tcoin),
	x(tx), y(ty), skill1_n(tskill1_n), skill1_cd(tskill1_cd)
{
}

bool player::ifBeAttacked(pair<int, int>pos, int radio)const
{
	if (pos.first >= x - radio && pos.first <= x + 50 + radio && pos.second >= y - radio && pos.second <= y + 50 + radio)
		return 1;
	else
		return 0;
}

void player::beAttacked(int a)
{
	hp -= (a > def ? a - def : 1);
}

void player::beCrash(int a_hp)
{
	hp -= a_hp / atk;
}

pair<int, int> player::PlayerOperate(void)const
{

	int  ud = 0, lr = 0;
	ud = bool(GetAsyncKeyState(VK_S)) - bool(GetAsyncKeyState(VK_W));
	lr = bool(GetAsyncKeyState(VK_D)) - bool(GetAsyncKeyState(VK_A));
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
	return def;
}

bool player::ifFire(void)
{
	if (GetAsyncKeyState(VK_J))
		++count;
	if (count >= rate)
		return true;
	return false;
}

std::vector<string> player::GetData(void) const
{
	vector<string>data;
	int digit = 1000000000, ihp = 0, ienergy = 0, uhp = hp, uenergy = energy;
	string shp = "", senergy = "";
	for (int i = 0; i < 10; ++i)
	{
		if (uhp / digit)
			++ihp;
		if (uenergy / digit)
			++ienergy;
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
		digit /= 10;
	}
	shp += '\0';
	senergy += '\0';
	data.push_back(shp);
	data.push_back(senergy);
	return data;
}

player_bullet & player::Fire(void)
{
	--energy;
	count = 0;
	player_bullet * t = new player_bullet(atk, x + PLAYER / 2, y - 7);
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

void enemy::beAttacked(int patk)
{
	hp -= (patk > def ? patk - def : 1);
}

bool enemy::countFire(void)
{
	++fire_cd;
	if (fire_cd >= fire_rate)
		return 1;
	return 0;
}

enemy_bullet & enemy::Fire(void)
{
	fire_cd = 0;
	enemy_bullet * teb = new enemy_bullet(atk, x + bullet_radio / 2, y + 25, fire_speed_x, fire_speed_y);
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
	if ((x > 575 && move_speed_x > 0) || (x < 0 && move_speed_x < 0))
		return 1;
	else
		return 0;
}

bool enemy::ifDownLimit(void) const
{
	if (y >= 575)
		return 1;
	else
		return 0;
}
bool enemy::ifCrashPlayer(int px, int py)const
{
	if (x >= px - 25 && x <= px + 50 && y >= py - 25 && y <= py + 50)
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
	if (x > pos.first - ENEMY && x<pos.first + PLAYER && y>pos.second - ENEMY && y < pos.second + PLAYER)
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

void enemy::SetFireList(std::vector<std::array<int, 2>> tfire_list)
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
	if (fire_count >= (*fire_step)[1])
	{
		fire_count = 0;
		++fire_step;
		fire_speed_x = (*fire_step)[0];
	}
}

void enemy::ChangeMoveState(void)
{
	++move_count;
	if (move_count >= (*move_step)[2])
	{
		++move_step;
		move_count = 0;
		move_speed_x = (*move_step)[0];
		move_speed_y = (*move_step)[1];
	}
}

void enemy::UseSkill(void)
{
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

enemy_bullet::enemy_bullet(int ta, int tx, int ty, int tspeedx, int tspeedy) :
	atk(ta), x(tx), y(ty), speed_x(tspeedx), speed_y(tspeedy), frame(1), now_frame(1), radio(BULLET)
{
}

bool enemy_bullet::CheckDisapear(void)
{
	return 0;
}

void enemy_bullet::Move(void)
{
	++now_frame;
	if (now_frame >= frame)
	{
		x += speed_x;
		y += speed_y;
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

void operate_system::DrawToScreen(pair<int, int>plp, vector<string>player_data)
{

	for (list<enemy*>::iterator i = EnemyList.begin(); i != EnemyList.end(); ++i)
	{
		int x = (**i).GetPos().first, y = (**i).GetPos().second, hp = (**i).GetHpPercent() / 4;
		setfillcolor(0x00FF00);
		solidrectangle(x, y, x + ENEMY, y + ENEMY);
		setfillcolor(0x0000FF);
		solidrectangle(x, y - 7, x + hp, y - 2);
	}
	setfillcolor(LIGHTBLUE);
	fillrectangle(plp.first, plp.second, plp.first + PLAYER, plp.second + PLAYER);
	setfillcolor(LIGHTCYAN);
	pair<int, int>pos;
	for (list<player_bullet*>::iterator i = PlayerBulletList.begin(); i != PlayerBulletList.end(); ++i)
	{
		pos = (**i).GetPos();
		solidcircle(pos.first, pos.second, (**i).GetRadio());
	}
	list<enemy_bullet*>::iterator i;
	setfillcolor(0xAA00AA);
	for (i = EnemyBulletList.begin(); i != EnemyBulletList.end(); ++i)
	{
		pos = (**i).GetPos();
		solidcircle(pos.first, pos.second, (**i).GetRadio());
	}
	settextcolor(BLACK);
	outtextxy(0, 620, "HP:");
	outtextxy(0, 650, "MP:");
	outtextxy(30, 620, &player_data[0][0]);
	outtextxy(30, 650, &player_data[1][0]);
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

void operate_system::PlayerAttackEnemy(void)
{
	list<enemy*>::iterator i;
	list<player_bullet*>::iterator j;
	for (i = EnemyList.begin(); i != EnemyList.end(); ++i)
	{
		for (j = PlayerBulletList.begin(); j != PlayerBulletList.end(); )
		{
			if ((**i).ifBeAttacked((**j).GetPos().first, (**j).GetPos().second))
			{
				(**i).beAttacked((**j).GetAtk());
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

void operate_system::MoveEnemyBullet(void)
{
	list<enemy_bullet*>::iterator i;
	for (i = EnemyBulletList.begin(); i != EnemyBulletList.end(); ++i)
	{
		(**i).Move();
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
			EnemyBulletList.push_back(&(**i).Fire());
		}
	}
}

void operate_system::EnemyAttackPlayer(player& plane)
{
	list<enemy_bullet*>::iterator i;
	for (i = EnemyBulletList.begin(); i != EnemyBulletList.end();)
	{
		if (plane.ifBeAttacked((**i).GetPos(), (**i).GetRadio()))
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
		if ((**i).GetPos().second > 600 || (**i).GetPos().first < 0 || (**i).GetPos().first>600 || (**i).GetPos().second < 0)
		{
			delete *i;
			EnemyBulletList.erase(i++);
		}
		else
			++i;
	}
}

void operate_system::SetAppearEnemyList(std::vector<std::array<int, 2>> tEnemyAppearList,
	std::vector<std::vector<std::array<int, 3>>>tAppearEnemyMoveList)
{
	appear_enemy_list = tEnemyAppearList;
	appear_enemy_step = appear_enemy_list.begin();
	appear_enemy_move_list = tAppearEnemyMoveList;
	appear_enemy_move_inf = appear_enemy_move_list.begin();
	appear_enemy_count = 0;
}

void operate_system::AppearEnemy(void)
{
	++appear_enemy_count;
	if (appear_enemy_count >= (*appear_enemy_step)[1])
	{
		appear_enemy_count = 0;
		switch ((*appear_enemy_step)[0])
		{
		case 0:
			break;
		default:
			enemy* e = new enemy{ 100,20,0,5,250,0,30,0,1,0,2,0 };
			break;
		}
		if ((appear_enemy_step + 1) != appear_enemy_list.end())
		{
			++appear_enemy_step;
			++appear_enemy_move_inf;
		}
	}
}
