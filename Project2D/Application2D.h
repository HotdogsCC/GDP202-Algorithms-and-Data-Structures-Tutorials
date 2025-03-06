#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "ParticleSystem.h"

class Application2D : public aie::Application {
public:

	Application2D();
	virtual ~Application2D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Texture*		m_texture;
	aie::Texture*		m_shipTexture;  
	aie::Texture* m_amongUsTexture;
	aie::Font*			m_font;
	ParticleSystem* m_particleSystem;
	

	float m_timer;
private:
	float shipX = 600;
	float shipY = 400;
};