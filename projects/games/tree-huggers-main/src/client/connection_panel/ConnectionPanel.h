#pragma once

#include <wx/wx.h>
#include "../input_field/InputField.h"


class ConnectionPanel : public wxPanel {

public:
    ConnectionPanel(wxWindow* parent);

    // getters for connection info
    wxString getServerAddress();
    wxString getServerPort();
    wxString getPlayerName();

private:
    InputField* _serverAddressField;
    InputField* _serverPortField;
    InputField* _playerNameField;

};
