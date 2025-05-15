#pragma once

#include "glm\mat3x3.hpp"
#include "glm\vec2.hpp"
#include "Application.h"
#include "Renderer2D.h"
#include <vector>
#include <iostream>

class Entity;  // Forward declaration
class Particle;

// Enum for the types of collision systems we support
enum CollisionType
{
	NONE,
	SIMPLE,
	SPATIAL_INDEX,
	END
};

// Externs so we can access these function from other files
void ChangeCollisionType();
const char* GetCollisionTypeString();
const CollisionType GetCollisionType();
extern CollisionType gCollisionType;

//Structure for extents for us in AABB
using namespace glm;
struct Extents
{
	vec2 min;
	vec2 max;
};

// Collision base class
class Collision2D
{
protected:
	glm::vec2 v2Pos;
public:
	Collision2D(glm::vec2 v2Pos): v2Pos(v2Pos){}
	virtual bool IsColliding(const Collision2D* otherCollision) = 0;
public:
	glm::vec2 GetPos() const { return v2Pos; }
};

// AABB collision class
class AABB : public Collision2D
{
	glm::vec2 v2HalfExtents;
public:
	AABB(glm::vec2 v2Pos, glm::vec2 v2HalfExtents) : Collision2D(v2Pos), v2HalfExtents(v2HalfExtents) {}
	virtual bool IsColliding(const Collision2D* pOtherCollision) override;
	const Extents GetExtents() const;
};

// CIrcle Collision class
class Circle : public Collision2D
{
	float fRadius;
public:
	Circle(glm::vec2 v2Pos, float fRadius) : Collision2D(v2Pos), fRadius(fRadius) {}
	virtual bool IsColliding(const Collision2D* pOtherCollision) override;
	float getRadius() const { return fRadius; }
};

// Spatial index class.  If we had different types of spatial partiioning then we could probably
// have a base class for SpatialPartition and inherit common properties for our different flavours
class SpatialIndex
{
	int iXResolution;
	int iYResolution;
	vec2 vMinCoord;
	vec2 vMaxCoord;
	Entity** pEntities = nullptr;
public:
	SpatialIndex(int iXResolution, int iYResolution, vec2 vMinCoord, vec2 vMaxCoord);
	~SpatialIndex();
	ivec2 getGridIndex(vec2 coord);
	void clear();
	void add(Entity* pNewEntity);
	Entity* getCell(Entity* entity);
	int getCells(Entity* pEntity, float radius, Entity* cells[]);
	void DebugRender(aie::Renderer2D* pRenderer);
	void RefillIndex(const std::vector<Particle*>& vPEntities);
};