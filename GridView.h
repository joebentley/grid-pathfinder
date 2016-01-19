#ifndef GRIDWINDOW_H_
#define GRIDWINDOW_H_

#include <FL/Fl_Button.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Choice.H>
#include <vector>

#include "Point.h"
#include "Grid.h"

/**
 * Set of possible states for editing the grid NORMAL corresponds to regular
 * editing (toggling the buttons) SETTING_WAYPOINT corresponds to setting the
 * waypoint indexed by `index`
 */
enum State {
    NORMAL,
    SETTING_WAYPOINT
};

/**
 * GridView state which is the composition of the State, and the index of the
 * waypoint that is being set.
 */
struct GridViewState {
    int index;
    State state;
};

class GridView : public Fl_Double_Window {
 public:
    GridView(Grid *grid, const char* title = 0);
	
    /**
     * Reset all the grid buttons to their original colors based upon whether
     * they are full, empty, start, end, or other waypoints, clearing any path
     * that is currently displayed on the grid. Also sets the numbers on the
     * waypoints (apart from start and end which have special colors)
     */
    void RefreshGrid();
	
    /**
     * Toggle the clicked grid square by comparing `pButton` with the elements
     * of this->squares to find out which grid point was clicked.
     *
     * If this->state.state == NORMAL, just toggle the square between FULL and
     * EMPTY. If this->state.state == SETTING_WAYPOINT, then set the waypoint
     * indexed by this->state.index to the selected Point.
     */
    void ToggleGridSquare(Fl_Widget *pButton);

    /**
     * Pathfind on the grid, setting the buttons in the path to green. If the
     * "Pathfind!" button was pressed, just pathfind the waypoints _in order of
     * creation_, else if the "w/ Heuristics" button is pressed, pathfind the
     * waypoints in order of their Manhattan heuristic with respect to the end
     * point, i.e. in order of distance from the end point (the furthest
     * waypoint is visited first)
     *
     * Displays a message in `result_output` depending on whether the
     * pathfinding was successful or not.
     */
    void DoPathfind(Fl_Widget *pButton);

    /**
     * Clear the grid, then randomly populate the grid with the number to fill
     * in given by `repopulation_input`'s text label. If the start or end points
     * are filled in, make them empty again, then refresh the grid.
     */
    void Repopulate(Fl_Widget *pButton);

    /**
     * Clear the grid, then refresh it.
     */
    void Clear(Fl_Widget *pButton);

    /**
     * Open a new GridView with a width and height taken from the inputs
     * `width_input` and `height_input` respectively
     */
    void NewGrid(Fl_Widget *pButton);

    /**
     * Get the index selected by `waypoints_selection` and set this->state.index
     * to it, and set this->state.state to SETTING_WAYPOINT. Set the output
     * textbox to yellow and tell the user that they are setting a waypoint.
     */
    void SetWaypoint(Fl_Widget *pButton);

    /**
     * Add a new waypoint at (0, 0) to `waypoints` and add a new entry to
     * `waypoints_selection`, then refresh the grid.
     */
    void AddWaypoint(Fl_Widget *pButton);

    /**
     * Delete the waypoint currently selected by `waypoints_selection`, removing
     * it from `waypoints` and decrementing all of the other values in the
     * drop-down box to compensate for the removed value. If the user attempts
     * to remove the start or end points, warn them the output textbox.
     */
    void DeleteWaypoint(Fl_Widget *pButton);
	
    /**
     * Static versions of the member functions to be used as callbacks
     */
    static void StaticToggleGridSquare(Fl_Widget *pButton, void *data) {
	((GridView*)data)->ToggleGridSquare(pButton);
    }
	
    static void StaticDoPathfind(Fl_Widget *pButton, void *data) {
	((GridView*)data)->DoPathfind(pButton);
    }
	
    static void StaticRepopulate(Fl_Widget *pButton, void *data) {
	((GridView*)data)->Repopulate(pButton);
    }
	
    static void StaticClear(Fl_Widget *pButton, void *data) {
	((GridView*)data)->Clear(pButton);
    }
	
    static void StaticNewGrid(Fl_Widget *pButton, void *data) {
	((GridView*)data)->NewGrid(pButton);
    }

    static void StaticSetWaypoint(Fl_Widget *pButton, void *data) {
	((GridView*)data)->SetWaypoint(pButton);
    }

    static void StaticAddWaypoint(Fl_Widget *pButton, void *data) {
	((GridView*)data)->AddWaypoint(pButton);
    }

    static void StaticDeleteWaypoint(Fl_Widget *pButton, void *data) {
	((GridView*)data)->DeleteWaypoint(pButton);
    }
	
    /* Refresh draw state */
    static void Redraw(void* pWindow);
	
 private:
    // Current grid being used
    Grid *grid;
	
    // Waypoints to be passed through
    std::vector<Point> waypoints;

    // Waypoint drop-down selection
    Fl_Choice *waypoints_selection;

    // map of points on the grid to their relevant buttons
    std::map<Point, Fl_Button*> squares;
	
    // Input field for the number of squares to fill on repopulation
    Fl_Int_Input *repopulation_input;
	
    // Input fields or the width and height of a new grid
    Fl_Int_Input *width_input;
    Fl_Int_Input *height_input;
	
    // For showing output/current state
    Fl_Output *result_output;
	
    // Current state of the app
    GridViewState state;
};

#endif /* GRIDWINDOW_H_ */
