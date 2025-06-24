#pragma once

#include <wx/wx.h>
#include <memory>
#include "../../../lib/common/game_state.h"

class EndScreenPanel : public wxPanel
{
public:	
	/**
		 * @brief Constructs a new end screen panel for a parent window with a given game state
		 * @param parent the parent window that will display the panel
		 * @param game_state the game state at the end of the game
		 */
	EndScreenPanel(wxWindow* parent, std::shared_ptr<const GameState> game_state);

private:
};