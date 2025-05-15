#include "Collision2D.h"
#include "Entity.h"

using namespace glm;

CollisionType gCollisionType = CollisionType::SIMPLE; // horrid global variable because I can't be bothered to pass it in

void ChangeCollisionType()
{
	int collInt = (int)gCollisionType;
	collInt++;
	if (collInt == CollisionType::END)
	{
		collInt = 0;
	}
	gCollisionType = (CollisionType)collInt;
}

const char* GetCollisionTypeString()
{
	switch (gCollisionType)
	{
	case SIMPLE:
		return "Collision = Simple (C to change)";
		break;
	case SPATIAL_INDEX:
		return "Collision = Spatial Index (C to change)";
		break;
	case NONE:
		return "Collision = None (C to change)";
		break;
	default:
		break;
	}
}

const CollisionType GetCollisionType()
{
	return gCollisionType;
}

//Is this AABB colliding with another collider?
bool AABB::IsColliding(const Collision2D* pOtherCollision)
{
	// Try casting other collider to AABB
	const AABB* pAABB = dynamic_cast<const AABB*>(pOtherCollision);
	// Perform AABB to AABB if other collider is AABB
	if (pAABB != nullptr)
	{
		const Extents thisExtents = GetExtents();
		const Extents otherExttents = pAABB->GetExtents();
		if (thisExtents.max.x < otherExttents.min.x) return false;
		if (thisExtents.min.x > otherExttents.max.x) return false;
		if (thisExtents.max.y < otherExttents.min.y) return false;
		if (thisExtents.min.y > otherExttents.max.y) return false;
		return true;
	}
	// Try casting other collider to Circle
	const Circle* pCircle = dynamic_cast<const Circle*>(pOtherCollision);
	// Perform AABB to Circle if other collider is Circle
	if (pCircle != nullptr)
	{
		Extents extent = GetExtents();
		vec2 clampPos;
		vec2 circleCentre = pCircle->GetPos();
		clampPos.x = clamp(circleCentre.x, extent.min.x, extent.max.x);
		clampPos.y = clamp(circleCentre.y, extent.min.y, extent.max.y);
		vec2 delta = circleCentre - clampPos;
		float distance = glm::length(delta);
		if (distance < pCircle->getRadius())
		{
			return true;
		}
		return false;
	}
	return false;
}

// Get the extents for an AABB collision volume
const Extents AABB::GetExtents() const
{
	Extents extents;
	extents.min = v2Pos - v2HalfExtents;
	extents.max = v2Pos + v2HalfExtents;
	return extents;
}

// Is this circle colliding with another collider?
bool Circle::IsColliding(const Collision2D* pOtherCollision)
{
	const AABB* pAABB = dynamic_cast<const AABB*>(pOtherCollision);
	if (pAABB != nullptr)
	{
		Extents extent = pAABB->GetExtents();
		vec2 clampPos;
		clampPos.x = clamp(v2Pos.x, extent.min.x, extent.max.x);
		clampPos.y = clamp(v2Pos.y, extent.min.y, extent.max.y);
		vec2 delta = v2Pos - clampPos;
		float distance = glm::length(delta);
		if (distance < fRadius)
		{
			return true;
		}
		return false;
	}
	const Circle* pCircle = dynamic_cast<const Circle*>(pOtherCollision);
	if (pCircle != nullptr)
	{
		float distance = glm::distance(pOtherCollision->GetPos(), v2Pos);
		if (distance < fRadius + pCircle->getRadius())
		{
			return true;
		}
		return false;
	}
	return false;
}

//Constructor, takes the resolution of the spatial index array and the resolution of the simulation world
SpatialIndex::SpatialIndex(int iXResolution, int iYResolution, vec2 vMinCoord, vec2 vMaxCoord) : iXResolution(iXResolution), iYResolution(iYResolution), vMinCoord(vMinCoord), vMaxCoord(vMaxCoord)
{
}

// Destructor
SpatialIndex::~SpatialIndex()
{
}

// Clears all elements in the spatial index
void SpatialIndex::clear()
{
}

// Convert a simualation world coordinate into a spatial index coordinate
ivec2 SpatialIndex::getGridIndex(vec2 pos)
{
	return { 0,0 };
}

// Get the pointer from the cell that an entities centre is in
Entity* SpatialIndex::getCell(Entity* pEntity)
{
	return nullptr;
}

// Get the pointers from any cells the entity overlaps
int SpatialIndex::getCells(Entity* pEntity, float radius, Entity* cells[])
{
	return 0;
}

// Add am entity to the spatial index
void SpatialIndex::add(Entity* pEntity)
{
}

// Debug renderer so we can see the cell boundaries
void SpatialIndex::DebugRender(aie::Renderer2D* m_2dRenderer)
{
}

// Clears and refills the spatial index with a list of entities
void SpatialIndex::RefillIndex(const std::vector<Particle*>& vPEntities)
{
}