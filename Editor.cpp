#include <FL/Fl.H>

#include "GridView.h"

int main(int argc, char **argv)
{
    // Generate a new grid
    Grid grid(20, 20);
    grid.populate(200);

    // Generate a new GridView with the new grid
    GridView *win = new GridView(&grid, "Grid Viewer");
	
    // Show it
    win->show();
	
    // Run FLTK main loop
    return Fl::run();
}
