#include"cell.h"

#include <gtest/gtest.h>
   
//Testing the functionality of placing a road on a cell
TEST(CellTest, PlaceRoad) {
    Cell cell(0,CellType::Forest);
    EXPECT_TRUE(cell.place_road());
    EXPECT_TRUE(cell.place_road());
    EXPECT_TRUE(cell.place_road());
    EXPECT_FALSE(cell.place_road());
    EXPECT_EQ(cell.get_forest_cover_max(), 4);
    //one should not be able to place roads on a city cell
    cell.set_cell_type(CellType::City);
    EXPECT_FALSE(cell.place_road());
}

//testing the functionality of placing trucks on a cell
TEST(CellTest, PlaceTruck) {
    Cell cell(0,CellType::Forest);

    //player 1 should not be able to place since he neither owns the cell
    //nor is there a road leading to a city.
    EXPECT_FALSE(cell.place_truck(std::make_pair(1, TruckType::Big)));

    //player should still be unable to place a road since there is 
    //no road connecting to a city
    cell.set_concessionaire(1);
    EXPECT_FALSE(cell.place_truck(std::make_pair(1, TruckType::Big)));

    //setting the shortest distance is equivalent to placing a road connecting to a city
    cell.set_shortest_distance(0, 1);

    //player 2 should not be able to place a truck since he is not the owner of the cell
    EXPECT_FALSE(cell.place_truck(std::make_pair(2, TruckType::Big)));

    //truck should be placeable
    EXPECT_TRUE(cell.place_truck(std::make_pair(1, TruckType::Big)));

    //only one truck should be placeable since there is only one road
    EXPECT_FALSE(cell.place_truck(std::make_pair(1, TruckType::Big)));
}

TEST(CellTest, RemoveTruck) {
    Cell cell(0,CellType::Forest);
    //shouldnt be able to remove a truck if there is no truck
    EXPECT_FALSE(cell.remove_trucks(1, 1, 1));

    //placing 3 roads on the cell
    //actually placing roads requires the gameboard, on the cell this is equivalent
    for(int i = 0; i < 3; i++) {
        cell.set_shortest_distance(i,1);
    }
    //player 1 places two small trucks and a big truck
    cell.place_truck(std::make_pair(1, TruckType::Big));
    cell.place_truck(std::make_pair(1, TruckType::Small));
    cell.place_truck(std::make_pair(1, TruckType::Small));

    //cant remove 2 big and 2 small trucks
    EXPECT_FALSE(cell.remove_trucks(2, 2, 1));
    //can remove a big and a small truck
    EXPECT_TRUE(cell.remove_trucks(1, 1, 1));
    //cant remove a big truck and a small truck anymore
    EXPECT_FALSE(cell.remove_trucks(1, 1, 1));
    //remove the last small truck
    EXPECT_TRUE(cell.remove_trucks(0, 1, 1));
}

TEST(CellTest, getters){
    Cell cell(0,CellType::Forest);
    //setters are combined into a single test since they are somewhat trivial functions

    //cell should have default forest cover of ten
    EXPECT_EQ(cell.get_forest_cover(),10);
    cell.set_forest_cover(8);
    EXPECT_EQ(cell.get_forest_cover(),8);

    EXPECT_EQ(cell.get_forest_cover_max(),10);
    cell.set_forest_cover_max(8);
    EXPECT_EQ(cell.get_forest_cover_max(),8);

    EXPECT_EQ(cell.get_num_roads(),0);
    cell.set_num_roads(2);
    EXPECT_EQ(cell.get_num_roads(),2);

    EXPECT_EQ(cell.get_cell_type(),CellType::Forest);
    cell.set_cell_type(CellType::City);
    EXPECT_EQ(cell.get_cell_type(),CellType::City);

    EXPECT_EQ(cell.get_cell_id(),0);

    cell.set_concessionaire(2);
    EXPECT_EQ(cell.get_concessionaire(),2);

    cell.set_shortest_distance(0,10);
    cell.set_shortest_distance(1,11);
    cell.set_shortest_distance(2,12);
    EXPECT_EQ(cell.get_shortest_distance(0),10);
    EXPECT_EQ(cell.get_shortest_distance(1),11);
    EXPECT_EQ(cell.get_shortest_distance(2),12);
    
}
