#include "Frame.h"
#include "StartDialog.h"
#include "SelectOpponentsPanel.h"
#include <exception>


using namespace std;

Frame::Frame(const wxString& title)
        : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(500, 400))
{

    busy = false;
    server_on = false;
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

    for(int i=0;i<3;i++){
        opponentScores[i] = 0;
    }

    menubar->Append(file, wxT("&File"));
    SetMenuBar(menubar);

    StartDialog *user = new StartDialog(wxT("LOG IN"));
    UserName = user->GetName();

    try
    {
        // is a check if the exit is pressed, so that the main window does not start, you need Close (true);
        if (UserName == "") {
            user->Destroy();
            Close(true);
        }
        // write to the value buffer here
        strcpy(BufferName, UserName.c_str());
    }
    catch (exception& e)
    {
        std::cout<<"ERROR Log in failed\n "<< std::endl;
    }

    m_text  = new wxTextCtrl(this, -1,
                             wxString::Format(wxT("Start page !\n Hello: %s \n You can start the game in menu!"), UserName),
                             wxDefaultPosition, wxDefaultSize,
                             wxTE_MULTILINE | wxTE_READONLY);

}

void Frame::Setbusy(bool meaning) {
    this->busy = meaning;
}


Frame::~Frame()
{
    this->Destroy();
    delete sock;
}

void Frame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
// true is to force the frame to close
    if(server_on)
        my_server->Destroy();
    Close(true);
}


void Frame::OnHelp(wxCommandEvent& WXUNUSED(event)) {
    Refresh();

    wxMessageBox( _(" You can do: \n"
                    " - Play allone. Without opponents\n"
                    " - Create game. Select number of opponents and enjoy\n"
                    " - Join game. Input opponent's id and enjoy\n"),
                  _("HELP"),
                  wxOK|wxICON_INFORMATION, this );
}


void Frame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(wxString::Format
                         (
                                 "Client\n"
                                 "\n"
                                 "Nazar&Yaroslav \n"
                                 "Tetris"
                         ),
                 "About Client",
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


// Встановлення з'єднання з сервером
void Frame::OpenConnection(bool t)
{
    // Create the address - defaults to localhost:0 initially
    IPaddress addr;
    wxString hostname;
    if (!t) {
        // Ask user for server address
        hostname = wxGetTextFromUser(
                _("Enter the address:"),
                _("Connect ..."),
                _("localhost"));
        if (hostname.empty())
            return;
    }else{
        hostname = "localhost";
    }
    addr.Hostname(hostname);
    addr.Service(3000);
//    std::cout<<"Trying to connect to " << addr.IPAddress() << " : " << addr.Service() << std::endl;

    // we connect asynchronously and will get a wxSOCKET_CONNECTION event when
    // the connection is really established
    //
    // if you want to make sure that connection is established right here you
    // could call WaitOnConnect(timeout) instead

    sock->Connect(addr, false);

    //update status
    UpdateStatusBar();
}

void Frame::sendLogin(){
    if(server_on){
        char wantplchar[8] = "wantpl";
        wantplchar[6] = static_cast<char>(48+want_players);
        size_t txn = strlen(wantplchar);
        unsigned char len;
        len = txn;

        sock->Write(&len, 1);
        sock->Write(&wantplchar, len);
    }
    char login[12] = "login";
    strcat(login, BufferName);
    size_t txn = strlen(login);

    unsigned char len;
    len = txn;
    try
    {
        sock->Write(&len,1);
        sock->Write(&login, len);
//        std::cout << "login_MSG:  " << login << "\n";
    }
    catch (exception& e)
    {
        std::cout<<"ERROR Can't send message!\n "<< std::endl;
    }
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
    }

    statusScore->SetStatusText(wxT("Your lvl: 1"));

    hbox = new wxBoxSizer(wxHORIZONTAL);
    m_rp = new InfoPanel(m_parent, fr, 0); // 0 - opponents
    m_lp = new GamePanel(m_parent, fr, sock, 0); // 0 - opponents


    //Start tetris
    m_lp->SetFocus();
    m_lp->Start();
    srand(time(NULL));

    hbox->Add(m_lp, 1, wxEXPAND | wxALL, 5);
    hbox->Add(m_rp, 1, wxEXPAND | wxALL, 5);

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

    SelectOpponentsPanel *setWantPlayersPanel = new SelectOpponentsPanel(wxT("OpponDialog"));

    setWantPlayersPanel->Show(true);
    want_players = setWantPlayersPanel->GetCountOpponents();

    if(want_players != 0 ){
        if (!server_on) {
            my_server = new Server(wxT("Server"), want_players);
            my_server->Show(false);
            server_on = true;
        }
        ClientSocket();
        OpenConnection(true);
        statusScore->SetStatusText(wxT("waiting for players"), 2);
    }else {
    file->Enable(ID_PLAY, true);
    file->Enable(ID_CREATE_GAME, true);
    file->Enable(ID_JOIN_GAME, true);
    }
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
    OpenConnection(false);
    statusScore->SetStatusText(wxT("waiting for players"), 2);


}


