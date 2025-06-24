#include "TruckDialog.h"
#include "../../GameController.h"

TruckDialog::TruckDialog(const wxString& title, int dialog_type) : 
        wxDialog(nullptr,wxID_ANY,title,wxDefaultPosition,wxSize(600,500)) {
	// create a panel, set its background color, and get the dialog type
    wxPanel* td = new wxPanel(this, wxID_ANY);
	td->SetBackgroundColour(wxColor(230,252,252));
    
    // display the correct dialog based on the dialog type
    if(dialog_type == truck_dialog_t::PLACE_OR_REMOVE) PlaceOrRemove(td);
    else if(dialog_type == truck_dialog_t::BIG_OR_SMALL) BigOrSmall(td);
    else RemoveHowMany(td);

}

void TruckDialog::PlaceOrRemove(wxPanel* td) {
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->AddStretchSpacer();

    // add title text
    wxStaticText* titletext = new wxStaticText(td, wxID_ANY, 
        "Do you want to place or remove a truck?");
    titletext->SetFont(titletext->GetFont().Scale(1.2));
    mainSizer->Add(titletext, 0, wxALIGN_CENTER | wxALL, 10);
    mainSizer->AddSpacer(20);
    
    // add truck place button and truck remove button
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* TruckPlaceButton = new wxButton(td, wxID_ANY, _T("Place Truck"));
    wxButton* TruckRemoveButton = new wxButton(td, wxID_ANY, _T("Remove Truck"));
    buttonSizer->Add(TruckPlaceButton, 1, wxALIGN_CENTER | wxALL, 10);
    buttonSizer->Add(TruckRemoveButton, 1, wxALIGN_CENTER | wxALL, 10);

    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 10);

    // add cancel button
    wxButton* cancel = new wxButton(td, wxID_CANCEL, "Cancel");
    mainSizer->Add(cancel, 0, wxALIGN_CENTER | wxALL, 10);
    mainSizer->AddStretchSpacer();

    // bind the buttons to their respective click events
    TruckPlaceButton->Bind(wxEVT_BUTTON, &TruckDialog::TruckPlaceClick, this);
    TruckRemoveButton->Bind(wxEVT_BUTTON, &TruckDialog::NotTruckPlaceClick, this);
    cancel->Bind(wxEVT_BUTTON, &TruckDialog::CancelClick, this);

    td->SetSizerAndFit(mainSizer);
}

void TruckDialog::BigOrSmall(wxPanel* td) {
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->AddStretchSpacer();
    
    // add title text
    wxStaticText* titletext = new wxStaticText(td, wxID_ANY, 
        "Do you want to place a big or small truck?");
    titletext->SetFont(titletext->GetFont().Scale(1.2));
    mainSizer->Add(titletext, 0, wxALIGN_CENTER | wxALL, 10);
    mainSizer->AddSpacer(20);

    // add big truck button and small truck button
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* big = new wxButton(td, wxID_ANY, _T("Big Truck"));
    wxButton* small = new wxButton(td, wxID_ANY, _T("Small Truck"));
    buttonSizer->Add(big, 1, wxALIGN_CENTER | wxALL, 10);
    buttonSizer->Add(small, 1, wxALIGN_CENTER | wxALL, 10);

    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 10);

    // add cancel button
    wxButton* cancel = new wxButton(td, wxID_CANCEL, "Cancel");
    mainSizer->Add(cancel, 0, wxALIGN_CENTER | wxALL, 10);
    mainSizer->AddStretchSpacer();

    // bind the buttons to their respective click events
    big->Bind(wxEVT_BUTTON, &TruckDialog::BigClick, this);
    small->Bind(wxEVT_BUTTON, &TruckDialog::SmallClick, this);
    cancel->Bind(wxEVT_BUTTON, &TruckDialog::CancelClick, this);

    td->SetSizerAndFit(mainSizer);
}

void TruckDialog::RemoveHowMany(wxPanel* td) {
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->AddStretchSpacer();

    // add title text
    wxStaticText* titletext = new wxStaticText(td, wxID_ANY, 
        "How many big or small trucks do you want to remove?");
    titletext->SetFont(titletext->GetFont().Scale(1.2));
    mainSizer->Add(titletext, 0, wxALIGN_CENTER | wxALL, 10);
    mainSizer->AddSpacer(20);

    // add texts and spin controls (input fields) for big and small trucks
    wxBoxSizer* bigSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* big_text = new wxStaticText(td, wxID_ANY, "Big Trucks:");
    big_num = new wxSpinCtrl(td, wxID_ANY, "", wxDefaultPosition, wxSize(-1, -1), wxSP_WRAP, 0, 3);
    bigSizer->Add(big_text, 1, wxALIGN_CENTER | wxALL, 10);
    bigSizer->Add(big_num, 1, wxALIGN_CENTER | wxALL, 10);

    wxBoxSizer* smallSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* small_text = new wxStaticText(td, wxID_ANY, "Small Trucks:");
    small_num = new wxSpinCtrl(td, wxID_ANY, "", wxDefaultPosition, wxSize(-1, -1), wxSP_WRAP, 0, 3);
    smallSizer->Add(small_text, 1, wxALIGN_CENTER | wxALL, 10);
    smallSizer->Add(small_num, 1, wxALIGN_CENTER | wxALL, 10);

    mainSizer->Add(bigSizer, 0, wxALIGN_CENTER | wxALL, 10);
    mainSizer->Add(smallSizer, 0, wxALIGN_CENTER | wxALL, 10);

    // add remove trucks button and cancel button
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* remove = new wxButton(td, wxID_OK, "Remove Trucks");
    wxButton* cancel = new wxButton(td, wxID_CANCEL, "Cancel");
    buttonSizer->Add(remove, 1, wxALIGN_CENTER | wxALL, 10);
    buttonSizer->Add(cancel, 1, wxALIGN_CENTER | wxALL, 10);

    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 10);
    mainSizer->AddStretchSpacer();

    // bind the buttons to their respective click events
    remove->Bind(wxEVT_BUTTON, &TruckDialog::TruckPlaceClick, this);
    cancel->Bind(wxEVT_BUTTON, &TruckDialog::CancelClick, this);

    td->SetSizerAndFit(mainSizer);
}


void TruckDialog::TruckPlaceClick(wxCommandEvent& evt) {
    place_truck = true;
    cancel = false;
    EndModal(0);
	Destroy();
}

void TruckDialog::NotTruckPlaceClick(wxCommandEvent& evt) {
    place_truck = false;
    cancel = false;
    EndModal(0);
	Destroy();
}

void TruckDialog::BigClick(wxCommandEvent& evt) {
    big_truck = true;
    cancel = false;
    EndModal(0);
    Destroy();
}

void TruckDialog::SmallClick(wxCommandEvent& evt) {
    big_truck = false;
    cancel = false;
    EndModal(0);
    Destroy();
}

void TruckDialog::CancelClick(wxCommandEvent& evt) {
    cancel = true;
    place_truck = false;
    EndModal(0);
    Destroy();
}