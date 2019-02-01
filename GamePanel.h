#ifndef _GAMEPANEL_HPP
#define _GAMEPANEL_HPP

#include "Piece.h"

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/statline.h>

const wxColour colors[] =
        {
                wxColour(211, 211, 211),       wxColour(204, 102, 102),
                wxColour(102, 204, 102), wxColour(102, 102, 204),
                wxColour(204, 204, 102), wxColour(204, 102, 204),
                wxColour(102, 204, 204), wxColour(218, 170, 0)
        };

const wxColour light[] =
        {
                wxColour(0, 0, 0),       wxColour(248, 159, 171),
                wxColour(121, 252, 121), wxColour(121, 121, 252),
                wxColour(252, 252, 121), wxColour(252, 121, 252),
                wxColour(121, 252, 252), wxColour(252, 198, 0)
        };

const wxColour dark[] =
        {
                wxColour(0, 0, 0),      wxColour(128, 59, 59),
                wxColour(59, 128, 59),  wxColour(59, 59, 128),
                wxColour(128, 128, 59), wxColour(128, 59, 128),
                wxColour(59, 128, 128), wxColour(128, 98, 0)
        };

class GamePanel : public wxPanel
{
public:
    GamePanel(wxPanel* parent_t, wxFrame *fr);
    void Start();
    void Pause();

    PieceShape GetNextShape() const { return next.GetShape(); }

    /* wx event handlers */
protected:
    void OnPaint(wxPaintEvent& event);
    void OnKeyDown(wxKeyEvent& event);
    void OnTimer(wxCommandEvent& event);

private:
    /* Little hack to predefine the size */
    enum
    {
        BoardWidth = 11,
        BoardHeight = 22
    };

    PieceShape& PieceAt(int x, int y) { return board[y * BoardWidth + x]; }
    int Width() { return GetClientSize().GetWidth() / BoardWidth; }
    int Height() { return GetClientSize().GetHeight() / BoardHeight; }
    static bool InBounds(int x, int y) { return x >= 0 && x < BoardWidth && y >= 0 && y < BoardHeight; }
    void Clear();
    void DropCurrentToBottom();
    void DropCurrentOneLine();
    void PieceHitBottom();
    void ClearFullLines();
    void MakeNewPiece();
    void RandomPiece();
    bool DoMove(const Piece& piece, int newX, int newY);
    void DrawPieceSquare(wxPaintDC& dc, int x, int y, PieceShape shape);
    int CalculatorScore(int points, int n);

    wxTimer* timer;
    wxStatusBar* status_scr;
    bool started;
    bool paused;
    bool pieceDoneFalling;
    Piece current;
    Piece next;
    int curX;
    int curY;
    int score;
    int lvl;
    wxPanel *panel;
    int TIMER_INTERVAL;
    PieceShape board[BoardWidth * BoardHeight];
};

#endif




