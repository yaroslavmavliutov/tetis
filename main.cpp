#include "main.h"
#include "Frame.h"

IMPLEMENT_APP(TetrisApp);

bool TetrisApp::OnInit()
{

    Frame *communicate = new Frame(wxT("TETRIS"));
    communicate->Show(true);

    return true;
}