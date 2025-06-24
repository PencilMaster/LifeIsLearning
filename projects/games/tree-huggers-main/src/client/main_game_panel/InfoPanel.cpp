#include "InfoPanel.h"
#include "GridPanel.h"
#include "../GameController.h"
#include <iostream>

// event table for the info panel that binds the paint event to the OnPaint function
BEGIN_EVENT_TABLE(InfoPanel, wxPanel)
    EVT_PAINT(InfoPanel::OnPaint)
END_EVENT_TABLE()

// initialize static variables
wxColour InfoPanel::client_color = wxColour(0, 0, 0);
int InfoPanel::me_idx = 0;

InfoPanel::InfoPanel(wxWindow* parent) : wxPanel(parent, wxID_ANY) {
}

void InfoPanel::initial_game_state(std::shared_ptr<const GameState> gs) {
    game_state = gs;
    SetBackgroundColour(GridPanel::background_color); 

    me_ptr = game_state->get_player_ptr(GameController::get_my_id());
    me_idx = me_ptr->get_player_idx();
    my_name = me_ptr->get_name();
    client_color = GridPanel::player_colors[me_idx];
}

void InfoPanel::update_game_state(std::shared_ptr<const GameState> gs) {
    game_state = gs;

    round = game_state->get_current_round();

    // choose the correct phase title to display
    phases phase_id = game_state->get_current_phase();
    switch(phase_id) {
        case road_place:
            phase = "Road Building";
            break;
        case truck_buy:
            phase = "Truck Buying";
            break;
        case truck_place:
            phase = "Truck Handling";
            break;
        default:
            phase = "Unknown";
            break;
    }

    // update player information
    curr_player_idx = game_state->get_current_player();
    // the player pointers change every time the game state is updated
    std::shared_ptr<Player> curr_player_ptr = game_state->get_player_ptr(curr_player_idx);
    curr_player_name = curr_player_ptr->get_name();

    me_ptr = game_state->get_player_ptr(GameController::get_my_id());
    me_idx = me_ptr->get_player_idx();
    my_name = me_ptr->get_name();
    client_color = GridPanel::player_colors[me_idx];

    balance = me_ptr->get_balance();
    big_trucks = me_ptr->get_num_trucks().first;
    small_trucks = me_ptr->get_num_trucks().second;
    wood = me_ptr->get_total_logged();

    // trigger the paint event to update the panel accordingly
    Refresh();
}

wxPoint InfoPanel::MakeCentered(int width, int height) {
    int panel_width, panel_height;
    // get the size of the panel
    GetClientSize(&panel_width, &panel_height);

    // center the drawing in the panel
    int x = (panel_width - width) / 2;
    int y = (panel_height - height) / 2;

    return wxPoint(x, y); // top left corner
}

void InfoPanel::OnPaint(wxPaintEvent& event) {
    // center the drawing in the panel
    wxPoint position = MakeCentered(width, height);

    // create a buffered paint device context to draw on
    // buffering avoids flickering
    wxBufferedPaintDC dc(this);

    // clear the panel before drawing to avoid overlap
    dc.Clear();
    
    DrawPanel(dc, position);
}

