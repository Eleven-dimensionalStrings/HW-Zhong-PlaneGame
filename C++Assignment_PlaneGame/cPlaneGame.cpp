#include "cPlaneGame.h"
#include<cassert>
using namespace std;
std::default_random_engine en(int(time(0)));
std::uniform_real_distribution<double>Ranf(0, 0.2);
std::uniform_int_distribution<int>RanPos(0, 24);
std::uniform_int_distribution<int>RanDirx(-2, 2);
std::uniform_int_distribution<int>RanDiry(0, 2);
std::uniform_int_distribution<int>RanTime(60, 180);
std::uniform_int_distribution<int>RanKind(1, 3);
player::player(int thp, int tenergy, int tr, int tc, int tatk, int tdef,
	int tcoin, int tx, int ty, int tskill1_n, int tskill1_cd) :
	hp(thp), energy(tenergy), rate(tr), count(tc), atk(tatk), def(tdef), coin(tcoin),
	x(tx), y(ty), skill1_n(tskill1_n), skill1_cd(tskill1_cd), skill1_level(0), level(0), steel_hp_persent(50)
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
	int digit = 1000000000, ihp = 0, ienergy = 0, uhp = hp, uenergy = energy, ucoin = coin, icoin = 0;
	string shp = "", senergy = "", scoin = "";
	for (int i = 0; i < 10; ++i)
	{
		if (uhp / digit)
			++ihp;
		if (uenergy / digit)
			++ienergy;
		if (ucoin / digit)
			++icoin;
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
		digit /= 10;
	}
	shp += '\0';
	senergy += '\0';
	data.push_back(shp);
	data.push_back(senergy);
	data.push_back(scoin);
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

}

void player::LearnSkill(void)
{
}

int player::GetSteelHpPersent(void) const
{
	return steel_hp_persent;
}

void player::SteelHp(int dhp)
{
	hp += dhp*steel_hp_persent / 100;
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
	hp -= (patk > def ? patk - def : 1);
	return (patk > def ? patk - def : 1);
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
	teb->SetRadio(5);
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
	outtextxy(0, 650, "MP:");
	outtextxy(0, 680, "COIN:");
	outtextxy(50, 620, &player_data[0][0]);
	outtextxy(50, 650, &player_data[1][0]);
	outtextxy(50, 680, &player_data[2][0]);
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
				plane.SteelHp((**i).beAttacked((**j).GetAtk()));
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

bool operate_system::AppearEnemy(void)
{
	++appear_enemy_count;
	if (appear_enemy_count >= (*appear_enemy_step)[1])
	{
		appear_enemy_count = 0;
		if ((*appear_enemy_step)[0] == 1)
		{
			if ((*appear_enemy_attribution_inf)[0] != -1)
			{
				enemy* e = new normal_1{ (*appear_enemy_attribution_inf)[0],(*appear_enemy_attribution_inf)[1],
					   (*appear_enemy_attribution_inf)[2], (*appear_enemy_attribution_inf)[3], (*appear_enemy_attribution_inf)[4],
					   (*appear_enemy_attribution_inf)[5], (*appear_enemy_attribution_inf)[6], (*appear_enemy_attribution_inf)[7],
					   (*appear_enemy_attribution_inf)[8], (*appear_enemy_attribution_inf)[9],(*appear_enemy_attribution_inf)[10],0 };
				(*e).SetMoveList(*appear_enemy_move_inf);
				(*e).SetFireList(*appear_enemy_fire_inf);
				(*e).SetBulletRadio(5);
				(*e).SetLength(25);
				this->CreateEnemy(*e);
			}
		}
		if ((*appear_enemy_step)[0] == 2)
		{
			enemy* e = new normal_2{ (*appear_enemy_attribution_inf)[0],(*appear_enemy_attribution_inf)[1],
				(*appear_enemy_attribution_inf)[2], (*appear_enemy_attribution_inf)[3], (*appear_enemy_attribution_inf)[4],
				(*appear_enemy_attribution_inf)[5], (*appear_enemy_attribution_inf)[6], (*appear_enemy_attribution_inf)[7],
				(*appear_enemy_attribution_inf)[8], (*appear_enemy_attribution_inf)[9],(*appear_enemy_attribution_inf)[10],0 }; (*e).SetMoveList(*appear_enemy_move_inf);
			(*e).SetFireList(*appear_enemy_fire_inf);
			(*e).SetBulletRadio(5);
			(*e).SetLength(25);
			this->CreateEnemy(*e);
		}
		if ((*appear_enemy_step)[0] == 3)
		{
			enemy* e = new normal_3{ (*appear_enemy_attribution_inf)[0],(*appear_enemy_attribution_inf)[1],
				(*appear_enemy_attribution_inf)[2], (*appear_enemy_attribution_inf)[3], (*appear_enemy_attribution_inf)[4],
				(*appear_enemy_attribution_inf)[5], (*appear_enemy_attribution_inf)[6], (*appear_enemy_attribution_inf)[7],
				(*appear_enemy_attribution_inf)[8], (*appear_enemy_attribution_inf)[9],(*appear_enemy_attribution_inf)[10],0 }; (*e).SetMoveList(*appear_enemy_move_inf);
			(*e).SetFireList(*appear_enemy_fire_inf);
			(*e).SetBulletRadio(5);
			(*e).SetLength(25);
			this->CreateEnemy(*e);
		}
		if ((appear_enemy_step + 2) != appear_enemy_list.end())
		{
			++appear_enemy_step;
			++appear_enemy_fire_inf;
			++appear_enemy_attribution_inf;
			++appear_enemy_move_inf;
		}
		else
		{
			outtextxy(0, 600, "END");
			return 0;
		}
	}
	return 1;
}

void operate_system::tSet(void)
{
	vector<array<int, 2>>s;
	vector<vector<array<int, 3>>>f;
	vector<array<int, 11>>d;
	vector<vector<array<int, 3>>>m;
	for (int i = 0; i < 200; ++i)
	{
		array<int, 2>sa;
		sa[0] = RanKind(en);
		sa[1] = 3 * RanTime(en);
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
		d.push_back(array<int, 11>{ 100 + int(100 * Ranf(en)), 10 + int(10 * Ranf(en)), 0, 5 + int(5 * Ranf(en)),
			25 * RanPos(en), 0, 60, RanDirx(en), RanDiry(en), 0, 3});
		vector<array<int, 3>>fa;
		for (int i = 0; i < 10; ++i)
		{
			array<int, 3>faa;
			faa[0] = 0;
			faa[1] = 3;
			faa[2] = 60;
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
change_num(1200), now_num(0)
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
