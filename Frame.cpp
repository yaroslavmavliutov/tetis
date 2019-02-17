#include "Frame.h"

using namespace std;

Frame::Frame(const wxString& title)
        : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(500, 380))
{

    busy = false;
    menubar = new wxMenuBar; // menubar

    file = new wxMenu; //menu

    file->Append(ID_PLAY, wxT("&Start play"));
    file->Append(ID_OPEN_CONNECTION_SERVER, wxT("&Server"));
    file->Append(ID_HELP, wxT("&Help"));
    file->Append(wxID_EXIT, wxT("&Quit\tCtrl+W"));
    file->Append(CLIENT_OPEN, "&Open session\tAlt-O","Connect to server");
    file->Append(CLIENT_CLOSE,"&Close session\tAlt-C","Close connection");
    file->Append(Minimal_Quit, "E&xit\tAlt-X", "Quit this program");
    file->AppendSeparator();

    // Quit
    Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(Frame::OnQuit));
    //Help
    Connect(ID_HELP, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(Frame::OnHelp));
    // Start play
    Connect(ID_PLAY, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(Frame::OnPlay));

    // Server but
    Connect(ID_OPEN_CONNECTION_SERVER, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(Frame::OnServer));


    Center();

    menubar->Append(file, wxT("&File"));
    SetMenuBar(menubar);

    sock = new wxSocketClient();

    // Setup the event handler and subscribe to most events
    sock->SetEventHandler( *this, SOCKET_ID);
    sock->SetNotify(wxSOCKET_CONNECTION_FLAG |
                    wxSOCKET_INPUT_FLAG |
                    wxSOCKET_LOST_FLAG);
    sock->Notify(true);
    m_text  = new wxTextCtrl(this, -1,
                             _("Welcome to wxSocket demo: Client\nClient ready\n"),
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

void Frame::OnPlay(wxCommandEvent& WXUNUSED(event)) {
    file->Enable(ID_PLAY, false);

    if (this->busy) {

        m_lp->Destroy();
        m_rp->Destroy();

    } else {
        m_text->Destroy();
        m_parent = new wxPanel(this, wxID_ANY);
        statusScore = CreateStatusBar(2);
    }

    statusScore->SetStatusText(wxT("Your lvl: 1"));

    hbox = new wxBoxSizer(wxHORIZONTAL);

    m_lp = new GamePanel(m_parent, this, sock);
    /*std::map<std::string, int> my_map = {
            { "A", 1 },
            { "B", 2 },
            { "C", 3 }
    };*/
    m_rp = new InfoPanel(m_parent, this);
    //Start tetris
    m_lp->SetFocus();
    m_lp->Start();

    srand(time(NULL));

    hbox->Add(m_lp, 0, wxEXPAND | wxALL, 5);
    hbox->Add(m_rp, 1, wxEXPAND | wxALL, 5);

    m_parent->SetSizer(hbox);

    this->Centre();
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


void Frame::OnOpenConnection(wxCommandEvent& WXUNUSED(event))
{
    // Create the address - defaults to localhost:0 initially
    IPaddress addr;
    //addr.AnyAddress();

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

    file->Enable(CLIENT_OPEN, false);
    file->Enable(CLIENT_CLOSE, false);
    // we connect asynchronously and will get a wxSOCKET_CONNECTION event when
    // the connection is really established
    //
    // if you want to make sure that connection is established right here you
    // could call WaitOnConnect(timeout) instead

    sock->Connect(addr, false);

    //update status
    UpdateStatusBar();
}

void Frame::OnCloseConnection(wxCommandEvent& WXUNUSED(event))
{
    sock->Close();
    //update status
    UpdateStatusBar();
}

void Frame::OnSocketEvent(wxSocketEvent& event)
{
    std::cout<<"OnSocketEvent: " << std::endl;
    wxSocketBase *sockBase = event.GetSocket();

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
                return;
            }
            else {
                std::cout<< "Read " <<lenRd <<" bytes."  << std::endl;
            }

            std::cout<< "Rx: " <<  wxString::FromUTF8(buf, len) << std::endl;
            // Enable input events again.
            sockBase->SetNotify(wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG);
            break;
        }
        default:;
    }

    //update status
    UpdateStatusBar();
}

void Frame::UpdateStatusBar()
{
    file->Enable(CLIENT_OPEN, !sock->IsConnected());
    file->Enable(CLIENT_CLOSE, sock->IsConnected());
    if (sock->IsConnected()) {
        SetStatusText(wxString::Format(wxT("Connected")), 1);
    }
    else {
        SetStatusText(wxString::Format(wxT("Not connected")), 1);
    }
}

//_______________________SERVER__________________________________
void Frame::OnServer(wxCommandEvent& WXUNUSED(event))
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
}

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
    SetStatusText(wxString::Format(wxT("%d  clients connected"),numClients), 1);
}

void Frame::ServerOnSocketEvent(wxSocketEvent& event){

    std::cout << "OnSocketEvent: ";
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
            sockBase_curr->Read(&len, 1);
            char buf[256];
            // Read the message
            wxUint32 lenRd = sockBase_curr->Read(buf, len).LastCount();
            if (!lenRd)		{
                std::cout << "Failed to read message.\n";
                return;
            }
            else {
                std::cout << "Read  "<< lenRd << "bytes.\n";
            }

            std::cout << "Rx: "<< wxString::FromUTF8(buf, len) <<" \n";


            wxSocketBase *sockBase_curr_2;
            for(auto it = clients.begin(); it != clients.end(); ++it){
                sockBase_curr_2 = *it;
                sockBase_curr_2->Write(&len,1);
                sockBase_curr_2->Write(buf, len);
                std::cout << "Tx: " << wxString::From8BitData(buf, len) << "\n";
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

    SetStatusText(wxString::Format(wxT("%d  clients connected"), numClients), 1);
}
//_______________________SERVER__________________________________