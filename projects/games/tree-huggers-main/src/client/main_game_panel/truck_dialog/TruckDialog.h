#pragma once

#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/spinctrl.h>

// types of possible truck dialogs
enum truck_dialog_t {
    PLACE_OR_REMOVE,
    BIG_OR_SMALL,
    REMOVE_HOW_MANY
};

class TruckDialog : public wxDialog {
    public:
        /**
        * @brief Constructs a new Truck Dialog object and selects the type of dialog to display
        * @param title the title of the dialog
        * @param dialog_type the type of dialog to display
         */
        TruckDialog(const wxString& title, int dialog_type);

        // getters

        // whether the user presses the place truck or remove truck button
        bool wants_to_place() { return place_truck; }

        // whether the user presses the big truck or small truck button
        bool wants_big_truck() { return big_truck; }

        // whether the user presses the cancel button
        bool wants_to_cancel() { return cancel; }

        // get the number of big or small trucks to remove
        int get_big_num() { return big_num->GetValue(); }
        int get_small_num() { return small_num->GetValue(); }

    private:
        // click event handlers for the buttons
        void TruckPlaceClick(wxCommandEvent& evt); // truck place button
        void NotTruckPlaceClick(wxCommandEvent& evt); // truck remove button
        void BigClick(wxCommandEvent& evt); // big truck button
        void SmallClick(wxCommandEvent& evt); // small truck button
        void CancelClick(wxCommandEvent& evt); // cancel button

        /**
        * @brief Asks the user if they want to place or remove a truck
        * @param td the panel to display the dialog on
        */
        void PlaceOrRemove(wxPanel* td);
        /**
        * @brief Asks the user if they want to place a big or small truck
        * @param td the panel to display the dialog on
         */
        void BigOrSmall(wxPanel* td);
        /**
        * @brief Asks the user how many big or small trucks they want to remove
        * @param td the panel to display the dialog on
         */
        void RemoveHowMany(wxPanel* td);

        // user inputs and choices
        wxSpinCtrl* small_num; // number of small trucks to remove
	    wxSpinCtrl* big_num; // number of big trucks to remove
        bool place_truck; // whether user wants to place a truck
        bool big_truck; // whether user wants to place a big truck
        bool cancel; // whether user wants to cancel the dialog
};

