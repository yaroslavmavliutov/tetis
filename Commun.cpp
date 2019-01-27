//
// Created by yaroslav on 25.01.19.
//

#include "Commun.h"
#include "TetrisGame.hpp"
#include "Piece.hpp"

Commun::Commun(const wxString& title)
        : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(500, 380))
{
    m_parent = new wxPanel(this, wxID_ANY);

    wxStatusBar *statusBar = CreateStatusBar();
    statusBar->SetStatusText(wxT("Score: 0"));

    wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

    Board *m_lp = new Board(m_parent, this);
    m_rp = new RightPanel(m_parent, this);
    //m_dp = new Friends(m_parent, this);

    //Board* board = new Board(t_parent, fr);
    m_lp->SetFocus();
    m_lp->Start();

    srand(time(NULL));

    hbox->Add(m_lp, 0, wxSHAPED | wxALL, 5);
    hbox->Add(m_rp, 1, wxEXPAND | wxALL, 5);
    //hbox->Add(m_dp, 1, wxEXPAND | wxALL, 5);

    m_parent->SetSizer(hbox);

    this->Centre();
}
