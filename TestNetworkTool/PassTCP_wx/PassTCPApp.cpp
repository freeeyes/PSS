/***************************************************************
 * Name:      PassTCPApp.cpp
 * Purpose:   Code for Application Class
 * Author:    smith ()
 * Created:   2019-02-19
 * Copyright: smith ()
 * License:
 **************************************************************/

#include "PassTCPApp.h"

//(*AppHeaders
#include "PassTCPDlg.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(PassTCPApp);

bool PassTCPApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();

    if ( wxsOK )
    {
        CPassTCPDlg* Frame = new CPassTCPDlg(0);
        Frame->Show();
        SetTopWindow(Frame);
    }

    //*)
    return wxsOK;

}
