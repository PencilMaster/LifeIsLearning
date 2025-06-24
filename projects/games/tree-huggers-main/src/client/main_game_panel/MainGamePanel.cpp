#include "MainGamePanel.h"
#include <iostream>
#include "../GameController.h"

// event table for the main game panel that binds the end phase button click event 
// to the EndPhaseButtonClick function and the paint event to the OnPaint function
BEGIN_EVENT_TABLE(MainGamePanel, wxPanel)
    EVT_BUTTON (1, MainGamePanel::EndPhaseButtonClick)
    EVT_PAINT(MainGamePanel::OnPaint)
END_EVENT_TABLE()

MainGamePanel::MainGamePanel(wxFrame* parent) : wxPanel(parent, wxID_ANY) {
    // create the main sizer
    wxBoxSizer* main_sizer = new wxBoxSizer(wxHORIZONTAL);

    // create the sub-panels
    gridPanel = new GridPanel(this);
    infoPanel = new InfoPanel(this);

    // create a sizer for the info panel and add the info panel as well as the end phase button
    wxBoxSizer* info_sizer = new wxBoxSizer(wxVERTICAL);
    // add a stretch spacer to center the info sizer vertically
    info_sizer->AddStretchSpacer(1); 
    info_sizer->Add(infoPanel, 2, // the info panel should take up half of the vertical space
        wxEXPAND); 

    EndPhaseButton = new wxButton(this, 1, _T("End Current Phase")); 
    // set the button's background and foreground colors to match the client's color
    EndPhaseButton->SetBackgroundColour(InfoPanel::client_color.ChangeLightness(195));
    EndPhaseButton->SetForegroundColour(InfoPanel::client_color);

    info_sizer->Add(EndPhaseButton, 0, wxALIGN_CENTER | wxALL, 10);
    // add a second stretch spacer to center the info sizer vertically
    info_sizer->AddStretchSpacer(1);

    // add the info sizer and the grid panel to the main sizer
    main_sizer->Add(info_sizer, 1, wxEXPAND);
    main_sizer->Add(gridPanel, 2, wxEXPAND);

    // set the main sizer and fit it to the main game panel
    SetSizerAndFit(main_sizer);
}

void MainGamePanel::initial_game_state(std::shared_ptr<const GameState> gs) {
    game_state = gs;
    gridPanel->initial_game_state(game_state);
    infoPanel->initial_game_state(game_state);
}

void MainGamePanel::update_game_state(std::shared_ptr<const GameState> gs) {
    game_state = gs;
    gridPanel->update_game_state(game_state);
    infoPanel->update_game_state(game_state);
}

void MainGamePanel::EndPhaseButtonClick(wxCommandEvent& event) {
    // end phase button can only be clicked if it's the player's turn
    if(game_state->get_current_player() == InfoPanel::me_idx) {
        GameController::end_phase();
    }
    else {
        wxMessageBox("It's not your turn");
    }
    
}

void MainGamePanel::OnPaint(wxPaintEvent& event) {
    // make the end phase button lighter if it's not the player's turn (indicates it shouldn't be clicked)
    if(game_state->get_current_player() != InfoPanel::me_idx) {
        EndPhaseButton->SetBackgroundColour(*wxWHITE);
        EndPhaseButton->SetForegroundColour(*wxLIGHT_GREY);
    }
    else {
        EndPhaseButton->SetBackgroundColour(InfoPanel::client_color.ChangeLightness(195));
        EndPhaseButton->SetForegroundColour(InfoPanel::client_color);
    }
}
