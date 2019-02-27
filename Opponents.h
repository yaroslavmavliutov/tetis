//
// Created by nazar on 2/21/19.
//

#ifndef TETIS_OPPONENTS_H
#define TETIS_OPPONENTS_H


#include "GamePanel.h"
#include "GamePanelOpponent.h"
#include <wx/wxprec.h>
#include "wx/wx.h"

#include "wx/socket.h"



class Opponents : public wxFrame
{
public:

    Opponents* opp;
    std::string UserName;
    char BufferName[6];

    Opponents(const wxString& title, int nb_opponents);
    ~Opponents();

    void UpdateStatusBar();
//-----------------------------------------------------------------------------------------
    GamePanelOpponent *m_lp;
    wxPanel *m_parent;
    wxBoxSizer *hbox;
    wxStatusBar *statusScore;

private:

};

#endif //TETIS_OPPONENTS_H
