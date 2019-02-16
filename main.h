#ifndef _MAIN_H
#define _MAIN_H

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/socket.h"

#include "Frame.h"

class TetrisApp : public wxApp
{
public:
    virtual bool OnInit();
};

wxBEGIN_EVENT_TABLE(Frame, wxFrame)
EVT_MENU(Minimal_Quit, Frame::OnQuit)
EVT_MENU(Minimal_About, Frame::OnAbout)
EVT_SOCKET(SOCKET_ID, Frame::OnSocketEvent)
EVT_MENU(CLIENT_OPEN, Frame::OnOpenConnection)
EVT_MENU(CLIENT_CLOSE, Frame::OnCloseConnection)
wxEND_EVENT_TABLE()

#endif
