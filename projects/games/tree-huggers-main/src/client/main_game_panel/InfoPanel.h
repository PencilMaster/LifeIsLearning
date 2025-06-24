#pragma once
#include <memory>
#include <wx/wx.h>
#include <wx/dcbuffer.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include "../../../lib/common/game_state.h"


class InfoPanel : public wxPanel {
public:
    /**
    * @brief Constructs a new Info Panel object
    * @param parent the parent frame where the panel will be placed
    */
    InfoPanel(wxWindow* parent);

    /**
    * @brief Initializes the info panel's game state and some variables which depend on it
    * @param gs the game state to initialize with
    */
    void initial_game_state(std::shared_ptr<const GameState> gs);

    /**
    * @brief Updates the info panel's game state and all variables which depend on it
    * @param gs the game state to update with
    */
    void update_game_state(std::shared_ptr<const GameState> gs);

    // the player's color
    static wxColour client_color;
    // the player's index (0-3)
    static int me_idx;

private:
    /**
    * @brief Event handler for the paint event that centers and draws the info panel
    * @param event the paint event
    */
    void OnPaint(wxPaintEvent& event);

    /**
    * @brief Centers the drawing inside the info panel
    * @param width the width of the drawing to center
    * @param height the height of the drawing to center
    * @return the centered point (top left corner)
    */
    wxPoint MakeCentered(int width, int height);
    
    /**
    * @brief Draws the rectancle, text, and icons within the info panel on the given device context
    * @param dc the device context to draw on
    * @param position the top left corner of the drawing
    */
    void DrawPanel(wxDC& dc, wxPoint position);
    
    // the width and height of the rectangle surrounding the text
    int width = 250;
    int height = 320;

    std::shared_ptr<const GameState> game_state;

    // game state dependent variables
    int round;
    std::string phase;
    std::string curr_player_name;
    int curr_player_idx;

    std::shared_ptr<Player> me_ptr;
    std::string my_name;
    int balance;
    int big_trucks;
    int small_trucks;
    int wood;

    // tells the compiler that this class will define its own event table to handle any events statically
    // this happens in the .cpp file
    DECLARE_EVENT_TABLE()
};
