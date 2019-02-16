#ifndef FRAME_H
#define FRAME_H

#include "GamePanel.h"
#include "InfoPanel.h"
#include <wx/wxprec.h>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "MyThread.h"



class Frame : public wxFrame
{
public:
    Frame(const wxString& title);

    GamePanel *m_lp;
    InfoPanel *m_rp;
    wxPanel *m_parent;
    wxStatusBar *statusScore;
    void OnQuit(wxCommandEvent & event);
    void OnHelp(wxCommandEvent & event);
    void OnCreate(wxCommandEvent & event);
    void OnJoin(wxCommandEvent & event);
    void OnPlay(wxCommandEvent & event);

    void Setbusy(bool);

    bool busy;
    wxBoxSizer *hbox;
    wxTextCtrl     *m_text;
    wxMenuBar *menubar; // new
    wxMenu *file; // File + vkladki
    wxMenuItem *quit;

    //wxStaticText *string_score;
    //wxFont *font;

};

const int ID_HELP = 110;
const int ID_CREATE = 111;
const int ID_JOIN = 112;
const int ID_PLAY = 113;

#endif
