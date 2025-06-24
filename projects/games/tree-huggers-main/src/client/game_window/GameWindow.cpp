#include "GameWindow.h"
#include "../main_game_panel/MainGamePanel.h"
#include "../end_screen_panel/EndScreenPanel.h"
#include <iostream>


GameWindow::GameWindow(const wxString& title, const wxPoint& pos, const wxSize& size) 
        : wxFrame(nullptr, wxID_ANY, title, pos, size)
{
    // Set up layout that will contain and center all content
    _mainLayout = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* outerLayout = new wxBoxSizer(wxHORIZONTAL);
    outerLayout->Add(_mainLayout, 1, wxEXPAND);
    SetSizerAndFit(outerLayout);
    _currentPanel = nullptr;

    // Set up status bar
    _statusBar = CreateStatusBar(1);

    // Set background
    const wxColour pastel_green(230, 240, 190);
    SetBackgroundColour(pastel_green);

    // Set the minimum size of the window. The user won't be able to resize the window to a size smaller than this
    SetMinSize(wxSize(1200, 800));

}


void GameWindow::showPanel(wxPanel* panel) {
    // if we are already showing the panel, we don't need to do anything
    if(_currentPanel == panel) {
        return;
    }

    // remove previous panel
    if(_currentPanel != nullptr) {
        _mainLayout->Detach(_currentPanel);
        _currentPanel->Show(false);
        _currentPanel->Destroy();  // Destroy the previous panel to free resources
        _currentPanel = nullptr;
    }

    // add new panel
    _mainLayout->Add(panel, 1, wxEXPAND); // Use wxEXPAND to make it fill horizontally
    panel->Show(true);
    _currentPanel = panel;

    // Trigger a full layout update
    Layout(); // Make sure to layout the frame itself
    GetSizer()->Fit(this); // Adjust window size based on new panel
    GetSizer()->FitInside(this);
    Refresh();  // Refresh the window to force a redraw
}


void GameWindow::setStatus(const std::string& message) {
    _statusBar->SetStatusText(message, 0);
}
