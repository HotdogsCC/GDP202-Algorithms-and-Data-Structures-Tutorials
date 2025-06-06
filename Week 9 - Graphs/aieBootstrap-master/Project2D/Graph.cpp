﻿#include "Graph.h"

#include <iostream>

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

	//draws the circle of the node
	pRenderer->setRenderColour(0, 0, 1, 1);
	if(bVisited)
	{
		pRenderer->setRenderColour(0, 0, 0.5f, 1);
	}
	pRenderer->drawCircle(position.x, position.y, GetNodeRadius());

	//draws text
	pRenderer->setRenderColour(1, 1, 1, 1);
	char cBuffer[32];
	sprintf_s(cBuffer, 32, "%i", payload);
	pRenderer->drawText(pFont, cBuffer, position.x - 8, position.y - 8);

	//draws lines coming from nodes
	for(const Edge* pEdge : edges)
	{
		pEdge->DebugDraw(pRenderer, pFont);
	}
	
}

void GraphEdgeAdjacencyEdgeImplementation::AddNode(int label, glm::vec2 v2Pos)
{
	Node* pNode = new Node(label, v2Pos);
	nodes.push_back(pNode);
}

GraphEdgeAdjacencyEdgeImplementation::~GraphEdgeAdjacencyEdgeImplementation()
{
	for (auto pNode : nodes)
	{
		if (pNode != nullptr)
		{
			delete(pNode);
		}
	}
}

void GraphEdgeAdjacencyEdgeImplementation::AddEdge(unsigned int startIndex, unsigned int endIndex, float fCost)
{
	//check the index of the start and end nodes are within bounds
	assert(startIndex < nodes.size() && endIndex < nodes.size());
	//get pointers to the nodes to make things clearer
	Node* pStartNode = nodes[startIndex];
	Node* pEndNode = nodes[endIndex];
	//get the physics distance between the nodes
	float distance = glm::distance(pStartNode->GetPos(), pEndNode->GetPos());
	//create the edge
	Edge* pEdge = new Edge(nodes[startIndex], nodes[endIndex], distance * fCost);
	//add it to the list of nodes at the start node
	pStartNode->AddEdge(pEdge);
}

void GraphEdgeAdjacencyEdgeImplementation::debugDrawGraph(aie::Renderer2D* renderer, aie::Font* pFont)
{
	for (auto pNode : nodes)
	{
		pNode->DebugDraw(renderer, pFont);
	}
}

bool GraphEdgeAdjacencyEdgeImplementation::DepthFirstSearch(
	const unsigned int startIndex, const unsigned int endIndex, bool bStartSearch)
{
	assert(startIndex < nodes.size() && endIndex < nodes.size());
	if(bStartSearch)
	{
		visited.clear();
		toVisitStack = {};
		toVisitStack.push(nodes[startIndex]);
		std::cout << "Depth First Search, starting from: ";
	}

	while(!toVisitStack.empty())
	{
		// add the next node to the visited list, so we don't push it again
		Node* pNextNode = toVisitStack.top(); //gets a copy from the top of the stack
		toVisitStack.pop(); // pop the node of the stack
		//set the nodes visited flag to it changes colour for debug
		pNextNode->SetVisited(true);

		//checks if we reached where we need to go
		if(pNextNode == nodes[endIndex])
		{
			std::cout << "Destination Node Reached\n";
			return true;
		}

		//otherwise
		//check if node is not in the visited list
		if(std::find(visited.begin(), visited.end(), pNextNode) == visited.end())
		{
			visited.push_back(pNextNode); //add node to visited list
			{
				std::cout << pNextNode->GetPayload() << ", ";
				// iterate over all the nodes edges adding the connected nodes to the stack
				for (auto pEdge : pNextNode->GetEdges())
				{
					toVisitStack.push(pEdge->GetEnd());
				}
				return false;
			}
		}
	}

	std::cout << "destination  node unreachable \n";
	return true;
}

void Node::SetVisited(bool bInput)
{
	bVisited = bInput;
}

