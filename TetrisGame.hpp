#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/statline.h>


/*
#ifndef TETRIS_H
#define TETRIS_H
class TetrisGame : public wxPanel
{
public:
   TetrisGame(wxPanel *parent, wxFrame *fr);
    wxPanel *t_parent;
};
#endif*/

#ifndef _BOARD_HPP
#define _BOARD_HPP

#include "Piece.hpp"

#define TIMER_INTERVAL 500

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

const int clearcoord[13][2] = {
        {0,  0}, {0,  -1}, {-1, 0}, {-1, 1},
        {1,  0}, {1,  1}, {0,  -1}, {0,  1},
        {0,  2}, {-1, -1}, {1,  -1}, { -1,  2},
        {1,  2}
};

class Board : public wxPanel
{
public:
    Board(wxPanel* parent_t, wxFrame *fr);
    void Start();
    void Pause();
    void Reset();

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

    wxTimer* timer;
    wxStatusBar* statusBar;
    bool started;
    bool paused;
    bool pieceDoneFalling;
    Piece current;
    Piece next;
    int curX;
    int curY;
    int score;
    wxPanel *panel;
    PieceShape board[BoardWidth * BoardHeight];
};

#endif



#ifndef PANEL_H
#define PANEL_H
class RightPanel : public wxPanel {
public:
    RightPanel(wxPanel *parent_t, wxFrame *fr);
    void ChangePeace();
    void ClearPeace();
    void DrawNextPeace(wxPaintDC& dc, int x, int y, PieceShape shape);
    Piece piece;
    wxPanel *panel;
    wxStaticLine *sl1;
    wxStaticLine *sl2;
    wxStaticText *string_nextpeace;

    int Width;
    int Height;
    int BoardHeight;
    int BoardWidth;
    int top;
};

#endif

/*
#ifndef FRIENDS_H
#define FRIENDS_H
class Friends : public wxPanel {
public:
    Friends(wxPanel *parent_t, wxFrame *fr);
    //void OnSetText(wxCommandEvent & event);
    //wxStaticText *m_text;
};
#endif*/

