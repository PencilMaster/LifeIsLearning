#include "InputField.h"

InputField::InputField(wxWindow* parent, const wxString& labelText, int labelWidth, const wxString& fieldValue, int fieldWidth)
    : wxPanel(parent, wxID_ANY) {

    wxBoxSizer* horizontalLayout = new wxBoxSizer(wxHORIZONTAL);

    _label = new wxStaticText(
        this, // parent element
        wxID_ANY, // element id
        labelText, // text that's displayed as label
        wxDefaultPosition, // position
        wxSize(labelWidth, -1) // size (-1 means default size)
    );
    horizontalLayout->Add(_label, 0, wxALIGN_CENTER);

    _field = new wxTextCtrl(
        this, // parent element
        wxID_ANY, // element id
        fieldValue, // default value
        wxDefaultPosition, // position
        wxSize(fieldWidth, -1) // size (-1 means default size)
    );

    horizontalLayout->Add(_field, 0, wxALIGN_CENTER);

    SetSizerAndFit(horizontalLayout);
    // trigger a layout update
    Layout();
}

wxString InputField::getValue() {
    return _field->GetValue();
}

void InputField::SetMaxLength(unsigned long len) {
    _field->SetMaxLength(len); // Use wxTextCtrl's SetMaxLength method
}