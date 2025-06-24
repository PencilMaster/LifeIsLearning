#include<gtest/gtest.h>
#include"game_state.h"

TEST(GameStateTest, constructor){
    //testing the constructor of the gamestate 
    GameState gamestate("testid");
    EXPECT_EQ(gamestate.get_game_id(),"testid");
}

TEST(GameStateTest, removing_players){
    //tests adding and removing players
    //one must first create a player and then add him via a pointer
    GameState gamestate("testid");
    std::shared_ptr<Player> homo_sapiens_genderreference_1 = std::make_shared<Player>(Player("testid","homo_sapiens_genderreference1"));
    gamestate.add_player(homo_sapiens_genderreference_1);
    std::shared_ptr<Player> homo_sapiens_genderreference_2 = std::make_shared<Player>(Player("testid","homo_sapiens_genderreference2"));
    std::shared_ptr<Player> homo_sapiens_genderreference_3 = std::make_shared<Player>(Player("testid","homo_sapiens_genderreference3"));
    std::shared_ptr<Player> homo_sapiens_genderreference_4 = std::make_shared<Player>(Player("testid","homo_sapiens_genderreference4"));
    gamestate.add_player(homo_sapiens_genderreference_2);
    gamestate.add_player(homo_sapiens_genderreference_3);
    gamestate.add_player(homo_sapiens_genderreference_4);
    
    //removing the player at player_idx 0
    gamestate.remove_player(0);
    
    std::shared_ptr<Player> homo_sapiens_genderreference_7 = std::make_shared<Player>(Player("testid","homo_sapiens_genderreference7"));
    gamestate.add_player(homo_sapiens_genderreference_7);

    //new player should be added succesfully
    EXPECT_EQ(gamestate.get_player_ptr(0)->get_name(),"homo_sapiens_genderreference7");
    
}

TEST(GameStateTest, setting_balance){
    //setting up the gamestate
    //**************//
    GameState gamestate("testid");
    std::shared_ptr<Player> homo_sapiens_genderreference_1 = std::make_shared<Player>(Player("testid","homo_sapiens_genderreference1"));
    gamestate.add_player(homo_sapiens_genderreference_1);
    std::shared_ptr<Player> homo_sapiens_genderreference_2 = std::make_shared<Player>(Player("testid","homo_sapiens_genderreference2"));
    std::shared_ptr<Player> homo_sapiens_genderreference_3 = std::make_shared<Player>(Player("testid","homo_sapiens_genderreference3"));
    std::shared_ptr<Player> homo_sapiens_genderreference_4 = std::make_shared<Player>(Player("testid","homo_sapiens_genderreference4"));
    gamestate.add_player(homo_sapiens_genderreference_2);
    gamestate.add_player(homo_sapiens_genderreference_3);
    gamestate.add_player(homo_sapiens_genderreference_4);
    //***************//

    //changing the balance of the player at player_idx 0
    gamestate.get_player_ptr(0)->set_balance(20);
    EXPECT_EQ(gamestate.get_player_ptr(0)->get_balance(),20);
    
}

TEST(GameStateTest, place_a_road){
   //setting up the gamestate
    //**************//
    GameState gamestate("testid");
    std::shared_ptr<Player> homo_sapiens_genderreference_1 = std::make_shared<Player>(Player("testid","homo_sapiens_genderreference1"));
    gamestate.add_player(homo_sapiens_genderreference_1);
    std::shared_ptr<Player> homo_sapiens_genderreference_2 = std::make_shared<Player>(Player("testid","homo_sapiens_genderreference2"));
    std::shared_ptr<Player> homo_sapiens_genderreference_3 = std::make_shared<Player>(Player("testid","homo_sapiens_genderreference3"));
    std::shared_ptr<Player> homo_sapiens_genderreference_4 = std::make_shared<Player>(Player("testid","homo_sapiens_genderreference4"));
    gamestate.add_player(homo_sapiens_genderreference_2);
    gamestate.add_player(homo_sapiens_genderreference_3);
    gamestate.add_player(homo_sapiens_genderreference_4);
    //***************//
    //giving player0 20 franken
    gamestate.get_player_ptr(0)->set_balance(20);

    //placing a road on cell 4
    gamestate.place_road(4,0);
    EXPECT_EQ(gamestate.get_cell(4).get_num_roads(),1);
    
    //placing a on cell 6 should not work since it isnt connected to a city
    gamestate.place_road(6,0);
    EXPECT_EQ(gamestate.get_cell(6).get_num_roads(),0);
    
}

