#ifndef _GAMEPANEL_HPP
#define _GAMEPANEL_HPP

#include "Piece.h"
#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/statline.h>
#include <cmath>
#include "wx/socket.h"

const wxColour colors[] =
        {
                wxColour(211, 211, 211), wxColour(102, 102, 204),
                wxColour(218, 170, 0),   wxColour(204, 204, 102),
                wxColour(204, 102, 204),  wxColour(138, 43, 226),
                wxColour(102, 204, 102),   wxColour(204, 102, 102)

        };

const wxColour light[] =
        {
                wxColour(0, 0, 0),       wxColour(121, 121, 252),
                wxColour(252, 198, 0), wxColour(252, 252, 121),
                wxColour(252, 121, 252), wxColour(147, 112, 219),
                wxColour(121, 252, 121), wxColour(248, 159, 171)

        };

const wxColour dark[] =
        {
                wxColour(0, 0, 0),      wxColour(59, 59, 128),
                wxColour(128, 98, 0), wxColour(128, 128, 59),
                wxColour(128, 59, 128), wxColour(75, 0, 130),
                wxColour(59, 128, 59),  wxColour(128, 59, 59)

        };

class GamePanel : public wxPanel
{
public:
    GamePanel(wxPanel* parent_t, wxFrame *fr, wxSocketClient *sock);
    void Start();
    void Pause();
    PieceShape GetNextShape() const { return next.GetShape(); }

    /* wx event handlers */

protected:
    void OnPaint(wxPaintEvent& event);
    void OnKeyDown(wxKeyEvent& event);
    void OnTimer(wxCommandEvent& event);

private:

    enum { BoardWidth = 10, BoardHeight = 22 };
    PieceShape& PieceCheck(int x, int y) { return board[y * BoardWidth + x]; }

    wxSocketClient *sock;

    int Width() { return GetClientSize().GetWidth() / BoardWidth; }
    int Height() { return GetClientSize().GetHeight() / BoardHeight; }
    static bool CheckBounds(int x, int y) { return x >= 0 && x < BoardWidth && y >= 0 && y < BoardHeight; }


    void ClearBoard();
    void DropDown();
    void DropOneLine();
    void PieceDropped();
    void RemoveFullLines();
    void MakeNewPiece();
    void RandomPiece();
    bool DoMove(const Piece& piece, int newX, int newY);
    void DrawPieceSquare(wxPaintDC& dc, int x, int y, PieceShape shape);
    int CalculatorScore(int points, int n);

    wxTimer* timer;
    wxStatusBar* status_scr;
    wxPanel *panel;
    bool started;
    bool paused;
    bool pieceFallingFinished;
    Piece current;
    Piece next;
    int curX;
    int curY;
    int score;
    int lvl;
    int TIMER_INTERVAL;
    PieceShape board[BoardWidth * BoardHeight];
};

#endif




