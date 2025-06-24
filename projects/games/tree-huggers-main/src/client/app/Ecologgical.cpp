#include "Ecologgical.h"
#include "../game_window/GameWindow.h"
#include "../GameController.h"

bool Ecologgical::OnInit() {

  // Allow loading of PNG image files
  wxImage::AddHandler(new wxPNGHandler());

  // Open main game window
  GameWindow* game_window = new GameWindow(
    wxString("EcoLOGgical"), // title of window,
    wxDefaultPosition, // position of the window
    wxDefaultSize // size of the window
  );
  game_window->Show(); // show the window
  // start game controller
  GameController::init(game_window);
  
  return true;
}