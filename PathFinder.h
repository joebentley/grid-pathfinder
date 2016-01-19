#ifndef PATH_FINDER_H_
#define PATH_FINDER_H_

#include "Grid.h"

typedef std::vector<Point> Path;

/**
 * Base class to all pathfinding algorithms.
 */
class PathFinder {
 public:
 PathFinder(Grid grid) : grid(grid) { }
	
    /**
     * Construct path between start and end point
     *
     * Return empty path if no path possible (i.e. there are obstacles, etc.)
     */
    virtual Path build(const Point &start, const Point &end) = 0;

    /**
     * Construct path from series of waypoints, visiting them in order
     *
     * Returns empty path if the length of the waypoints vector is less than 2
     * as we need at least one start and end point
     */
    Path buildFromWaypoints(std::vector<Point> waypoints);

    /**
     * Construct path from series of waypoints, start with the first element,
     * ending with the final element, and visiting the waypoints in order of the
     * Manhattan heuristic of the points with respect to the end point, i.e. the
     * furthest points will be visited first.
     *
     * Returns empty path if the length of the waypoints vector is less than 2
     * as we need at least one start and end point
     */
    Path buildWithHeuristic(std::vector<Point> waypoints);

    /**
     * The grid to pathfind on.
     */
    Grid grid;
};

#endif /* PATH_FINDER_H_ */
