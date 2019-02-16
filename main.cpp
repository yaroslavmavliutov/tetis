#include "main.h"
#include "Frame.h"
#include <unistd.h>
#include <chrono>

IMPLEMENT_APP(TetrisApp);

bool TetrisApp::OnInit()
{

    Frame *server = new Frame(wxT("Tetris"));
    server->ShowFullScreen(FALSE);
    server->SetMinSize(wxSize(500, 380));
    server->SetMaxSize(wxSize(500, 380));

    server->Show(true);

    return true;
}
