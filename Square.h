#ifndef SQUARE_H_
#define SQUARE_H_

/**
 * Enum representing the current state of a square on the Grid. Either the
 * square is EMPTY, and hence it has no obtacles and is freely traversable, or
 * it is FULL and not traversable at all
 */
enum Square {
    EMPTY, FULL
};

/**
 * Return a char representing the Square, 'o' for EMPTY, 'x' for full
 */
char toCharRep(Square square);

#endif /* SQUARE_H_ */
