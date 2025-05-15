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
		std::cout << "tweak if true";
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
	//get memory for our spatital index
	pEntities = new Entity * [iXResolution * iYResolution];
	assert(pEntities != nullptr);
	clear();
}

// Destructor
SpatialIndex::~SpatialIndex()
{
	delete[] pEntities;
}

// Clears all elements in the spatial index
void SpatialIndex::clear()
{
	assert(pEntities != nullptr);
	Entity** pEntity = pEntities;
	for (int i = 0; i < iXResolution * iYResolution; i++)
	{
		*pEntity++ = nullptr;
	}
}

// Convert a simualation world coordinate into a spatial index coordinate
ivec2 SpatialIndex::getGridIndex(vec2 pos)
{
	pos -= vMinCoord;
	pos.x /= (vMaxCoord.x - vMinCoord.x);
	pos.y /= (vMaxCoord.y - vMinCoord.y);
	pos = clamp(pos, { 0, 0 }, { 0.999f, 0.999f });
	int gridX = (int)(pos.x * iXResolution); //scale to get index in the output array
	int gridY = (int)(pos.y * iYResolution); //scale to get index in the output array

	assert(gridX < iXResolution);
	assert(gridY < iYResolution);

	return { gridX, gridY };

	//return { 0,0 };
}

// Get the pointer from the cell that an entities centre is in
Entity* SpatialIndex::getCell(Entity* pEntity)
{
	ivec2 gridCoord = getGridIndex(pEntity->GetPos());
	return *(pEntities + (gridCoord.y * iXResolution) + gridCoord.x);
	//return pEntity;

	//return nullptr;
}

// Get the pointers from any cells the entity overlaps
int SpatialIndex::getCells(Entity* pEntity, float radius, Entity* cells[])
{
	int cellsUsed = 0;
	// Bottom Left Quadtrant
	ivec2 gridCoord1 = getGridIndex(pEntity->GetPos() + vec2(-radius, -radius));
	cells[cellsUsed++] = *(pEntities + (gridCoord1.y * iXResolution) + gridCoord1.x);
	// Bottom Right Quadtrant
	bool spansx = false;
	ivec2 gridCoord2 = getGridIndex(pEntity->GetPos() + vec2(radius, -radius));
	// Check if the coordinate spans two cells acros the X
	if (gridCoord2.x != gridCoord1.x)
	{
		// Remember that this coordinate spans two cells across the X 
		// so that if it also spans 2 across the Y we can do the fourth one
		cells[cellsUsed++] = *(pEntities + (gridCoord2.y * iXResolution) + gridCoord2.x);
		spansx = true;
	}
	// Top Left Quadtrant
	ivec2 gridCoord3 = getGridIndex(pEntity->GetPos() + vec2(-radius, radius));
	// Check if the coordinate spans two cells acros the Y
	if (gridCoord3.y != gridCoord1.y)
	{
		cells[cellsUsed++] = *(pEntities + (gridCoord3.y * iXResolution) + gridCoord3.x);
		// if it also spans two across the X then we need to set the fourth quadrant
		if (spansx)
		{
			// Top Right Quadtrant
			ivec2 gridCoord4 = getGridIndex(pEntity->GetPos() + vec2(radius, radius));
			cells[cellsUsed++] = *(pEntities + (gridCoord4.y * iXResolution) + gridCoord4.x);
		}
	}

	return cellsUsed;
}

// Add am entity to the spatial index
void SpatialIndex::add(Entity* pEntity)
{
	ivec2 gridCoord = getGridIndex(pEntity->GetPos());
	Entity** pInsertionPoint = pEntities + (gridCoord.y * iXResolution) + gridCoord.x;
	if (*pInsertionPoint != nullptr)
	{
		pEntity->nextInChain = *pInsertionPoint;
	}
	else
	{
		pEntity->nextInChain = nullptr;
	}
	*pInsertionPoint = pEntity;

}

// Debug renderer so we can see the cell boundaries
void SpatialIndex::DebugRender(aie::Renderer2D* m_2dRenderer)
{
	vec2 spacing = (vMaxCoord - vMinCoord);
	spacing.x /= (float)iXResolution;
	spacing.y /= (float)iYResolution;
	m_2dRenderer->setRenderColour(1, 0, 0, 1);
	for (int x = 1; x < iXResolution; x++)
	{
		m_2dRenderer->drawLine(vMinCoord.x + (x * spacing.x), vMinCoord.y, vMinCoord.x + (x * spacing.x), vMaxCoord.y);
	}
	for (int y = 0; y < iYResolution; y++)
	{
		m_2dRenderer->drawLine(vMinCoord.x, vMinCoord.y + (y * spacing.y), vMaxCoord.x, vMinCoord.y + (y * spacing.y));
	}

}

// Clears and refills the spatial index with a list of entities
void SpatialIndex::RefillIndex(const std::vector<Particle*>& vPEntities)
{
	clear();
	for (auto pParticle : vPEntities)
	{
		add(pParticle);
	}

}