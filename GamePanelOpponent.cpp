//
// Created by nazar on 2/25/19.
//

#include "GamePanelOpponent.h"
#include "Piece.h"
#include "InfoPanel.h"
#include "Frame.h"

GamePanelOpponent::GamePanelOpponent(wxPanel* parent_t, wxFrame *fr)
        : Board(parent_t, fr)
{
    started = true;

    std::cout << "GamePanelOpponent - constructor vide" << std::endl;
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

    wxString str;
    str.Printf(wxT("Lvl: %d"), lvl);
    status_scr->SetStatusText(str);
    pieceFallingFinished = true;
    current.SetShape(None);

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
