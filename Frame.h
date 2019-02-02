#ifndef FRAME_H
#define FRAME_H

#include "GamePanel.h"
#include "InfoPanel.h"
#include <wx/wxprec.h>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <mpi.h>


class Frame : public wxFrame
{
public:
    Frame(const wxString& title, int type_machine);

    GamePanel *m_lp;
    InfoPanel *m_rp;
    wxPanel *m_parent;
    wxStatusBar *statusScore;
    void OnQuit(wxCommandEvent & event);
    void OnNew(wxCommandEvent & event);

    wxMenuBar *menubar;
    wxMenu *file;
    wxMenuItem *quit;
};
const int ID_NEW = 111;
#endif
