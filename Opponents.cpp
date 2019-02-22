//
// Created by nazar on 2/21/19.
//

#include "Opponents.h"

Opponents::Opponents(const wxString& title, int nb_opponents)
        : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(500, 400))
{

    opp = this;
    m_parent = new wxPanel(this, wxID_ANY);
    statusScore = CreateStatusBar(3);
    statusScore->SetStatusText(wxT("Your lvl: 1"));

    hbox = new wxBoxSizer(wxHORIZONTAL);

    m_lp = new GamePanel(m_parent, opp); // 0 - opponents
    std::cout<<"ICI" << std::endl;
    //Start tetris
    m_lp->SetFocus();
    m_lp->Start();
    std::cout<<"ICI 1" << std::endl;
    srand(time(NULL));

    hbox->Add(m_lp, 1, wxSHAPED | wxALL, 5);


    m_parent->SetSizer(hbox);
    std::cout<<"ICI 2" << std::endl;
//    this->Centre();
}



Opponents::~Opponents()
{
    // No delayed deletion here, as the frame is dying anyway
    Close(true);
}