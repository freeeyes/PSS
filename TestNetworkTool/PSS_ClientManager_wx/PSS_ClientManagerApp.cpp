/***************************************************************
 * Name:      PSS_ClientManagerApp.cpp
 * Purpose:   Code for Application Class
 * Author:    smith ()
 * Created:   2019-02-21
 * Copyright: smith ()
 * License:
 **************************************************************/

#include "PSS_ClientManagerApp.h"

//(*AppHeaders
#include "PSS_ClientManagerMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(PSS_ClientManagerApp);

bool PSS_ClientManagerApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();

    if ( wxsOK )
    {
        PSS_ClientManagerFrame* Frame = new PSS_ClientManagerFrame(0);
        Frame->Show();
        SetTopWindow(Frame);
    }

    //*)
    return wxsOK;

}
