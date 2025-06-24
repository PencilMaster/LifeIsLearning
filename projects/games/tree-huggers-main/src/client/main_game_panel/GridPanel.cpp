#include "GridPanel.h"
#include <iostream>
#include <limits.h>
#include <wx/colour.h>
#include "truck_dialog/TruckDialog.h"
#include "../GameController.h"

// event table for the grid panel that binds the paint and left click events to the OnPaint and HexClick functions
BEGIN_EVENT_TABLE(GridPanel, wxPanel)
    EVT_PAINT(GridPanel::OnPaint)
    EVT_LEFT_DOWN(GridPanel::HexClick)
END_EVENT_TABLE();

// initialize static non const variables
const wxColour GridPanel::purple{184, 21, 161}; 
const wxColour GridPanel::yellow{255, 197, 34};
const wxColour GridPanel::blue{13, 180, 206};
const wxColour GridPanel::red{184, 21, 62};
const std::vector<wxColour> GridPanel::player_colors = {purple, yellow.ChangeLightness(80), blue, red};
const wxColour GridPanel::background_color{230, 240, 190}; // pastel green

GridPanel::GridPanel(wxWindow* parent) : wxPanel(parent, wxID_ANY) {
}

void GridPanel::initial_game_state(std::shared_ptr<const GameState> gs) {
    game_state_ptr = gs;
    InitBoard();
}

void GridPanel::update_game_state(std::shared_ptr<const GameState> gs) {
    game_state_ptr = gs;
    // redraw the panel
    Refresh();
}

void GridPanel::InitBoard() {
    SetBackgroundColour(background_color); 
    const double radius_with_border = radius + border_width;
    double x_spacing = 1.5 * radius_with_border;  // Horizontal distance between hex centers
    double y_spacing = sqrt(3) * radius_with_border;  // Vertical distance between hex centers

    // grid bounding box variables
    int smallest_x = INT_MAX;
    int smallest_y = INT_MAX;
    int largest_x = 0;
    int largest_y = 0;

    // Create the hexagons from the game state's cell indices
    for(auto& cell_pos : game_state_ptr->get_cell_indices()) {
        // Calculate the center of the current hexagon
        auto row = cell_pos.first;
        auto col = cell_pos.second;
        double x = col * x_spacing; // x coordinate of the center
        double y = row * y_spacing - col * y_spacing / 2; // y coordinate of the center

        // bounding box calculations
        if(x < smallest_x) smallest_x = x;
        else if (x > largest_x) largest_x = x;
        if(y < smallest_y) smallest_y = y;
        else if(y > largest_y) largest_y = y;

        // Create the 6 points for the hexagon centered at (x, y)
        std::vector<wxPoint> hex_points;
        for (int i = 0; i < 6; i++) {
            hex_points.push_back(wxPoint(
                x + radius * std::cos(i * M_PI / 3), 
                y + radius * std::sin(i * M_PI / 3)
            ));
        }

        // determine the color of the hexagon based on the cell type
        Cell cell = game_state_ptr->get_cell(row, col);
        wxColour fill, border;
        if(cell.get_cell_type() == City) {
            fill = city_col;
            border = city_col;
        }
        else if(cell.get_forest_cover() == 0) {
            fill = dead_forest_col;
            border = dead_forest_col;
        }
        else {
            fill = full_forest_col;
            border = full_forest_col;
        }
        auto conc = cell.get_concessionaire();
        if(conc != -1) {
            border = player_colors[conc];
        }

        // create the hexagon and add it to the vector
        int index = cell.get_cell_id();
        wxPoint center(x, y);
        Hexagon hex = {index, center, fill, border, hex_points};
        hexagons.push_back(hex);
    }

    // calculate the grid's bounding box dimensions
    grid_width = (largest_x - smallest_x);
    grid_height = (largest_y - smallest_y);
}

void GridPanel::OnPaint(wxPaintEvent& event) {
    // create a buffered paint device context to draw on
    // this avoids flickering
    wxBufferedPaintDC dc(this);
    // clear the panel before drawing to avoid overlaps
    dc.Clear();
    // draw the grid and its tokens
    DrawGrid(dc);
    DrawRoads(dc);
    DrawTrucks(dc);
}

