#define _USE_MATH_DEFINES
#include "ParticleSystem.h"

void ParticleSystem::SpawnParticle()
{
	vec2 startSize(mStartingSize);
	float angle = (float)((std::rand() / (float)RAND_MAX) * M_PI * 2.0);
	float speed = mStartingSize * (1.0f + (std::rand() / (float)RAND_MAX)) * 0.5f;
	vec2 startVelocity(sin(angle) * speed, cos(angle) * speed);

	ParticleSprite* pNewParticle = new ParticleSprite(mSpawnPosition + (startVelocity * 0.1f), startVelocity, startSize, mStartingColour, mParticleDuration, 0);
	return this->mParticles.push_back(pNewParticle);

}

void ParticleSystem::Update(float deltaTime)
{
	//check if it is time to spawn another particle
	mSpawnTimer -= deltaTime;
	while (mSpawnTimer < 0)
	{
		mSpawnTimer += mSpawnRate;
		SpawnParticle();
	}

	//update all the particles
	for (ParticleSprite* particle : mParticles)
	{
		particle->Update(deltaTime, { 0,0,0,-0.01 * deltaTime }, { deltaTime * 10, deltaTime* 10 }, { 0,0 }, 0.001f);
	}

	//check for particles that need to be YEETED (deleted)
	for (auto i = mParticles.begin(); i < mParticles.end(); i++)
	{
		if (!(*i)->isActive())
		{
			delete(*i);
			i = mParticles.erase(i);
		}
	}
}

void ParticleSystem::Draw(aie::Renderer2D* renderer)
{
	for (ParticleSprite* particle : mParticles)
	{
		particle->Draw(renderer, mTexture);
	}
}

void ParticleSprite::Update(
	const float deltaTime, 
	const vec4& colourDelta, 
	const vec2& velocityDelta, 
	const vec2& sizeDelta, 
	const float rotationDelta)
{
	mTime -= deltaTime;
	mColour += colourDelta;
	mVelocity += velocityDelta;
	mSize += sizeDelta;
	mRotation += rotationDelta;
	mPosition += mVelocity * deltaTime;

	
}

void ParticleSprite::Draw(aie::Renderer2D* const m_2dRenderer, aie::Texture* texture)
{
	m_2dRenderer->setRenderColour(mColour.r, mColour.g, mColour.b, mColour.a);
	m_2dRenderer->drawSprite(texture, mPosition.x, mPosition.y, mSize.x, mSize.y, mRotation);
}

bool ParticleSprite::isActive()
{
	return (mTime > 0) && (mColour.a > 0);
}