#pragma once
#include <vector>

//ëOï˚êÈåæ
class Player;
class Enemy;

class Stage
{
public:
	Stage();
	~Stage();
	void Initialize();
	void TitleUpdate();
	void PlayUpdate();
	void GameOverUpdate();

	void TitleDraw();
	void PlayDraw();
	void GameOverDraw();

	void Update();
	void Draw();
	void Release();
private:
	//Player* player_;
	//Enemy* enemy_;
	unsigned long long gameScore_;
	void Enemy_vs_Bullet();
	void Player_vs_Enemy();
	void DeleteBullet();
	void DeleteEnemy();
	void DeleteEffect();
	void ShootBullet();
};

