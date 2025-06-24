# Ecologgical Game Overview

Players in *Ecologgical* take on the role of CEOs of logging companies, managing finances, planning actions, harvesting resources, and selling them in the market. The aim is to strategically interact with both the landscape and other players.

## Board

The board consists of hexagonal cells, representing either **city** or **forest** areas:

- **City Cells** (orange hexagons):  
  - Nothing can be built or placed here.
  
- **Forest Cells**:  
  - Each forest cell has three attributes:
    - **Maximal forest cover (Fmax)**: Ranges from 0 to 10.
    - **Current forest cover (F)**: Ranges from 0 to Fmax.
    - **Accessibility number**: Minimum number of roads on any cell that lies on a path connected to a city (0 if no path exists).
  - The value of **F** determines the cell's color:
    - `F = 0`: grey
    - `0 < F <= 5`: yellow
    - `5 < F <= 7`: light green
    - `7 < F <= 10`: dark green

### Tokens

Tokens that can be placed on the board include:
- **Roads**: Black thin rectangles, usable by all players.
- **Big Trucks**: Colored squares, representing each player.
- **Small Trucks**: Colored triangles, also representing each player.


## Starting State

- **Initial Board State**:
  - Two cities connected by a main road.
  - Forest cover is 10/10 (F/Fmax) on each dark green cell and 0/0 on each grey cell.
  - Forest cells can be:
    - Part of a concession of seven cells (marked by colored borders).
    - Shared (no border).

- **Initial Player State**:
  - Each player starts with a balance of 10, no wood, and no trucks.


## Gameplay

The game is structured in rounds, each consisting of three phases:

### 1. Road Building Phase
- Players build one road per turn until they stop or run out of money.
- **Cost**: 1 unit of money per road.
- Each cell can hold a maximum of three roads, which must be placed adjacent to a city or a road-connected field.
- **Effect**: Each road reduces the maximum forest cover (Fmax) of its cell by 2.
- Roads cannot be removed once placed.

### 2. Truck Buying Phase
- Players can buy as many big or small trucks as they can afford in a single turn.
- **Cost**: 1 unit of money per truck.
- Purchased trucks are stored until the **Truck Placing Phase**.

### 3. Truck Placing Phase
- Players place one truck per turn until they run out of trucks, legal placement fields, or choose to stop.
- **Placement**: Trucks can be placed on the player’s concession cells or shared cells, limited by the cell’s access number.
- Trucks can be removed, with no cost, and are returned to storage.

### End of Round Calculations

1. **Forest Cover**:
   - Forest cover reduces based on the trucks placed:
     - **Small Truck**: -1 to forest cover.
     - **Big Truck**: -2 to forest cover.
   - Forest cover regenerates by 1 each round, up to Fmax, unless it is 0 (then the forest is dead and never regenerates).

2. **Bank Balance**:
   - **Fuel Costs**: For every 5 cells traveled, players pay 1 unit.
   - **Wood Sale**: Players sell logged wood at the current market price, increasing their balance.

3. **Biodiversity**:
   - Calculated as the maximum number of cells that form a continuous forest without roads.
   - If biodiversity falls below half its starting value, the wood price for future rounds is halved.

## Model Elements

| Element     | Effect on Balance         |
|-------------|---------------------------|
| **Road**    | -2 when placed, -1 per piece |
| **Big Truck** | -2 per round, -1 per piece |
| **Small Truck** | -1 per round, -1 per piece |
| **Fuel** | -1 (path < 5), -2 (path >= 5) |
| **Wood** | Dynamic price (+2 initially) |

## End of Game

The game ends after 10 rounds.

### Special Cases

- **Shared Cells Logging Conflict**:
  - If multiple players log on the same cell and exceed its forest cover, no wood is logged, and **F** is set to 0.

- **Multiple Paths to Cities**:
  - The shortest path with the required access number is used for fuel calculation.

- **Negative Balance**:
  - If a player’s balance falls below zero, they cannot buy trucks or build roads until the balance is positive. Players must remove or reposition trucks to become profitable.

---
