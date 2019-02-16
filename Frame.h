#ifndef FRAME_H
#define FRAME_H

#include "GamePanel.h"
#include "InfoPanel.h"
#include <wx/wxprec.h>
#include "wx/wx.h"
#include "wx/socket.h"




typedef wxIPV4address IPaddress;

// Define a new frame type: this is going to be our main frame
class Frame : public wxFrame
{
public:
    // ctor(s)
    Frame(const wxString& title);
    ~Frame();
    // event handlers (these functions should _not_ be virtual)
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    // event handlers for Socket menu
    void OnOpenConnection(wxCommandEvent& event);
    void OnCloseConnection(wxCommandEvent& event);
    void OnSend(wxCommandEvent& event);
    void OnSocketEvent(wxSocketEvent& event);

    // convenience functions
    void UpdateStatusBar();
//-----------------------------------------------------------------------------------------
    GamePanel *m_lp;
    InfoPanel *m_rp;

    wxPanel *m_parent;
<<<<<<< HEAD
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
=======
    wxSocketClient *sock;

private:


/*    wxButton *btnSend;
    wxTextCtrl *txtSend;
    wxTextCtrl *txtRx; */
    wxMenu *fileMenu;
    wxMenu *helpMenu;
    // any class wishing to process wxWidgets events must use this macro
    wxDECLARE_EVENT_TABLE();
};

// IDs for the controls and the menu commands
enum
{
//    ID_BTNSEND=101,
//    ID_TXTSEND,
//    ID_TXTRX,
            SOCKET_ID,
    CLIENT_OPEN=wxID_OPEN,
    CLIENT_CLOSE=wxID_CLOSE,
    // menu items
            Minimal_Quit = wxID_EXIT,
    Minimal_About = wxID_ABOUT
};

>>>>>>> 730d63129accab1d1d376ed3ab4885e11df74d90

#endif
