//
// Created by nazar on 2/27/19.
//

#ifndef TETIS_BOARD_H
#define TETIS_BOARD_H

#include "Piece.h"

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/statline.h>


#include <cmath>
#include <string>

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


class Board : public wxPanel {

public:
        Board(wxPanel* parent_t, wxFrame *fr);
        PieceShape GetNextShape() const { return next.GetShape(); }
        void setNextOrCurrentPiece(char c, int current_next);

protected:
        enum { BoardWidth = 10, BoardHeight = 22 };
        PieceShape& PieceCheck(int x, int y) { return board[y * BoardWidth + x]; }

        int Width() { return GetClientSize().GetWidth() / BoardWidth; }
        int Height() { return GetClientSize().GetHeight() / BoardHeight; }
        static bool CheckBounds(int x, int y) { return x >= 0 && x < BoardWidth && y >= 0 && y < BoardHeight; }
        bool DoMove(const Piece& piece, int newX, int newY);
        void ClearBoard();
        void DrawPieceSquare(wxPaintDC& dc, int x, int y, PieceShape shape);
        int CalculatorScore(int points, int n);

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
        PieceShape board[BoardWidth * BoardHeight];

};

#endif //TETIS_BOARD_H
