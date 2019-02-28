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
    Server(const wxString& title, int PL);
    ~Server();
    static char* substr(char *str, int start, int length);

protected:
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnServerEvent(wxSocketEvent& event);
    void OnSocketEvent(wxSocketEvent& event);


private:
    wxSocketServer *sock;
    wxTextCtrl *txtRx;
    int numClients;
    int want_players;
    int losers;
    char logins[35];
    char *tabellog [3];
    int opponentScores[3];
    std::list<wxSocketBase *> clients;

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
