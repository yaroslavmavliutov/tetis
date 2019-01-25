//
// Created by yaroslav on 25.01.19.
//

#ifndef TETIS_COMMUN_H
#define TETIS_COMMUN_H

#include "TetrisGame.hpp"
#include <wx/wxprec.h>

class Commun : public wxFrame
{
public:
    Commun(const wxString& title);


    Board *m_lp;
    RightPanel *m_rp;
    wxPanel *m_parent;

};
#endif //TETIS_COMMUN_H
