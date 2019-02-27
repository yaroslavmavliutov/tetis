#include "Frame.h"
#include "StartDialog.h"
#include "SelectOpponentsPanel.h"
#include "GamePanelOpponent.h"
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

    menubar->Append(file, wxT("&File"));
    SetMenuBar(menubar);

    StartDialog *user = new StartDialog(wxT("LOG IN"));
    //username типу std::string
    UserName = user->GetName();

    try
    {
        // це перевірка якшо хрестик для виходу нажали, то шоб не запустилось основне вікно, треба Close(true);
        if (UserName == "") {
            user->Destroy();
            Close(true);
        }
        // тут записуємо в буфер значення
        strcpy(BufferName, UserName.c_str());
    }
    catch (exception& e)
    {
        std::cout<<"ERROR\n "<< std::endl;
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
void Frame::OpenConnection()
{
    // Create the address - defaults to localhost:0 initially
    IPaddress addr;

    // Ask user for server address
    wxString hostname = wxGetTextFromUser(
            _("Enter the address:"),
            _("Connect ..."),
            _("localhost"));
    if ( hostname.empty() )
        return;

    addr.Hostname(hostname);
    addr.Service(3000);
    std::cout<<"Trying to connect to " << addr.IPAddress() << " : " << addr.Service() << std::endl;

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
    char login[12] = "login";
    strcat(login, BufferName);
    size_t txn = strlen(login);

    unsigned char len;
    len = txn;
    try
    {
        sock->Write(&len,1);
        sock->Write(&login, len);
        std::cout << "login_MSG:  " << login << "\n";
    }
    catch (exception& e)
    {
        std::cout<<"ERROR\n "<< std::endl;
    }


    //sock->SetNotify(wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG);
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
//
    setWantPlayersPanel->Show(true);
    want_players = setWantPlayersPanel->GetCountOpponents();
    std::cout << "Want_player -> " << want_players << std::endl;

    if(want_players != 0 ){
        if (!server_on) {
            Server *my_server = new Server(wxT("Server"), want_players);
            my_server->Show(true);
        }
        ClientSocket();
        OpenConnection();
        std::cout << "SERVER waiting for players" << std::endl;
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
    OpenConnection();

    std::cout<<"CLIENT waiting for all players" << std::endl;

}


void Frame::StartPanels(int N) {

    if (N == 1) {
        opPanel = new Opponents(wxT("Opponents"), N);
        opPanel->Show(true);
    }

    statusScore->SetStatusText(wxT("Your lvl: 1"));

    hbox = new wxBoxSizer(wxHORIZONTAL);

    std::cout<<"N -> "<< N <<std::endl;
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
            try
            {
                wxUint32 lenRd = sockBase->Read(buf, len).LastCount();
                if (!lenRd) {
                    std::cout<< "Failed to read message." << std::endl;
                    return;
                }
                else {
                    std::cout<< "CLIENT Read  -> " <<lenRd <<" bytes."  << std::endl;
                }
            }
            catch (exception& e)
            {
                std::cout<<"ERROR\n "<< std::endl;
            }

            try
            {
                // обробка повідомлення з сервера
                if(strncmp( buf, "conn", (size_t) 4 )==0){
                    sendLogin();
                }else if (strncmp( buf, "start", (size_t) 5 )==0){

                    std::string mystr(buf);
                    int n = mystr[5] - '0';//std::stoi(std::to_string((int) mystr[5]));

                    //зчитати логіни
                    int tmp;
                    tmp = 5+n+1;
                    for(int i=0; i < n; i++ ){

                        lenlogins[i] = buf[6+i]-'0';
                        std::cout << "login n -> " << lenlogins[i]  << std::endl;
                        opponentslog[i] = Server::substr(buf, tmp, lenlogins[i]);
                        tmp = tmp + lenlogins[i];
                        std::cout << "opponentslog[i] -> " << opponentslog[i]<< std::endl;
                        if(strncmp(opponentslog[i], BufferName, (size_t) strlen(BufferName) )==0){
                            index = i;
                        }
                    }

//             запустити груу!!!!

                    std::cout << "LETS START THE GAME! with " << n << " players" << std::endl;
                    nb_op = n;
                    StartPanels(n-1); // 0 - це ти один , 1 ти і суперник
                }
                else if (strncmp( buf, "lose", (size_t) 4 )==0){
                    std::cout << "Non start MSG";
                    int pos;
                    for(int i = 0; i<nb_op; i++){
                        if(strncmp( Server::substr(buf, 4, lenlogins[i]), opponentslog[i], (size_t) lenlogins[i] )==0) {
                            score = std::stoi( Server::substr(buf, (4+lenlogins[i]), (len - 4 - lenlogins[i])) );
                            if(i<index){
                                pos = i+1;
                            }else
                            {
                                pos = i;
                            }
                            m_rp->strings_score[pos]->SetLabel(wxString::Format(wxT("%s lose final score: %d"), opponentslog[i], score));
                            break;
                        }
                    }

                }else if(strncmp( buf, "gameover", (size_t) 8 )==0){
                    //
                    int tmp = len - 8;
                    std::cout<<"GAMEOVER MSG-> "<<buf << " lenlog->"<< tmp <<std::endl;
                    if(strncmp( Server::substr(buf, 8, tmp), BufferName, (size_t) strlen(BufferName) )==0){
                        ShowMessageWin();
                    }else{
                        ShowMessageLose();
                    }
                    CloseConnection();
                    file->Enable(ID_CREATE_GAME, true);
                    file->Enable(ID_JOIN_GAME, true);


                }else if (strncmp( buf, "move", (size_t) 4 )==0){
                    std::cout << "MOVE      -> " << buf << std::endl;
                    std::cout << "END       -> " << buf[4] << std::endl;
                    if(nb_op == 2)
                        opPanel->m_lp->SetMovement(buf[4]);
                }else if(strncmp( buf, "next", (size_t) 4 )==0){
                    std::cout << "NEXT fig      -> " << buf[4] << std::endl;
                    if(nb_op == 2)
                        opPanel->m_lp->setNextPiece(buf[4]);
                }
<<<<<<< HEAD
                else if(strncmp( buf, "curr", (size_t) 4 )==0){
                    std::cout << "CURR fig      -> " << buf[4] << std::endl;
                    if(nb_op == 2)
                        opPanel->m_lp->setCurrentPiece(buf[4]);
                }
                else if(strncmp( buf, "score", (size_t) 5 )==0)
                {
                    std::cout << "Non start MSG";
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
                            m_rp->strings_score[pos]->SetLabel(wxString::Format(wxT("%s score: %d"), opponentslog[i], score));
                            break;
=======
                CloseConnection();
                file->Enable(ID_CREATE_GAME, true);
                file->Enable(ID_JOIN_GAME, true);


            }else if (strncmp( buf, "move", (size_t) 4 )==0){
                std::cout << "MOVE      -> " << buf << std::endl;
                std::cout << "END       -> " << buf[4] << std::endl;
                if(nb_op == 2)
                    opPanel->m_lp->SetMovement(buf[4]);
            }else if(strncmp( buf, "next", (size_t) 4 )==0){
                std::cout << "NEXT fig      -> " << buf[4] << std::endl;
                if(nb_op == 2)
                    opPanel->m_lp->setNextOrCurrentPiece(buf[4], 1);
            }
            else if(strncmp( buf, "curr", (size_t) 4 )==0){
                std::cout << "CURR fig      -> " << buf[4] << std::endl;
                if(nb_op == 2)
                    opPanel->m_lp->setNextOrCurrentPiece(buf[4], 0);
            }
            else if(strncmp( buf, "score", (size_t) 5 )==0)
            {
                std::cout << "Non start MSG";
                int pos;
                for(int i = 0; i<nb_op; i++){
                    if(strncmp( Server::substr(buf, 5, lenlogins[i]), opponentslog[i], (size_t) lenlogins[i] )==0) {
                        score = std::stoi( Server::substr(buf, (5+lenlogins[i]), (len - 5 - lenlogins[i])) );
                        if(i<index){
                            pos = i+1;
                        }else
                        {
                            pos = i;
>>>>>>> 27b2905ba529b71c035c7bc5776d36698404f85d
                        }
                    }

                }
            }
            catch (exception& e)
            {
                std::cout<<"ERROR\n "<< std::endl;
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

