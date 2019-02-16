
#include "Frame.h"

// frame constructor
Frame::Frame(const wxString& title)
        : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(1000, 650)
        ,wxDEFAULT_FRAME_STYLE ^ wxRESIZE_BORDER)
{
    // set the frame icon


#if wxUSE_MENUS
    // create a menu bar
	fileMenu = new wxMenu;

	// the "About" item should be in the help menu
	helpMenu = new wxMenu;
	helpMenu->Append(Minimal_About, "&About\tF1", "Show about dialog");

	fileMenu->Append(CLIENT_OPEN, "&Open session\tAlt-O","Connect to server");
	fileMenu->Append(CLIENT_CLOSE,"&Close session\tAlt-C","Close connection");
	fileMenu->Append(Minimal_Quit, "E&xit\tAlt-X", "Quit this program");

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
	SetStatusText("TCP client using wxWidgets");
#endif // wxUSE_STATUSBAR
/*    btnSend = new wxButton(this, ID_BTNSEND, wxT("Send"),
                           wxPoint(5, 5), wxSize(100, 25));
    txtSend = new wxTextCtrl(this,ID_TXTSEND, wxT("Hello!"),
                             wxPoint(120, 5), wxSize(250, 25));./
    txtRx = new wxTextCtrl(this,ID_TXTRX, wxT(""),
                           wxPoint(5, 35), wxSize(365, 125), wxTE_MULTILINE);

    Connect(ID_BTNSEND, wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(Frame::OnSend));
*/
    sock = new wxSocketClient();

    // Setup the event handler and subscribe to most events
    sock->SetEventHandler( *this, SOCKET_ID);
    sock->SetNotify(wxSOCKET_CONNECTION_FLAG |
                    wxSOCKET_INPUT_FLAG |
                    wxSOCKET_LOST_FLAG);
    sock->Notify(true);

    m_parent = new wxPanel(this, wxID_ANY);
    wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

    GamePanel *m_lp = new GamePanel(m_parent, this, sock);
    m_rp = new InfoPanel(m_parent, this);


    m_lp->SetFocus();
    m_lp->Start();

    srand(time(NULL));

    hbox->Add(m_lp, 0, wxSHAPED | wxALL, 5);


    hbox->Add(m_rp, 1, wxEXPAND | wxALL, 5);

    m_parent->SetSizer(hbox);
    this->Centre();

    // Create the socket

}

Frame::~Frame()
{
    // No delayed deletion here, as the frame is dying anyway
    delete sock;
}

// event handlers
void Frame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
// true is to force the frame to close
    Close(true);
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
/*
void Frame::OnSend(wxCommandEvent& WXUNUSED(event))
{
    wxString str = txtSend->GetValue();
    wxCharBuffer buffer = str.ToUTF8();
    size_t txn = str.length();

    unsigned char len;
    len = txn;
    sock->Write(&len, 1);//send the length of the message first
    if (sock->Write(buffer.data(), txn).LastCount() != txn)
    {
    txtRx->AppendText(wxT("Write error.\n"));
    return;
    }
    else {
    txtRx->AppendText("Tx: " + str + "\n");
    }
}
*/
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

    fileMenu->Enable(CLIENT_OPEN, false);
    fileMenu->Enable(CLIENT_CLOSE, false);
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
    fileMenu->Enable(CLIENT_OPEN, !sock->IsConnected());
    fileMenu->Enable(CLIENT_CLOSE, sock->IsConnected());
    if (sock->IsConnected()) {
        //SetStatusText(wxString::Format(wxT("%s:%u"),
        //    addr.IPAddress(), addr.Service()), 1);
        SetStatusText(wxString::Format(wxT("Connected")), 1);
    }
    else {
        SetStatusText(wxString::Format(wxT("Not connected")), 1);
    }
}