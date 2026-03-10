#include "ExplosionEffect.h"
#include "DxLib.h"
#include <algorithm>
#include "easefunction.h"

namespace
{
	const float PARTICLE_LIFE = 1.0f;//パーティクルの寿命（秒）
	const float PARTICLE_RADIUS = 1.5f;//パーティクルの半径
	const float PARTICLE_SPEED[3] = { 50.0f,80.0f,120.0f };//パーティクルの速度
	const float PARTICLE_DECAY = 0.95f;//パーティクルの減衰率
	const float PARTICLE_LIFE_FADE_START = PARTICLE_LIFE / 2.0f;//パーティクルの寿命のうち、フェードが始まる時間
}

ExplosionEffect::ExplosionEffect(const Vector2D& pos, int particleCount)
	:Base(pos, { 0.0f,0.0f },GetColor(255,255,255))
{
	SetObjType(OBJ_TYPE::EFFECT);
	particles_.clear();
	isFinished_ = false;

	for (int i = 0;i < particleCount; i++)
	{
		//パーティクル一個の初期化
		Particle particle;
		particle.Offset = { 0.0f,0.0f };
		particle.vel = { 0.0f,0.0f };
		particle.life = PARTICLE_LIFE;
		particle.radius = PARTICLE_RADIUS;
		particle.alpha = 1.0f;

		float angle_rad = (float)GetRand(360) * (Math2D::PI / 180.0f);
		Vector2D direction = Math2D::FromAngle(angle_rad);
		particle.vel = Math2D::Mul(direction, PARTICLE_SPEED[GetRand(2)]);


		particles_.push_back(particle);

	}
}


void ExplosionEffect::Update()
{
	float dt = GetDeltaTime();
	bool allDead = true;

	for (auto& particle : particles_)
	{
		if (particle.life > 0.0f)
		{
			allDead = false;

			//位置更新
			particle.Offset = Math2D::Add(particle.Offset, Math2D::Mul(particle.vel, dt));
			//速度更新
			//particle.vel = Math2D::Mul(particle.vel, PARTICLE_DECAY);
			//寿命更新
			particle.life -= dt;
			if (particle.life < 0.0f)
				particle.life = 0.0f;
		}

		//if (particle.life < PARTICLE_LIFE_START)
		//{
		//	particle.alpha = particle.life / PARTICLE_LIFE_START;
		//}
		//else
		//{
		//	particle.alpha = 1.0f;
		//}
		//float.life = particle.life

		float lifeRatio = std::clamp(1.0f - particle.life / PARTICLE_LIFE, 0.0f, 1.0f);
		particle.alpha = 1.0f - Direct3D::EaseFunc["InOutExpo"](lifeRatio);

	}
	//全パーティクルが寿命切れならエフェクト終了
	if (allDead)
	{
		isFinished_ = true;
	}
}


void ExplosionEffect::Draw()
{
	int col[3];
	GetColor2(GetCharaColor(), &col[0], &col[1], &col[2]);

	if (isFinished_) return;

	for (auto& particle : particles_)
	{
		if (particle.life > 0.0f)
		{
			int r, g, b;
			GetColor2(GetCharaColor(), &r, &g, &b);
			//パーティクルの描写
			Vector2D drawPos = Math2D::Add(GetPos(), particle.Offset);
			Vector2D screenPos = Math2D::World2Screen(drawPos);
			int particleColor = GetColor(
				(int)(col[0] * particle.alpha),
				(int)(col[1] * particle.alpha),
				(int)(col[2] * particle.alpha)
			);
			DrawCircle((int)screenPos.x,
				(int)screenPos.y,
				particle.radius,
				particleColor);

		}
	}

}