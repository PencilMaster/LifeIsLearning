#pragma once

#include <wx/wx.h>

class InputField : public wxPanel {
    public:
        // Constructs a new Input Field object with a label and a text field
        InputField(wxWindow* parent, const wxString& labelText, int labelWidth, const wxString& fieldValue, int fieldWidth);
        
        /**
        * @brief Gets the value of the text field
        */
        wxString getValue();

        /**
        * @brief Sets the maximum length of the text field
        * @param len the maximum length of the text field
        */
        void SetMaxLength(unsigned long len); 

    private:
        // the label and text field of the input field
        wxStaticText* _label;
        wxTextCtrl* _field;

};