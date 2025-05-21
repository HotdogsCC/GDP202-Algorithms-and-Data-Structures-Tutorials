#pragma once
#include <glm/detail/type_vec.hpp>

namespace aie
{
    class Renderer2D;
    class Font;
}

class Graph
{
public:
    virtual void AddNode(int label, glm::vec2 vec2pos) = 0;
    virtual void AddEdge(unsigned int startIndex, unsigned int endIndex, float fCost) = 0;
    virtual void debugDrawGraph(aie::Renderer2D* renderer, aie::Font* pFont) = 0;
    
};

class GraphEdgeAdjacencyEdgeImplementation : public Graph
{
    
};
