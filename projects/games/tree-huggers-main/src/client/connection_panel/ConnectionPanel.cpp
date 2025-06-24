#include "ConnectionPanel.h"

#include "../GameController.h"
#include "../../../lib/common/network/default.conf"
#include <iostream>

ConnectionPanel::ConnectionPanel(wxWindow* parent) : wxPanel(parent, wxID_ANY, wxPoint(200,100)) {
    //Set background color
    wxColor white = wxColor(255, 255, 255);
    SetBackgroundColour(white);

    //Declare 2 sizers
    wxBoxSizer* horizontalLayout = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* verticalLayout = new wxBoxSizer(wxVERTICAL);

    //Add the three input fields for the required connection information
    _serverAddressField = new InputField(
        this, // parent element
        "Server address:", // label
        100, // width of label
        default_server_host, // default value (variable from "default.conf")
        240 // width of field
    );
    verticalLayout->Add(_serverAddressField, 0, wxALIGN_CENTER | wxTOP | wxLEFT | wxRIGHT, 10);

    _serverPortField = new InputField(
        this, // parent element
        "Server port:", // label
        100, // width of label
        wxString::Format("%i", default_port), // default value (variable from "default.conf")
        240 // width of field
    );
    verticalLayout->Add(_serverPortField, 0, wxALIGN_CENTER | wxTOP | wxLEFT | wxRIGHT, 10);

    _playerNameField = new InputField(
        this, // parent element
        "Player name:", // label
        100, // width of label
        "", // default value
        240 // width of field
    );
    _playerNameField->SetMaxLength(12); //player Name should be maximum 12 characters
    verticalLayout->Add(_playerNameField, 0, wxALIGN_CENTER | wxTOP | wxLEFT | wxRIGHT, 10);

    // Add the button to try connecting
    wxButton* connectButton = new wxButton(this, wxID_ANY, "Connect", wxDefaultPosition, wxSize(100, 40));
    connectButton->Bind(wxEVT_BUTTON, [](wxCommandEvent& event) {
        GameController::connectToServer();
        });
    verticalLayout->Add(connectButton, 0, wxALIGN_CENTER | wxALL, 10);

    horizontalLayout->Add(verticalLayout, 1, wxALIGN_CENTER | wxALL, 10);

    // Apply the sizers to the window
    SetSizerAndFit(horizontalLayout);
    Layout();
}


wxString ConnectionPanel::getServerAddress() {
    return _serverAddressField->getValue();
}


wxString ConnectionPanel::getServerPort() {
    return _serverPortField->getValue();
}


wxString ConnectionPanel::getPlayerName() {
    return _playerNameField->getValue();
}
