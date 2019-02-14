#include "Frame.h"
#include "GamePanel.h"
#include "InfoPanel.h"
#include "Piece.h"

using namespace std;
using namespace sf;

Frame::Frame(const wxString& title)
        : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(500, 380))
{
<<<<<<< HEAD
    //MPI init
    /*int rank, size;
    int argc = 1;
    //MPI_Init (&argc, NULL);      // starts MPI
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);        // get current process id
    MPI_Comm_size (MPI_COMM_WORLD, &size);        // get number of processes
    //MPI init
    printf( "FRAME from process %d of %d\n", rank, size );*/

    //if(rank == 0) // for 2 threads
    if (true)
    {
        m_parent = new wxPanel(this, wxID_ANY);
        menubar = new wxMenuBar; // menu
        file = new wxMenu; //menu
=======
    m_parent = new wxPanel(this, wxID_ANY);
    menubar = new wxMenuBar; // menubar
    file = new wxMenu; //menu
>>>>>>> 4334ef3886ac46ff8d03ea817f60801ffe69db14

    file->Append(ID_NEW, wxT("&Start page"));
    file->Append(ID_CREATE, wxT("&Create game"));
    file->Append(ID_JOIN, wxT("&Join game"));
    file->Append(wxID_ANY, wxT("&Help"));
    file->Append(wxID_ANY, wxT("&Get_IP"));

    file->AppendSeparator();

    file->Append(wxID_EXIT, wxT("&Quit\tCtrl+W"));

    Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(Frame::OnQuit));

    Connect(ID_NEW, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(Frame::OnNew));

    Connect(ID_CREATE, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(Frame::OnCreate));

    Connect(ID_JOIN, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(Frame::OnJoin));

    Center();

    menubar->Append(file, wxT("&File"));
    SetMenuBar(menubar);

<<<<<<< HEAD
        //printf( "CREATION 1from process %d of %d\n", rank, size );
        wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
        //printf( "CREATION 2from process %d of %d\n", rank, size );

        GamePanel *m_lp = new GamePanel(m_parent, this);
        //printf( "CREATION  3 from process %d of %d\n", rank, size );
        m_rp = new InfoPanel(m_parent, this);
        //printf( "CREATION  4 from process %d of %d\n", rank, size );

        m_lp->SetFocus();
        m_lp->Start();
        srand(time(NULL));

        hbox->Add(m_lp, 0, wxSHAPED | wxALL, 5);
        hbox->Add(m_rp, 0, wxSHAPED | wxALL, 5);
=======
    statusScore = CreateStatusBar();
    statusScore->SetStatusText(wxT("Your lvl: 1"));


    wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

    GamePanel *m_lp = new GamePanel(m_parent, this);
    m_rp = new InfoPanel(m_parent, this);

>>>>>>> 4334ef3886ac46ff8d03ea817f60801ffe69db14

    m_lp->SetFocus();
    m_lp->Start();

<<<<<<< HEAD
        this->Centre();
        //printf( "CREATION  5 from process %d of %d\n", rank, size );
    }
    //MPI_Finalize();
=======
    srand(time(NULL));

    hbox->Add(m_lp, 0, wxSHAPED | wxALL, 5);
    hbox->Add(m_rp, 1, wxEXPAND | wxALL, 5);

    m_parent->SetSizer(hbox);

    this->Centre();
>>>>>>> 4334ef3886ac46ff8d03ea817f60801ffe69db14
}

void Frame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

void Frame::OnNew(wxCommandEvent& WXUNUSED(event)) {

    m_rp->Destroy();
}

void Frame::OnNew() {


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

