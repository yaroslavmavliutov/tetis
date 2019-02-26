//
// Created by nazar on 2/21/19.
//

#include "Opponents.h"

Opponents::Opponents(const wxString& title, int nb_opponents)
        : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(250, 400))
{

    opp = this;
    m_parent = new wxPanel(this, wxID_ANY);
    statusScore = CreateStatusBar(3);
    statusScore->SetStatusText(wxT("Lvl: 1"));

    hbox = new wxBoxSizer(wxHORIZONTAL);

    m_lp = new GamePanelOpponent(m_parent, opp); // 0 - opponents

    m_lp->SetFocus();

//    srand(time(NULL));
    hbox->Add(m_lp, 1, wxSHAPED | wxALL, 5);

    m_parent->SetSizer(hbox);
}



Opponents::~Opponents()
{
    // No delayed deletion here, as the frame is dying anyway
    Close(true);
}