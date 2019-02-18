//
// Created by yaroslav on 17.02.19.
//

#ifndef TETIS_STARTDIALOG_H
#define TETIS_STARTDIALOG_H

#include <wx/wx.h>
class StartDialog : public wxDialog
{
public:
    StartDialog(const wxString& title);

    void Verification(wxCommandEvent& event);
    std::string GetName();

    wxTextCtrl *input_name;
    wxString name;
    std::string namestr;
};

const int ID_OK = 100;



#endif //TETIS_STARTDIALOG_H
