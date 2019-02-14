//
// Created by nazar on 2/12/19.
//

#ifndef TETIS_MYTHREAD_H
#define TETIS_MYTHREAD_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <wx/wx.h>
using namespace std;
using namespace sf;

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
