#define _USE_MATH_DEFINES
#include "ParticleSystem.h"
#include <chrono>
#include <iostream>

void ParticleSystem::SpawnParticle()
{
	vec2 startSize(mStartingSize);
	float angle = (float)((std::rand() / (float)RAND_MAX) * M_PI * 2.0);
	float speed = mStartingSize * (1.0f + (std::rand() / (float)RAND_MAX)) * 0.5f;
	vec2 startVelocity(sin(angle) * speed, cos(angle) * speed);

	ParticleSprite newParticle(mSpawnPosition + (startVelocity * 0.1f), startVelocity, startSize, mStartingColour, mParticleDuration, 0);
	mParticles.addItem(newParticle);

}

void ParticleSystem::Update(float deltaTime)
{
	mNumberUpdates++; //used for checking how many times update is called
	auto start = std::chrono::high_resolution_clock::now(); //stores the value of the system clock at the start of update


	//check if it is time to spawn another particle
	mSpawnTimer -= deltaTime;
	while (mSpawnTimer < 0)
	{
		mSpawnTimer += mSpawnRate;
		SpawnParticle();
	}

	int updatecount = 0;
	//update all the particles
	for (ParticleSprite* particle : mParticles)
	{
		particle->Update(deltaTime, { 0,0,0,-0.01 * deltaTime }, { deltaTime * 10, deltaTime* 10 }, { 0,0 }, 0.001f);
		updatecount++;
		if (!particle->isActive())
		{
			mParticles.erase(particle);
		}
	}

	//check for particles that need to be YEETED (deleted)
	//for (auto i = mParticles.begin(); i < mParticles.end(); i++)
	//{
	//	if (!(*i)->isActive())
	//	{
	//		i = mParticles.erase(i);
	//	}
	//}

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = end - start;
	mAverage = (mAverage * ((double)mNumberUpdates - 1) + (double)duration.count()) / (double)mNumberUpdates;
	std::cout << "Number Particles: " << updatecount << ", Update took: " << mAverage << "\n";
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

ParticleSprite::ParticleSprite()
{
	mColour = { 0, 0, 0, 0 };
	mPosition = { 0, 0 };
	mRotation = 0;
	mSize = { 0, 0 };
	mTime = 0;
	mVelocity = { 0, 0 };
}