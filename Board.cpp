//
// Created by nazar on 2/27/19.
//

#include "Board.h"

Board::Board(wxPanel* parent_t, wxFrame *fr)
        : wxPanel(parent_t, -1, wxPoint(5, 5), wxSize(170, 310), wxBORDER_SUNKEN)
{
    status_scr = fr->GetStatusBar();
    pieceFallingFinished = false;
    started = false;
    paused = false;
    score = 0;

    lvl = 1;
    curX = 0;
    curY = 0;
    panel = parent_t;

    PieceShape tmp;
    tmp = PieceShape(rand()%7+1);
    next.SetShape(tmp);
    ClearBoard();

//    std::cout << "BOARD mather - constructor vide" << std::endl;
}


void Board::ClearBoard()
{
    for (int i = 0; i < BoardHeight * BoardWidth; i++)
        board[i] = None;
}


int Board::CalculatorScore(int points, int n) {
    return points*(n+1);
}


bool Board::DoMove(const Piece& piece, int newX, int newY)
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


void Board::DrawPieceSquare(wxPaintDC& dc, int x, int y, PieceShape pieceShape)
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


void Board::setNextOrCurrentPiece(char c, int current_next){
    PieceShape ps;

    if(current_next == 1) {
        curX = BoardWidth / 2;
        curY = BoardHeight - 1 + current.MinY();
    }
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
    if(current_next==1){
        next.SetShape(ps);
    }else{
        current.SetShape(ps);
    }
}



