#include "Frame.h"

using namespace std;

Frame::Frame(const wxString& title)
        : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(500, 380))
{

    busy = false;
    menubar = new wxMenuBar; // menubar

    file = new wxMenu; //menu

    file->Append(ID_PLAY, wxT("&Start play"));
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