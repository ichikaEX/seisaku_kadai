#pragma once
#include "Base.h"
#include <vector>

class ExplosionEffect :
	public Base
{
public:
	ExplosionEffect(const Vector2D& pos, int particlaeCount = 20);
	~ExplosionEffect() = default;
	void Update() override;
	void Draw() override;
	bool IsFinished() const { return isFinished_; }

	struct Particle
	{
		Vector2D Offset;//位置
		Vector2D vel;//速度
		float life;//寿命
		float radius;//半径
		float alpha;//不透明度
	};
private:
	bool isFinished_;
	std::vector<Particle>particles_;

};