TEST(GameStateTest, calculate_biodiversity){
    //setting up the gamestate
    //**************//
    GameState gamestate("testid");
    std::shared_ptr<Player> homo_sapiens_genderreference_1 = std::make_shared<Player>(Player("testid","homo_sapiens_genderreference1"));
    gamestate.add_player(homo_sapiens_genderreference_1);
    std::shared_ptr<Player> homo_sapiens_genderreference_2 = std::make_shared<Player>(Player("testid","homo_sapiens_genderreference2"));
    std::shared_ptr<Player> homo_sapiens_genderreference_3 = std::make_shared<Player>(Player("testid","homo_sapiens_genderreference3"));
    std::shared_ptr<Player> homo_sapiens_genderreference_4 = std::make_shared<Player>(Player("testid","homo_sapiens_genderreference4"));
    gamestate.add_player(homo_sapiens_genderreference_2);
    gamestate.add_player(homo_sapiens_genderreference_3);
    gamestate.add_player(homo_sapiens_genderreference_4);
    //***************//
    gamestate.get_player_ptr(0)->set_balance(20);

    //testing the functionality of calculate_biodiverstiy
    EXPECT_EQ(gamestate.calculate_biodiversity(),18);
    gamestate.place_road(21,0);
    //this shouldnt change biodiversity
    EXPECT_EQ(gamestate.calculate_biodiversity(),18);
    gamestate.place_road(23,0);
    //this should change biodiversity
    EXPECT_EQ(gamestate.calculate_biodiversity(),17);
    gamestate.place_road(11,0);
    gamestate.place_road(23,0);
    gamestate.place_road(24,0);
    gamestate.place_road(20,0);
    gamestate.place_road(21,0);
    gamestate.place_road(33,0);
    gamestate.place_road(9,0);
    //this cuts all the forests in half, strongly reducing biodiverstiy
     EXPECT_EQ(gamestate.calculate_biodiversity(),8);
}

TEST(GameStateTest, place_a_truck){
    //setting up the gamestate
    //**************//
    GameState gamestate("testid");
    std::shared_ptr<Player> homo_sapiens_genderreference_1 = std::make_shared<Player>(Player("testid","homo_sapiens_genderreference1"));
    gamestate.add_player(homo_sapiens_genderreference_1);
    std::shared_ptr<Player> homo_sapiens_genderreference_2 = std::make_shared<Player>(Player("testid","homo_sapiens_genderreference2"));
    std::shared_ptr<Player> homo_sapiens_genderreference_3 = std::make_shared<Player>(Player("testid","homo_sapiens_genderreference3"));
    std::shared_ptr<Player> homo_sapiens_genderreference_4 = std::make_shared<Player>(Player("testid","homo_sapiens_genderreference4"));
    gamestate.add_player(homo_sapiens_genderreference_2);
    gamestate.add_player(homo_sapiens_genderreference_3);
    gamestate.add_player(homo_sapiens_genderreference_4);
    //***************//
    gamestate.get_player_ptr(0)->set_balance(20);

    //player 0 doesnt buy any trucks and should therefore not be able to place any
    gamestate.buy_truck(0,0,0);
    EXPECT_FALSE(gamestate.place_truck(3,0,TruckType::Big));
    EXPECT_EQ(gamestate.get_cell(3).get_trucks().size(),0);

    //player 0 now buys 2 big and 2 small trucks
    gamestate.buy_truck(0,2,2);
    EXPECT_EQ(gamestate.get_cell(3).get_trucks().size(),0);
    auto player = gamestate.get_player_ptr(0);

    EXPECT_EQ(player->get_num_trucks().first,2);
    
    gamestate.place_road(2,0);
    gamestate.update_shortest_distances();
    
    //gamestate.get_cell_ptr(2)->set_shortest_distance(1, 1);
    EXPECT_EQ(gamestate.get_cell_ptr(2)->get_num_roads(),1);
    EXPECT_EQ(gamestate.place_truck(2,0,TruckType::Big),1);
    EXPECT_EQ(gamestate.get_cell(2).get_shortest_distance(0),2);
    
    
}

