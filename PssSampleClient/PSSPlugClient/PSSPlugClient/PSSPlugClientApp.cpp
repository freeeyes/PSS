/***************************************************************
 * Name:      PSSPlugClientApp.cpp
 * Purpose:   Code for Application Class
 * Author:    smith ()
 * Created:   2019-02-22
 * Copyright: smith ()
 * License:
 **************************************************************/

#include "PSSPlugClientApp.h"

//(*AppHeaders
#include "PSSPlugClientMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(PSSPlugClientApp);

bool PSSPlugClientApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	PSSPlugClientFrame* Frame = new PSSPlugClientFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
