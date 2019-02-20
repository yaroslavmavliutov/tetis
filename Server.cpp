//
// Created by nazar on 2/20/19.
//

#include "Server.h"


// frame constructor
Server::Server(const wxString& title, int PL)
        : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(390, 280),
                  wxDEFAULT_FRAME_STYLE ^ wxRESIZE_BORDER)
{

    #if wxUSE_MENUS
    // create a menu bar
    wxMenu *fileMenu = new wxMenu;

    // the "About" item should be in the help menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(Minimal_About_s, "&About\tF1", "Show about dialog");

    fileMenu->Append(Minimal_Quit_s, "E&xit\tAlt-X", "Quit this program");

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(helpMenu, "&Help");

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);
    #endif // wxUSE_MENUS

    #if wxUSE_STATUSBAR
    // create a status bar just for fun (by default with 1 pane only)
    CreateStatusBar(2);
    SetStatusText("TCP server using wxWidgets");
    #endif // wxUSE_STATUSBAR
    txtRx = new wxTextCtrl(this, ID_TXTRX, wxT(""), wxPoint(5, 5),
                           wxSize(365, 125), wxTE_MULTILINE);

    // Create the address - defaults to localhost:0 initially
    IPaddress addr;
    addr.AnyAddress();
    addr.Service(3000);
    txtRx->AppendText(wxString::Format(wxT("Creating server at %s:%u \n")
            ,addr.IPAddress(), addr.Service()));

    // Create the socket
    sock = new wxSocketServer(addr);

    // We use IsOk() here to see if the server is really listening
    if (!sock->IsOk()){
        txtRx->AppendText(wxString::Format(wxT("Could not listen at the specified port !\n")));
    return;
    }

    IPaddress addrReal;
    if (!sock->GetLocal(addrReal)){
        txtRx->AppendText(wxString::Format(wxT("ERROR: couldn't get the address we bound to. \n")));
    }
    else{
        txtRx->AppendText(wxString::Format(wxT("Server listening at %s:%u \n"),
                                       addrReal.IPAddress(), addrReal.Service()));
    }

    // Setup the event handler and subscribe to connection events
    sock->SetEventHandler( *this, SERVER_ID);
    sock->SetNotify(wxSOCKET_CONNECTION_FLAG);
    sock->Notify(true);
    numClients = 0;
    want_players = PL;

}

Server::~Server()
{
    // No delayed deletion here, as the frame is dying anyway
    delete sock;
}

// event handlers
void Server::OnQuit(wxCommandEvent& WXUNUSED(event))
{
// true is to force the frame to close
    Close(true);
}

void Server::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(wxString::Format
                         (
                                 "wxWidgets TCP server sample\n"
                                 "\n"
                                 "Author: Yan Naing Aye \n"
                                 "Web: http://cool-emerald.blogspot.com"
                         ),
                 "About wxWidgets TCP server sample",
                 wxOK | wxICON_INFORMATION,
                 this);
}


