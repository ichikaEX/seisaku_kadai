#pragma once
#include "Base.h"
#include <vector>

class Enemy :
    public Base
{
public:
    enum Size
    {
        LARGE,
        MEDIUM,
        SMALL
    };
    Enemy(int segment);
    Enemy(Size size, int segment);
    Enemy(const Vector2D& pos, const Vector2D& vel, Size size, int segment);

    void Update() override;
    void Draw() override;
    bool IsAlive() { return isAlive_; }
	float GetCollisionRadius() const { return radius_; }
	void Dead() { isAlive_ = false; }
    Size GetSize() { return size_; }
    Size checkSize()const;
private:
    void MakeShape();//’¸“_À•W‚Ì‰Šú‰»
    float RandomRadius(Size size);
private:
    int segment_;//‰~‚Ì•ªŠ„”
    float radius_;//”¼Œa
    float omega_;
    float angle_;

    Size size_;

    bool isAlive_;
    std::vector<Vector2D>vertex_;//‰~‚Ì’¸“_À•W
};
