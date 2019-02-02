#include "InfoPanel.h"
#include <wx/stattext.h>


InfoPanel::InfoPanel(wxPanel * parent_t, wxFrame *fr)
        :wxPanel(parent_t, wxID_ANY, wxDefaultPosition, wxSize(320, 385), wxBORDER_SUNKEN)
{
    panel = parent_t;
    Width = 23;
    Height = 23;
    y_draw = 80;
    x_draw = 135;
    string_nextpeace = new wxStaticText(this, -1, wxString::Format(wxT("Next Peace")), wxPoint(105, 20));
    sl1 = new wxStaticLine(this, wxID_ANY, wxPoint(0, 40), wxSize(285,1));
    sl2 = new wxStaticLine(this, wxID_ANY, wxPoint(0, 170), wxSize(285,1));
    //string_score = new wxStaticText(this, -1, wxString::Format(wxT("Score: 0")), wxPoint(110, 190));
    string_score = new wxStaticText(this, -1, wxT("Score: 0"), wxPoint(100, 190));
}

void InfoPanel::DrawNextPeace(wxPaintDC& dc, int x, int y, PieceShape pieceShape) {

    wxPen lightPen(light[int(pieceShape)]);
    lightPen.SetCap(wxCAP_PROJECTING);
    dc.SetPen(lightPen);

    dc.DrawLine(x, y + Height - 1, x, y);
    dc.DrawLine(x, y, x + Width - 1, y);

    wxPen darkPen(dark[int(pieceShape)]);
    darkPen.SetCap(wxCAP_PROJECTING);
    dc.SetPen(darkPen);

    dc.DrawLine(x + 1, y + Height - 1, x + Width - 1, y + Height - 1);
    dc.DrawLine(x + Width - 1, y + Height - 1, x + Width - 1, y + 1);

    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush(wxBrush(colors[int(pieceShape)]));

    dc.DrawRectangle(x + 1, y + 1, Width - 2, Height - 2);
}

void InfoPanel::ChangePeace() {
    wxPaintDC dc(this);
    for (int i = 0; i < 4; i++)
    {
        int x = this->piece.x(i);
        int y = this->piece.y(i);
        DrawNextPeace(dc, x * Width + x_draw, y_draw + y*Height, this->piece.GetShape());
    }
}

void InfoPanel::ClearPeace() {
    wxPaintDC dc(this);
    for (int i = 0; i < 13; i++) {
        DrawNextPeace(dc, clearcoord[i][0] * Width + x_draw, y_draw + clearcoord[i][1]*Height, this->piece.GetShape());
    }
}

void InfoPanel::DrawScore(int score) {
    this->string_score = new wxStaticText(this, -1, wxString::Format(wxT("Score: %d"), score), wxPoint(105, 190));
}