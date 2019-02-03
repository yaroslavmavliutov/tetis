//
// Created by nazar on 2/2/19.
//

#include "client_server.h"

void client_server::client(void)
{
    bool done = true;
    string pseudo;

    cout << "Name: ";
    cin >> pseudo;
    cout<< "IP";
    IPADDRESS = "127.0.0.1";
    cout<< IPADDRESS;

    if (socket.connect(IPADDRESS, PORT) == Socket::Done)
    {
        cout << "Connection accepted" << endl;
        socket.setBlocking(false);
        Packet sendPacket;
        sendPacket << pseudo;
        socket.send(sendPacket);
    }
    else
        cout << "Connection non accepted" << endl;

    Clock timer;
    while (done)
    {

        /*Packet receivePacket;
        string username, message;

        socket.receive(receivePacket);
        receivePacket >> username >> message;*/

        //cout << "Message de " << username << ": " << message << "----" << receivePacket.getData() << endl;

        std::string s_tex;
        std::cout << "\nYour text: ";

        std::cin >> s_tex;

        if (s_tex == "exit")
            done = false;

        msgSend = s_tex;

        Packet sendPacket;
        //globalMutex.lock();
        sendPacket << pseudo << msgSend;
        socket.send(sendPacket);
        //globalMutex.unlock();

    }
}


void client_server::server(void)
{
    bool done = true;

    if (listener.listen(PORT) == Socket::Done)
    {
        cout << "Server success" << endl;
    }
    else
        exit(0);

    cout<< "IP";
    cout<< IPADDRESS;

    selector.add(listener);

    while (done)
    {
        if (selector.wait())
        {
            if (selector.isReady(listener))
            {
                TcpSocket *socket = new TcpSocket();
                listener.accept(*socket);
                selector.add(*socket);
                Clients.push_back(socket);

                Packet receivePacket;

            }

            else
            {
                for (int i = 0;i < (int)Clients.size();i++)
                {
                    if (selector.isReady(*Clients[i]))
                    {
                        Packet receivePacket;
                        if (Clients[i]->receive(receivePacket) == Socket::Done)
                        {
                            string pseudo;
                            short int message;
                            receivePacket >> pseudo >> msgSend;
                            cout << "Received message. Client " << pseudo << " : " << msgSend << endl;

                            Packet sendPacket;
                            sendPacket << pseudo << msgSend;
                            /*for (int j = 0;j < (int)Clients.size();j++)
                            {
                                cout << "Send for client " <<j<<" ("<<pseudo << "): " << msgSend << endl;
                                Clients[j]->send(sendPacket);
                            }*/
                        }
                    }
                }
            }
        }
    }
}

void client_server::main_v()
{
    sf::Thread* thread = 0;
    char who;
    std::cout << "Do you want to be a server (s) or a client (c) ? ";
    std::cin >> who;

    if (who == 's') {
        server();
    }
    if (who == 'c'){
        client();
    }
}