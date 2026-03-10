#include "Bullet.h"
#include "globals.h"
#include <DxLib.h>

Bullet::Bullet(const Vector2D& pos, const Vector2D& vel, 
	unsigned int color, float radius, float life)
	:Base(pos,vel,color),radius_(radius),life_(life)
{
	SetObjType(BULLET);
}

void Bullet::Update()
{
	if (IsDead() == true)
		return;//死んでるならスルー

	float dt = GetDeltaTime();
	pos_ = Math2D::Add(pos_, Math2D::Mul(vel_, dt));

	//画面端ワープ
	if (pos_.x < 0) pos_.x = WIN_WIDTH;
	if (pos_.x > WIN_WIDTH) pos_.x = 0;
	if (pos_.y < 0) pos_.y = WIN_WIDTH;
	if (pos_.y > WIN_HEIGHT) pos_.y = 0;
	//弾が何秒生きるか
	life_ = life_ - dt;
}

void Bullet::Draw()
{
	if (IsDead() == true)return;//死んでるならスルー
	Vector2D sPos = Math2D::World2Screen(pos_);
	DrawCircle(sPos.x, sPos.y, radius_, 16, Color_, TRUE);
	//DrawCircle(sPos.x, sPos.y, radius_, 16, Color_, (255,255,255));

}
