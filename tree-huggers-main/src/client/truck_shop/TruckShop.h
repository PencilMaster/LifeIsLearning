#pragma once

#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/spinctrl.h>
#include "../../../lib/common/game_state.h"

class TruckShop : public wxDialog
{
public:
	TruckShop(const wxString& title);

	/**
	 * @brief Gets the number of small trucks from the player input
	 * @return the number of small trucks that the player wants to buy
	 */
	int get_small() const {
		return small_num->GetValue();
	}

	/**
	 * @brief Gets the number of big trucks from the player input
	 * @return the number of big trucks that the player wants to buy
	 */
	int get_big() const {
		return big_num->GetValue();
	}
	
	// setters for the player inputs
	void set_small(int val) const {
		small_num->SetValue(val);
	}

	void set_big( int val) const {
		big_num->SetValue(val);
	}

	/**
	 * @brief Sets the game state for the truck shop
	 * @param gs the game state to set
	 */
	void set_game_state(std::shared_ptr<const GameState> gs) {
		if(gs == nullptr) {
			std::cerr<<"TruckShop::set_game_state - game state is null"<<std::endl;
		}
		game_state = gs;
	}

private:
	// the number of small and big trucks that the player wants to buy
	wxSpinCtrl* small_num;
	wxSpinCtrl* big_num;

	std::shared_ptr<const GameState> game_state;
	
	/**
	 * @brief checks if the trucks are buyable and either buys them or gives an error message
	 * @param evt the event that is triggered when the player clicks the buy trucks button
	 */
	void OnClicked(wxCommandEvent& evt);
};

