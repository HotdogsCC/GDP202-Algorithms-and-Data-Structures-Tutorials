#include "Application2D.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "glm\mat3x3.hpp"

Application2D::Application2D() {

}

Application2D::~Application2D() {

}

void Application2D::AddParticles(int targetNumberParticles, int iSpatialX, int iSpatialY )
{
	int iParticleRadius = 8;
	int iHeight = aie::Application::getWindowHeight() -  (20 + (iParticleRadius*2));
	int iWidth = aie::Application::getWindowWidth() - (20 + (iParticleRadius*2));
	pSpatialIndex = new SpatialIndex(iSpatialX, iSpatialY, { 0,0 }, { aie::Application::getWindowWidth() , aie::Application::getWindowHeight() });
	iNumberParticles = 0; // The actual number of particles we have been able to spawn (maybe less than the target)
	for (int i = 0; i < targetNumberParticles; i++)
	{
		vec2 vPos = { 0,0 };
		int goodPos = true;
		int maxTries = 100;
		do
		{
			goodPos = true;
			vPos = { 10 + iParticleRadius + (rand() % (iWidth)), 10 + iParticleRadius + (rand() % iHeight) };
			for (auto pParticle : vPParticles)
			{
				Circle circleCollider(vPos, pParticle->getRadius());
				AABB newCollider(vPos, { pParticle->getRadius(),pParticle->getRadius()});
				if (pParticle->Colliding(circleCollider))
				{
					goodPos = false;
					break;
				}
			}
			maxTries--;
			if (goodPos) maxTries = 0;
		} while (maxTries>0);
		if (goodPos)
		{
			float fRotation = ((float)rand() / RAND_MAX) * 2 * 3.14159f;
			Particle* pNewParticle = new Particle(vPos, fRotation, (float)iParticleRadius, pParticleTexture, vPParticles, vPWalls, pSpatialIndex);
			vPParticles.push_back(pNewParticle);
			iNumberParticles++;
		}
	}
}

// Delete all the particles
void Application2D::DeleteParticles()
{
	// Delete the previous spatial index
	if (pSpatialIndex != nullptr)
	{
		delete pSpatialIndex;
		pSpatialIndex = nullptr;
	}

	// Free all the memory used by the particle instances
	for (auto pParticle : vPParticles)
	{
		assert(pParticle != nullptr);
		delete(pParticle);
	}

	vPParticles.clear();  // Clear the vector of particles
}

// Reset the particle system, so we can easily change the parameters of the system whilst the simulation is running
bool Application2D::ResetParticleSimulation()
{
	iFrameCountForTest = 0;
	iTargetNumberParticles = 400;
	iSpatialIndexResX = 2;
	iSpatialIndexResY = 2;
	DeleteParticles();  // Delete all the particles from the previous run
	AddParticles(iTargetNumberParticles, iSpatialIndexResX, iSpatialIndexResY);
	return true;
}

// Set up the simulation
bool Application2D::startup() {
	// Open a file for our results to be written to
	testResultsFile.open(pResultsFileName);
	fTestTimer = fSecondsPerTest;
	int seed = (int)time(NULL);
	srand(seed);
	p2dRenderer = new aie::Renderer2D();
	pParticleTexture = new aie::Texture("./textures/ball.png");
	pFont = new aie::Font("./font/consolas.ttf", 32);
	m_timer = 0;
	// Add bounding walls to our simulation to keep the particles in
	int height = aie::Application::getWindowHeight();
	int width = aie::Application::getWindowWidth();	
	Wall* pWall = new Wall({ width/2,5 }, { width , 10 });
	vPWalls.push_back(pWall);
	vPEntities.push_back(pWall);
	pWall = new Wall({ width / 2,height - 5 }, { width , 10 });
	vPWalls.push_back(pWall);
	vPEntities.push_back(pWall);
	pWall = new Wall({5, height / 2}, { 10 , height });
	vPWalls.push_back(pWall);
	vPEntities.push_back(pWall);
	pWall = new Wall({ width- 5, height / 2 }, { 10 , height });
	vPWalls.push_back(pWall);
	vPEntities.push_back(pWall);
	// Set up the simulation
	ResetParticleSimulation();
	return true;
}

void Application2D::shutdown() 
{	
	delete pFont;
	delete p2dRenderer;
	DeleteParticles();
	testResultsFile.close();
}

bool bChangeCollisionTypeDown = false;

void Application2D::update(float deltaTime) {

	fTestTimer -= deltaTime;
	iFrameCountForTest++; // count how many actual updates there where for this simulation run so we can accurately calculate the average frame rate
	if (fTestTimer <= 0)
	{
		fTestTimer = fSecondsPerTest;
		// write out the results
		char cBuffer[32];
		sprintf_s(cBuffer, 32, "%i, %i, %i \n", iTargetNumberParticles, iSpatialIndexResX, iFrameCountForTest);
		testResultsFile << cBuffer;
		if (!ResetParticleSimulation())
		{
			quit();
			return;
		}
	}

	m_timer += deltaTime;
	// Update the particles in the simulation
	for (Entity* particle : vPParticles)
	{
		particle->UpDate(deltaTime);
	}
	// This could be done more efficiently by only updating the particles which have changed cells
	pSpatialIndex->RefillIndex(vPParticles);
	aie::Input* input = aie::Input::getInstance();
	if (input->isKeyDown(aie::INPUT_KEY_C))
	{
		if (!bChangeCollisionTypeDown)
		{
			ChangeCollisionType();
			bChangeCollisionTypeDown = true;
		}
	}
	else
	{
		bChangeCollisionTypeDown = false;
	}

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void Application2D::draw() {

	const unsigned int gLineSpacing = 32;
	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	p2dRenderer->begin();
	for (Entity* entity : vPEntities)
	{
		entity->Render(p2dRenderer);
	}

	for (Entity* particle : vPParticles)
	{
		particle->Render(p2dRenderer);
	}
	char cBuffer[32];
	unsigned y = aie::Application::getWindowHeight();
	p2dRenderer->setRenderColour(1, 1, 1, 1);
	sprintf_s(cBuffer, 32, "FPS: %i", getFPS());
	p2dRenderer->drawText(pFont, cBuffer, 0, (float)(y -= gLineSpacing));
	sprintf_s(cBuffer, 32, "Particles: %i", iNumberParticles);
	p2dRenderer->drawText(pFont, cBuffer, 0, (float)(y -= gLineSpacing));
	p2dRenderer->drawText(pFont, GetCollisionTypeString(), 0, (float)(y -= gLineSpacing));

	p2dRenderer->drawText(pFont, "Press ESC to quit!", 0, (float)(y -= gLineSpacing));
	if (GetCollisionType() == CollisionType::SPATIAL_INDEX)
	{
		pSpatialIndex->DebugRender(p2dRenderer);
	}

	// done drawing sprites
	p2dRenderer->end();
}