void GridPanel::HexClick(wxMouseEvent& event) {
    // Get the mouse click position
    wxPoint click_pos = event.GetPosition();

    // Check if the click is inside any of the hexagons
    for (auto hex : hexagons) {
        if (IsInsideHexagon(click_pos, hex.offset_center)) {
            auto hex_i = hex.index;
            phases phase = game_state_ptr->get_current_phase();
            int player_idx = game_state_ptr->get_player_ptr(GameController::get_my_id())->get_player_idx();
            auto cell_type = game_state_ptr->get_cell(hex_i).get_cell_type();
            bool city_cell = cell_type == City;
            
            if(player_idx != game_state_ptr->get_current_player() && !city_cell) { 
                // don't show the error on city cells since they're not supposed to be interacted with
                GameController::showError("User Error", "It's not your turn!");
                return;
            }

            if (phase == truck_place) {
                // at least 1 truck placeable
                bool truck_placeable = game_state_ptr->is_truck_placeable(hex_i, player_idx, TruckType::Small) || 
                                    game_state_ptr->is_truck_placeable(hex_i, player_idx, TruckType::Big);
                // at least 1 truck removable
                bool truck_removable = game_state_ptr->are_trucks_removable(hex_i, player_idx, 1, 0) || 
                                        game_state_ptr->are_trucks_removable(hex_i, player_idx, 0, 1);
                
                // Handle scenarios based on truck placeability and removability
                if (truck_placeable && truck_removable) {
                    // Both placeable and removable: ask user whether to place or remove
                    TruckDialog* dialog = new TruckDialog("Truck Handling", truck_dialog_t::PLACE_OR_REMOVE);
                    dialog->ShowModal();

                    if (dialog->wants_to_cancel()) { // user clicked cancel
                        return;
                    } else if (dialog->wants_to_place()) { // user clicked place
                        HandleTruckPlacing(hex_i);
                    } else { // user clicked remove
                        HandleTruckRemoving(hex_i);
                    }

                } else if (truck_placeable) { // truck placeable but not removable
                    HandleTruckPlacing(hex_i);
                } else if (truck_removable) { // truck removable but not placeable
                    HandleTruckRemoving(hex_i);
                } else { // trucks neither removable nor placeable
                    if(game_state_ptr->get_player_ptr(player_idx)->get_num_trucks().first == 0 && game_state_ptr->get_player_ptr(player_idx)->get_num_trucks().second == 0) {
                        // the player doesn't have any trucks in the garage
                        GameController::showError("User Error", "You don't have any trucks to place");
                    }
                    else {
                        // there are no free roads or no trucks belonging to the player on this cell
                        GameController::showError("User Error", "You cannot place or remove trucks here");
                    }
                }
            }
            else if(phase == road_place) {
                if(game_state_ptr->is_road_placeable(hex_i, player_idx)) {
                    GameController::placeRoad(hex_i);
                }
                else {
                    if(game_state_ptr->get_player_ptr(player_idx)->get_balance() <= 0){
                        GameController::showError("User Error", "You don't have enough money to place a road");
                    }
                    else if (game_state_ptr->get_cell(hex_i).get_num_roads() == 3){
                        GameController::showError("User Error", "This cell is already filled with roads");
                    }
                    else if (game_state_ptr->get_cell(hex_i).get_cell_type() == City){
                        GameController::showError("User Error", "You cannot place roads on city cells");
                    }
                    else{
                        GameController::showError("User Error", "This cell is not connected to a city");
                    }
                }
                
            }
            
            return; 
        }
    }
}

bool GridPanel::IsInsideHexagon(const wxPoint& point, const wxPoint& hexCenter) {
    double dx = point.x - hexCenter.x;
    double dy = point.y - hexCenter.y;
    double distance = sqrt(dx * dx + dy * dy);
    return distance < radius - border_width;
}

void GridPanel::HandleTruckPlacing(int hex_i) {  
    // Ask user if they want to place a big or small truck  
    TruckDialog* dialog = new TruckDialog("Truck Management", truck_dialog_t::BIG_OR_SMALL);
    dialog->ShowModal();

    int player_idx = game_state_ptr->get_current_player();
    
    if(dialog->wants_to_cancel()) { // player pressed cancel button
        return;
    }
    else if(dialog->wants_big_truck()) { 
        // user wants to place big truck
        if(game_state_ptr->is_truck_placeable(hex_i, player_idx, TruckType::Big)) {
            GameController::placeTruck(hex_i, TruckType::Big);
        }
        else if(game_state_ptr->get_cell(hex_i).get_num_trucks() >= 3){
            GameController::showError("User Error", "You cannot place any more big trucks on this cell");
        }
        else if(game_state_ptr->get_player_ptr(player_idx)->get_num_trucks().first == 0){
            GameController::showError("User Error", "You don't have any big trucks in your garage");
        }
        else {
            GameController::showError("User Error", "You cannot place any (more) big trucks here");
        }
    }
    else { // user wants to place small truck
        if(game_state_ptr->is_truck_placeable(hex_i, player_idx, TruckType::Small)) {
            GameController::placeTruck(hex_i, TruckType::Small);
        }
        else if(game_state_ptr->get_cell(hex_i).get_num_trucks() >= 3){
            GameController::showError("User Error", "You cannot place any more small trucks on this cell");
        }
        else if(game_state_ptr->get_player_ptr(player_idx)->get_num_trucks().second == 0){
            GameController::showError("User Error", "You don't have any small trucks in your garage");
        }
        else {
            GameController::showError("User Error", "You cannot place any (more) small trucks here");
        }
    }
    
}

