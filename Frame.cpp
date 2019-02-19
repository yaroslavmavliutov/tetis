#include "Frame.h"
#include "StartDialog.h"

using namespace std;

Frame::Frame(const wxString& title)
        : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(500, 400))
{

    busy = false;
    menubar = new wxMenuBar; // menubar
    fr = this;
    file = new wxMenu; //menu

    file->Append(ID_PLAY, wxT("&Play alone"));
    file->Append(ID_CREATE_GAME, wxT("&Create Game"));
    file->Append(ID_JOIN_GAME, "&Join Game\tAlt-O","Join game");
    file->Append(ID_HELP, wxT("&Help"));
    file->Append(Minimal_Quit, "E&xit\tAlt-X", "Quit this program");
    file->AppendSeparator();


    //Help
    Connect(ID_HELP, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(Frame::OnHelp));
    // Start play
    Connect(ID_PLAY, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(Frame::OnPlay));


    //Create game
    Connect(ID_CREATE_GAME, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(Frame::OnCreate));
    // Join game
    Connect(ID_JOIN_GAME, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(Frame::OnJoin));



    Center();

    menubar->Append(file, wxT("&File"));
    SetMenuBar(menubar);

    StartDialog *user = new StartDialog(wxT("LOG IN"));
    //username типу std::string
    UserName = user->GetName();
    // це перевірка якшо хрестик для виходу нажали, то шоб не запустилось основне вікно, треба Close(true);
    if (UserName == "") {
        user->Destroy();
        Close(true);
    }
    // тут записуємо в буфер значення
    strcpy(BufferName, UserName.c_str());

    m_text  = new wxTextCtrl(this, -1,
                             wxString::Format(wxT("Your Score: %s"), UserName),
                             wxDefaultPosition, wxDefaultSize,
                             wxTE_MULTILINE | wxTE_READONLY);

}

void Frame::Setbusy(bool meaning) {
    this->busy = meaning;
}


Frame::~Frame()
{
    // No delayed deletion here, as the frame is dying anyway
    delete sock;
    delete SERVER_sock;
}

void Frame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
// true is to force the frame to close
    Close(true);
}


void Frame::OnHelp(wxCommandEvent& WXUNUSED(event)) {
    Refresh();

    wxMessageBox( _("wxWidgets Hello World example."),
                  _("About Hello World"),
                  wxOK|wxICON_INFORMATION, this );
}


void Frame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(wxString::Format
                         (
                                 "wxWidgets TCP client sample\n"
                                 "\n"
                                 "Author: Yan Naing Aye \n"
                                 "Web: http://cool-emerald.blogspot.com"
                         ),
                 "About wxWidgets TCP client sample",
                 wxOK | wxICON_INFORMATION,
                 this);
}



// відкриття клієнтського сокету
void Frame:: ClientSocket(){

    sock = new wxSocketClient();

    // Setup the event handler and subscribe to most events
    sock->SetEventHandler( *this, SOCKET_ID);
    sock->SetNotify(wxSOCKET_CONNECTION_FLAG |
                    wxSOCKET_INPUT_FLAG |
                    wxSOCKET_LOST_FLAG);
    sock->Notify(true);
}

// відкриття сокету сервера
void Frame::ServerSocket()
{
    IPaddress addr;
    addr.AnyAddress();
    addr.Service(3000);
    std::cout << "Creating server at " << addr.IPAddress() << ": " <<  addr.Service() << std::endl;

    // Create the socket
    SERVER_sock = new wxSocketServer(addr);

    // We use IsOk() here to see if the server is really listening
    if (!SERVER_sock->IsOk()){
        std::cout << "Could not listen at the specified port !"<< std::endl;
    }

    IPaddress addrReal;
    if (!SERVER_sock->GetLocal(addrReal)){
        std::cout << "ERROR: couldn't get the address we bound to. " << std::endl;
    }
    else{
        std::cout << "Server listening at" << addrReal.IPAddress() << " : " <<  addrReal.Service() << std::endl;
    }

    // Setup the event handler and subscribe to connection events
    SERVER_sock->SetEventHandler( *this, SERVER_ID);
    SERVER_sock->SetNotify(wxSOCKET_CONNECTION_FLAG);
    SERVER_sock->Notify(true);
    numClients = 0;
    clients_in_game = 0;
}

