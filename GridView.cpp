
#include <cstdlib>
#include <cstring>

#include <algorithm>
#include <sstream>
#include <iostream>

#include <FL/Fl.H>

#include "AStar.h"
#include "Square.h"
#include "GridView.h"

GridView::GridView(Grid *grid, const char* title)
    : Fl_Double_Window(650, 450, title), grid(grid)
{
    // Set color of window to white
    color(FL_WHITE);
    // Begin adding children to this window
    begin();
    
    // Set default state
    state.state = NORMAL;
    
    // Button dimensions for grid
    const int buttonXOffset = 10;
    const int buttonYOffset = 10;
    const int buttonHeight = 20;
    const int buttonWidth = 20;
    
    // Width of inputs
    const int input_width = 120;

    // Minimum width and height are as if the grid is (20, 20)
    int winWidth;
    int winHeight;

    // Get grid dimensions
    int gridWidth = grid->getWidth();
    int gridHeight = grid->getHeight();

    winWidth = (gridWidth < 20) ? buttonWidth * 20 + buttonXOffset * 2 + 220
    : buttonWidth * gridWidth + buttonXOffset * 2 + 120 + input_width;
    
    winHeight = (gridHeight < 20) ? buttonHeight * 20 + buttonYOffset * 2
    : buttonHeight * gridHeight + buttonYOffset * 2;

    // x offset for the other inputs
    const int x_offset = winWidth - input_width - 20;

    // Set size so that the new grid fits
    size(winWidth, winHeight);
    
    // Generate all of the grid view buttons
    for (int x = 0; x < gridWidth; ++x)
    for (int y = 0; y < gridHeight; ++y) {
        Fl_Button *button = new Fl_Button(buttonXOffset + x * buttonWidth,
            buttonYOffset + y * buttonHeight,
            buttonWidth - 2, buttonHeight - 2);
            
        if (grid->getSquare(Point(x, y)) == EMPTY)
            button->color(FL_WHITE);
        else
            button->color(FL_RED);
            
        button->callback(GridView::StaticToggleGridSquare, this);
            
        squares[Point(x, y)] = button;
    }
    
    
    // Add button to trigger pathfinding
    Fl_Button *pathfindButton = new Fl_Button(x_offset, 20, input_width, 20, "Pathfind!");
    pathfindButton->callback(GridView::StaticDoPathfind, this);
    
    // Add button to trigger pathfinding in heuristic order
    Fl_Button *pathfindHeuristicButton = new Fl_Button(x_offset, 50, input_width,
						       20, "w/ Heuristic");
    pathfindHeuristicButton->callback(GridView::StaticDoPathfind, this);
    
    // Add button and textbox to repopulate the grid
    repopulation_input = new Fl_Int_Input(x_offset, 80, input_width, 20, "Num. to fill");
    // Set default repopulation amount as 200
    repopulation_input->value("200");
    
    Fl_Button *repopulateButton = new Fl_Button(x_offset, 110, input_width,
						20, "Repopulate!");
    repopulateButton->callback(GridView::StaticRepopulate, this);
    
    
    // Add button to clear the grid
    Fl_Button *clearButton = new Fl_Button(x_offset, 140, input_width, 20, "Clear!");
    clearButton->callback(GridView::StaticClear, this);
    
    // Add success/fail label for pathfinding
    result_output = new Fl_Output(x_offset - 10, 180, input_width + 20, 20);
    
    // Add new grid width and height textboxes
    width_input = new Fl_Int_Input(x_offset, 210, input_width, 20, "Width:");
    width_input->value("20");
    
    height_input = new Fl_Int_Input(x_offset, 240, input_width, 20, "Height:");
    height_input->value("20");
    
    // Add new grid button
    Fl_Button *newGridButton = new Fl_Button(x_offset, 270, input_width, 20, "New Grid!");
    newGridButton->callback(GridView::StaticNewGrid, this);
    
    // Add waypoint drop-down menu
    waypoints_selection = new Fl_Choice(x_offset, 300, 50, 20);

    // Add start and end point by default
    waypoints_selection->add("start");
    waypoints_selection->add("end");

    // Set to start by default
    waypoints_selection->value(0);

    // Set default start/end points to upper-left/lower-right
    waypoints.push_back(Point(0, 0));
    waypoints.push_back(Point(gridWidth - 1, gridHeight - 1));

    // Add button to set selected waypoint
    Fl_Button *setWaypointButton = new Fl_Button(x_offset, 330, input_width,
						 20, "Set Waypoint");
    setWaypointButton->callback(GridView::StaticSetWaypoint, this);

    // Add button to add new waypoint
    Fl_Button *addWaypointButton = new Fl_Button(x_offset, 360, input_width,
						 20, "Add Waypoint");
    addWaypointButton->callback(GridView::StaticAddWaypoint, this);

    // Add button to delete selected waypoint
    Fl_Button *deleteWaypointButton = new Fl_Button(x_offset, 390, input_width,
						    20, "Delete Waypoint");
    deleteWaypointButton->callback(GridView::StaticDeleteWaypoint, this);

    // Stop adding children to this window
    end();
    
    RefreshGrid();
    
    // Generate a timeout of 0.1s to redraw the window
    Fl::add_timeout(0.1, GridView::Redraw, this);
}