void GridPanel::HandleTruckRemoving(int hex_i) {
    // Ask user how many big or small trucks they want to remove from the cell
    TruckDialog* dialog = new TruckDialog("Truck Management", truck_dialog_t::REMOVE_HOW_MANY);
    dialog->ShowModal();
    // get the numbers that were entered
    int num_small_trucks = dialog->get_small_num();
    int num_big_trucks = dialog->get_big_num();
    int player_idx = game_state_ptr->get_current_player();

    if(dialog->wants_to_cancel() || (num_small_trucks == 0 && num_big_trucks == 0)) {
        // the player doesn't want to do anything
        return;
    }
    if(game_state_ptr->are_trucks_removable(hex_i,player_idx, num_big_trucks, num_small_trucks)) {
        GameController::removeTruck(hex_i, num_big_trucks, num_small_trucks);
    }
    else { // the player entered too many trucks
        GameController::showError("Error", "You cannot remove the specified number of trucks here");
    }
}

void GridPanel::DrawGrid(wxDC& dc) {
    // Get the panel's size
    int panel_width, panel_height;
    GetClientSize(&panel_width, &panel_height);

    // Calculate offsets to center the grid
    int x_offset = (panel_width - grid_width) / 2;
    int y_offset = (panel_height - grid_height) / 2;

    // for greyed out hexagons feature
    // bool my_turn = InfoPanel::me_idx == game_state_ptr->get_current_player();

    // Draw each hexagon with the calculated offset
    int no_hexagons = hexagons.size();
    for (int i = 0; i < no_hexagons; ++i) {
        // update the hexagon's fill color based on the current game state
        UpdateHexagon(i); // include my_turn for greyed out hexagons feature
        Hexagon& hex = hexagons[i];
        // ensure the grid is centered
        std::vector<wxPoint> centered_vertices;
        for (const auto& vertex : hex.vertices) {
            centered_vertices.push_back(wxPoint(vertex.x + x_offset, vertex.y + y_offset));
        }
        hex.offset_center = wxPoint(hex.center.x + x_offset, hex.center.y + y_offset);

        DrawHexagon(dc, i, centered_vertices);
    }
}

void GridPanel::DrawHexagon(wxDC& dc, int idx, const std::vector<wxPoint>& vertices) {
    Hexagon& hex = hexagons[idx];
    // set fill color
    dc.SetBrush(hex.fillColor); 
    // Set border color and width
    dc.SetPen(wxPen(hex.borderColor, border_width)); 
    dc.DrawPolygon(hex.vertices.size(), vertices.data());
    // dc.DrawText(wxString::Format("%d", hex.index), hex.offset_center - wxPoint(radius/5, radius/5));
}

void GridPanel::DrawRoads(wxDC& dc) {
    wxPoint start, end; // start and end pixels of each road
    int cells = hexagons.size()-1; // exclude city cells
    
    for(int i = 1; i < cells; ++i) {
        // get the number of roads on this cell
        auto roads = game_state_ptr->get_cell(i).get_num_roads();

        if(roads != 0) {
            // offsets for placing the roads within the hexagon
            double x_sc = 3*radius/10;
            double y_sc = radius/5;

            wxPoint center = hexagons[i].offset_center;

            // set road color and width
            dc.SetPen(wxPen(*wxBLACK, border_width));

            // draw the first road at the bottom of the hexagon
            start = center + wxPoint(-x_sc, 3*y_sc);
            end = center + wxPoint(x_sc, 3*y_sc);
            dc.DrawLine(start, end);

            if(roads >= 2) {
                // scale the x offset so this road is a little longer
                x_sc = 4.3*radius/10;
                // draw the second road on top of the first
                start = center + wxPoint(-x_sc, 2*y_sc);
                end = center + wxPoint(x_sc, 2*y_sc);
                dc.DrawLine(start, end);
            }

            if(roads == 3) {
                // scale the x offset so this road is a little longer
                x_sc = 5.5*radius/10;
                // draw the third road on top of the second
                start = center + wxPoint(-x_sc, y_sc);
                end = center + wxPoint(x_sc, y_sc);
                dc.DrawLine(start, end);
            }
        }
    }
    return;
}

