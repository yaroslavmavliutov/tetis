//
// Created by nazar on 2/20/19.
//

#ifndef TETIS_SERVER_H
#define TETIS_SERVER_H
#include <wx/wxprec.h>
#include "wx/wx.h"
#include <list>
#include <string>
#include "wx/socket.h"


typedef wxIPV4address IPaddress;
class Server: public wxFrame
{
    public:
    // ctor(s)
    //Frame * fr;
    Server(const wxString& title, int PL);
    ~Server();
    // event handlers (these functions should _not_ be virtual)
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnServerEvent(wxSocketEvent& event);
    void OnSocketEvent(wxSocketEvent& event);
    //wxSocketBase *clients[3];
    std::list<wxSocketBase *> clients;
    char logins[35];

    private:

    wxSocketServer *sock;
    wxTextCtrl *txtRx;
    int numClients;
    int clients_in_game;
    int want_players;
    // any class wishing to process wxWidgets events must use this macro
    wxDECLARE_EVENT_TABLE();
};

enum{
    ID_TXTRX=101,
    SERVER_SOCKET_ID,
    SERVER_ID,
    Minimal_Quit_s = wxID_EXIT,
    Minimal_About_s = wxID_ABOUT
};


#endif //TETIS_SERVER_H
