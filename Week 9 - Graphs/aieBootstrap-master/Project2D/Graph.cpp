#include "Graph.h"
#include "Font.h"
#include "Renderer2D.h"
#include <glm/detail/func_geometric.hpp>

using glm::vec2;



//draws the edge
void Edge::DebugDraw(aie::Renderer2D* pRenderer, aie::Font* pFont) const
{
	//how big the arrow is graphically
	const float arrowSize = 10.0f;

	//getting initial positions
	vec2 startPosition = pStart->GetPos();
	vec2 endPosition = pEnd->GetPos();

	//getting direction vector
	vec2 startToEndVector = endPosition - startPosition;
	vec2 directionVector = glm::normalize(startToEndVector);

	//generates offsets (so arrows dont start in the centre of the node)
	vec2 offset = pStart->GetNodeRadius() * directionVector;
	startPosition += offset;
	endPosition -= offset;

	//makes the arrow head
	vec2 arrowBase = endPosition - offset;
	vec2 tangent = { -directionVector.y, directionVector.x };
	vec2 arrowEnd1 = arrowBase + (tangent * arrowSize);
	vec2 arrowEnd2 = arrowBase - (tangent * arrowSize);

	//makes the lines green
	pRenderer->setRenderColour(0, 1, 0, 1);

	//draws the arrow head
	pRenderer->drawLine(endPosition.x, endPosition.y, arrowEnd1.x, arrowEnd1.y, 2);
	pRenderer->drawLine(endPosition.x, endPosition.y, arrowEnd2.x, arrowEnd2.y, 2);

	//makes the lines dark green
	pRenderer->setRenderColour(0, 0.6f, 0);

	//draws the arrow
	pRenderer->drawLine(startPosition.x, startPosition.y, endPosition.x, endPosition.y, 2);

	//resets colour back to white
	pRenderer->setRenderColour(1, 1, 1);

}

Node::~Node()
{
	for (auto pEdge : edges)
	{
		delete(pEdge); // delete the edges attached to the node
	}
}

void Node::AddEdge(const Edge* edge)
{
	edges.push_back(edge);
}

void Node::DebugDraw(aie::Renderer2D* pRenderer, aie::Font* pFont)
{
	vec2 position = GetPos();

	pRenderer->setRenderColour(0, 0, 1, 1);
	pRenderer->drawCircle(position.x, position.y, GetNodeRadius());

	pRenderer->setRenderColour(1, 1, 1, 1);
	char cBuffer[32];
}