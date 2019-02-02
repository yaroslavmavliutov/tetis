#ifndef _MAIN_H
#define _MAIN_H

#include <wx/wx.h>
#include "Frame.h"
#include "client_server.h"

class TetrisApp : public wxApp
{
public:
    virtual bool OnInit();
};

#endif
