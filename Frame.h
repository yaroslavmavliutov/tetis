#ifndef FRAME_H
#define FRAME_H

#include "GamePanel.h"
#include "InfoPanel.h"
#include <wx/wxprec.h>
#include "wx/wx.h"

#include "wx/socket.h"
#include <list>
#include <string>


//#define __GXX_ABI_VERSION 1010
typedef wxIPV4address IPaddress;

// Define a new frame type: this is going to be our main frame
class Frame : public wxFrame
{
public:

    Frame * fr;
    std::string UserName;
    char BufferName[6];

    // ctor(s)
    Frame(const wxString& title);
    ~Frame();
    // event handlers (these functions should _not_ be virtual)
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    // event handlers for Socket menu _ CLIENT
    void OpenConnection();
    void CloseConnection();
    void OnSocketEvent(wxSocketEvent& event);
    void ClientSocket();
    void ServerSocket();


   // ________CLIENT________________

   // SERVER
    void ServerOnServerEvent(wxSocketEvent& event);
    void ServerOnSocketEvent(wxSocketEvent& event);

    std::list<wxSocketBase *> clients;
    wxSocketServer *SERVER_sock;
    int numClients;
    int want_players;

    void StartPanels(int N);
    //SERVER
    // convenience functions
    void UpdateStatusBar();
//-----------------------------------------------------------------------------------------
    GamePanel *m_lp;
    InfoPanel *m_rp;
    wxPanel *m_parent;

    wxStatusBar *statusScore;
    void OnHelp(wxCommandEvent & event);
    void OnCreate(wxCommandEvent & event);
    void OnJoin(wxCommandEvent & event);
    void OnPlay(wxCommandEvent & event);


    void Setbusy(bool);

    IPaddress IP_addr;
    int clients_in_game;
    bool busy;
    wxBoxSizer *hbox;
    wxTextCtrl *m_text;
    wxMenuBar *menubar; // new
    wxMenu *file; // File + vkladki


    wxSocketClient *sock;

private:
    wxMenu *helpMenu;
    // any class wishing to process wxWidgets events must use this macro
    wxDECLARE_EVENT_TABLE();
};

const int ID_HELP = 110;
const int ID_CREATE = 111;
const int ID_JOIN = 112;
const int ID_CREATE_GAME = 115;
const int ID_JOIN_GAME = 118;
const int ID_PLAY = 113;

// IDs for the controls and the menu commands
enum
{
    ID_TXTRX=101,
    SERVER_SOCKET_ID,
    SERVER_ID,
    SOCKET_ID, // CLIENT
    CLIENT_OPEN=wxID_OPEN,
    CLIENT_CLOSE=wxID_CLOSE,

    // menu items
    Minimal_Quit = wxID_EXIT,
    Minimal_About = wxID_ABOUT
};



#endif
