#include"game_state.h"
#include<iostream>

int main(){
    GameState gs("Id_example");
    std::cout<<"The game id is "<<gs.get_game_id()<<"\n";
}