/***************************************************************
 * Name:      PssFtpClientApp.cpp
 * Purpose:   Code for Application Class
 * Author:    smith ()
 * Created:   2019-02-21
 * Copyright: smith ()
 * License:
 **************************************************************/

#include "PssFtpClientApp.h"

//(*AppHeaders
#include "PssFtpClientMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(PssFtpClientApp);

bool PssFtpClientApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	PssFtpClientFrame* Frame = new PssFtpClientFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
