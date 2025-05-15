#pragma once
#include "glm\mat3x3.hpp"
#include "glm\vec2.hpp"
#include "Application.h"
#include "Renderer2D.h"
#include <iostream>
#include <vector>
#include "Collision2D.h"

using namespace glm;

// Forward declarations
class Wall;
class Bullet;

class Entity
{
protected:
	vec2 v2Pos;
	float fRotation;
	aie::Texture* pTexture;
public:
	Entity()
	{
		v2Pos = { 0, 0 };
		fRotation = 0;
		pTexture = nullptr;
	}
	Entity(vec2 v2Pos, float fRotation, aie::Texture* pTexture) : v2Pos(v2Pos), fRotation(fRotation), pTexture(pTexture), nextInChain(nullptr)
	{
	}
	virtual void UpDate(float delta) = 0;
	virtual void Render(aie::Renderer2D* pRenderer);
	virtual bool Colliding(const Collision2D& other) { return false; }
	virtual void getCollisionData(const Collision2D& other, vec2& collisionVector);
//	void changeCollisionType();
	vec2 GetPos() { return v2Pos;}
	Entity* nextInChain;
};

class Wall : public Entity
{
	vec2 v2Dimensions;
public:
	Wall(vec2 v2Pos, vec2 dimensions) : Entity(v2Pos, 0, nullptr) , v2Dimensions(dimensions) {}
	virtual void Render(aie::Renderer2D* pRenderer) override;
	virtual void UpDate(float delta) {};
	virtual bool Colliding(const Collision2D& other) override;
	bool isHorizontal() { return v2Dimensions.x > v2Dimensions.y; }
	vec2 getDelta(const vec2 other) { return v2Pos - other;}
};

class Particle : public Entity
{
	vec3 vColour;
	const float fRadius; // particle is square or round
	const std::vector<Particle*>& vParticles;
	const std::vector<Wall*>& vWalls;
	SpatialIndex* pSpatialIndex;
	const float fSpeed = 80.0f;
	vec2 v2Velocity;
	void doWallCollision(float delta);
	void doPartlicleSimpleCollision(float delta);
	void doPartlicleSpatialIndexCollision(float delta);
	virtual void getCollisionData(const Collision2D& other, vec2& collisionVector) override;
public:
	Particle(vec2 v2Pos, float fRotation, float fRadius, aie::Texture* texture, std::vector<Particle*>& vParticles, std::vector<Wall*>& vWall, SpatialIndex* pSpatialIndex);
	virtual void UpDate(float delta) override;
	virtual void Render(aie::Renderer2D* pRenderer) override;
	virtual bool Colliding(const Collision2D& other) override;
	float getRadius() { return fRadius; }

};