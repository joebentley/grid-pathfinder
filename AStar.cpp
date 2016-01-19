
#include "AStar.h"

#include <algorithm>
#include <cstdlib>
#include <set>

/* Build path from `start` to `end` */
Path AStar::build(const Point &start, const Point &end)
{
    /* Indices referring to the nodes in this->allNodes in the open or closed
       sets */
    std::set<int> openSet;
    std::set<int> closedSet;

    // Create initial node
    Node initialNode = Node(start, start.getManhattanDistanceTo(end));
        
    // Create final node (with heuristic zero)
    Node finalNode = Node(end, 0);
        
    // If initial or final nodes are full, return empty path
    if (this->grid.getSquare(initialNode.getPosition()) == FULL ||
        this->grid.getSquare(finalNode.getPosition()) == FULL)
        {
            return Path();
        }
        
    // Add initial node to the vector of all nodes
    allNodes.push_back(initialNode);

    // And get its index in the nodes vector, adding it to the closed set
    int initialIndex = allNodes.size() - 1;
    closedSet.insert(initialIndex);
        
    // Add all empty neighbours of the start point to the open set
    std::set<Point> neighbours = this->grid.getEmptyNeighbours(start);
        
    // If the start point is surrounded by obstacles, return empty path
    if (neighbours.size() == 0) {
        return Path();
    }
        
    /* For each empty neighbour of the initial node, create the node and
       construct an initial open set */
    for (std::set<Point>::const_iterator it = neighbours.begin();
	 it != neighbours.end(); ++it) {

	// Create node with heuristic
        Node node(*it, it->getManhattanDistanceTo(end));
                
        // Calculate gvalue, set parent index, and add to vector of all nodes
        node.setgvalue(this->calculategvalue(initialNode, node));
        node.setParentIndex(initialIndex);
        allNodes.push_back(node);

        // Add current index to open set
        openSet.insert(allNodes.size() - 1);
    }

    // Index of minimum fvalue node
    int minimum = 0;

    // While not next to final node, and open set isn't empty
    while (!this->isNextTo(minimum, end) && openSet.size() != 0) {
        // Get smallest Node
        minimum = this->getSmallest(openSet);
                
        // Move from open set to closed set
        openSet.erase(minimum);
        closedSet.insert(minimum);
                
	// Iterate over each empty neighbour of the minimum f-value node
        neighbours = this->grid.getEmptyNeighbours(allNodes.at(minimum).getPosition());
        for (std::set<Point>::const_iterator it = neighbours.begin();
	     it != neighbours.end(); ++it) {

            // Add new node to the open set if not already in either closed or open set
            if (getNodeByPoint(openSet, *it) == -1 &&
		getNodeByPoint(closedSet, *it) == -1) {
                                        
                Node node(*it, it->getManhattanDistanceTo(end));
                
                // Calculate gvalue, set parent to minimum fvalue node, add node
                // to vector of all nodes
                node.setgvalue(this->calculategvalue(allNodes.at(minimum), node));
                node.setParentIndex(minimum);
                allNodes.push_back(node);

                // Add new index to open set
                openSet.insert(allNodes.size() - 1);
            }
                        
            // Check whether the node is in the open set (and not the closed set)
            int openNeighbour = getNodeByPoint(openSet, *it);
                        
            // If it is in the open set, do the g-value test
            if (openNeighbour != -1) {
                // Decide whether to set parent of the node to the minimum
                // f-value node by calculating gvalue
                                
                // Calculate direct gvalue
                int gvalueToTest = this->calculategvalue(allNodes.at(minimum),
                                                         allNodes.at(openNeighbour));
                                
		// If the current gvalue is greater than the g-value would be
		// with the minimum node, set the parent to the minimum node
                if (allNodes.at(openNeighbour).getgvalue() > gvalueToTest) {
                    allNodes.at(openNeighbour).setParentIndex(minimum);
                }
            }
        }
    }
        
    // We didn't find a path, so return an empty path
    if (!this->isNextTo(closedSet, end)) {
        return Path();
    }
        
    // Add final parent
    finalNode.setParentIndex(minimum);
        
    // Reconstruct path backwards
    Path reversed;
    reversed.push_back(finalNode.getPosition());
        
    Node previous = allNodes.at(finalNode.getParentIndex());
        
    // Loop through the parent of each node in order, adding it to the path
    while (previous != initialNode) {
        reversed.push_back(previous.getPosition());
                
	// Get the parent of the current node
        previous = allNodes.at(previous.getParentIndex());
    }

    // Add the initial point
    reversed.push_back(initialNode.getPosition());
        
    return reversed;
}

/* Calculate g-value from `from` to `to` */
int AStar::calculategvalue(const Node &from, const Node &to) const {
    Point difference = from.getPosition() - to.getPosition();
    int x = std::abs(difference.getx());
    int y = std::abs(difference.gety());
        
    if (x == 1 && y == 1)
        return from.getgvalue() + this->diagonalCost;
    else
        return from.getgvalue() + this->cardinalCost;
}

/* Get node from `set` at point `p` */
int AStar::getNodeByPoint(const std::set<int> &set, const Point &p) const {
    for (std::set<int>::iterator it = set.begin(); it != set.end(); ++it) {
        if (allNodes.at(*it).getPosition() == p)
            return *it;
    }
        
    return -1;
}

/* Return true if `p` neighbour of `p2` */
bool AStar::isNextTo(const Point &p, const Point &p2) const {
    std::set<Point> neighbours = this->grid.getNeighbours(p2);

    /* return true if p is in the neighbours of p2 */
    return neighbours.find(p) != neighbours.end();
}

/* Return true if Node at index `n` is neighbour of `p2` */
bool AStar::isNextTo(int n, const Point &p2) const {
    return this->isNextTo(this->allNodes.at(n).getPosition(), p2);
}

/* Return true if any of the Nodes indexed in `closedSet` are neighbour of `p2` */
bool AStar::isNextTo(const std::set<int> &closedSet, const Point &p2) const {
    std::set<Point> neighbours = this->grid.getNeighbours(p2);
        
    for (std::set<int>::iterator it = closedSet.begin(); it != closedSet.end(); ++it) {
                
        if (neighbours.find(allNodes.at(*it).getPosition()) != neighbours.end()) {
            return true;
        }
    }
        
    return false;
}

/* Get the node from `openSet` with the smallest f-value */
int AStar::getSmallest(const std::set<int> &openSet) const {
    int minimum = *openSet.begin();
    for (std::set<int>::const_iterator it = openSet.begin(); it != openSet.end(); ++it) {
        if (allNodes.at(*it).getfvalue() < allNodes.at(minimum).getfvalue())
            minimum = *it;
    }
        
    return minimum;
}

/* Define comparison operators for nodes to compare by their positions */
bool Node::operator<(const Node &n2) const {
    return this->p < n2.p;
}

bool Node::operator==(const Node &n2) const {
    return this->p == n2.p;
}

bool Node::operator!=(const Node &n2) const {
    return !(this->p == n2.p);
}