void InfoPanel::DrawPanel(wxDC& dc, wxPoint position) {
    //read relevant data

    //global stats
    curr_player_idx = game_state->get_current_player();
    std::shared_ptr<Player> curr_player_ptr = game_state->get_player_ptr(curr_player_idx);
    std::string curr_player_name = curr_player_ptr->get_name();
    std::string curr_player_id = curr_player_ptr->get_player_id();
    
    // my stats
    me_ptr = game_state->get_player_ptr(GameController::get_my_id());
    balance = me_ptr->get_balance();
    big_trucks = me_ptr->get_num_trucks().first;
    small_trucks = me_ptr->get_num_trucks().second;
    wood = me_ptr->get_total_logged();

    // rectangle border color and with
    wxPen pen(client_color, GridPanel::border_width);
    dc.SetPen(pen);
    // rectangle fill color
    dc.SetBrush(*wxWHITE);
    // draw the rectangle onto the device context
    dc.DrawRectangle(position, wxSize(width, height));

    // text alignment and spacing variables
    int truck_x = position.x + 30;
    int text_x = truck_x + 10 + GridPanel::truck_size;
    int alignment = text_x + 130;
    int text_y = position.y + 30;
    int text_spacing = 40;
    int font_size = 12;
    int size_diff = (GridPanel::truck_size - font_size) / 2;

    // set the font and text color
    wxFont font = wxFont(font_size, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    dc.SetFont(font.Scaled(1.1).Bold());
    dc.SetTextForeground(*wxBLACK);

    // current round
    wxSize len = dc.GetTextExtent(wxString::Format("Round %d", round));
    dc.DrawText(wxString::Format("Round %d", round),        
        MakeCentered(len.x, len.y).x, // center the text in the panel
        text_y);
    
    // current phase
    len = dc.GetTextExtent(wxString::Format("%s Phase", phase));
    dc.DrawText(wxString::Format("%s Phase", phase),
        MakeCentered(len.x, len.y).x, // center the text in the panel
        text_y += text_spacing - 5); 

    // current player
    // choose the correct color based on the current player
    dc.SetTextForeground(GridPanel::player_colors[curr_player_idx]);
    if(GameController::get_my_id() == curr_player_id) {
        // if it's the player's turn, display "Your Turn"
        len = dc.GetTextExtent(wxString::Format("Your Turn"));
        dc.DrawText(wxString::Format("Your Turn"), 
            MakeCentered(len.x, len.y).x, // center the text in the panel
            text_y += text_spacing - 5); 
    }
    else {
        // if it's not the player's turn, display the current player's name
        len = dc.GetTextExtent(wxString::Format("%s's Turn", curr_player_name));
        dc.DrawText(wxString::Format("%s's Turn", curr_player_name), 
            MakeCentered(len.x, len.y).x, 
            text_y += text_spacing - 5); 
    }
    dc.SetTextForeground(*wxBLACK);
    dc.SetFont(font);

    // draw this client's player stats and the corresponding icons
    dc.DrawText(wxString::Format("Balance: "), 
        text_x, 
        text_y += text_spacing + 10); 
    dc.DrawText(wxString::Format("%d", balance),//game_state->get_player_ptr(GameController::get_my_id())->get_balance()), 
        alignment, 
        text_y); 
    dc.SetFont(wxFont(GridPanel::truck_size, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    dc.DrawText(wxString::Format("$"), 
        truck_x - GridPanel::truck_size/2 + size_diff, 
        text_y - size_diff*2.5);
    dc.SetFont(font);

    dc.DrawText(wxString::Format("Big Trucks:"), 
        text_x, 
        text_y += text_spacing); 
    dc.DrawText(wxString::Format("%d", big_trucks), 
        alignment, 
        text_y); 
    GridPanel::DrawBigTruck(dc, 
        wxPoint(truck_x, text_y + GridPanel::truck_size - size_diff), 
        GridPanel::truck_size, 
        client_color);

    dc.DrawText(wxString::Format("Small Trucks:"), 
        text_x, 
        text_y += text_spacing); 
    dc.DrawText(wxString::Format("%d", small_trucks), 
        alignment, 
        text_y); 
    GridPanel::DrawSmallTruck(dc, 
        wxPoint(truck_x, text_y + GridPanel::truck_size - size_diff), 
        GridPanel::truck_size, 
        client_color);

    dc.DrawText(wxString::Format("Wood Logged:"), 
        text_x, 
        text_y += text_spacing);
    dc.DrawText(wxString::Format("%d", wood), 
        alignment, 
        text_y);

    // insert the wood png
    // ensure the file is found wherever the executable is run from
    wxStandardPaths& paths = wxStandardPaths::Get();
    wxString exePath = paths.GetExecutablePath();

    wxFileName fn(exePath);
    wxString file = fn.GetPath() + "/graphics/wood-simple.png";

    wxImage image;

    if(!wxFileExists(file)) {
        wxMessageBox("Could not find file: " + file, "File error", wxICON_ERROR);
        return;
    }
    if(!image.LoadFile(file)) {
        wxMessageBox("Could not load file: " + file, "File error", wxICON_ERROR);
        return;
    }
    // create a bitmap from the image and scale it to match the truck icon size
    wxBitmap bitmap = wxBitmap(image.Scale(GridPanel::truck_size*2, GridPanel::truck_size*2));
    // draw the bitmap onto the device context in the correct position
    dc.DrawBitmap(bitmap, truck_x - GridPanel::truck_size + 1, text_y - GridPanel::truck_size/2 - 2);
    
}


