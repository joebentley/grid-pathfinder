
#include <iostream>

#include "AStar.h"
#include "PathFinder.h"
#include "Grid.h"
#include "Point.h"
#include "Square.h"

int main()
{
    Grid grid(10, 12);
    //grid.populate(40);

    AStar pathfinder(grid);

    std::vector<Point> waypoints;
    waypoints.push_back(Point(5, 5));
    waypoints.push_back(Point(3, 11));
    waypoints.push_back(Point(8, 3));
    waypoints.push_back(Point(3, 3));
    waypoints.push_back(Point(0, 0));
    waypoints.push_back(Point(7, 1));
   
    Path path = pathfinder.buildFromWaypoints(waypoints);
    Path path2 = pathfinder.buildWithHeuristic(waypoints);

    std::cout << grid.getDimensions() << std::endl;
    
    std::cout << grid;
    std::cout << std::endl;
    std::cout << grid.toStringWithPath(path);
    std::cout << grid.toStringWithPath(path2);
    
    return 0;
}
