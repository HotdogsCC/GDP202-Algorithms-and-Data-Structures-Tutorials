#include "Application2D.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Graph.h"

Application2D::Application2D() {

}

Application2D::~Application2D() {

}

bool Application2D::startup() {
	
	m_2dRenderer = new aie::Renderer2D();

	m_texture = new aie::Texture("./textures/numbered_grid.tga");
	m_shipTexture = new aie::Texture("./textures/ship.png");

	m_font = new aie::Font("./font/consolas.ttf", 32);
	
	m_timer = 0;

	pGraph = new GraphEdgeAdjacencyEdgeImplementation();
	pGraph->AddNode(0, { 200,300 });
	pGraph->AddNode(1, { 600,300 });
	pGraph->AddNode(2, { 400,700 });
	pGraph->AddNode(3, { 1000,0 });
	pGraph->AddEdge(0, 1, 1);
	pGraph->AddEdge(0, 2, 1);
	pGraph->AddEdge(0, 3, 1);
	pGraph->AddEdge(1, 2, 1);
	pGraph->AddEdge(2, 1, 1);
	pGraph->AddEdge(2, 3, 1);
	pGraph->AddEdge(3, 0, 1);
	pGraph->AddEdge(3, 1, 1);
	pGraph->AddEdge(3, 2, 1);


	return true;
}

void Application2D::shutdown() {
	
	delete m_font;
	delete m_texture;
	delete m_shipTexture;
	delete m_2dRenderer;
}

void Application2D::update(float deltaTime) {

	m_timer += deltaTime;

	// input example
	aie::Input* input = aie::Input::getInstance();

	// Update the camera position using the arrow keys
	float camPosX;
	float camPosY;
	m_2dRenderer->getCameraPos(camPosX, camPosY);

	if (input->isKeyDown(aie::INPUT_KEY_UP))
		camPosY += 500.0f * deltaTime;

	if (input->isKeyDown(aie::INPUT_KEY_DOWN))
		camPosY -= 500.0f * deltaTime;

	if (input->isKeyDown(aie::INPUT_KEY_LEFT))
		camPosX -= 500.0f * deltaTime;

	if (input->isKeyDown(aie::INPUT_KEY_RIGHT))
		camPosX += 500.0f * deltaTime;

	m_2dRenderer->setCameraPos(camPosX, camPosY);

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void Application2D::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	

	
	pGraph->debugDrawGraph(m_2dRenderer, m_font);
	// done drawing sprites
	m_2dRenderer->end();
}