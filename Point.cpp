
#include <cstdlib>
#include <iostream>

#include "Point.h"

bool Point::operator<(const Point &p2) const {
    return (x < p2.x) || (x == p2.x && y < p2.y);
}

int Point::getManhattanDistanceTo(const Point &p2) const {
    return std::abs(this->x - p2.x) + std::abs(this->y - p2.y);
}

Point operator+(const Point &p1, const Point &p2)
{
    return Point(p1.getx() + p2.getx(), p1.gety() + p2.gety());
}
 
Point operator-(const Point &p1, const Point &p2)
{
    return Point(p1.getx() - p2.getx(), p1.gety() - p2.gety());
}

bool operator==(const Point &p1, const Point &p2) {
    return (p1.getx() == p2.getx()) && (p1.gety() == p2.gety());
}

bool operator!=(const Point &p1, const Point &p2) {
    return !(p1.getx() == p2.getx()) && (p1.gety() == p2.gety());
}

std::ostream& operator<<(std::ostream &os, const Point &p) {
    return os << p.getx() << ", " << p.gety();
}
