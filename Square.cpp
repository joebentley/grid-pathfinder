
#include "Square.h"

char toCharRep(Square square) {
    if (square == EMPTY) {
	return 'o';
    } else {
	return 'x';
    }
}
