#include "main.h"
#include <unistd.h>
#include <chrono>

IMPLEMENT_APP(TetrisApp);

bool TetrisApp::OnInit()
{
    Frame *server = new Frame(wxT("Tetris"));
    server->Show(true);

    return true;
}
