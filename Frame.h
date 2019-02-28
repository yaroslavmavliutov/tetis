#ifndef FRAME_H
#define FRAME_H

#include "Board.h"
#include "Opponents.h"
#include "GamePanel.h"
#include "GamePanelOpponent.h"
#include "InfoPanel.h"
#include "Server.h"

#include <wx/wxprec.h>
#include "wx/wx.h"
#include "wx/socket.h"

#include <list>
#include <string>
#include <cstring>


//#define __GXX_ABI_VERSION 1010
typedef wxIPV4address IPaddress;

// Define a new frame type: this is going to be our main frame
class Frame : public wxFrame
{
public:
    Frame * fr;
    Frame(const wxString& title);
    ~Frame();

    void ShowMessageLose();
    void ShowMessageWin();
    void Setbusy(bool);

    InfoPanel* getm_rp();
    char *getBufferName();
    bool isServerOn();
    void setServerOn(bool t);
    std::string getUserName();
    int getIndex();
    char *getopponentslog(int i);

    wxMenu *file; // File

protected:
    void StartPanels(int N);

    void OnHelp(wxCommandEvent & event);
    void OnCreate(wxCommandEvent & event);
    void OnJoin(wxCommandEvent & event);
    void OnPlay(wxCommandEvent & event);

    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    // event handlers for CLIENT Socket
    void OpenConnection();
    void CloseConnection();
    void OnSocketEvent(wxSocketEvent& event);
    void ClientSocket();
    void UpdateStatusBar();
    void sendLogin();

private:
    std::string UserName;
    char BufferName[6];
    char *opponentslog [3];
    int lenlogins[3];
    int opponentScores[3];
    int want_players;
    int nb_op;
    int index;    // для правильного розташування панельок

    int clients_in_game;
    bool busy;
    bool server_on;

    IPaddress IP_addr;

    Opponents* opPanel;
    Server *my_server;
    wxMenu *helpMenu;
    wxBoxSizer *hbox;
    wxTextCtrl *m_text;
    wxMenuBar *menubar; // new

    GamePanel *m_lp;
    InfoPanel *m_rp;
    wxPanel *m_parent;
    wxSocketClient *sock;
    wxStatusBar *statusScore;

    wxDECLARE_EVENT_TABLE();
};

const int ID_HELP = 110;
const int ID_CREATE = 111;
const int ID_JOIN = 112;
const int ID_CREATE_GAME = 115;
const int ID_JOIN_GAME = 118;
const int ID_PLAY = 113;

const int ID_INFO = 1;

// IDs for the controls and the menu commands
enum
{
    SOCKET_ID,
    CLIENT_OPEN=wxID_OPEN,
    CLIENT_CLOSE=wxID_CLOSE,

    // menu items
    Minimal_Quit = wxID_EXIT,
    Minimal_About = wxID_ABOUT
};



#endif
