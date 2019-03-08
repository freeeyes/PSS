/***************************************************************
 * Name:      PurenessLineApp.cpp
 * Purpose:   Code for Application Class
 * Author:    smith ()
 * Created:   2019-02-22
 * Copyright: smith ()
 * License:
 **************************************************************/

#include "PurenessLineApp.h"

//(*AppHeaders
#include "PurenessLineMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(PurenessLineApp);

bool PurenessLineApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	PurenessLineFrame* Frame = new PurenessLineFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
