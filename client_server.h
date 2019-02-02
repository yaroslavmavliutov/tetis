//
// Created by nazar on 2/2/19.
//

#ifndef TETIS_CLIENT_SERVER_H
#define TETIS_CLIENT_SERVER_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

using namespace std;
using namespace sf;

class client_server {
public:

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
    void client(void);
    void server(void);
    void main_v(void);

};


#endif //TETIS_CLIENT_SERVER_H