TEST(GameStateTest, update_shortest_distances){
    GameState gamestate("testid");
    std::shared_ptr<Player> homo_sapiens_genderreference_1 = std::make_shared<Player>(Player("testid","homo_sapiens_genderreference1"));
    gamestate.add_player(homo_sapiens_genderreference_1);
    gamestate.get_player_ptr(0)->set_balance(20);

    //setting up some roads to test the function
    //look at the gameboard to verify the results
    gamestate.place_road(3,0);
    gamestate.place_road(2,0);
    gamestate.place_road(8,0);
    gamestate.place_road(14,0);
    EXPECT_EQ(gamestate.get_cell_ptr(14)->get_num_roads(),1);
    gamestate.update_shortest_distances();
    EXPECT_EQ(gamestate.get_cell_ptr(3)->get_shortest_distance(0),1);
    EXPECT_EQ(gamestate.get_cell_ptr(3)->get_shortest_distance(1),1);
    EXPECT_EQ(gamestate.get_cell_ptr(3)->get_shortest_distance(2),1);
    
    EXPECT_EQ(gamestate.get_cell_ptr(2)->get_shortest_distance(0),2);
    EXPECT_EQ(gamestate.get_cell_ptr(8)->get_shortest_distance(0),3);

    EXPECT_EQ(gamestate.get_cell_ptr(14)->get_shortest_distance(0),4);
    EXPECT_EQ(gamestate.get_cell_ptr(14)->get_shortest_distance(2),0);

}

TEST(GameStateTest, getters){
    //getters and setters are bunched together since they are rather trivial functions

    //setting up the gamestate
    //**************//
    GameState gamestate("testid");
    std::shared_ptr<Player> homo_sapiens_genderreference_1 = std::make_shared<Player>(Player("testid","homo_sapiens_genderreference1"));
    gamestate.add_player(homo_sapiens_genderreference_1);
    std::shared_ptr<Player> homo_sapiens_genderreference_2 = std::make_shared<Player>(Player("testid","homo_sapiens_genderreference2"));
    std::shared_ptr<Player> homo_sapiens_genderreference_3 = std::make_shared<Player>(Player("testid","homo_sapiens_genderreference3"));
    std::shared_ptr<Player> homo_sapiens_genderreference_4 = std::make_shared<Player>(Player("testid","homo_sapiens_genderreference4"));
    gamestate.add_player(homo_sapiens_genderreference_2);
    gamestate.add_player(homo_sapiens_genderreference_3);
    gamestate.add_player(homo_sapiens_genderreference_4);
    //***************//
    gamestate.get_player_ptr(0)->set_balance(20);
    gamestate.get_player_ptr(1)->set_balance(20);
    gamestate.get_player_ptr(2)->set_balance(20);
    gamestate.get_player_ptr(3)->set_balance(20);

    EXPECT_EQ(gamestate.get_cell_indices()[0],std::make_pair(3,0));
    EXPECT_EQ(gamestate.get_cell_indices()[35],std::make_pair(7,6));

    //setting the phases
    gamestate.set_current_phase(phases::lobby);
    EXPECT_EQ(gamestate.get_current_phase(),phases::lobby);
    gamestate.set_current_phase(phases::truck_buy);
    EXPECT_EQ(gamestate.get_current_phase(),phases::truck_buy);

    //changing whose turn it is
    EXPECT_EQ(gamestate.get_current_player(),0);
    gamestate.set_current_player(3);
    EXPECT_EQ(gamestate.get_current_player(),3);

    EXPECT_EQ(gamestate.get_num_players(),4);

    //testing the end current phase functionality
    EXPECT_EQ(gamestate.get_end_phase(2),false);
    gamestate.set_end_phase(2);
    EXPECT_EQ(gamestate.get_end_phase(2),true);

    //getting an setting wood prices
    EXPECT_EQ(gamestate.get_wood_price(),2);
    gamestate.set_wood_price(6);
    EXPECT_EQ(gamestate.get_wood_price(),6);

    //getting and setting fuel prices
    EXPECT_EQ(gamestate.get_fuel_price(),1);
    gamestate.set_fuel_price(6);
    EXPECT_EQ(gamestate.get_fuel_price(),6);

    //getting and setting the current round
    EXPECT_EQ(gamestate.get_current_round(),0);




}

