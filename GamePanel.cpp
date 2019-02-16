#include "GamePanel.h"
#include "InfoPanel.h"
#include "Frame.h"
#include <wx/stattext.h>
//<<<<<<< HEAD
//#include <cmath>
//=======
#include <chrono>
//>>>>>>> 4334ef3886ac46ff8d03ea817f60801ffe69db14

GamePanel::GamePanel(wxPanel* parent_t, wxFrame *fr)
        : wxPanel(parent_t, -1, wxPoint(5, 5), wxSize(175, 345), wxBORDER_SUNKEN)
{
    timer = new wxTimer(this, 1);
    status_scr = fr->GetStatusBar();
    pieceFallingFinished = false;
    started = false;
    paused = false;
    score = 0;
    lvl = 1;
    curX = 0;
    curY = 0;
    panel = parent_t;
    TIMER_INTERVAL = 500;
    next.SetShape(PieceShape(rand()%7+1));
    //current.SetShape(PieceShape(rand()%7+1));

    //RandomPiece(); - цю треба тут, але з нею виходить переповнення
    ClearBoard();

    Connect(wxEVT_PAINT, wxPaintEventHandler(GamePanel::OnPaint));
    Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(GamePanel::OnKeyDown));
    Connect(wxEVT_TIMER, wxCommandEventHandler(GamePanel::OnTimer));
}

void GamePanel::Start()
{
    if (paused)
        return;

    started = true;
    pieceFallingFinished = false;
    paused = false;
    lvl = 1;

//<<<<<<< HEAD
    //current.SetShape(PieceShape(rand()%7+1));
//=======

    //current.SetRandomShape();
//>>>>>>> 4334ef3886ac46ff8d03ea817f60801ffe69db14
    //RandomPiece();

    status_scr->SetStatusText(wxT("Your lvl: 1"));
    MakeNewPiece();
    timer->Start(this->TIMER_INTERVAL);


}

void GamePanel::Pause()
{
    if (!started)
        return;

    paused = !paused;

    if (paused)
    {
        timer->Stop();
        status_scr->SetStatusText(wxT("Game Paused"));
    }
    else
    {
        timer->Start(this->TIMER_INTERVAL);
        wxString str;
        str.Printf(wxT("Your lvl: %d"), lvl);
        status_scr->SetStatusText(str);
    }
    Refresh();
}

void GamePanel::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);

    wxSize size = GetClientSize();
    int top = size.GetHeight() - BoardHeight * Height();

    for (int i = 0; i < BoardHeight; i++)
        for (int j = 0; j < BoardWidth; j++)
        {
            PieceShape pieceShape = PieceCheck(j, BoardHeight - i - 1);
            if (pieceShape == None)
                continue;
            DrawPieceSquare(dc, j * Width(), top + i * Height(), pieceShape);
        }

    if (current.GetShape() == None)
        return;

    for (int i = 0; i < 4; i++)
    {
        int x = curX + current.x(i);
        int y = curY - current.y(i);
        DrawPieceSquare(dc, x * Width(), top + (BoardHeight - y - 1) * Height(),
                        current.GetShape());
    }
}

void GamePanel::OnKeyDown(wxKeyEvent& event)
{
    int keyCode = event.GetKeyCode();

    switch (keyCode)
    {
        case 'P':
            Pause();
            return;
    }

    if (!started || current.GetShape() == None || paused)
    {
        event.Skip();
        return;
    }

    switch (keyCode)
    {
        case WXK_SPACE:
            DropDown();
            break;
        case WXK_UP:
            DoMove(current.Rotation(), curX, curY);
            break;
        case WXK_DOWN:
            DropOneLine();
            break;
        case WXK_LEFT:
            //if (!CheckBounds(curX, current))
            DoMove(current, curX - 1, curY);
            break;
        case WXK_RIGHT:
            //if (!CheckBounds(curX, current))
            DoMove(current, curX + 1, curY);
            break;
        default:
            event.Skip();
    }
}

void GamePanel::OnTimer(wxCommandEvent& event)
{
    if (pieceFallingFinished)
    {
        pieceFallingFinished = false;
        MakeNewPiece();
    }
    else
        DropOneLine();
}

void GamePanel::ClearBoard()
{
    for (int i = 0; i < BoardHeight * BoardWidth; i++)
        board[i] = None;
}

void GamePanel::DropDown()
{
    int y = curY;
    while (y)
        if (!DoMove(current, curX, --y))
            break;
    PieceDropped();
}