/* Refresh the grid display with a given grid; resets button color state */
void GridView::RefreshGrid()
{
    for (int x = 0; x < grid->getWidth(); ++x)
    for (int y = 0; y < grid->getHeight(); ++y) {

        Point p(x, y);
            
        // Set color depending on type of square 
        if (p == waypoints.front())
            squares.at(p)->color(FL_BLUE);
        else if (p == waypoints.back())
            squares.at(p)->color(FL_MAGENTA);
        else if (std::find(waypoints.begin(), waypoints.end(), p) != waypoints.end())
            squares.at(p)->color(FL_YELLOW);
        else if (grid->getSquare(p) == EMPTY)
            squares.at(p)->color(FL_WHITE);
        else if (grid->getSquare(p) == FULL)
            squares.at(p)->color(FL_RED);

        // Clear button's label
        squares.at(p)->label("");
    }

    // Add numbers to waypoints (except for start and end)
    for (int i = 1; i < waypoints.size() - 1; ++i) {
        // Convert the index to a const char*
        std::stringstream strs;
        strs << i;
        std::string str = strs.str();

        // Make non-vanishing copy
        char* temp = (char*)str.c_str();
        char* label = (char*)std::malloc(1 + std::strlen(temp));
        std::strcpy(label, temp);

        // Set the waypoint's square's label
        squares.at(waypoints.at(i))->label((const char*)label);
    }
}

/* Callback for clicking a grid button to toggle it */
void GridView::ToggleGridSquare(Fl_Widget *pButton)
{
    // Iterate through each point/button pair
    for (std::map<Point, Fl_Button*>::iterator it = squares.begin(); it != squares.end();
	 ++it) {

        /* Reset all green buttons (from pathfinding) back to white, except for
	   start and end, which we color blue and magenta respectively, and
	   waypoints, which we color yellow */
        if (it->first == waypoints.front())
            it->second->color(FL_BLUE);
        
        else if (it->first == waypoints.back())
            it->second->color(FL_MAGENTA);

        else if (std::find(waypoints.begin(), waypoints.end(), it->first)
		 != waypoints.end())
            it->second->color(FL_YELLOW);
           
        else if (it->second->color() == FL_GREEN)
            it->second->color(FL_WHITE);
        
        // If it->second is the just pressed button
        if (it->second == pButton) {
            // We are just clicking a button fill it in/empty it
            if (state.state == NORMAL) {
                // Don't allow setting the start/end/waypoint points
                if (it->second->color() == FL_BLUE ||
                    it->second->color() == FL_MAGENTA ||
                    it->second->color() == FL_YELLOW)
                    return;
                        
                // Turn grid point to FULL
                if (it->second->color() == FL_WHITE) {
                    grid->setSquare(it->first, FULL);
                    it->second->color(FL_RED);
                }
                        
                // Turn grid point to EMPTY
                else if (it->second->color() == FL_RED) {
                    grid->setSquare(it->first, EMPTY);
                    it->second->color(FL_WHITE);
                }
            }
            
            // We are setting a waypoint
            else if (state.state == SETTING_WAYPOINT) {
                grid->setSquare(it->first, EMPTY);

                if (state.index == 0)
                    it->second->color(FL_BLUE);
                else if (state.index == waypoints.size() - 1)
                    it->second->color(FL_MAGENTA);
                else
                    it->second->color(FL_YELLOW);

                waypoints.at(state.index) = it->first;
            }
                
            RefreshGrid();
        }
    }
    
    // Reset the gameview state to normal editing
    state.state = NORMAL;
    result_output->value("");
    result_output->color(FL_WHITE);
}

