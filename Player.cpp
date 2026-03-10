#include "Player.h"
#include "Math2D.h"
#include <DxLib.h>
#include <cmath>
#include "globals.h"
#include "Input.h"

namespace {
	const float DAMP = 0.995f; //å∏êä
	const float ACC =170.0f;//â¡ë¨ìx
}

Player::Player()
	:Base(Vector2D(0,0), Vector2D(0, 0), GetColor(0,0,0)),
	dir_({ 0,-1 }), radius_(1.0f), omega_{1.0f}, angle_(0.0f),isAlive_(true)
{
	collisionRadius_ = radius_;

	vertex_[0] = { 0,0 };
	vertex_[1] = { 0,1 };
	vertex_[2] = { 0,2 };
	SetObjType(PLAYER);
}

Player::Player(const Vector2D& pos, const Vector2D& vel, unsigned int color,
	           const Vector2D& dir, float r, float omega)
	:Base(pos, vel, color), dir_(dir), radius_(r), omega_(omega), isAlive_(true)
{
	collisionRadius_ = radius_;
	vertex_[0] = { 0, 0 }; //0î‘
	vertex_[1] = { 0, 0 }; //1î‘
	vertex_[2] = { 0, 0 }; //2î‘
	angle_ = 0.0f; //äpìxèâä˙âª
	SetObjType(PLAYER);

}


Player::~Player()
{
}

void Player::Update()
{
	if (IsAlive() == false)return;//éÄÇÒÇ≈ÇÈÇ»ÇÁèàóùÇµÇ»Ç¢
	const float PI = 3.1415926359f;
	Vector2D p[3];

	p[0] = { 0.0f, 1.0f };
	p[1] = { cos(-60.0f * (PI / 180.0f)), sin(-60.0f * (PI / 180.0f)) };
	p[2] = { cos(240.0f * (PI / 180.0f)), sin(240.0f * (PI / 180.0f)) };

	p[0].x = radius_ * p[0].x; p[0].y = radius_ * p[0].y;
	p[1].x = radius_ * p[1].x; p[1].y = radius_ * p[1].y;
	p[2].x = radius_ * p[2].x; p[2].y = radius_ * p[2].y;
	
	//ÉèÅ[ÉãÉhí∏ì_ÇàÍíUçÏÇÈ
	vertex_[0] = { pos_.x + p[0].x, pos_.y + p[0].y };
	vertex_[1] = { pos_.x + p[1].x, pos_.y + p[1].y };
	vertex_[2] = { pos_.x + p[2].x, pos_.y + p[2].y };
	//âÒì]ì¸óÕ
	if (Input::IsKeepKeyDown(KEY_INPUT_LEFT))
	{
		angle_ = angle_ + omega_ * GetDeltaTime();
	}
	if (Input::IsKeepKeyDown(KEY_INPUT_RIGHT))
	{
		angle_ = angle_ - omega_ * GetDeltaTime();
	}

	Mat2 toOrigin = Math2D::Translation({ -pos_.x, -pos_.y });
	Mat2 rotMat = Math2D::Rotation(angle_);
	Mat2 toPos = Math2D::Translation({ pos_.x,pos_.y });
	Mat2 tmp = Math2D::Multiply(rotMat, toOrigin);
	Mat2 M = Math2D::Multiply(toPos, tmp);

	for (int i = 0;i < 3;i++)
	{
		vertex_[i] = Math2D::TransformPoint(vertex_[i], M);
	}

	dir_ = Math2D::FromAngle(angle_ + PI / 2.0f);

	if (Input::IsKeepKeyDown(KEY_INPUT_UP))
	{
		vel_.x = vel_.x + dir_.x * ACC * GetDeltaTime();
		vel_.y = vel_.y + dir_.y * ACC * GetDeltaTime();
	}
	pos_ = Math2D::Add(pos_, Math2D::Mul(vel_, GetDeltaTime()));

	vel_ = Math2D::Mul(vel_, DAMP);//å∏êä

	if (pos_.x < 0) pos_.x = WIN_WIDTH;
	if (pos_.x > WIN_WIDTH) pos_.x = 0;
	if (pos_.y < 0) pos_.y = WIN_HEIGHT; 
	if (pos_.y > WIN_HEIGHT) pos_.y = 0;

}

void Player::Draw()
{
	if (IsAlive() == false)return;//éÄÇÒÇ≈ÇÈÇ»ÇÁñ≥éã

	Vector2D scrPos[3];
	scrPos[0] = Math2D::World2Screen(vertex_[0]);
	scrPos[1] = Math2D::World2Screen(vertex_[1]);
	scrPos[2] = Math2D::World2Screen(vertex_[2]);

	DrawTriangleAA(
		scrPos[0].x, scrPos[0].y,
		scrPos[1].x, scrPos[1].y,
		scrPos[2].x, scrPos[2].y,
		GetColor(255, 0, 0), TRUE
	);
	Vector2D cp = Math2D::World2Screen(pos_);
	//DrawCircle((int)cp.x, (int)cp.y, (int)collisionRadius_, GetColor(255, 255, 0), FALSE);
}