void GridPanel::DrawTrucks(wxDC& dc) {
    int cells = hexagons.size();
    for(int i = 0; i < cells; ++i) {
        wxPoint center = hexagons[i].offset_center;

        // offsets within the hexagon for placing the trucks
        double x_off = 4.5*radius/10;
        double y_off = 3.8*radius/10;
        std::vector<wxPoint> offsets = {
            wxPoint(0, -y_off),
            wxPoint(-x_off, 0),
            wxPoint(x_off, 0)
        }; 

        // draw all the trucks on this cell in the correct positions and colors
        auto trucks = game_state_ptr->get_cell(i).get_trucks(); 
        int num_trucks = trucks.size();
        for(int t = 0; t < num_trucks; ++t) {
            auto truck = trucks[t];
            wxColor color = player_colors[truck.first];
            wxPoint position = center + offsets[t];

            if(truck.second == TruckType::Small) {
                DrawSmallTruck(dc, position, GridPanel::truck_size, color);
            }
            else {
                DrawBigTruck(dc, position, GridPanel::truck_size, color);
            } 
        }
    }
    return;
}

void GridPanel::DrawSmallTruck(wxDC& dc, const wxPoint& position, double sc_fac, const wxColour& color) {
    dc.SetBrush(wxBrush(color));
    dc.SetPen(wxPen(*wxBLACK, border_width/2));
    // calculate vertices of a triangle of base side length sc_fac
    wxPoint points[3] = {wxPoint(sc_fac * -0.5, 0),
                        wxPoint(sc_fac * 0.5, 0),
                        wxPoint(0, sc_fac * -1)};
    // draw the triangle onto the device context
    dc.DrawPolygon(3, points, position.x, position.y);
}

void GridPanel::DrawBigTruck(wxDC& dc, const wxPoint& position, double sc_fac, const wxColour& color) {
    dc.SetBrush(wxBrush(color));
    dc.SetPen(wxPen(*wxBLACK, border_width/2));
    // calculate vertices of a square of side length sc_fac
    wxPoint points[4] = {wxPoint(sc_fac * -0.5, 0),
                        wxPoint(sc_fac * 0.5, 0),
                        wxPoint(sc_fac * 0.5, sc_fac * -1),
                        wxPoint(sc_fac * -0.5, sc_fac * -1)};
    // draw the square onto the device context
    dc.DrawPolygon(4, points, position.x, position.y);
}

void GridPanel::UpdateHexagon(int i) { // include bool my_turn for greyed out hexagons feature
    Hexagon& hex = hexagons[i];
    Cell cell = game_state_ptr->get_cell(i);

    // greyed out hexagons feature (TO BE IMPLEMENTED)
    // bool not_clickable = (
    //     (game_state_ptr->get_current_phase() == truck_place)
    //     && !game_state_ptr->is_truck_placeable(i, InfoPanel::me_idx, TruckType::Big) 
    //     && !game_state_ptr->is_truck_placeable(i, InfoPanel::me_idx, TruckType::Small)
    //     && !game_state_ptr->are_trucks_removable(i, 1, 0, InfoPanel::me_idx)
    //     && !game_state_ptr->are_trucks_removable(i, 0, 1, InfoPanel::me_idx)
    // )
    // || (
    //     (game_state_ptr->get_current_phase() == road_place)
    //     && !game_state_ptr->is_road_placeable(i, InfoPanel::me_idx)
    // );
    // hex is greyed out if it's not the player's turn or the hex is not clickable
    // greyed = !my_turn || (my_turn && not_clickable);
            
    // update hex colors based on forest cover

    // skip city cells
    if(cell.get_cell_type() == City) return;

    auto cover = cell.get_forest_cover();
    wxColour fill;
    if(cover >= 8) {
        //fill = greyed ? full_forest_col.ChangeLightness(140) : full_forest_col;
        fill = full_forest_col;
    }
    else if(cover >= 5) {
        //fill = greyed ? almost_full_forest_col.ChangeLightness(140) : almost_full_forest_col;
        fill = almost_full_forest_col;
    }
    else if(cover >= 1) {
        //fill = greyed ? critical_forest_col.ChangeLightness(140) : critical_forest_col;
        fill = critical_forest_col;
    }
    else {
        fill = dead_forest_col;
    }
    // ensure correct border color on shared cells
    if(cell.get_concessionaire() == -1) {
        hex.borderColor = fill;
    }

    hex.fillColor = fill;
}