void Frame::StartPanels(int N) {

    if (N == 1) {
        opPanel = new Opponents(wxT("Opponents"), N);
        opPanel->Show(true);
    }

    statusScore->SetStatusText(wxT("Your lvl: 1"));

    hbox = new wxBoxSizer(wxHORIZONTAL);

    m_rp = new InfoPanel(m_parent, fr, N); // 0 - opponents
    m_lp = new GamePanel(m_parent, fr, sock, N); // 0 - opponents


    //Start tetris

    m_lp->SetFocus();
    m_lp->Start();

    srand(time(NULL));



    hbox->Add(m_lp, 1, wxEXPAND | wxALL, 5);
    hbox->Add(m_rp, 1, wxEXPAND | wxALL, 5);

    m_parent->SetSizer(hbox);

    //Centre();
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
    wxSocketBase *sockBase = event.GetSocket();
    int score;
    // First, print a message
    /*switch (event.GetSocketEvent())
    {
        case wxSOCKET_INPUT: std::cout<< "wxSOCKET_INPUT" << std::endl; break;
        case wxSOCKET_LOST: std::cout<<"wxSOCKET_LOST" << std::endl; break;
        case wxSOCKET_CONNECTION: std::cout<<"wxSOCKET_CONNECTION" << std::endl; break;
        default: std::cout<< "Unexpected event !" << std::endl; break;
    }*/

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
            try
            {
                wxUint32 lenRd = sockBase->Read(buf, len).LastCount();
                if (!lenRd) {
                    throw "Failed to read message";
                }
            }
            catch (exception& e)
            {
                std::cout<<"ERROR Failed to read message.\n "<< std::endl;
            }

            try
            {
                // Processing the message from the server
                // if received a movement command
                if (strncmp( buf, "move", (size_t) 4 )==0){
                    if(nb_op == 2)
                        opPanel->m_lp->SetMovement(buf[4]);
                // if received a score
                }else if(strncmp( buf, "score", (size_t) 5 )==0)
                {
                    int pos;
                    for(int i = 0; i<nb_op; i++){
                        if(strncmp( Server::substr(buf, 5, lenlogins[i]), opponentslog[i], (size_t) lenlogins[i] )==0) {
                            score = std::stoi( Server::substr(buf, (5+lenlogins[i]), (len - 5 - lenlogins[i])) );

                            if(i<index){
                                pos = i+1;
                            }else
                            {
                                pos = i;
                            }
                            if((score - opponentScores[pos]) > 1000){
                                m_lp->SetMovement('d');
                            }
                            opponentScores[pos] = score;
                            m_rp->strings_score[pos]->SetLabel(wxString::Format(wxT("%s score: %d"), opponentslog[i], score));
                            break;
                        }
                    }
                // if received a next piece command
                }else if(strncmp( buf, "next", (size_t) 4 )==0){
                    if(nb_op == 2)
                        opPanel->m_lp->setNextOrCurrentPiece(buf[4], 1);
                // if you received a current piece command
                }else if(strncmp( buf, "curr", (size_t) 4 )==0){
                    if(nb_op == 2)
                        opPanel->m_lp->setNextOrCurrentPiece(buf[4], 0);
                }
                // if received a connection command
                else if(strncmp( buf, "conn", (size_t) 4 )==0){
                    sendLogin();
                // if received a start game command
                }else if (strncmp( buf, "start", (size_t) 5 )==0){

                    std::string mystr(buf);
                    int n = mystr[5] - '0';

                    //read logins
                    int tmp;
                    tmp = 5+n+1;
                    for(int i=0; i < n; i++ ){

                        lenlogins[i] = buf[6+i]-'0';
                        opponentslog[i] = Server::substr(buf, tmp, lenlogins[i]);
                        tmp = tmp + lenlogins[i];
                        if(strncmp(opponentslog[i], BufferName, (size_t) strlen(BufferName) )==0){
                            index = i;
                        }
                    }
                    //Run game
                    nb_op = n;
                    StartPanels(n-1); // -1 because one player is you and n need for game.
                }
                // if received a lose command from players
                else if (strncmp( buf, "lose", (size_t) 4 )==0){
                    int pos;
                    for(int i = 0; i<nb_op; i++){
                        if(strncmp( Server::substr(buf, 4, lenlogins[i]), opponentslog[i], (size_t) lenlogins[i] )==0) {
                            int tmp_score = std::stoi( Server::substr(buf, (4+lenlogins[i]), (len - 4 - lenlogins[i])) );
                            if(i<index){
                                pos = i+1;
                            }else
                            {
                                pos = i;
                            }
                            m_rp->strings_score[pos]->SetLabel(wxString::Format(wxT("%s final score: %d"), opponentslog[i], tmp_score));
                            break;
                        }
                    }
                // if received a gameover command
                }else if(strncmp( buf, "gameover", (size_t) 8 )==0){
                    //
                    int tmp = len - 8;

                    if(strncmp( Server::substr(buf, 8, tmp), BufferName, (size_t) strlen(BufferName) )==0){
                        SetStatusText(wxString::Format(wxT("You win!")), 0);
                        ShowMessageWin();
                    }else{
                        SetStatusText(wxString::Format(wxT("You lose!")), 0);
                        ShowMessageLose();
                    }
                    if(nb_op==2)
                        opPanel->Destroy();

                    CloseConnection();

                    file->Enable(ID_CREATE_GAME, true);
                    file->Enable(ID_JOIN_GAME, true);


                }
            }
            catch (exception& e)
            {
                std::cout<<"ERROR 201\n "<< std::endl;
            }


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

    if (sock->IsConnected()) {
        SetStatusText(wxString::Format(wxT("Connected")), 2);
    }
    else {
        SetStatusText(wxString::Format(wxT("Not connected")), 2);
    }
}

void Frame::ShowMessageWin()
{
    wxMessageDialog *dial = new wxMessageDialog(NULL,
                                                wxT("You win this game!!"), wxT("Congratulation"), wxOK);
    dial->ShowModal();
}


void Frame::ShowMessageLose()
{
    wxMessageDialog *dial = new wxMessageDialog(NULL,
                                                wxT("You lose this game!!"), wxT("Congratulation"), wxOK);
    dial->ShowModal();
}

InfoPanel* Frame::getm_rp(){
    return  m_rp;
}

char* Frame::getBufferName() {
    return BufferName;
}
bool Frame::isServerOn(){
    return server_on;
}
void Frame::setServerOn(bool t){
    server_on = t;
}

std::string Frame::getUserName(){
    return UserName;
}
int Frame::getIndex(){
    return index;
}
char *Frame::getopponentslog(int i){
    return opponentslog[i];
}

int Frame::getwant_players() {
    return want_players;
}