// Встановлення з'єднання з сервером
void Frame::OpenConnection()
{
    // Create the address - defaults to localhost:0 initially
    IPaddress addr;

    // Ask user for server address
    wxString hostname = wxGetTextFromUser(
            _("Enter the address of the wxSocket demo server:"),
            _("Connect ..."),
            _("localhost"));
    if ( hostname.empty() )
        return;

    addr.Hostname(hostname);
    addr.Service(3000);
    std::cout<<"Trying to connect to " << addr.IPAddress() << " : " << addr.Service() << std::endl;

//    file->Enable(CLIENT_OPEN, false);
//    file->Enable(CLIENT_CLOSE, false);
    // we connect asynchronously and will get a wxSOCKET_CONNECTION event when
    // the connection is really established
    //
    // if you want to make sure that connection is established right here you
    // could call WaitOnConnect(timeout) instead

    sock->Connect(addr, false);

    //update status
    UpdateStatusBar();
}


void Frame::OnPlay(wxCommandEvent& WXUNUSED(event)) {

    file->Enable(ID_PLAY, false);
    file->Enable(ID_CREATE_GAME, false);
    file->Enable(ID_JOIN_GAME, false);

    if (this->busy) {

        m_lp->Destroy();
        m_rp->Destroy();

    } else {
        m_text->Destroy();
        m_parent = new wxPanel(this, wxID_ANY);
        statusScore = CreateStatusBar(3);
//        ServerSocket();
//        ClientSocket();
//        OpenConnection();
    }


    statusScore->SetStatusText(wxT("Your lvl: 1"));

    hbox = new wxBoxSizer(wxHORIZONTAL);

    m_lp = new GamePanel(m_parent, this, sock, 0); // 0 - opponents
    /*std::map<std::string, int> my_map = {
            { "A", 1 },
            { "B", 2 },
            { "C", 3 }
    };*/
    m_rp = new InfoPanel(m_parent, this, 0); // 0 - opponents
    //Start tetris
    m_lp->SetFocus();
    m_lp->Start();

    srand(time(NULL));

    hbox->Add(m_lp, 1, wxEXPAND | wxALL, 5);
    hbox->Add(m_rp, 1, wxEXPAND | wxALL, 5);

//    hbox->Add(m_lp, 0, wxSHAPED | wxALL, 5);
//    hbox->Add(m_rp, 0, wxSHAPED | wxALL, 5);


    m_parent->SetSizer(hbox);

    this->Centre();
}



void Frame::OnCreate(wxCommandEvent& WXUNUSED(event)) {

    file->Enable(ID_PLAY, false);
    file->Enable(ID_CREATE_GAME, false);
    file->Enable(ID_JOIN_GAME, false);

    if (this->busy) {

        m_lp->Destroy();
        m_rp->Destroy();

    } else {
        m_text->Destroy();
        m_parent = new wxPanel(this, wxID_ANY);
        statusScore = CreateStatusBar(3);

    }

    want_players = 2;


    ServerSocket();
    ClientSocket();
    OpenConnection();


    std::cout << "SERVER waiting for players" << std::endl;

}



void Frame::OnJoin(wxCommandEvent& WXUNUSED(event)) {
    file->Enable(ID_PLAY, false);
    file->Enable(ID_CREATE_GAME, false);
    file->Enable(ID_JOIN_GAME, false);

    if (this->busy) {

        m_lp->Destroy();
        m_rp->Destroy();

    } else {
        m_text->Destroy();
        m_parent = new wxPanel(this, wxID_ANY);
        statusScore = CreateStatusBar(3);

    }


    ClientSocket();
    OpenConnection();
    std::cout<<"CLIENT waiting for all players" << std::endl;

}


