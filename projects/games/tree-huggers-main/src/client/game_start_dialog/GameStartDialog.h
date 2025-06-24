#pragma once

#include <wx/wx.h>
#include <wx/dialog.h>

class GameStartDialog : public wxDialog
{
	public:
		/**
		 * @brief Constructs a new Game Start Dialog object with the given title, 
		 * 		initializes all the texts that give starting information:
		 * 		started, initial balance, concession color, initial biodiversity number
		 * @param title the title of the dialog
		 */
		GameStartDialog(const wxString& title);
	private:
		/**
		 * @brief Event handler for the button click event that closes the dialog
		 * @param evt the button click event
		 */
		void OnClicked(wxCommandEvent& evt);
};