#include "cPlaneGame.h"
using namespace std;

player::player(int thp, int tenergy, int tr, int tc, int tatk, int tdef, int tcoin, int tx, int ty, int tskill1_n, int tskill1_cd) :
	hp(thp), energy(tenergy), rate(tr), count(tc), atk(tatk), def(tdef), coin(tcoin),
	x(tx), y(ty), skill1_n(tskill1_n), skill1_cd(tskill1_cd)
{
}

bool player::ifBeAttacked(pair<int, int>pos)const
{
	if (pos.first >= x - BULLET && pos.first <= x + 50 + BULLET && pos.second >= y - BULLET && pos.second <= y + 50 + BULLET)
		return 1;
	else
		return 0;
}

void player::beAttacked(int a)
{
	hp -= (a > def ? a - def : 1);
}

pair<int, int> player::PlayerOperate(void)const
{

	int  ud = 0, lr = 0;
	ud = bool(GetAsyncKeyState(VK_S)) - bool(GetAsyncKeyState(VK_W));
	lr = bool(GetAsyncKeyState(VK_D)) - bool(GetAsyncKeyState(VK_A));
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

player_bullet & player::Fire(void)
{
	count = 0;
	player_bullet * t = new player_bullet(atk, x + PLAYER / 2, y - 7);
	return *t;
}

enemy::enemy(int thp, int tatk, int tdef, int tcoin, int tx, int ty,
	int tatkr, int tdir_x, int tdir_y, int tfire_dir, int tfire_count) :
	hp(thp), atk(tatk), def(tdef), coin(tcoin), x(tx), y(ty), fire_rate(tatkr), move_dir_x(tdir_x),
	move_dir_y(tdir_y), fire_dir(tfire_dir), fire_count(tfire_count), total_hp(thp)
{
}

void enemy::move(void)
{
	if (ifLeftRightLimit())
		move_dir_x *= -1;
	x += move_dir_x;
	y += move_dir_y;
}

bool enemy::ifBeAttacked(int bx, int by)const
{
	if (bx >= x - 5 && bx <= x + 25 && by <= y + 25 && by >= y - 5)
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
	++fire_count;
	if (fire_count >= fire_rate)
		return 1;
	return 0;
}

enemy_bullet & enemy::Fire(void)
{
	fire_count = 0;
	enemy_bullet * teb = new enemy_bullet(atk, x + 10, y + 25, fire_dir);
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
	if (x > 575 || x < 0)
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

bool enemy::CheckDisapear(void)const
{
	if (x < -25 || y < -25 || x>600 || y>575)
		return 1;
	return 0;
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
		(**i).move();
	}
}

player_bullet::player_bullet(int ta, int tx, int ty) :atk(ta), x(tx), y(ty)
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
	y -= 4;
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

enemy_bullet::enemy_bullet(int ta, int tx, int ty, int td) : atk(ta), x(tx), y(ty), dir(td)
{
}

bool enemy_bullet::CheckDisapear(void)
{
	return 0;
}

void enemy_bullet::Move(void)
{
	y += 3;
	x += dir;
	if (dir)
		--y;
}

pair<int, int> enemy_bullet::GetPos(void) const
{
	return pair<int, int>(x, y);
}

int enemy_bullet::GetAtk(void) const
{
	return atk;
}

void operate_system::DrawToScreen(pair<int, int>plp)
{
	setlinecolor(LIGHTCYAN);
	setfillcolor(0x0000FF);
	for (list<enemy*>::iterator i = EnemyList.begin(); i != EnemyList.end(); ++i)
	{
		int x = (**i).GetPos().first, y = (**i).GetPos().second, hp = (**i).GetHpPercent() / 4;
		rectangle(x, y, x + ENEMY, y + ENEMY);
		solidrectangle(x, y - 7, x + hp, y - 2);
	}
	setfillcolor(LIGHTBLUE);
	fillrectangle(plp.first, plp.second, plp.first + PLAYER, plp.second + PLAYER);
	setfillcolor(LIGHTCYAN);
	pair<int, int>pos;
	for (list<player_bullet*>::iterator i = PlayerBulletList.begin(); i != PlayerBulletList.end(); ++i)
	{
		pos = (**i).GetPos();
		solidcircle(pos.first, pos.second, BULLET);
	}
	list<enemy_bullet*>::iterator i;
	setfillcolor(0xAA00AA);
	for (i = EnemyBulletList.begin(); i != EnemyBulletList.end(); ++i)
	{
		pos = (**i).GetPos();
		solidcircle(pos.first, pos.second, BULLET);
	}
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
		if ((**i).GetHp() <= 0)
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
			EnemyBulletList.push_back(&(**i).Fire());
		}
	}
}

void operate_system::EnemyAttackPlayer(player& plane)
{
	list<enemy_bullet*>::iterator i;
	for (i = EnemyBulletList.begin(); i != EnemyBulletList.end();)
	{
		if (plane.ifBeAttacked((**i).GetPos()))
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
