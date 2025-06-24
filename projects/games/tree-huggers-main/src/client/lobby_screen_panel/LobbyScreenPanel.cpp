#include "LobbyScreenPanel.h"
#include "../GameController.h"

// event table for the lobby screen panel that binds the start game button to the OnStartGame function
wxBEGIN_EVENT_TABLE(LobbyScreenPanel, wxPanel)
    EVT_BUTTON(wxID_ANY, LobbyScreenPanel::OnStartGame)
wxEND_EVENT_TABLE()

LobbyScreenPanel::LobbyScreenPanel(wxWindow* parent) : wxPanel(parent, wxID_ANY) {
    SetBackgroundColour(*wxWHITE);
    wxBoxSizer* horizontalLayout = new wxBoxSizer(wxHORIZONTAL);
    verticalLayout = new wxBoxSizer(wxVERTICAL);

    // add possible titles to the panel
    waiting = new wxStaticText(this, wxID_ANY, "4 players needed! Waiting for more players to join the game...",wxDefaultPosition, wxSize(500,100));
    waiting->SetFont(waiting->GetFont().Scale(2));
    verticalLayout->Add(waiting, 0, wxALIGN_CENTER | wxALL, 5);

    ready = new wxStaticText(this, wxID_ANY, "Ready to start!",wxDefaultPosition, wxSize(500,100));
    ready->SetFont(waiting->GetFont());
    verticalLayout->Add(ready, 0, wxALIGN_CENTER | wxALL, 5);
    ready->Hide();

    // add start game button
    startGameButton = new wxButton(this, wxID_ANY, wxT("Start Game"));
    verticalLayout->Add(startGameButton, 0, wxALIGN_CENTER | wxALL, 5);
    startGameButton->Hide();

    verticalLayout->AddSpacer(50);

    horizontalLayout->Add(verticalLayout, 1, wxALIGN_CENTER | wxALL, 10);

    SetSizerAndFit(horizontalLayout);
}

void LobbyScreenPanel::AttemptAddPlayer(const std::string& playerName, const std::string& player_id) {
    // player ids are unique. if the player id is already in the list, return
    // (that is, if the find function returns an iterator that is not the end of the list)
    // this avoids adding the same player multiple times
    if(std::find(player_ids.begin(), player_ids.end(), player_id) != player_ids.end() ) {
        return;
    }
    // otherwise, we know it's a new player, so add the player
    players.push_back(playerName);
    player_ids.push_back(player_id);
    // add the player's name to the layout
    wxStaticText* p_txt = new wxStaticText(this, wxID_ANY, playerName);
    verticalLayout->Add(p_txt, 0, wxALIGN_CENTER | wxALL, 5);

    // update the layout to take into account the new player
    AdjustLayout();
}

// NOT IMPLEMENTED YET
// void LobbyScreenPanel::AttemptRemovePlayer(const std::string& playerName, const std::string& player_id) {
//     auto it = std::find(player_ids.begin(), player_ids.end(), player_id);
//     // player ids are unique. if the player id is not in the list, return
//     if(it == player_ids.end() ) {
//         return;
//     }
//     else {
//         player_ids.erase(it);
//         players.erase(players.begin() + std::distance(player_ids.begin(), it));
//     }

//    AdjustLayout();
// }

void LobbyScreenPanel::AdjustLayout() {
     if(players.size() == 4) {
        // ready to start the game
        // show the start game button and the ready to start text
        ready->Show();
        startGameButton->Show();
        // hide the waiting for players text
        waiting->Hide();
    }
    else {
        // waiting for more players
        // hide the ready to start text and the start game button
        ready->Hide();
        startGameButton->Hide();
        // show the waiting for players text
        waiting->Show();
    }
    // trigger a layout update
    Layout();
}

void LobbyScreenPanel::OnStartGame(wxCommandEvent& event) {
    // this shouldn't happen, but just in case
    if (players.size() < 4) {
        wxMessageBox("You can only start the game with four players.", "Error", wxOK | wxICON_ERROR);
        return;
    }
    GameController::startGame();
    // trigger a layout update
    Layout();
}