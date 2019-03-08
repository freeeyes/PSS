/***************************************************************
 * Name:      PSSProxyClientApp.cpp
 * Purpose:   Code for Application Class
 * Author:    smith ()
 * Created:   2019-02-22
 * Copyright: smith ()
 * License:
 **************************************************************/

#include "PSSProxyClientApp.h"

//(*AppHeaders
#include "PSSProxyClientMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(PSSProxyClientApp);

bool PSSProxyClientApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	PSSProxyClientFrame* Frame = new PSSProxyClientFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
