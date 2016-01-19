
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>

#include "Grid.h"

Grid::Grid(int width, int height) {
    std::srand(std::time(0));
        
    for (int x = 0; x < width; ++x)
        for (int y = 0; y < height; ++y)
            grid[Point(x, y)] = EMPTY;
}

void Grid::setSquare(Point p, Square s) {
    grid[p] = s;
}

Square Grid::getSquare(Point p) const {
    return grid.find(p)->second;
}

Point Grid::getDimensions() const {
    Point max;
        
    // Iterate through each grid point, getting the maximum point
    for (gridMap::const_iterator it = grid.begin(); it != grid.end(); ++it) {
        int newMax;
                
	// If the new max has a greater x-component, set the current max's
	// x-component to it
        if ((newMax = it->first.getx()) > max.getx())
            max.setx(newMax);
                
	// Same for y-component
        if ((newMax = it->first.gety()) > max.gety())
            max.sety(newMax);
    }
        
    // Add one to the max, so that max is equal to the width and height
    return max + Point(1, 1);
}

int Grid::getWidth() const
{
    return getDimensions().getx();
}

int Grid::getHeight() const
{
    return getDimensions().gety();
}

std::set<Point> Grid::getNeighbours(const Point &p) const {
    std::set<Point> points;
	
    // For each point cardinally and diagonally
    for (int x = p.getx() - 1; x < p.getx() + 2; ++x)
	for (int y = p.gety() - 1; y < p.gety() + 2; ++y) {
            // Skip if going over the boundaries
            if (x < 0 || y < 0 || x > getWidth() - 1 || y > getHeight() - 1) {
                continue;
            }
                        
            // Skip this point
            if (Point(x, y) == p) {
                continue;
            }
                        
	    // If the point is on the grid, add to the list of points to return
            if (grid.find(Point(x, y)) != grid.end()) {
                points.insert(Point(x, y));
            }
        }
                
    return points;
}

std::set<Point> Grid::getEmptyNeighbours(const Point &p) const {
    std::set<Point> points;
        
    // For each point cardinally and diagonally
    for (int x = p.getx() - 1; x < p.getx() + 2; ++x)
        for (int y = p.gety() - 1; y < p.gety() + 2; ++y) {
            // Skip if going over the boundaries
            if (x < 0 || y < 0 || x > getWidth() - 1 || y > getHeight() - 1) {
                continue;
            }
                        
            // Skip this point
            if (Point(x, y) == p) {
                continue;
            }
                        
	    /* If the point is on the grid and it is empty, add to the list of
	       points to return */
            if (grid.find(Point(x, y)) != grid.end() && getSquare(Point(x, y)) == EMPTY) {
                points.insert(Point(x, y));
            }
        }
                
    return points;
}

Point Grid::getEmptyPoint() const {
    Point dim = getDimensions();
    int x, y;

    // Generate random points until we generate an empty point
    do {
        x = std::rand() % dim.getx();
        y = std::rand() % dim.gety();
    } while (getSquare(Point(x, y)) == FULL);

    return Point(x, y);
}

Grid &Grid::populate(int nFull) {
    // If greater than size of grid, set every square to full (for speed)
    if (nFull > (getWidth() * getHeight())) {
        for (int x = 0; x < getWidth(); ++x)
            for (int y = 0; y < getHeight(); ++y)
                grid[Point(x, y)] = FULL;
        
        return *this;
    }
        
    // Else randomly set points
    for (int i = 0; i < nFull; ++i) {
        Point dim = getDimensions();
                
        int x = std::rand() % dim.getx();
        int y = std::rand() % dim.gety();
                
        grid[Point(x, y)] = FULL;
    }

    return *this;
}

void Grid::clear() {
    for (int y = 0; y < getHeight(); ++y)
        for (int x = 0; x < getWidth(); ++x)
            grid[Point(x, y)] = EMPTY;

}

std::string Grid::toString() const {
    std::stringstream ss;
        
    // For each point on the grid
    for (int y = 0; y < getHeight(); ++y) {
        for (int x = 0; x < getWidth(); ++x) {
	    // Draw the representation of the square as a char
	    ss << toCharRep(getSquare(Point(x, y)));
        }
                
        ss << std::endl;
    }
        
    return ss.str();
}

std::string Grid::toStringWithPath(Path path) const {
    std::stringstream ss;
        
    for (int y = 0; y < getHeight(); ++y) {
        for (int x = 0; x < getWidth(); ++x) {
            // If this point is on the path, draw it
            if (std::find(path.begin(), path.end(), Point(x, y)) != path.end()) {
                ss << ".";
            } else {
                ss << toCharRep(getSquare(Point(x, y)));
            }
        }
                
        ss << std::endl;
    }
        
    return ss.str();
}

std::ostream& operator<<(std::ostream &os, const Grid &grid) {
    return os << grid.toString();
}