void Frame::StartPanels(int N) {


    statusScore->SetStatusText(wxT("Your lvl: 1"));

    hbox = new wxBoxSizer(wxHORIZONTAL);

    std::cout<<"N -> "<< N <<std::endl;
    m_lp = new GamePanel(m_parent, fr, sock, N); // 0 - opponents
//    std::map<std::string, int> my_map = {
//            { "A", 1 },
//            { "B", 2 },
//            { "C", 3 }
//    };
    std::cout<<"INFO"<<std::endl;
    m_rp = new InfoPanel(m_parent, fr, N); // 0 - opponents
    //Start tetris
    std::cout<<"ICI_start"<<std::endl;

    m_lp->SetFocus();
    m_lp->Start();
    std::cout<<"ICI_after start"<<std::endl;
    srand(time(NULL));

    hbox->Add(m_lp, 1, wxEXPAND | wxALL, 5);
    hbox->Add(m_rp, 1, wxEXPAND | wxALL, 5);

    m_parent->SetSizer(hbox);
    std::cout<<"ICI_4"<<std::endl;

    Centre();
}


void Frame::CloseConnection()
{
    sock->Close();
    //update status
    UpdateStatusBar();
}

// обробка отриманого повідомлення від сервера, або підключає, або читає, або закриває
void Frame::OnSocketEvent(wxSocketEvent& event)
{
    std::cout<<"OnSocketEvent:CLIENT " << std::endl;
    wxSocketBase *sockBase = event.GetSocket();
    int score;
    // First, print a message
    switch (event.GetSocketEvent())
    {
        case wxSOCKET_INPUT: std::cout<< "wxSOCKET_INPUT" << std::endl; break;
        case wxSOCKET_LOST: std::cout<<"wxSOCKET_LOST" << std::endl; break;
        case wxSOCKET_CONNECTION: std::cout<<"wxSOCKET_CONNECTION" << std::endl; break;
        default: std::cout<< "Unexpected event !" << std::endl; break;
    }

    // Now we process the event
    switch (event.GetSocketEvent())
    {
        case wxSOCKET_INPUT:
        {
            // We disable input events, so that the test doesn't trigger
            // wxSocketEvent again.
            sockBase->SetNotify(wxSOCKET_LOST_FLAG);

            // Receive data from socket and send it back. We will first
            // get a byte with the buffer size, so we can specify the
            // exact size and use the wxSOCKET_WAITALL flag. Also, we
            // disabled input events so we won't have unwanted reentrance.
            // This way we can avoid the infamous wxSOCKET_BLOCK flag.

            sockBase->SetFlags(wxSOCKET_WAITALL);
            // Read the size @ first byte
            unsigned char len;
            sockBase->Read(&len, 1);
            char buf[256];
            // Read the message
            wxUint32 lenRd = sockBase->Read(buf, len).LastCount();
            if (!lenRd) {
                std::cout<< "Failed to read message." << std::endl;
//                return;
            }
            else {
                std::cout<< "CLIENT Read  -> " <<lenRd <<" bytes."  << std::endl;
            }
            std::cout << "BUF -> " << buf <<std::endl;

            // обробка повідомлення з сервера

            std::cout << "AA";
            if (strncmp( buf, "start", (size_t) 5 )==0){
//             запустити груу!!!!
                std::string mystr(buf);
                int n = std::stoi(std::to_string((int) mystr[5]));
                std::cout << "LETS START THE GAME! with " << n << " players" << std::endl;

                StartPanels(1);
            }else
            {
                std::cout << "Non start MSG";
                score = std::stoi( buf );
                m_rp->strings_score[1]->SetLabel(wxString::Format(wxT("Opponent Score: %d"), score));
            }



//            score = std::stoi( buf );
//            m_rp->strings_score[1]->SetLabel(wxString::Format(wxT("Server Score: %d"), score));

            //std::cout<< "Rx: " <<  wxString::FromUTF8(buf, len) << std::endl;
            // Enable input events again.
            sockBase->SetNotify(wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG);
            break;
        }
        default: {
            std::cout<<"default" << std::endl;
            break;
        }
    }

    //update status
    UpdateStatusBar();
}

