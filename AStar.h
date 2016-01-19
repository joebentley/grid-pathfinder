#ifndef ASTAR_H_
#define ASTAR_H_

#include "PathFinder.h"
#include "Point.h"

/**
 * Node class used in the A* pathfinding to represent a node on the grid
 */
class Node {
 public:
 Node() : p(Point(0, 0)), heuristic(0), gvalue(0), parentIndex(-1) { }
 Node(Point pos, int heuristic) : p(pos), heuristic(heuristic),
	gvalue(0), parentIndex(-1) { }

    /* Position of the node on the grid */
    Point getPosition() const { return p; }
    void setPosition(const Point &p) { this->p = p; }

    int getfvalue() const { return heuristic + gvalue; }

    int getHeuristic() const { return heuristic; }
    void setHeuristic(int heuristic) { this->heuristic = heuristic; }
	
    int getgvalue() const { return gvalue; }
    void setgvalue(int gvalue) { this->gvalue = gvalue; }

    int getParentIndex() const { return parentIndex; }
    void setParentIndex(int parentIndex) { this->parentIndex = parentIndex; }

    /* Comparison operators just compare the points of the nodes */
    bool operator<(const Node &n2) const;
    bool operator==(const Node &n2) const;
    bool operator!=(const Node &n2) const;
	
 private:
    Point p;
    int heuristic;
    int gvalue;
	
    /**
     * Index of the parent node of this node in this->allNodes, set equal to -1
     * if not set to a node */
    int parentIndex;
};

/**
 * Class to find a path between a start and end point on a Grid using the 
 * A* pathfinding algortihm.
 */
class AStar : public PathFinder {
 public:

 AStar(Grid grid) : PathFinder(grid), cardinalCost(10), diagonalCost(14) { }
    
    /**
     * Build and return a Path between the start and end points, returning
     * an empty path on failure, or on success the path in reverse order
     */
    Path build(const Point &start, const Point &end);
	
    /**
     * Get and set the cardinal and diagonal movement costs
     */
    int getCardinalCost() const { return cardinalCost; }
    void setCardinalCost(int cardinalCost) { this->cardinalCost = cardinalCost; }
	
    int getDiagonalCost() const { return diagonalCost; }
    void setDiagonalCost(int diagonalCost) { this->diagonalCost = diagonalCost; }
	
 private:
    int cardinalCost; /* Cost of moving north/south/east/west */
    int diagonalCost; /* Cost of moving north-east/north-west/south-east/south-west */

    /**
     * Vector containing all of the nodes, which we reference by index
     */
    std::vector<Node> allNodes;

    /**
     * Calculate g-value based on relative positions of to and from nodes.
     *
     * Uses diagonal cost if on a diagonal one square away from each other, else
     * uses the cardinal cost.
     */
    int calculategvalue(const Node &from, const Node &to) const;

    /**
     * Given a `set` of integers corresponding to nodes indexed by
     * this->allNodes, return the index of the node at the grid position `p` or
     * -1 if there is no node in this->allNodes with position `p`
     */
    int getNodeByPoint(const std::set<int> &set, const Point &p) const;

    /**
     * Given a point `p` on the grid, return true if it is a neighbour of the
     * the point `p2`
     */
    bool isNextTo(const Point &p, const Point &p2) const;

    /**
     * Given an int `n` corresponding to the index of a node in this->allNodes,
     * return true if it is a neighbour of the point `p2`
     */
    bool isNextTo(int n, const Point &p2) const;

    /**
     * Given a set, `closedSet`, of integers corresponding to nodes indexed by
     * this->allNodes, return true if any of the nodes referred to by
     * `closedSet` are neighbours of the point `p2`
     */
    bool isNextTo(const std::set<int> &closedSet, const Point &p2) const;
    
    /**
     * Given a set, `openSet`, of integers corresponding to nodes indexed by
     * this->allNodes, return the index of the node with the smallest f-value
     */
    int getSmallest(const std::set<int> &openSet) const;
};

#endif /* ASTAR_H_ */
