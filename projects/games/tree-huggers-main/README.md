# EcoLOGgical

## Description
EcoLoggical is a game that can be played by four people on any UNIX machine. It aims to emulate logging dynamics in a simplified environment.

The game is based on work done by [LEAF](https://leafic.ch/), specifically on a game called mineset. More resources can be found [here](https://leafic.ch/mineset/).

The current set of game rules can be found [here](./GAMERULES.md)




## Requirements
 - [wxWidgets](https://www.wxwidgets.org/downloads/)
## Build the current project
To build the current state of the project you can simply run the python script *build_project.py* in the *build_scripts* subdirectory. 
To also build and compile tests simply provide a debug argument: *python build_project.py debug*.

Tests will not be part of standard make targets, to compile them in the build directory use *make tests*.


## Starting the game
To play, first you need to start the server by running the executable in *build/src/server* called Ecologgical_Server. This will set up the server clients can connect to later.

Then four players need to run the *Ecologgical_Client* executable in *build/src/client* , enter their name and connect to the server.


## Playing

After four players have joined, each player has the option to start the game.

As specified in the game rules, the game works in a round system, where each round is subdivided into three phases. 

The first phase of each round is the road placing phase. Here players can build new roads on fields connected to a city that have less than three roads on them. Placing a road always costs one unit of money. Roads can be placed by clicking on a valid field on the game board when it is your turn. In any other case, you will get an error telling you what went wrong. Each player can place one road per turn or end their road placing phase leaving the others to continue placing roads.

After all players have completed the road placing phase, the truck shop opens and each player is prompted to buy as many big or small trucks as they want/ can afford. Each truck (big or small), costs 1 unit of money. After clicking buy truck the player's garage is updated. When all players have bought their trucks, the next phase starts.

The third place is the truck handling phase. Here a player can place trucks on a field provided that each of the following is met: The field is in the player's concession: The concessions are marked by coloured borders. The fields in the colour of the player mark their concession. Alternatively, if the field has no coloured border, all players can place trucks on it. Also the field needs to have at least as many roads on it as trucks will be placed on it and a connection to a city with the same capacity has to exist. Finally, the player needs to have enough trucks in their garage.

If there are already trucks on a field that belong to the player, the player will have the option to remove trucks from the field. Naturally, they can only remove as many trucks as they placed.

After all players ended their truck placing phase end of round calculations are performed. The forest cover of each cell is updated according to the logging done and the balances are updated depending on fuel cost as well as on the amount of wood players have logged and on the wood price. The details of this can be found in the rules.

The game automatically ends after ten rounds.

## Complaints
Forward all complaints to nerlbacher@ethz.ch <3
