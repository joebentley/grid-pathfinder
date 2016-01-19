#ifndef GRID_H_
#define GRID_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "Point.h"
#include "Square.h"

typedef std::vector<Point> Path;

/**
 * Class that represents a Grid of Squares as an interface to a map from Point
 * to Square
 */
class Grid {
 public:
    Grid(int width, int height);
	
    typedef std::map<Point, Square> gridMap;

    /**
     * Return the grid map
     */
    gridMap getGrid() const { return grid; }

    /**
     * Set the Square at point `p` to `s`
     */
    void setSquare(Point p, Square s);

    /**
     * Return the square at point `p`
     */
    Square getSquare(Point p) const;
	
    /**
     * Return a Point representing the maximum x and y value _plus one_ of the
     * grid which is equal to the width and height assuming that the first grid
     * Point is at (0, 0)
     */
    Point getDimensions() const;

    /**
     * Return the x or y coordinate of the above
     */
    int getWidth() const;
    int getHeight() const;
	
    /**
     * Return a set of points that are adjacent either cardinally or diagonally
     * to Point `p`, skipping the point `p` itself, and checking if on the
     * boundaries, i.e. it won't check past x=0 or y=0 or x=getWidth()-1 or
     * y=getWidth()-1
     */
    std::set<Point> getNeighbours(const Point &p) const;

    /**
     * Same as above but only returns neighbours that are EMPTY Squares
     */
    std::set<Point> getEmptyNeighbours(const Point &p) const;
	
    /**
     * Get Point corresponding to a random EMPTY Square on the grid.
     * Can be slow if on a well-populated grid.
     */
    Point getEmptyPoint() const;

    /**
     * Add `nFull` random FULL squares to the grid, returning the grid
     */
    Grid &populate(int nFull);
	
    /**
     * Reset current grid so that all squares are EMPTY
     */
    void clear();
	
    /**
     * Render the grid as a string using toCharRep(Square) to render the Squares
     */
    std::string toString() const;

    /**
     * Same as above but overlaying `path` over the top represented by '.'
     */
    std::string toStringWithPath(Path path) const;
 private:
    gridMap grid;
};

/**
 * Output to `os` using grid.toString()
 */
std::ostream& operator<<(std::ostream &os, const Grid &grid);

#endif /* GRID_H_ */
