#ifndef POINT_H_
#define POINT_H_

/**
 * Class that represents a Point on the Grid. Interface to an (x, y) integer pair
 */
class Point {
 public:
 Point() : x(0), y(0) { }
 Point(int x, int y) : x(x), y(y) { }
	
    int getx() const { return x; }
    int gety() const { return y; }
	
    void setx(int x) { this->x = x; }
    void sety(int y) { this->y = y; }
	
    /**
     * Distance between this and `p2` going only parallel to the x and y axes
     */
    int getManhattanDistanceTo(const Point &p2) const;

    /**
     * Lexographically compare this and `p2`, first by x, then by y components.
     * For when Point is used as the key in a map.
     */
    bool operator<(const Point &p2) const;
	
 private:
    int x, y;
};

/**
 * Component-wise arithmetic
 */
Point operator+(const Point &p1, const Point &p2);
Point operator-(const Point &p1, const Point &p2);

/**
 * Component-wise equality
 */
bool operator==(const Point &p1, const Point &p2);
bool operator!=(const Point &p1, const Point &p2);

/**
 * Output `p` to `os` in the format sprintf("%d,%d", p.x, p.y)
 */
std::ostream& operator<<(std::ostream &os, const Point &p);

#endif /* POINT_H_ */
