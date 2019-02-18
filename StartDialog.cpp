//
// Created by yaroslav on 17.02.19.
//

#include "StartDialog.h"
#include <string>
#include <cstring>

StartDialog::StartDialog(const wxString & title)
        : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(250, 230))
{
    wxPanel *panel = new wxPanel(this, -1);
    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *pre =  new wxStaticText(panel, -1, wxT("What's your login?"), wxPoint(65, 70));
    wxStaticText *len =  new wxStaticText(panel, -1, wxT("min-1, max-6"), wxPoint(80, 85));
    input_name = new wxTextCtrl(panel, -1, wxT(""),
                                    wxPoint(80, 105));

    wxButton *okButton = new wxButton(this, ID_OK, wxT("Ok"),
                                      wxDefaultPosition, wxSize(70, 30));
    Connect(ID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(StartDialog::Verification));

    hbox->Add(okButton, 1);
    vbox->Add(panel, 1);
    vbox->Add(hbox, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);
    SetSizer(vbox);
    Centre();
    ShowModal();
}

void StartDialog::Verification(wxCommandEvent& WXUNUSED(event))
{
    this->name=this->input_name->GetValue();
    if (this->name.empty() || this->name.length()>6)
        ;
    else {
        this->namestr = std::string(this->name.mb_str(wxConvUTF8));
        Destroy();
    }
}

std::string StartDialog::GetName() {

    return this->namestr;
}

