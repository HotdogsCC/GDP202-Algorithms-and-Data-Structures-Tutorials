#pragma once
#include <queue>
#include <stack>
#include <glm/vec2.hpp>
#include <vector>

class Node;
class Edge;

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
    virtual bool DepthFirstSearch(
        const unsigned int startIndex,
        const unsigned int endIndex,
        bool bStartSearch) = 0;
    
};

class GraphEdgeAdjacencyEdgeImplementation : public Graph
{
    std::vector<Node*> nodes;
    std::stack<Node*> toVisitStack;
    std::queue<Node*> visitedQueue;
    std::vector<Node*> visited;
public:
    GraphEdgeAdjacencyEdgeImplementation() = default;
    ~GraphEdgeAdjacencyEdgeImplementation();
    virtual void AddNode(int label, glm::vec2 v2Pos) override;
    virtual void AddEdge(unsigned int startIndex, unsigned int endIndex, float fCost) override;
    virtual void debugDrawGraph(aie::Renderer2D* renderer, aie::Font* pFont) override;
    virtual bool DepthFirstSearch(const unsigned int startIndex, const unsigned int endIndex, bool bStartSearch) override;
    
};



class Edge
{
    float fCost; //cost of traversing along this edge
    Node* pStart;
    Node* pEnd;

public:
    Edge(Node* pStart, Node* pEnd, float fCost) : pEnd(pEnd), pStart(pStart), fCost(fCost) {};
    void DebugDraw(aie::Renderer2D* pRenderer, aie::Font* pFont) const;
    float GetConst() const { return fCost; }
    Node* GetEnd() const { return pEnd; }
    Node* GetStart() const { return pStart; }
};

class Node
{
    //the value of this node
    const int payload;
    //position of the node on the screen
    glm::vec2 v2Pos = { 0, 0 };
    // List of edges which this node connect to
    std::vector<const Edge*> edges;

    // debug purposes, whether this is visited
    bool bVisited = false;

    //radius of the node
    float nodeRadius = 50.0f;

public:
    Node(const int payload, glm::vec2 v2Pos) : payload(payload), v2Pos(v2Pos) {}
    ~Node();
    const glm::vec2 GetPos() { return v2Pos; }
    const int GetPayload() { return payload; }
    void AddEdge(const Edge* edge);
    const std::vector<const Edge*>& GetEdges() { return edges; }
    void DebugDraw(aie::Renderer2D* pRenderer, aie::Font* pFont);
    const float GetNodeRadius() { return nodeRadius; }

    void SetVisited(bool bInput);

};