void GamePanel::DropOneLine()
{
    if (!DoMove(current, curX, curY - 1))
        PieceDropped();
}

void GamePanel::PieceDropped()
{
    for (int i = 0; i < 4; i++)
    {
        int x = curX + current.x(i);
        int y = curY - current.y(i);
        PieceCheck(x, y) = current.GetShape();
    }
    RemoveFullLines();
    if (!pieceFallingFinished)
        MakeNewPiece();
}

void GamePanel::RemoveFullLines()
{
    int lines = 0;

    for (int i = BoardHeight - 1; i >= 0; i--)
    {
        bool lineFull = true;
        for (int j = 0; j < BoardWidth; j++)
            if (PieceCheck(j, i) == None)
            {
                lineFull = false;
                break;
            }

        if (!lineFull)
            continue;

        lines++;
        for (int j = i; j < BoardHeight - 1; j++)
            for (int k = 0; k < BoardWidth; k++)
                PieceCheck(k, j) = PieceCheck(k, j + 1);
    }

    if (!lines)
        return;
    if (lines == 1) score+= CalculatorScore(40, lvl);
    else if (lines == 2) score+= CalculatorScore(100, lvl);
    else if (lines == 3) score+= CalculatorScore(300, lvl);
    else if (lines == 4) score+= CalculatorScore(1200, lvl);

    lvl = score/500 + 1;
    this->TIMER_INTERVAL = this->TIMER_INTERVAL - lines*25;
    wxString str;
    str.Printf(wxT("Your lvl: %d"), lvl);
    status_scr->SetStatusText(str);

    Frame *comm = (Frame *) panel->GetParent();
    comm->m_rp->string_score->SetLabel(wxString::Format(wxT("Score: %d"), score));
    //comm->m_rp->DrawScore(score);

    pieceFallingFinished = true;
    current.SetShape(None);
    timer->Start(this->TIMER_INTERVAL);
    Refresh();
}

int GamePanel::CalculatorScore(int points, int n) {
    return points*(n+1);
}

void GamePanel::RandomPiece()
{
    current.SetShape(next.GetShape());
    next.SetShape(PieceShape(rand()%7+1));

    Frame *comm = (Frame *) panel->GetParent();

    comm->m_rp->piece.SetShape(None);
    comm->m_rp->ClearPeace();

    comm->m_rp->piece.SetShape(next.GetShape());
    comm->m_rp->ChangePeace();
}

void GamePanel::MakeNewPiece()
{
    RandomPiece();
    curX = BoardWidth / 2;
    curY = BoardHeight - 1 + current.MinY();

    if (!DoMove(current, curX, curY))
    {
        current.SetShape(None);
        timer->Stop();
        started = false;
        status_scr->SetStatusText(wxT("You Lose :("));

        Frame *comm = (Frame *) panel->GetParent();
        comm->file->Enable(ID_PLAY, true);
        comm->file->Enable(ID_CREATE, true);
        comm->file->Enable(ID_JOIN, true);
        std::cout << "Do you want to be a server (s)";
        comm->Setbusy(true);
        //panel->Destroy();
    }
}

bool GamePanel::DoMove(const Piece& piece, int newX, int newY)
{

    for (int i = 0; i < 4; i++)
    {
        int x = newX + piece.x(i);
        int y = newY - piece.y(i);

        if (!CheckBounds(x, y) || PieceCheck(x, y) != None)
            return false;
    }

    current = piece;
    curX = newX;
    curY = newY;

    Refresh();

    return true;
}

void GamePanel::DrawPieceSquare(wxPaintDC& dc, int x, int y, PieceShape pieceShape)
{
    wxPen lightPen(light[int(pieceShape)]);
    lightPen.SetCap(wxCAP_PROJECTING);
    dc.SetPen(lightPen);

    dc.DrawLine(x, y + Height() - 1, x, y);
    dc.DrawLine(x, y, x + Width() - 1, y);

    wxPen darkPen(dark[int(pieceShape)]);
    darkPen.SetCap(wxCAP_PROJECTING);
    dc.SetPen(darkPen);

    dc.DrawLine(x + 1, y + Height() - 1, x + Width() - 1, y + Height() - 1);
    dc.DrawLine(x + Width() - 1, y + Height() - 1, x + Width() - 1, y + 1);

    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush(wxBrush(colors[int(pieceShape)]));

    dc.DrawRectangle(x + 1, y + 1, Width() - 2, Height() - 2);
}


