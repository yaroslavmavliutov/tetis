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


#endif
