/***************************************************************
 * Name:      PSSLoginClientApp.cpp
 * Purpose:   Code for Application Class
 * Author:    smith ()
 * Created:   2019-02-21
 * Copyright: smith ()
 * License:
 **************************************************************/

#include "PSSLoginClientApp.h"

//(*AppHeaders
#include "PSSLoginClientMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(PSSLoginClientApp);

bool PSSLoginClientApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	PSSLoginClientFrame* Frame = new PSSLoginClientFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
