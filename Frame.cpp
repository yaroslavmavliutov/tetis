#include "Frame.h"
#include "GamePanel.h"
#include "InfoPanel.h"
#include "Piece.h"

Frame::Frame(const wxString& title)
        : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(500, 380))
{
    m_parent = new wxPanel(this, wxID_ANY);

    statusScore = CreateStatusBar();
    statusScore->SetStatusText(wxT("Your lvl: 1"));


    wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

    GamePanel *m_lp = new GamePanel(m_parent, this);
    m_rp = new InfoPanel(m_parent, this);

    m_lp->SetFocus();
    m_lp->Start();

    srand(time(NULL));

    hbox->Add(m_lp, 0, wxSHAPED | wxALL, 5);
    hbox->Add(m_rp, 1, wxEXPAND | wxALL, 5);

    m_parent->SetSizer(hbox);

    this->Centre();
}
