#include "GameStartDialog.h"

GameStartDialog::GameStartDialog(const wxString& title) : wxDialog(nullptr, wxID_ANY, title) {
    // Declare sizer
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    // Set Background color and size
    SetBackgroundColour(wxColor(255,255,255));
    SetSize(wxSize(600, 400));

    // Declare all texts to display and their fonts
    wxStaticText* started = new wxStaticText(this, wxID_ANY, "Game Started!", wxDefaultPosition);
    started->SetFont(started->GetFont().Scale(3));

    wxStaticText* initial_balance = new wxStaticText(this, wxID_ANY, wxString::Format("Your starting balance: %d", 10));
    wxStaticText* concession_color = new wxStaticText(this, wxID_ANY, wxString::Format("Your concessions: %s", "red"));
    wxStaticText* initial_biodiversity = new wxStaticText(this, wxID_ANY, wxString::Format("Initial biodiversity number: %d", 10));

    // Declare sizer flags
    wxSizerFlags flags = wxSizerFlags().Align(wxALIGN_CENTER_HORIZONTAL).Border(wxALL, 5);
    
    // "Ok" Button
    wxButton* button = new wxButton(this, wxID_ANY, "Ok");
    button->Bind(wxEVT_BUTTON, &GameStartDialog::OnClicked, this);

    //Add everything to sizer
    sizer->AddStretchSpacer();
    sizer->Add(started, flags);
    sizer->AddSpacer(30);
    sizer->Add(initial_balance, flags);
    sizer->Add(concession_color, flags);
    sizer->Add(initial_biodiversity, flags);
    sizer->AddSpacer(30);
    sizer->Add(button,flags);
    sizer->AddStretchSpacer();

    //Apply sizer to this dialog window
    this->SetSizerAndFit(sizer);
}

void GameStartDialog::OnClicked(wxCommandEvent& evt) {
    EndModal(wxID_OK);
    Destroy();
}
