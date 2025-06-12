#include "game_instance.h"
#include "gtest/gtest.h"


TEST(GameInstanceTest, constructor) {
    //testing the constructor
    std::shared_ptr<GameInstance> game_instance = std::make_shared<GameInstance>();
    //should be empty
    EXPECT_TRUE(game_instance->is_empty());
}

TEST(GameInstanceTest, AddPlayer) {
    //testing the addplayer functionality
    std::shared_ptr<GameInstance> game_instance = std::make_shared<GameInstance>();
    std::shared_ptr<Player> player = std::make_shared<Player>("testid","player_name");
    //adding a player via the a pointer
    game_instance->add_player(player);
    //getting the corresponding gamestate
    std::shared_ptr<GameState> game_state = game_instance->get_game_state();

    //testing whether the player was added 
    EXPECT_EQ(game_state->get_num_players(), 1);
    EXPECT_EQ(game_state->get_player_ptr(0)->get_name(), "player_name");
}

TEST(GameInstanceTest, RemovePlayer) {
    //testing the remove player functionality
    std::shared_ptr<GameInstance> game_instance = std::make_shared<GameInstance>();
    std::shared_ptr<Player> player = std::make_shared<Player>("testid","player_name");
    
    std::shared_ptr<GameState> game_state = game_instance->get_game_state();
    //adding a player
    game_instance->add_player(player);
    //creating a player object and adding it via a pointer
    std::shared_ptr<Player> player2 = std::make_shared<Player>("testid","player_name2");
    game_instance->add_player(player2);
    game_instance->remove_player(0);

    EXPECT_EQ(game_state->get_num_players(), 1);

    game_instance->remove_player(1);

    EXPECT_EQ(game_state->get_num_players(), 0);

}

TEST(GameInstanceTest, IsJoinable) {
    //empty gameinstance should be joinable
    std::shared_ptr<GameInstance> game_instance = std::make_shared<GameInstance>();
    EXPECT_TRUE(game_instance->is_joinable());
}

TEST(GameInstanceTest, IsEmpty) {
    //it should also be empty
    std::shared_ptr<GameInstance> game_instance = std::make_shared<GameInstance>();
    EXPECT_TRUE(game_instance->is_empty());
}

TEST(GameInstanceTest, StartGame) {
    //setting up a gameinstance
    std::shared_ptr<GameInstance> game_instance = std::make_shared<GameInstance>();
    std::shared_ptr<Player> player1 = std::make_shared<Player>("testid","player1");
    std::shared_ptr<Player> player2 = std::make_shared<Player>("testid","player2");
    std::shared_ptr<Player> player3 = std::make_shared<Player>("testid","player3");
    std::shared_ptr<Player> player4 = std::make_shared<Player>("testid","player4");
    game_instance->add_player(player1);
    game_instance->add_player(player2);

    //game shouldnt start with only two players
    EXPECT_FALSE(game_instance->start_game(player1));
    game_instance->add_player(player3);
    game_instance->add_player(player4);
    
    //start game should only work with four players
    EXPECT_TRUE(game_instance->start_game(player1));
}
