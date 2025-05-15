#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "Entity.h"
#include <vector>
#include <fstream>
#include <sstream>
class SpatialIndex;

class Application2D : public aie::Application {
public:

	Application2D();
	virtual ~Application2D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();
	void AddParticles(int targetNumberParticles, int iSpatialX, int iSpatialY);
	void DeleteParticles();
protected:

	std::vector<Entity*> vPEntities;
	std::vector<Wall*>	vPWalls;
	std::vector<Particle*> vPParticles;
	aie::Renderer2D*	p2dRenderer = nullptr;;
	aie::Texture*		pParticleTexture = nullptr;;
	aie::Font*			pFont = nullptr;;
	SpatialIndex*		pSpatialIndex = nullptr;
	unsigned int		iSpatialIndexResX = 0;
	unsigned int		iSpatialIndexResY = 0;
	float m_timer;
private:
	int iNumberParticles = 0;
	int iTargetNumberParticles = 0;
	const float fSecondsPerTest = 10.0f;
	std::ofstream testResultsFile;
	float fTestTimer;
	int iFrameCountForTest;
	bool ResetParticleSimulation();
	const char* pResultsFileName = "dataSP.csv";
};