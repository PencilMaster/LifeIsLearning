#pragma once

#include <wx/wx.h>
#include <memory>
#include "../../../lib/common/game_state.h"

// organizational unit for all the game panels
class GameWindow : public wxFrame
{
public:
    // Constructor
    GameWindow(const wxString& title, const wxPoint& pos, const wxSize& size);

    /** 
		@brief shows a panel in the game window
        @param panel the panel to show
    **/
    void showPanel(wxPanel* panel);

    /** 
		@brief sets the status bar message
        @param message the message to set

    **/
    void setStatus(const std::string& message);

private:
    wxBoxSizer* _mainLayout;
    wxStatusBar* _statusBar;
    wxPanel* _currentPanel;

};

