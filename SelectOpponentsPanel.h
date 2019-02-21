//
// Created by yaroslav on 21.02.19.
//

#ifndef TETIS_SELECTOPPONENTSPANEL_H
#define TETIS_SELECTOPPONENTSPANEL_H

#include <wx/wx.h>
class SelectOpponentsPanel : public wxDialog
{
public:
    SelectOpponentsPanel(const wxString& title);

    int GetCountOpponents();
private:
    void Verification(wxCommandEvent& event);
    void Exit(wxCommandEvent& event);

    wxRadioButton *chice1;
    wxRadioButton *chice2;
    int count_of_opponents;
};

const int ID_OKEY = 100;
const int ID_CLOSE = 101;


#endif //TETIS_SELECTOPPONENTSPANEL_H
