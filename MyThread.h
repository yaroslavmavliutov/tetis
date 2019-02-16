//
// Created by nazar on 2/12/19.
//

#ifndef TETIS_MYTHREAD_H
#define TETIS_MYTHREAD_H
#include <wx/wx.h>
#include "wx/busyinfo.h"
#include "wx/socket.h"
#include <iostream>
//#include "Frame.h"
/*
// this example is currently written to use only IP or only IPv6 sockets, it
// should be extended to allow using either in the future
#if wxUSE_IPV6
typedef wxIPV6address IPaddress;
#else
typedef wxIPV4address IPaddress;
#endif


using namespace std;

class MyThread: public wxThread {
public:
    MyThread(int inter, Frame* obj) :
            th_iter(inter) th_obj(*obj) {}

    virtual void *Entry();

    void OnWaitForAccept(wxCommandEvent &event);

    void OnServerEvent(wxSocketEvent &event);

    void OnSocketEvent(wxSocketEvent &event);

    void Test2(wxSocketBase *sock);

//    wxDECLARE_EVENT_TABLE();

private:
    wxSocketServer *m_server;
    wxTextCtrl *m_text;

    bool m_busy;
    int m_numClients;

    // any class wishing to process wxWidgets events must use this macro


private:
    int th_iter;
    Frame th_obj;
};



// IDs for the controls and the menu commands
enum
{
    // menu items
    SERVER_UDPTEST = 10,
    SERVER_WAITFORACCEPT,
    SERVER_QUIT = wxID_EXIT,
    SERVER_ABOUT = wxID_ABOUT,

    // id for sockets
    SERVER_ID = 100,
    SOCKET_ID
};
*/

//wxBEGIN_EVENT_TABLE(MyThread, wxFrame)
//EVT_MENU(SERVER_WAITFORACCEPT, MyThread::OnWaitForAccept)
//EVT_SOCKET(SERVER_ID,  MyThread::OnServerEvent)
//EVT_SOCKET(SOCKET_ID,  MyThread::OnSocketEvent)
//wxEND_EVENT_TABLE()

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>



using namespace sf;
using namespace std;


class MyThread: public wxThread {
public:
    MyThread(int inter):
        th_iter(inter){}
    virtual void *Entry();
    const unsigned short PORT = 4444;
    IpAddress IPADDRESS = sf::IpAddress::getLocalAddress();

//client
    TcpSocket socket;
    Mutex globalMutex;

//server
    TcpListener listener;
    SocketSelector selector;
    bool quit = false;
    vector<TcpSocket*>Clients;

//Message
    std::string msgSend;
    std::string msgReceived;
private:
    int th_iter;
};

#define ID_COUNTED 100
#endif //TETIS_MYTHREAD_H
