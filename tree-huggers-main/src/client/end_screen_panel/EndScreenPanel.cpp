#include "EndScreenPanel.h"
#include "../GameController.h"


EndScreenPanel::EndScreenPanel(wxWindow* parent, std::shared_ptr<const GameState> game_state) : wxPanel(parent, wxID_ANY){
    // Declare sizer
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    // Get player_ptr
    std::shared_ptr<Player> me = game_state->get_player_ptr(GameController::get_my_id());
    
    // Set window size
    SetSize(wxSize(400,300));

    // Set Texts and Fonts of info to display
    wxStaticText* completed = new wxStaticText(this, wxID_ANY, "Game Completed!",wxDefaultPosition, wxSize(300,100));
    completed->SetFont(completed->GetFont().Scale(2));
    
    wxStaticText* profit = new wxStaticText(this, wxID_ANY, wxString::Format("Final balance: %d", me->get_balance()));
    wxStaticText* logged = new wxStaticText(this, wxID_ANY, wxString::Format("Wood Logged: %d", me->get_total_logged()));
    wxStaticText* diversity = new wxStaticText(this, wxID_ANY, wxString::Format("Final Biodiversity number: %.2f", game_state->calculate_biodiversity()));

    // Set sizer flags
    wxSizerFlags flags = wxSizerFlags().Align(wxALIGN_CENTER_HORIZONTAL).Border(wxALL, 5);

    //Add everything to the sizer
    sizer->AddStretchSpacer();
    sizer->Add(completed, flags);
    sizer->Add(profit, flags);
    sizer->Add(logged, flags);
    sizer->Add(diversity, flags);
    sizer->AddStretchSpacer();

    // Apply sizer to the panel
    this->SetSizerAndFit(sizer);
}