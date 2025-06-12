#include"player.h"
#include <gtest/gtest.h>

TEST(PlayerTest, Constructors) {
    //testing the constructor and all the default values
    Player player_1("testid","alex");
    EXPECT_EQ(player_1.get_name(), "alex");
    EXPECT_EQ(player_1.get_balance(), 10);
    EXPECT_EQ(player_1.get_num_trucks(), std::make_pair(0, 0));
    EXPECT_EQ(player_1.get_total_logged(), 0);
    //testing the alternative constructor
    Player player_2("testid",0,"alex",10,0,0,0);
    EXPECT_EQ(player_2.get_name(), "alex");
    EXPECT_EQ(player_2.get_balance(), 10);
    EXPECT_EQ(player_2.get_num_trucks(), std::make_pair(0, 0));
    EXPECT_EQ(player_2.get_total_logged(), 0);
}

TEST(PlayerTest, SetBalance) {
    //testing the set_balance function
    Player player("testid","Alex");
    player.set_balance(100);
    EXPECT_EQ(player.get_balance(), 100);
}

TEST(PlayerTest, AddToBalance) {
    //testing the add to balance function
    Player player("testid","Alex");
    player.add_to_balance(100);
    EXPECT_EQ(player.get_balance(), 110);
    player.add_to_balance(50);
    EXPECT_EQ(player.get_balance(), 160);
}

TEST(PlayerTest, ReduceBalance) {

    //testing add to balance and remove balnace functions
    Player player("testid","Alex");
    player.set_balance(100);
    player.reduce_balance(50);
    EXPECT_EQ(player.get_balance(), 50);
    player.reduce_balance(25);
    EXPECT_EQ(player.get_balance(), 25);
}

TEST(PlayerTest, SetNumTrucks) {
    //testing set and get number of trucks
    Player player("testid","Alex");
    //buying 2 big trucks and 3 small trucks
    player.set_num_trucks(2, 3);
    EXPECT_EQ(player.get_num_trucks(), std::make_pair(2, 3));
}

TEST(PlayerTest, AddNumTrucks) {
    Player player("testid","Alex");
    //adding two big trucks and three small trucks to the players garage
    player.add_num_trucks(2, 3);
    EXPECT_EQ(player.get_num_trucks(), std::make_pair(2, 3));
    //adding another big and another small truck
    player.add_num_trucks(1, 1);
    EXPECT_EQ(player.get_num_trucks(), std::make_pair(3, 4));
}

TEST(PlayerTest, ReduceNumTrucks) {
    Player player("testid","Alex");
    player.set_num_trucks(2, 3);
    //testing reduce number of truck function
    EXPECT_TRUE(player.reduce_num_trucks(1, 2));
    EXPECT_EQ(player.get_num_trucks(), std::make_pair(1, 1));
    //you shouldnt be able to reduce the number of trucks to a value below zero
    EXPECT_FALSE(player.reduce_num_trucks(2, 2));
    EXPECT_EQ(player.get_num_trucks(), std::make_pair(1, 1));
}

TEST(PlayerTest, GetTotalLogged) {
    Player player("testid","Alex");
    //testing gettotallogged
    EXPECT_EQ(player.get_total_logged(), 0);
}

TEST(PlayerTest, Getters) {
    Player player("testid","Alex");
    //testing some of the trivial getters

    player.set_player_id("testid2");
    EXPECT_EQ(player.get_player_id(), "testid2");

    player.set_player_idx(0);
    EXPECT_EQ(player.get_player_idx(), 0);

    player.set_game_id("testid");
    EXPECT_EQ(player.get_game_id(), "testid");


}

