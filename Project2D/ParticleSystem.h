#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include <vector>
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "DynamicClass.h"

using namespace glm;
using namespace std;
class ParticleSprite
{
	vec2 mPosition;
	vec2 mVelocity;
	vec2 mSize;
	vec4 mColour;
	float mTime;
	float mRotation;

public:
	ParticleSprite(
		vec2 Pos,
		vec2 Vel,
		vec2 Size,
		vec4 Colour,
		float TimeLeft,
		float Rotation)
	{
		mPosition = Pos;
		mVelocity = Vel;
		mSize = Size;
		mColour = Colour;
		mTime = TimeLeft;
		mRotation = Rotation;
	}

	void Draw(aie::Renderer2D* const m_2dRenderer, aie::Texture* texture);
	void Update(const float deltaTime, const vec4& colourDelta, const vec2& velocityDelta, const vec2& sizeDelta, const float rotationDelta);
	bool isActive();
};

class ParticleSystem
{
	int mNumberParticles;
	vec2 mSpawnPosition;
	float mStartingSpeed;
	float mStartingSize;
	vec4 mStartingColour;
	float mParticleDuration;
	float mSpawnRate;
	float mSpawnTimer;
	int mNumberUpdates;
	double mAverage;
	aie::Texture* mTexture;
	DynamicClass<ParticleSprite> mParticles;

	void SpawnParticle();
	
public:
	ParticleSystem(
		int NumberParticles,
		vec2 SpawnPosition,
		float StartingSpeed,
		float StartingSize,
		vec4 StartingColour,
		float ParticleDuration,
		aie::Texture* Texture
	)
	{
		mNumberParticles = NumberParticles;
		mSpawnPosition = SpawnPosition;
		mStartingSpeed = StartingSpeed;
		mStartingSize = StartingSize;
		mStartingColour = StartingColour;
		mParticleDuration = ParticleDuration;
		mTexture = Texture;
		mSpawnRate = ParticleDuration / (float)NumberParticles;
		mSpawnTimer = 0.0f;
		mAverage = 0.0f;
		mNumberUpdates = 0;
	}

	void Update(float deltaTime);
	void Draw(aie::Renderer2D* renderer);
};


