#include "TruckShop.h"
#include "../GameController.h"

#include <wx/wx.h>
#include <wx/spinctrl.h>

enum IDs {
	BIG_ID = 3,
	SMALL_ID = 4
};

TruckShop::TruckShop(const wxString& title)
    : wxDialog(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(500, 400)) {
    // create the truck shop panel
    wxPanel* shop = new wxPanel(this, wxID_ANY);
    shop->SetBackgroundColour(wxColor(230, 252, 252));

    // create the main sizer for proper layouting
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // create the title text and add it to the main sizer
    wxStaticText* titletext = new wxStaticText(shop, wxID_ANY, "Truck Shop:");
	titletext->SetFont(titletext->GetFont().Scale(1.5));
    mainSizer->Add(titletext, 0, wxALIGN_LEFT | wxALL, 30);

    // create the big truck label and input field and add them to a big truck sizer
    wxBoxSizer* bigSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* big_t = new wxStaticText(shop, wxID_ANY, "Big Trucks:");
    big_num = new wxSpinCtrl(shop, BIG_ID, "", wxDefaultPosition, wxSize(-1, -1), wxSP_WRAP, 0, 100);
    bigSizer->Add(big_t, 1, wxALIGN_CENTER | wxALL, 10);
    bigSizer->Add(big_num, 1, wxALIGN_CENTER | wxALL, 10);

    // create the small truck label and input field and add them to a small truck sizer
    wxBoxSizer* smallSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* small_t = new wxStaticText(shop, wxID_ANY, "Small Trucks:");
    small_num = new wxSpinCtrl(shop, SMALL_ID, "", wxDefaultPosition, wxSize(-1, -1), wxSP_WRAP, 0, 100);
    smallSizer->Add(small_t, 1, wxALIGN_CENTER | wxALL, 10);
    smallSizer->Add(small_num, 1, wxALIGN_CENTER | wxALL, 10);

    // add the big and small truck sizers to the main sizer
    mainSizer->Add(bigSizer, 0, wxALIGN_CENTER | wxALL, 10);
    mainSizer->Add(smallSizer, 0, wxALIGN_CENTER | wxALL, 10);

    // create the buy trucks button and add it to the main sizer
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* buy_trucks = new wxButton(shop, wxID_OK, "Buy Trucks");
    buttonSizer->Add(buy_trucks, 1, wxALIGN_CENTER | wxALL, 10);
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 10);

    // bind the event created by the buy trucks button to the OnClicked function
    buy_trucks->Bind(wxEVT_BUTTON, &TruckShop::OnClicked, this);

    // set and the main sizer to the shop panel
    shop->SetSizerAndFit(mainSizer);
}

void TruckShop::OnClicked(wxCommandEvent& evt) {
    if(game_state == nullptr) {
        std::cerr<<"TruckShop::OnClicked - game state is null"<<std::endl;
    }
	if(game_state->are_trucks_buyable(game_state->get_current_player(), get_big(), get_small())) {
        // the number of trucks that the player wants to buy is buyable, 
        // so show a message box with the number of trucks bought
		wxMessageBox(wxString::Format("You bought %d big truck%s and %d small truck%s",  
                                        get_big(), get_big() == 1 ? "" : "s",
                                        get_small(), get_small() == 1 ? "" : "s"),
                        "Info", wxOK | wxICON_INFORMATION);
        
        // buy the trucks via the game controller
		GameController::buyTruck();
        
        // close the truck shop
        EndModal(wxID_OK);
	}
	else {
        // the number of trucks that the player wants to buy is not buyable, 
        // so show an error message
		GameController::showError("Input Error", "You don't have enough money to buy that many trucks");
	}
}