/* Callback to execute the pathfinder */
void GridView::DoPathfind(Fl_Widget *pButton)
{
    // Refresh the grid first
    RefreshGrid();
    
    AStar pathfinder(*grid);
    
    Path path;

    // Depending on which button was pressed to trigger this, call either with
    // or without using heuristic
    if (!std::strcmp(pButton->label(), "Pathfind!"))
        path = pathfinder.buildFromWaypoints(waypoints);
    else
        path = pathfinder.buildWithHeuristic(waypoints);

    for (int y = 0; y < grid->getHeight(); ++y)
        for (int x = 0; x < grid->getWidth(); ++x) {
            // If this point is on the path, set the button's color to green
            if (std::find(path.begin(), path.end(), Point(x, y)) != path.end()) {
                squares[Point(x, y)]->color(FL_GREEN);
            }
        }
        
    // Set result label
    if (path.size() == 0) {
        result_output->value("Couldn't find path");
        result_output->color(FL_RED);
    } else {
        result_output->value("Success!");
        result_output->color(FL_GREEN);
    }
}

/* Randomly populate the grid again depending on the input box */
void GridView::Repopulate(Fl_Widget *pButton)
{
    grid->clear();
    
    // Get the repopulation amount from the text input
    std::stringstream strValue;
    int value;
    
    strValue << repopulation_input->value();
    strValue >> value;
    
    grid->populate(value);
    
    // Set all of the waypoints to empty squares so they don't accidentally get set
    for (std::vector<Point>::const_iterator it = waypoints.begin(); it != waypoints.end(); ++it) {
        grid->setSquare(*it, EMPTY);
    }
    
    // Refresh everything
    RefreshGrid();
}

/* Clear the grid and refresh the grid buttons */
void GridView::Clear(Fl_Widget *pButton)
{
    grid->clear();
    RefreshGrid();
}

/* Generate new grid of width/height in new window */
void GridView::NewGrid(Fl_Widget *pButton)
{
    std::stringstream strValue;
    int width, height;
    
    // Get the width
    strValue << width_input->value();
    strValue >> width;
    
    // Clear for the height input
    strValue.clear();
    
    // Get the height
    strValue << height_input->value();
    strValue >> height;
    
    // Generate the new grid
    Grid *newGrid = new Grid(width, height);
    GridView *win = new GridView(newGrid, "Grid Viewer");
    
    win->show();
}

/* Start setting the waypoint selected by `waypoints_selection` */
void GridView::SetWaypoint(Fl_Widget *pButton)
{
    int index = waypoints_selection->value();

    // Set program input state
    state.state = SETTING_WAYPOINT;
    state.index = index;

    result_output->value("Setting Waypoint");
    result_output->color(FL_YELLOW);
}

/* Add a new waypoint at (0, 0) to `waypoints` and `waypoints_selection` */
void GridView::AddWaypoint(Fl_Widget *pButton)
{
    // Get next available index in the waypoints
    int index = waypoints.size() - 1;

    // Use index as the string label
    std::stringstream strs;
    strs << index;
    std::string str = strs.str();
    const char* label = str.c_str();

    // Insert it into the drop down box behind the end point
    waypoints_selection->replace(index, label);
    waypoints_selection->add("end");

    // Set drop down box to new index
    waypoints_selection->value(index);

    // Add new waypoint at (0, 0) to the list
    waypoints.insert(waypoints.begin() + index, Point(0, 0));

    RefreshGrid();
}

/* Deleted the waypoint selected from `waypoints_selection` and `waypoints` */
void GridView::DeleteWaypoint(Fl_Widget *pButton)
{
    int index = waypoints_selection->value();

    // Don't remove start and end waypoints
    if (index == 0 || index == waypoints.size() - 1) {
        result_output->value("Can't delete start/end");
        result_output->color(FL_RED);

        return;
    }

    // Decrement all higher numbers in the drop down box
    for (int i = index; i < waypoints.size() - 1; ++i) {
        // Convert label to int
        const char* label = waypoints_selection->text(i);
        std::stringstream oldStrVal;
        oldStrVal << label;

        unsigned int intVal;
        oldStrVal >> intVal;

	// Decrement the index
        intVal--;

	// Convert int to const char*
        std::stringstream newStrVal;
        newStrVal << intVal;
        std::string newStr = newStrVal.str();

        waypoints_selection->replace(i, newStr.c_str());
    }

    // Remove the waypoint from the drop down box and the vector
    waypoints_selection->remove(index);
    waypoints.erase(waypoints.begin() + index);

    RefreshGrid();
}

// Redraw window to refresh the draw state
void GridView::Redraw(void* pWindow)
{
    //Redrawing the window; notice the casting
    ((Fl_Window*)pWindow)->redraw();
    //Regenerate the timeout, passing again the pointer
    Fl::repeat_timeout(0.1, GridView::Redraw, pWindow);
}

