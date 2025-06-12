#pragma once
#include "GridPanel.h"
#include "InfoPanel.h"

class MainGamePanel : public wxPanel {
    public:
        /**
        * @brief Constructs a new Main Game Panel object, including the info and grid panels
        *           as well as the end phase button and layouts them in a sizer
        * @param parent the parent frame where the panel will be placed
        */
        MainGamePanel(wxFrame* parent);

        /**
        * @brief Initializes the main game panel's as well as its sub-panels' game state
        * @param gs the game state to initialize with
        */
        void initial_game_state(std::shared_ptr<const GameState> gs);

        /**
        * @brief Updates the main game panel's as well as its sub-panels' game state
        * @param gs the game state to update with
        */
        void update_game_state(std::shared_ptr<const GameState> gs);

    private:
        std::shared_ptr<const GameState> game_state;

        // sub-panels
        GridPanel* gridPanel;
        InfoPanel* infoPanel;

        wxButton* EndPhaseButton;

        /**
        * @brief Event handler for the end phase button that interacts with the GameController
        * @param event the button click event
        */
        void EndPhaseButtonClick(wxCommandEvent& event);

        /**
        * @brief Event handler for the paint event
        * @param event the paint event
        */
        void OnPaint(wxPaintEvent& event);

        // tells the compiler that this class will define its own event table to handle the above events
        // this happens in the .cpp file
        DECLARE_EVENT_TABLE()
};