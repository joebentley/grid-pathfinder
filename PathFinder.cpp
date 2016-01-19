
#include <utility>
#include "PathFinder.h"


/**
 * Build path from waypoints in order.
 */
Path PathFinder::buildFromWaypoints(std::vector<Point> waypoints)
{
    // Need at least a start and end point
    if (waypoints.size() < 2)
	return Path();

    // Path to return
    Path path;

    /* For each pair of points starting with the second point, get the path
     * between the current and previous point and append it to the current list
     * of points. */
    for (int i = 1; i < waypoints.size(); ++i) {
	// Generate path between previous and this waypoint
	Path path2 = this->build(waypoints.at(i - 1), waypoints.at(i));

	// If any of the paths are empty, return empty path
	if (path2.size() == 0)
	    return Path();

	// Concatenate the new path onto the current path
	path.insert(path.end(), path2.begin(), path2.end());
    }

    return path;
}

/**
 * Compare two point, int pairs by their second field i.e. their manhattan
 * heuristic with respect to the end point in the method below.
 */
bool compareByManhattan(const std::pair<Point, int> &p1, const std::pair<Point, int> &p2)
{
    return p2.second < p1.second;
}

/**
 * Build path in order of Manhattan heuristic i.e. furthest from the end
 * first.
 */
Path PathFinder::buildWithHeuristic(std::vector<Point> waypoints)
{
    // Need at least a start and end point
    if (waypoints.size() < 2)
	return Path();

    // If only two points, just build from start to end
    if (waypoints.size() == 2)
	return this->build(waypoints.at(0), waypoints.at(1));

    Path path;

    Point start = waypoints.front();
    Point end = waypoints.back();

    // Get subvector of waypoints excluding start and end points
    std::vector<Point> midpoints(waypoints.begin() + 1, waypoints.end() - 1);

    std::vector<std::pair<Point, int> > midpointsAndHeuristics;

    // Generate Manhattan heuristic with respect to end for each point
    for (std::vector<Point>::const_iterator it = midpoints.begin(); it != midpoints.end();
	 ++it) {

	midpointsAndHeuristics.push_back(std::make_pair(*it,
	    it->getManhattanDistanceTo(end)));
    }

    // Sort by Manhattan heuristic
    std::sort(midpointsAndHeuristics.begin(),
	      midpointsAndHeuristics.end(),
	      compareByManhattan);
    
    // Build the path from start to the first point
    path = this->build(start, midpointsAndHeuristics.at(0).first);

    // If start path empty, return empty path
    if (path.size() == 0)
	return Path();

    // Now generate the paths in order of heuristic
    for (int i = 1; i < midpointsAndHeuristics.size(); ++i) {
	// Generate path between previous and this waypoint
	Path path2 = this->build(midpointsAndHeuristics.at(i - 1).first,
				 midpointsAndHeuristics.at(i).first);

	// If any of the paths are empty, return empty path
	if (path2.size() == 0)
	    return Path();

	// Concatenate the new path onto the current path
	path.insert(path.end(), path2.begin(), path2.end());
    }

    // Add final path
    Path path2 = this->build(midpointsAndHeuristics.back().first, end);

    // If final path empty, return empty path
    if (path2.size() == 0)
	return Path();

    // Concatenate final path
    path.insert(path.end(), path2.begin(), path2.end());

    return path;
}
