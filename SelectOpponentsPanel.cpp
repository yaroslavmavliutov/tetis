//
// Created by yaroslav on 21.02.19.
//

#include "SelectOpponentsPanel.h"

SelectOpponentsPanel::SelectOpponentsPanel(const wxString & title)
        : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(250, 230))
{
    count_of_opponents = 0;
    wxPanel *panel = new wxPanel(this, -1);

    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

    wxStaticBox *st = new wxStaticBox(panel, -1, wxT("Colors"),
                                      wxPoint(5, 5), wxSize(240, 150));

    chice1 = new wxRadioButton(panel, -1, wxT("2 users"), wxPoint(15, 30), wxDefaultSize, wxRB_GROUP);

    chice2 = new wxRadioButton(panel, -1, wxT("3 users"), wxPoint(15, 80));

    // ok
    wxButton *okButton = new wxButton(this, ID_OKEY, wxT("Ok"),
                                      wxDefaultPosition, wxSize(70, 30));
    Connect(ID_OKEY, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SelectOpponentsPanel::Verification));

    // close
    wxButton *closeButton = new wxButton(this, ID_CLOSE, wxT("Close"),
                                         wxDefaultPosition, wxSize(70, 30));
    Connect(ID_CLOSE, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SelectOpponentsPanel::Exit));

    hbox->Add(okButton, 1);
    hbox->Add(closeButton, 1, wxLEFT, 5);

    vbox->Add(panel, 1);
    vbox->Add(hbox, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

    SetSizer(vbox);

    Centre();
    ShowModal();

}

void SelectOpponentsPanel::Verification(wxCommandEvent& WXUNUSED(event))
{
    if (this->chice1->GetValue()) {
        this->count_of_opponents = 2;
        Destroy();
    }
    else if (this->chice2->GetValue()) {
        this->count_of_opponents = 3;
        Destroy();
    }
}

void SelectOpponentsPanel::Exit(wxCommandEvent& WXUNUSED(event))
{
    Destroy();
}

int SelectOpponentsPanel::GetCountOpponents() {
    return this->count_of_opponents;
}
