#include "Entity.h"
#include "Input.h"
#include "Collision2D.h"
#include "Texture.h"


using namespace glm;

// Entitiy renderer for base class
void Entity::Render(aie::Renderer2D* m_2dRenderer)
{
	assert(pTexture != nullptr); //pTexcture should not be null
	m_2dRenderer->setRenderColour(1, 1, 1, 1);
	m_2dRenderer->setUVRect(0, 0, 1, 1);
	m_2dRenderer->drawSprite(pTexture, v2Pos.x, v2Pos.y, 0, 0, fRotation, 1);
}

void Entity::getCollisionData(const Collision2D& other, vec2& collisionVector)
{
	// base doesn't do anything in the base class
}

// Wall rendering code
void Wall::Render(aie::Renderer2D* m_2dRenderer)
{
	m_2dRenderer->setRenderColour(1, 0, 0, 1);
	m_2dRenderer->drawBox(v2Pos.x, v2Pos.y, v2Dimensions.x, v2Dimensions.y);
}

// Wall is AABB
bool Wall::Colliding(const Collision2D& other)
{
	AABB wallAABB(v2Pos, v2Dimensions * 0.5f);
	return wallAABB.IsColliding(&other);
}

//Particle Constructor
Particle::Particle(vec2 v2Pos, float fRotation, float fRadius, aie::Texture* texture, std::vector<Particle*>& vParticles, std::vector<Wall*>& vWalls, SpatialIndex* pSpatialIndex) : vParticles(vParticles), pSpatialIndex(pSpatialIndex), vWalls(vWalls), fRadius(fRadius), Entity(v2Pos, fRotation, texture)
{
	// Initial velocity is speed rotated by the particles initial rotation
	v2Velocity = { sin(fRotation) * fSpeed,cos(fRotation) * fSpeed };
	// Set particles colour based on it's position so we can see particles moving
	vColour = {v2Pos.x / 500.f, v2Pos.y / 200.f , v2Pos.x / 500.f };
}

// Particle collision is circle
bool Particle::Colliding(const Collision2D& other)
{
	Circle circleCollider(v2Pos, fRadius);
	return circleCollider.IsColliding(&other);
}

// Simple sprite particle renderer
void Particle::Render(aie::Renderer2D* m_2dRenderer)
{
	assert(pTexture != nullptr); //pTexcture should not be null
	m_2dRenderer->setRenderColour(vColour.x, vColour.y, vColour.z,1);
	m_2dRenderer->setUVRect(0, 0, 1, 1);
	m_2dRenderer->drawSprite(pTexture, v2Pos.x, v2Pos.y, fRadius * 2, fRadius * 2, fRotation, 1);
}

// Check particle collision with walls
void Particle::doWallCollision(float delta)
{
	Circle circleCollider(v2Pos, fRadius);
	// check against asll walls
	for (auto wall : vWalls)
	{
		// Is particle colliding with one of the wall?
		if (wall->Colliding(circleCollider))
		{
			// Following code reflects the particle vector back from the wall
			// I couldn't be bothered to get a normal for the wall and do the full collision
			// overlap between particle and wall
			vec2 delta = wall->getDelta(v2Pos);
			// If the wall is horizontal then flip the Y component of the velocity
			if (wall->isHorizontal())
			{
				// Above or below wall?
				if (delta.y > 0)
				{
					if (v2Velocity.y > 0)
					{
						v2Velocity.y *= -1;
					}
				}
				else
				{
					if (v2Velocity.y < 0)
					{
						v2Velocity.y *= -1;
					}
				}
			}
			else
			// If the wall is vertical then flip the X component of the velocity
			{
				// Right or left of wall?
				if (delta.x > 0)
				{
					if (v2Velocity.x > 0)
					{
						v2Velocity.x *= -1;
					}
				}
				else
				{
					if (v2Velocity.x < 0)
					{
						v2Velocity.x *= -1;
					}
				}
			}
		}
	}

}

// Simple version of collision where we test every particle agains every other particle O(n squared)
void Particle::doPartlicleSimpleCollision(float delta)
{
	Circle circleCollider(v2Pos, fRadius);
	// check against walls
	for (auto particle : vParticles)
	{
		if (particle != this)
		{
			if (particle->Colliding(circleCollider))
			{
				vec2 collisionVector;
				particle->getCollisionData(circleCollider, collisionVector);
				float closingSpeed = glm::dot(collisionVector, v2Velocity);
				if (closingSpeed > 0)
				{
					v2Velocity -= 2.0f * closingSpeed * collisionVector;

				}
			}
		}
	}
}

// Spatial Index collision for particles
void Particle::doPartlicleSpatialIndexCollision(float delta)
{
	Circle circleCollider(v2Pos, fRadius);
	// check against walls
	Entity* cellsOverlapped[4]; 
	int numberCellsOverlaped = pSpatialIndex->getCells(this, this->getRadius() * 2, cellsOverlapped);
	for (int cellIndex = 0; cellIndex < numberCellsOverlaped; cellIndex++)
	{
		Entity* pEntity = cellsOverlapped[cellIndex];
		while (pEntity != nullptr)
		{
			if (pEntity->Colliding(circleCollider))
			{
				// If this entitiy is colliding with another entity in this cell
				vec2 collisionVector;
				pEntity->getCollisionData(circleCollider, collisionVector);
				float closingSpeed = glm::dot(collisionVector, v2Velocity);
				if (closingSpeed > 0)
				{
					v2Velocity -= 2.0f * closingSpeed * collisionVector;
				}
			}
			pEntity = pEntity->nextInChain;
		}
	}
}

// Update particle
void Particle::UpDate(float delta)
{
	// Euler integration
	v2Pos += delta * v2Velocity;
	// check particle collision with walls
	doWallCollision(delta);
	// check particle collision with other particles
	switch (gCollisionType)
	{
	case SIMPLE:
		doPartlicleSimpleCollision(delta);
		break;
	case SPATIAL_INDEX:
		doPartlicleSpatialIndexCollision(delta);
		break;
	default:
		break;
	}
}

void Particle::getCollisionData(const Collision2D& other, vec2& collisionVector)
{
	const Circle* pCircle = dynamic_cast<const Circle*> (&other);
	if (pCircle != nullptr)
	{
		vec2 delta = v2Pos - other.GetPos(); // displacement vector between centre of the two collision circles
		collisionVector = glm::normalize(delta); // normalize the displacement vector to get the collision vector length 1
	}
}