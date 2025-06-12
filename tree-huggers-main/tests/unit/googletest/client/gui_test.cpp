#include<gtest/gtest.h>
#include "game_window/GameWindow.h"
#include"game_state.h"

// class App : public wxApp {
// 	public:
// 		bool OnInit();
// };

// wxIMPLEMENT_APP(App);

// bool App::OnInit() {


//     GameWindow* game_window = new GameWindow(
//     "EcoLOGgical", // title of window,
//     wxDefaultPosition, // position of the window
//     wxDefaultSize, // size of the window
//     game_state
//     );

//     game_window->showMainGamePanel(game_state);
//     game_window->Show();

// 	return true;
// }

TEST(GUITest, gameboard){
    GameState gamestate("testid");
    std::shared_ptr<Player> boy_1 = std::make_shared<Player>(Player("boy1"));
    gamestate.add_player(boy_1);
    std::shared_ptr<Player> boy_2 = std::make_shared<Player>(Player("boy2"));
    std::shared_ptr<Player> boy_3 = std::make_shared<Player>(Player("boy3"));
    std::shared_ptr<Player> boy_4 = std::make_shared<Player>(Player("boy4"));
    gamestate.add_player(boy_2);
    gamestate.add_player(boy_3);
    gamestate.add_player(boy_4);
    gamestate.get_player_ptr(0)->set_balance(20);
    gamestate.place_road(4,0);
    gamestate.place_road(6,0);
    gamestate.place_road(6,0);
    gamestate.place_road(6,0);
    gamestate.place_road(20,0);
    gamestate.place_road(21,0);
    gamestate.buy_truck(0,0,0);
    gamestate.place_truck(3,0,TruckType::Big);
    gamestate.buy_truck(0,2,2);

    std::shared_ptr<const GameState> game_state = std::make_shared<const GameState>(gamestate);

    GameWindow* game_window = new GameWindow(
        "EcoLOGgical", // title of window,
        wxDefaultPosition, // position of the window
        wxDefaultSize, // size of the window
        game_state
    );

    // show main game panel containing grid and stats
    game_window->showMainGamePanel(game_state);
    game_window->Show();
    
    EXPECT_EQ(gamestate.get_player_ptr(0)->get_name(),"boy1");
    
}