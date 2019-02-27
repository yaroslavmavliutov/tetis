#ifndef _GAMEPANEL_HPP
#define _GAMEPANEL_HPP

#include "Board.h"
#include "wx/socket.h"

class GamePanel : public Board
{

public:
    GamePanel(wxPanel* parent_t, wxFrame *fr, wxSocketClient *sock, int nb_opponent);
    void Start();
    void Pause();
    int nb_opponent;
    void SetMovement(char c);

protected:
    void OnPaint(wxPaintEvent& event);
    void OnKeyDown(wxKeyEvent& event);
    void OnTimer(wxCommandEvent& event);

private:
    wxTimer* timer;
    int TIMER_INTERVAL;
    wxSocketClient *sock;
    void sendShapeToServer(PieceShape ps, int next_or_curr);
    void sendMoveToServer(char c);
    void DropDown();
    void DropOneLine();
    void PieceDropped();
    void RemoveFullLines();
    void MakeNewPiece();
    void RandomPiece();
};


#endif




