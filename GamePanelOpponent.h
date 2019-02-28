//
// Created by nazar on 2/25/19.
//

#ifndef TETIS_GAMEPANELOPPONENT_H
#define TETIS_GAMEPANELOPPONENT_H


#include "Board.h"


class GamePanelOpponent : public Board{
public:
    GamePanelOpponent(wxPanel* parent_t, wxFrame *fr);
    void SetMovement(char c);

protected:
    void OnPaint();

private:

    void DropDown();
    void DropOneLine();
    void PieceDropped();
    void RemoveFullLines();

};


#endif //TETIS_GAMEPANELOPPONENT_H
