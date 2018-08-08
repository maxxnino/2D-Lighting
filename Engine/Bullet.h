#pragma once
#include "Graphics.h"
#include "Mouse.h"
#include "BulletState.h"
#include <memory>
class Actor;
class Bullet
{
public:
	//update game loop
	void Draw(Graphics& gfx) const
	{
		gfx.DrawRect({ int(pos.x - size), int(pos.y - size), int(pos.x + size),  int(pos.y + size) }, color);
	}
	void Update(float dt)
	{
		bulletState->excute(this, dt);
		if (pos.x - size < 0.0f)
		{
			isDead = true;
			return;
		}
		if (pos.y - size < 0.0f)
		{
			isDead = true;
			return;
		}
		if (pos.x + size >= Graphics::ScreenWidth)
		{
			isDead = true;
			return;
		}
		if (pos.y + size >= Graphics::ScreenHeight)
		{
			isDead = true;
			return;
		}
	}
	//update state
	virtual void OnStart(float dt) = 0;
	virtual void OnFlying(float dt) = 0;
	virtual void OnImpact(float dt) = 0;
	void ChangeState(std::unique_ptr<BulletState> newState)
	{
		bulletState = std::move(newState);
	}
	bool CheckCollision(std::vector<Actor>& enemies) const;
	const bool& GetIsRemove() const
	{
		return isDead;
	}
	const bool& GetIsImpact() const
	{
		return isImpact;
	}
protected:
	Bullet(VecF pos, float size, VecF dir, float speed, Color color, std::unique_ptr<BulletState> bulletState)
		:
		pos(pos), size(size), dir(dir), speed(speed), color(color), bulletState(std::move(bulletState))
	{}
	std::unique_ptr<BulletState> bulletState;
	VecF pos;
	float size;
	VecF dir;
	float speed;
	Color color;
	bool isDead = false;
	bool isImpact = false;
};

class CycloneBullet : public Bullet
{
public:
	CycloneBullet(VecF pos, float size, VecF dir, float speed, Color color, std::unique_ptr<BulletState> bulletState = std::make_unique<StateStart>())
		:
		Bullet(pos, size, dir, speed, color, std::move(bulletState)),
		cycloneDir(dir)
	{}
private:
	virtual void OnStart(float dt) override;
	virtual void OnFlying(float dt) override;
	virtual void OnImpact(float dt) override;
private:
	float alpha = 3.14f / 30.0f;
	float cosAlpha = cosf(alpha);
	float sinAlpha = sinf(alpha);
	float curTimeImpact = 0.0f;
	float EndTimeImpact = 2.0f;
	VecF cycloneDir;
};