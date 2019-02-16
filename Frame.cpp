#include "Frame.h"
#include "GamePanel.h"
#include "InfoPanel.h"
#include "Piece.h"

using namespace std;
using namespace sf;

Frame::Frame(const wxString& title)
        : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(500, 380))
{
//<<<<<<< HEAD
    //MPI init
    /*int rank, size;
    int argc = 1;
    //MPI_Init (&argc, NULL);      // starts MPI
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);        // get current process id
    MPI_Comm_size (MPI_COMM_WORLD, &size);        // get number of processes
    //MPI init
    printf( "FRAME from process %d of %d\n", rank, size );*/

    //if(rank == 0) // for 2 threads
//    if (true)
//    {
//        m_parent = new wxPanel(this, wxID_ANY);
//        menubar = new wxMenuBar; // menu
//        file = new wxMenu; //menu
//=======
    busy = false;
    menubar = new wxMenuBar; // menubar
    file = new wxMenu; //menu
//>>>>>>> 4334ef3886ac46ff8d03ea817f60801ffe69db14

    file->Append(ID_PLAY, wxT("&Start play"));
    file->Append(ID_CREATE, wxT("&Create game"));
    file->Append(ID_JOIN, wxT("&Join game"));
    file->Append(ID_HELP, wxT("&Help"));
    file->Append(wxID_EXIT, wxT("&Quit\tCtrl+W"));

    file->AppendSeparator();

    // Quit
    Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(Frame::OnQuit));

    //Help
    Connect(ID_HELP, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(Frame::OnHelp));

    // Create game
    Connect(ID_CREATE, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(Frame::OnCreate));

    // Join game
    Connect(ID_JOIN, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(Frame::OnJoin));

    // Start play
    Connect(ID_PLAY, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(Frame::OnPlay));

    Center();

    menubar->Append(file, wxT("&File"));
    SetMenuBar(menubar);


    m_text  = new wxTextCtrl(this, -1,
                             _("Welcome to wxSocket demo: Client\nClient ready\n"),
                             wxDefaultPosition, wxDefaultSize,
                             wxTE_MULTILINE | wxTE_READONLY);

    //SetStatusText(_("Welcome to wxWidgets!"));
    //string_score = new wxStaticText(this, -1, wxString::Format(wxT("Score")), wxPoint(110, 190));
    //wxPaintDC dc(this);
    //dc.DrawText(wxT("Лев Николaевич Толстoй"), 150, 200);
    //font = new wxFont(18, wxDECORATIVE, wxITALIC, wxNORMAL);
    //string_score->SetFont(font);
    //string_score = new wxStaticText(this, -1, wxString::Format(wxT("Score")), wxPoint(110, 190));

}

void Frame::Setbusy(bool meaning) {
    this->busy = meaning;
}


void Frame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
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
    file->Enable(ID_CREATE, false);
    file->Enable(ID_JOIN, false);

    if (this->busy) {

        m_lp->Destroy();
        m_rp->Destroy();
        //m_parent->Destroy();
        //hbox = nullptr;
        //hbox->Clear(TRUE);
    } else {
        m_text->Destroy();
        m_parent = new wxPanel(this, wxID_ANY);
        statusScore = CreateStatusBar();
    }


    statusScore->SetStatusText(wxT("Your lvl: 1"));

//    wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
    hbox = new wxBoxSizer(wxHORIZONTAL);

    //GamePanel *m_lp = new GamePanel(m_parent, this);
    m_lp = new GamePanel(m_parent, this);

    std::map<std::string, int> my_map = {
            { "A", 1 },
            { "B", 2 },
            { "C", 3 }
    };
    m_rp = new InfoPanel(m_parent, this, my_map);

    m_lp->SetFocus();
    m_lp->Start();

    srand(time(NULL));

    hbox->Add(m_lp, 0, wxEXPAND | wxALL, 5);

    //hbox->Add(m_rp, 1, wxEXPAND | wxALL, 5);

    hbox->Add(m_rp, 1, wxEXPAND | wxALL, 5);

    m_parent->SetSizer(hbox);

    this->Centre();

}

void Frame::OnCreate(wxCommandEvent& WXUNUSED(event))
{
     //--------------SERVER-----------------
        MyThread *thread = new MyThread(0); //0 - server
        if ( thread->Create() != wxTHREAD_NO_ERROR )
        {
                wxLogError(wxT("Can’t create server thread!"));
        }
        thread->Run();
     //-------------------------------
}


void Frame::OnJoin(wxCommandEvent& WXUNUSED(event))
{
    //----------------CLIENT---------------
    MyThread *thread = new MyThread(1); // 1 - client
    if ( thread->Create() != wxTHREAD_NO_ERROR )
    {
        wxLogError(wxT("Can’t create client thread!"));
    }
    thread->Run();
    //-------------------------------
}

