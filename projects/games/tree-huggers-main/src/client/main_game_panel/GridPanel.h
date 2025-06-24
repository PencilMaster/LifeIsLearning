#pragma once
#include <wx/wx.h>
#include <wx/dcbuffer.h>
#include <cmath>
#include "../../../lib/common/game_state.h"

// struct to represent a hexagon on the grid
struct Hexagon {
    int index; // index corresponding to the cell in the game state
    wxPoint center; // initial center of the hexagon before centering the whole grid
    wxColour fillColor;
    wxColour borderColor;
    std::vector<wxPoint> vertices;
    wxPoint offset_center = wxPoint(0, 0); // center of the hexagon after centering the whole grid
};

class GridPanel : public wxPanel {
    public:
        /**
         * @brief Constructs a new Grid Panel object
         * @param parent the parent frame where the panel will be placed
         */
	    GridPanel(wxWindow* parent);
        
        /**
         * @brief Initializes the grid panel's game state calls InitBoard
         * @param gs the game state to initialize with
         */
        void initial_game_state(std::shared_ptr<const GameState> gs);

        /**
         * @brief Updates the grid panel's game state and redraws the panel
         * @param gs the game state to update with
         */
        void update_game_state(std::shared_ptr<const GameState> gs);

        // size variables for the hexagons and trucks
        static const int radius = 45;
        static const int border_width = radius/10;
        static const int truck_size = 7*radius/20;

        // functions for drawing trucks (static to be usable for the info panel icons)
        /**
        * @brief Draws a small truck at the given position with the given scale factor and color
        * @param dc the device context to draw on
        * @param position the center of the truck
        * @param sc_fac the scale factor for the truck size
        */
        static void DrawSmallTruck(wxDC& dc, const wxPoint& position, double sc_fac, const wxColour& color);
        /**
        * @brief Draws a big truck at the given position with the given scale factor and color
        * @param dc the device context to draw on
        * @param position the center of the truck
        * @param sc_fac the scale factor for the truck size
        */
        static void DrawBigTruck(wxDC& dc, const wxPoint& position, double sc_fac, const wxColour& color);

        // player colors (initialized in GridPanel.cpp)
        static const wxColour purple; 
        static const wxColour yellow;
        static const wxColour blue;
        static const wxColour red;
        static const std::vector<wxColour> player_colors;

        static const wxColour background_color; // pastel green

    private:
        std::shared_ptr<const GameState> game_state_ptr;

        // colors for the different cell types
        const wxColour full_forest_col{30, 140, 0}; // green
        const wxColour almost_full_forest_col{142, 192, 27}; // light green
        const wxColour critical_forest_col{206, 218, 104}; // yellow
        const wxColour dead_forest_col{130, 140, 130}; // grey
        const wxColour city_col{198, 91, 10}; // brown-orange
        
        // vector of hexagons representing the game board
        std::vector<Hexagon> hexagons;
        // bool greyed; // optional feature to grey out hexagons when they are not clickable

        // dimensions of the hexagon grid's bounding box (dynamically calculated)
        int grid_width;
        int grid_height;
        
        // functions for drawing the grid and its tokens
        /**
        * @brief creates the hexagons for the game board and calculates the grid's bounding box
        */
        void InitBoard();
        /**
        * @brief draws the grid and its tokens on the device context in the center of the panel
        * @param dc the device context to draw on
        */
        void DrawGrid(wxDC& dc);
        /**
        * @brief updates the color of the hexagon with index i based on the game state
        * @param i the index of the hexagon to update
        */
        void UpdateHexagon(int i); // include bool my_turn for greyed out hexagons feature
        /**
        * @brief draws the hexagon with index idx on the device context with the given vertices
        * @param dc the device context to draw on
        * @param idx the index of the hexagon in the hexagons vector
        * @param vertices the vertices of the hexagon
        */
        void DrawHexagon(wxDC& dc, int idx, const std::vector<wxPoint>& vertices);
        /**
        * @brief draws the roads on the grid according to the current game state
        * @param dc the device context to draw on
        */
        void DrawRoads(wxDC& dc);
        /**
        * @brief draws the trucks on the grid according to the current game state
        * @param dc the device context to draw on
        */
        void DrawTrucks(wxDC& dc);

        // event handlers
        /** 
        * @brief Event handler for the paint event that draws the grid and its tokens
        * @param event the paint event
        */
        void OnPaint(wxPaintEvent& paint);
        /**
        * @brief Event handler for the mouse click event that handles hexagon clicks
                depending on the current phase, player and game state, 
                it calls GameController functions or shows errors
        * @param event the mouse click event
        */
        void HexClick(wxMouseEvent& event);

        // functions needed for handling hexagon clicks
        /**
        * @brief checks whether the point is inside a circle with center hexCenter and radius of the hexagons
        * @param point the point to check
        * @param hexCenter the center of the hexagon in question
        * @return whether the point is inside the circle approximating the hexagon
        */
        bool IsInsideHexagon(const wxPoint& point, const wxPoint& hexCenter);
        /**
        * @brief opens a truck dialogue for the user to place a big or small truck on the hexagon,
                calls the corresponding game controller functions and handles the user error cases
        * @param hex_i the hexagon with hex_i where the truck placing is handled
        */
        void HandleTruckPlacing(int hex_i);
        /**
        * @brief opens a truck dialogue for the user to remove a number big or small trucks on the hexagon,
                calls the corresponding game controller functions and handles the user error cases
        * @param hex_i the hexagon with hex_i where the truck removing is handled
        */
        void HandleTruckRemoving(int hex_i);
        
        // tells the compiler that this class will define its own event table to handle any events statically
        // this happens in the .cpp file
        DECLARE_EVENT_TABLE()

};

