#include "main.h"
#include "Frame.h"
#include <unistd.h>
#include <chrono>

#define __GXX_ABI_VERSION 1011

IMPLEMENT_APP(TetrisApp);

bool TetrisApp::OnInit()
{

    Frame *server = new Frame(wxT("Tetris"));
    server->ShowFullScreen(FALSE);
    server->SetMinSize(wxSize(500, 400));
    server->SetMaxSize(wxSize(500, 400));

    server->Show(true);

    return true;
}
