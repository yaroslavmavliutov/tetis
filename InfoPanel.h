#ifndef TETIS_INFOPANEL_H
#define TETIS_INFOPANEL_H

#include "Piece.h"
#include "GamePanel.h"

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/statline.h>
#include "wx/socket.h"


const int clearcoord[13][2] = {
        {0,  0}, {0,  -1}, {-1, 0}, {-1, 1},
        {1,  0}, {1,  1}, {0,  -1}, {0,  1},
        {0,  2}, {-1, -1}, {1,  -1}, { -1,  2},
        {1,  2}
};

class InfoPanel : public wxPanel {
public:
    InfoPanel(wxPanel *parent_t, wxFrame *fr, int nb_opponent);
    void ChangePeace();
    void ClearPeace();
    //void DrawScore(int score);
    void DrawOpponents(std::string Name, int score);
    Piece piece;

    wxStaticText *strings_score[5];


private:
    void DrawNextPeace(wxPaintDC& dc, int x, int y, PieceShape shape);

    wxPanel *panel;
    wxStaticLine *sl1;
    wxStaticLine *sl2;
    wxStaticText *string_nextpeace;

    int Width;
    int Height;
    int y_draw;
    int x_draw;

    int count_of_opponents;
    //std::map<std::string, int> my_opponents;
};


#endif //TETIS_INFOPANEL_H