void Frame::UpdateStatusBar()
{
//    file->Enable(CLIENT_OPEN, !sock->IsConnected());
//    file->Enable(CLIENT_CLOSE, sock->IsConnected());
    if (sock->IsConnected()) {
        SetStatusText(wxString::Format(wxT("Connected")), 2);
    }
    else {
        SetStatusText(wxString::Format(wxT("Not connected")), 2);
    }
}

//_______________________SERVER__________________________________


// під'єднання КЛІЄНТІВ
void Frame::ServerOnServerEvent(wxSocketEvent& event){
    std::cout << "OnServerEvent: " ;
    wxSocketBase *sockBase;

    switch (event.GetSocketEvent())
    {
        case wxSOCKET_CONNECTION: std::cout <<"wxSOCKET_CONNECTION\n"; break;
        default: std::cout << "Unexpected event !\n"; break;
    }

    // Accept new connection if there is one in the pending
    // connections queue, else exit. We use Accept(false) for
    // non-blocking accept (although if we got here, there
    // should ALWAYS be a pending connection).
    if(numClients <= 3) {
        sockBase = SERVER_sock->Accept(false);
        clients.push_back(sockBase);
    }

    if (sockBase )
    {
        IPaddress addr;
        if (!sockBase->GetPeer(addr))
        {
            std::cout << "New connection from unknown client accepted.\n";
        }
        else
        {
            std::cout << "New client connection from " <<  addr.IPAddress() << " : " << addr.Service() << " accepted \n";
        }
    }
    else
    {
        std::cout << "Error: couldn't accept a new connection \n";
        //return;
    }

    sockBase ->SetEventHandler( *this, SOCKET_ID);
    sockBase ->SetNotify(wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
    sockBase ->Notify(true);

    numClients++;
    SetStatusText(wxString::Format(wxT("%d  clients connected"),numClients), 2);

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
          std::cout << "send start_MSG:  " << start << "\n";
          // Enable input events again.
          sockBase_curr_2->SetNotify(wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG);

      }


  }
}


// сервер читає отримане повідомлення і відправляє всім його
void Frame::ServerOnSocketEvent(wxSocketEvent& event){

    std::cout << "OnSocketEvent: SERVER";
    wxSocketBase *sockBase = event.GetSocket();
    wxSocketBase *sockBase_curr;

    // find the right client
    for(auto it = clients.begin(); it != clients.end(); ++it){
        if(*it == sockBase){
            sockBase_curr = *it;
            break;
        }
    }

    // First, print a message
    switch (event.GetSocketEvent())
    {
        case wxSOCKET_INPUT: std::cout << "wxSOCKET_INPUT\n"; break;
        case wxSOCKET_LOST: std::cout << "wxSOCKET_LOST\n"; break;
        default: std::cout << "Unexpected event !\n"; break;
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
                std::cout << "Failed to read message.\n";
                return;
            }
            else {
                std::cout << "SERVER_ Read -> "<< lenRd << "bytes.\n";
            }


            std::cout << "Server buf Rx: "<< buf <<" \n";


            wxSocketBase *sockBase_curr_2;
            for(auto it = clients.begin(); it != clients.end(); ++it){
                sockBase_curr_2 = *it;
// НЕ треба відправляти самому собі дані
                if(sockBase_curr_2 == sockBase_curr)
                    continue;
                sockBase_curr_2->Write(&len,1);
                sockBase_curr_2->Write(&buf, len);
                std::cout << "Server send Tx:  " << buf << "\n";
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

            std::cout << "Deleting socket.\n";
            clients.remove(sockBase_curr);
            sockBase_curr->Destroy();

            break;
        }
        default:;
    }

    SetStatusText(wxString::Format(wxT("%d  clients connected"), numClients), 2);
}
//_______________________SERVER__________________________________