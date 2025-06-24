#pragma once

#include <wx/wx.h>
#include <vector>
#include <string>

// #include "game_state.h"

class LobbyScreenPanel : public wxPanel {
public:
    LobbyScreenPanel(wxWindow* parent);

    void AttemptAddPlayer(const std::string& playerName, const std::string& player_id);
    
    // removing is not yet implemented
    // void AttemptRemovePlayer(const std::string& playerName, const std::string& player_id);

    void AdjustLayout();

    void OnStartGame(wxCommandEvent& event);

private:
    wxBoxSizer* verticalLayout;
    wxStaticText* waiting;
    wxStaticText* ready;
    wxStaticText* reduce;
    wxButton* startGameButton;

    std::vector<std::string> players;
    std::vector<std::string> player_ids;

    wxDECLARE_EVENT_TABLE();
};