// під'єднання КЛІЄНТІВ
void Server::OnServerEvent(wxSocketEvent& event)
{
    txtRx->AppendText(wxT("OnServerEvent: "));
    wxSocketBase *sockBase;

    switch (event.GetSocketEvent())
    {
        case wxSOCKET_CONNECTION: txtRx->AppendText(wxT("wxSOCKET_CONNECTION\n")); break;
        default: txtRx->AppendText(wxT("Unexpected event !\n")); break;
    }

    // Accept new connection if there is one in the pending
    // connections queue, else exit. We use Accept(false) for
    // non-blocking accept (although if we got here, there
    // should ALWAYS be a pending connection).
    if(numClients <= 3) {
        sockBase = sock->Accept(false);
        clients.push_back(sockBase);
    }

    if (sockBase )
    {
        IPaddress addr;
        if (!sockBase->GetPeer(addr))
        {
            txtRx->AppendText(wxT("New connection from unknown client accepted.\n"));
        }
        else
        {
            txtRx->AppendText(wxString::Format(wxT("New client connection from %s:%u accepted \n"),
                                               addr.IPAddress(), addr.Service()));
        }
    }
    else
    {
        txtRx->AppendText(wxT("Error: couldn't accept a new connection \n"));
        return;
    }

    sockBase ->SetEventHandler( *this, SERVER_SOCKET_ID);
    sockBase ->SetNotify(wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
    sockBase ->Notify(true);
    numClients++;
    SetStatusText(wxString::Format(wxT("%d  clients connected"),numClients), 1);

    //    відправлення повідомлення про старт гри
    if(numClients == want_players){
        //send start to all players
        char start[7] = "start";
        start[5] = static_cast<char>(48+numClients);
        size_t txn = strlen(start);

        unsigned char len;
        len = txn;

        wxSocketBase *sockBase_curr_2;
        for(auto it = clients.begin(); it != clients.end(); ++it){
            sockBase_curr_2 = *it;
            sockBase_curr_2->Write(&len,1);
            sockBase_curr_2->Write(&start, len);
            txtRx->AppendText(wxString::Format(wxT("send start_MSG : %s \n"), start));
            //std::cout << "send start_MSG:  " << start << "\n";
            // Enable input events again.
            sockBase_curr_2->SetNotify(wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG);

        }


    }

}


// сервер читає отримане повідомлення і відправляє всім його
void Server::OnSocketEvent(wxSocketEvent& event)
{
    txtRx->AppendText(wxT("OnSocketEvent: "));
    wxSocketBase *sockBase = event.GetSocket();
    wxSocketBase *sockBase_curr;
    int current = -1;

    for(auto it = clients.begin(); it != clients.end(); ++it){
        if(*it == sockBase){
            sockBase_curr = *it;
            break;
        }
    }

    // First, print a message
    switch (event.GetSocketEvent())
    {
        case wxSOCKET_INPUT: txtRx->AppendText(wxT("wxSOCKET_INPUT\n")); break;
        case wxSOCKET_LOST: txtRx->AppendText(wxT("wxSOCKET_LOST\n")); break;
        default: txtRx->AppendText(wxT("Unexpected event !\n")); break;
    }

    // Now we process the event
    switch (event.GetSocketEvent())
    {
        case wxSOCKET_INPUT:
        {
            // We disable input events, so that the test doesn't trigger
            // wxSocketEvent again.
            sockBase_curr->SetNotify(wxSOCKET_LOST_FLAG);

            // Receive data from socket and send it back. We will first
            // get a byte with the buffer size, so we can specify the
            // exact size and use the wxSOCKET_WAITALL flag. Also, we
            // disabled input events so we won't have unwanted reentrance.
            // This way we can avoid the infamous wxSOCKET_BLOCK flag.

            sockBase_curr->SetFlags(wxSOCKET_WAITALL);



            // Read the size @ first byte
            unsigned char len;
            sockBase->Read(&len, 1);
            char buf[256];
            // Read the message
            wxUint32 lenRd = sockBase->Read(buf, len).LastCount();
            if (!lenRd)		{
                txtRx->AppendText(wxT("Failed to read message.\n"));
                return;
            }
            else {
                txtRx->AppendText(wxString::Format(wxT("Read %d bytes.\n"), lenRd));
            }

            txtRx->AppendText(wxString::Format(wxT("Server Read x: %s \n"),wxString::FromUTF8(buf, len)));


            wxSocketBase *sockBase_curr_2;
            for(auto it = clients.begin(); it != clients.end(); ++it){
                sockBase_curr_2 = *it;
                // НЕ треба відправляти самому собі дані
                if(sockBase_curr_2 == sockBase_curr){
                    sockBase_curr_2->SetNotify(wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG);
                    continue;
                }

                sockBase_curr_2->Write(&len,1);
                sockBase_curr_2->Write(&buf, len);
                txtRx->AppendText("Server: Tx: " + wxString::From8BitData(buf, len) + "\n");
                // Enable input events again.
                sockBase_curr_2->SetNotify(wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG);

            }

            break;
        }
        case wxSOCKET_LOST:
        {
            numClients--;

            // Destroy() should be used instead of delete wherever possible,
            // due to the fact that wxSocket uses 'delayed events' (see the
            // documentation for wxPostEvent) and we don't want an event to
            // arrive to the event handler (the frame, here) after the socket
            // has been deleted. Also, we might be doing some other thing with
            // the socket at the same time; for example, we might be in the
            // middle of a test or something. Destroy() takes care of all
            // this for us.

            txtRx->AppendText(wxT("Deleting socket.\n"));
            clients.remove(sockBase_curr);
            sockBase_curr->Destroy();

            break;
        }
        default:;
    }

    SetStatusText(wxString::Format(wxT("%d  clients connected"), numClients), 1);
}