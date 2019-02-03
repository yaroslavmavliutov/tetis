#include "Frame.h"
#include "GamePanel.h"
#include "InfoPanel.h"
#include "Piece.h"

using namespace std;
using namespace sf;

Frame::Frame(const wxString& title, int type_machine)
        : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(500, 380))
{
    //MPI init
    int rank, size;
    int argc = 1;
    //MPI_Init (&argc, NULL);      // starts MPI
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);        // get current process id
    MPI_Comm_size (MPI_COMM_WORLD, &size);        // get number of processes
    //MPI init
    printf( "FRAME from process %d of %d\n", rank, size );

    if(rank == 0) // for 2 threads
    {
        m_parent = new wxPanel(this, wxID_ANY);
        menubar = new wxMenuBar; // menu
        file = new wxMenu; //menu


        file->Append(ID_NEW, wxT("&New"));
        file->Append(wxID_ANY, wxT("&Help"));
        file->Append(wxID_ANY, wxT("&Get_IP"));

        file->AppendSeparator();

        file->Append(wxID_EXIT, wxT("&Quit\tCtrl+W"));

        Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED,
                wxCommandEventHandler(Frame::OnQuit));

        Connect(ID_NEW, wxEVT_COMMAND_MENU_SELECTED,
                wxCommandEventHandler(Frame::OnNew));
        Center();

        menubar->Append(file, wxT("&File"));
        SetMenuBar(menubar);


        statusScore = CreateStatusBar();
        statusScore->SetStatusText(wxT("Your lvl: 1"));

        printf( "CREATION 1from process %d of %d\n", rank, size );
        wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
        printf( "CREATION 2from process %d of %d\n", rank, size );

        GamePanel *m_lp = new GamePanel(m_parent, this);
        printf( "CREATION  3 from process %d of %d\n", rank, size );
        m_rp = new InfoPanel(m_parent, this);
        printf( "CREATION  4 from process %d of %d\n", rank, size );

<<<<<<< HEAD
    hbox->Add(m_lp, 0, wxSHAPED | wxALL, 5);
    hbox->Add(m_rp, 0, wxSHAPED | wxALL, 5);
=======
        m_lp->SetFocus();
        m_lp->Start();
>>>>>>> 2f706f5b0557031414007c3ca63092560d6aa1cc

        srand(time(NULL));

        hbox->Add(m_lp, 0, wxSHAPED | wxALL, 5);
        hbox->Add(m_rp, 1, wxEXPAND | wxALL, 5);

        m_parent->SetSizer(hbox);

        this->Centre();
        printf( "CREATION  5 from process %d of %d\n", rank, size );
    }
    //MPI_Finalize();
}

void Frame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

void Frame::OnNew(wxCommandEvent& WXUNUSED(event))
{
    Centre();
}