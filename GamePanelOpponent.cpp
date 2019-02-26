//
// Created by nazar on 2/25/19.
//

#include "GamePanel.h"
#include "InfoPanel.h"
#include "Frame.h"
#include <wx/stattext.h>
#include <chrono>



GamePanelOpponent::GamePanelOpponent(wxPanel* parent_t, wxFrame *fr)
        : wxPanel(parent_t, -1, wxPoint(5, 5), wxSize(170, 310), wxBORDER_SUNKEN)
{

    special = true;
    status_scr = fr->GetStatusBar();
    pieceFallingFinished = false;
    started = false;
    paused = false;
    score = 0;
    lvl = 1;

    curX = 0;
    curY = 0;
    panel = parent_t;

    ClearBoard();
    started = true;
    pieceFallingFinished = false;
    paused = false;
    lvl = 1;


    status_scr->SetStatusText(wxT("Your lvl: 1"));

    std::cout << "GamePanel - constructor vide" << std::endl;
}


void GamePanelOpponent::Pause()
{
    if (!started)
        return;

    paused = !paused;

    if (paused)
    {
//        timer->Stop();
        status_scr->SetStatusText(wxT("Game Paused"));
    }
    else
    {
//        timer->Start(this->TIMER_INTERVAL);
        wxString str;
        str.Printf(wxT("Your lvl: %d"), lvl);
        status_scr->SetStatusText(str);
    }
//    Refresh();
}

void GamePanelOpponent::OnPaint()
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


void GamePanelOpponent::ClearBoard()
{
    for (int i = 0; i < BoardHeight * BoardWidth; i++)
        board[i] = None;
}

void GamePanelOpponent::DropDown()
{
    int y = curY;
    while (y)
        if (!DoMove(current, curX, --y))
            break;

}

void GamePanelOpponent::DropOneLine()
{
    if (!DoMove(current, curX, curY - 1))
    {}

    RemoveFullLines();
}

void GamePanelOpponent::PieceDropped()
{
    for (int i = 0; i < 4; i++)
    {
        int x = curX + current.x(i);
        int y = curY - current.y(i);
        PieceCheck(x, y) = current.GetShape();
    }
    RemoveFullLines();

}

void GamePanelOpponent::RemoveFullLines()
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
//    this->TIMER_INTERVAL = this->TIMER_INTERVAL - lines*25;
    wxString str;
    str.Printf(wxT("Lvl: %d"), lvl);
    status_scr->SetStatusText(str);


    pieceFallingFinished = true;
    current.SetShape(None);
//    timer->Start(this->TIMER_INTERVAL);
//    Refresh();
}


int GamePanelOpponent::CalculatorScore(int points, int n) {
    return points*(n+1);
}


bool GamePanelOpponent::DoMove(const Piece& piece, int newX, int newY)
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

void GamePanelOpponent::DrawPieceSquare(wxPaintDC& dc, int x, int y, PieceShape pieceShape)
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



void GamePanelOpponent::setNextPiece(char c){
    PieceShape ps;

    curX = BoardWidth / 2;
    curY = BoardHeight - 1 + current.MinY();

    switch (c)
    {
        case 'I':
            ps = I_long;
            break;
        case 'O':
            ps = O_bloc;
            break;
        case 'T':
            ps = T;
            break;
        case 'L':
            ps = L;
            break;
        case 'J':
            ps = J;
            break;
        case 'Z':
            ps = Z;
            break;
        case 'S':
            ps = S;
            break;
        default:
            ps = None;
    }
    next.SetShape(ps);
}

void GamePanelOpponent::setCurrentPiece(char c) {
    PieceShape ps;
    switch (c)
    {
        case 'I':
            ps = I_long;
            break;
        case 'O':
            ps = O_bloc;
            break;
        case 'T':
            ps = T;
            break;
        case 'L':
            ps = L;
            break;
        case 'J':
            ps = J;
            break;
        case 'Z':
            ps = Z;
            break;
        case 'S':
            ps = S;
            break;
        default:
            ps = None;
    }
    current.SetShape(ps);
}

void GamePanelOpponent::SetMovement(char c) {
    switch (c) {
        case 'd':
            DropDown();
            break;
        case 'm':
            DoMove(current.Rotation(), curX, curY);
            break;
        case 'o':
            DropOneLine();
            break;
        case 'l':
            DoMove(current, curX - 1, curY);
            break;
        case 'r':
            DoMove(current, curX + 1, curY);
            break;
        case 'p':
            OnPaint();
            break;
        case 'x':
            PieceDropped();
            break;
        default:
            //event.Skip();
            std::cout << "Non move value" << std::endl;
    }
}

void GamePanelOpponent::setCurX(int newX){
    curX = newX;
}

void GamePanelOpponent::setCurY(int newY){
    curY = newY;
}



