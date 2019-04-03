/***************************************************************
 * Name:      PSSProxyClientMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    smith ()
 * Created:   2019-02-22
 * Copyright: smith ()
 * License:
 **************************************************************/

#include "PSSProxyClientMain.h"
#include <wx/msgdlg.h>

//(*InternalHeaders(PSSProxyClientFrame)
#include <wx/bitmap.h>
#include <wx/icon.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(PSSProxyClientFrame)
const long PSSProxyClientFrame::ID_STATICBOX1 = wxNewId();
const long PSSProxyClientFrame::ID_STATICTEXT1 = wxNewId();
const long PSSProxyClientFrame::ID_STATICTEXT2 = wxNewId();
const long PSSProxyClientFrame::ID_STATICTEXT3 = wxNewId();
const long PSSProxyClientFrame::ID_TEXTCTRL1 = wxNewId();
const long PSSProxyClientFrame::ID_TEXTCTRL2 = wxNewId();
const long PSSProxyClientFrame::ID_TEXTCTRL3 = wxNewId();
const long PSSProxyClientFrame::ID_BUTTON1 = wxNewId();
const long PSSProxyClientFrame::ID_PANEL1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(PSSProxyClientFrame,wxFrame)
    //(*EventTable(PSSProxyClientFrame)
    //*)
END_EVENT_TABLE()

PSSProxyClientFrame::PSSProxyClientFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(PSSProxyClientFrame)
    Create(parent, id, wxT("PSS代理测试客户端"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("id"));
    SetClientSize(wxSize(532,410));
    {
    	wxIcon FrameIcon;
    	FrameIcon.CopyFromBitmap(wxBitmap(wxImage(_T("./Icon.jpg"))));
    	SetIcon(FrameIcon);
    }
    Panel1 = new wxPanel(this, ID_PANEL1, wxPoint(144,96), wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    StaticBox1 = new wxStaticBox(Panel1, ID_STATICBOX1, wxT("代理测试"), wxPoint(8,8), wxSize(520,168), 0, _T("ID_STATICBOX1"));
    StaticText1 = new wxStaticText(Panel1, ID_STATICTEXT1, wxT("测试IP"), wxPoint(32,40), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    StaticText2 = new wxStaticText(Panel1, ID_STATICTEXT2, wxT("测试端口"), wxPoint(256,40), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    StaticText3 = new wxStaticText(Panel1, ID_STATICTEXT3, wxT("测试代码"), wxPoint(32,88), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    TextCtrl_ServerIP = new wxTextCtrl(Panel1, ID_TEXTCTRL1, wxT("127.0.0.1"), wxPoint(88,32), wxSize(160,35), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    TextCtrl_ServerIP->SetMaxLength(16);
    TextCtrl_ServerPort = new wxTextCtrl(Panel1, ID_TEXTCTRL2, wxT("10002"), wxPoint(328,32), wxSize(-1,35), 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
    TextCtrl_SendData = new wxTextCtrl(Panel1, ID_TEXTCTRL3, wxT("01 00 30 10 08 00 00 00 46 45 45 45 45 59 45 53 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 be cd aa 8f 3c 01 00 00"), wxPoint(88,80), wxSize(432,35), 0, wxDefaultValidator, _T("ID_TEXTCTRL3"));
    Button_StartTest = new wxButton(Panel1, ID_BUTTON1, wxT("开始测试短连接"), wxPoint(32,128), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));

    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSSProxyClientFrame::OnButton_StartTestClick);
    //*)


}

PSSProxyClientFrame::~PSSProxyClientFrame()
{
    //(*Destroy(PSSProxyClientFrame)
    //*)
    delete pNetOperation;
}

void PSSProxyClientFrame::OnQuit(wxCommandEvent& event)
{
    pNetOperation->Close();
}

void PSSProxyClientFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, wxT("Welcome to..."));
}


void PSSProxyClientFrame::Send_Data( const char* pSendBuff, int nLen )
{
	int nTotalSendLen = nLen;
	int nBeginSend    = 0;
	int nCurrSendLen  = 0;
	bool blSendFlag   = false;
	int nBeginRecv    = 0;
	int nCurrRecvLen  = 0;
	bool blRecvFlag   = false;
    
    
    _LoginInfo objLoginInfo;
    const   char * CmdPacket    =(const char*)pSendBuff + nBeginSend;   
    pNetOperation->SendPacket(objLoginInfo,CmdPacket,nTotalSendLen);

    int RetPacketLength =30;
    char *Cmd_RevBuff	=new char[RetPacketLength];
    pNetOperation->RecvPacket(objLoginInfo,Cmd_RevBuff,RetPacketLength);   //接收报文
    
	int nRecvLength = 0;
    memcpy(&nRecvLength,  &Cmd_RevBuff[4], sizeof(int));
	if (Cmd_RevBuff)	delete Cmd_RevBuff;
        
	char *pRecvBuff = new char[nRecvLength];// = { '\0' };
    pNetOperation->RecvPacket(objLoginInfo,pRecvBuff,nRecvLength);   //接收报文
	if (pRecvBuff)	delete pRecvBuff;

    wxMessageBox(wxT("接收数据成功"), wxT("提示信息"), wxOK | wxICON_INFORMATION, this);

}


void PSSProxyClientFrame::OnButton_StartTestClick(wxCommandEvent& event)
{

// TODO: 在此添加控件通知处理程序代码

        std::string    strServerIP    =std::string(this->TextCtrl_ServerIP->GetValue());
        int nPort = wxAtoi(this->TextCtrl_ServerPort->GetValue());

        if(strServerIP == wxT("") || nPort == 0)
        {
            wxString msg = wxT("请输入完整的服务器IP地址和端口信息。");
            wxMessageBox(msg, wxT("服务器信息"), wxOK | wxICON_INFORMATION, this);
            return;
        }

        std::string    strSendData    =std::string(this->TextCtrl_SendData->GetValue());
        if(strSendData == wxT("") )
        {
            wxMessageBox(wxT("测试数据不能为空。"), wxT("测试数据"), wxOK | wxICON_INFORMATION, this);
            return;
        };


        int nDecLen = strSendData.length();
        char* pSendData = new char[nDecLen];
        //nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, pSendData, nBufferSize, NULL,NULL);
        memcpy(pSendData,strSendData.c_str(),strSendData.length());
        pSendData[nDecLen] = '\0';


	CConvertBuffer objConvertBuffer;
	//获得要转换的数据块大小
	int nBuffSize = objConvertBuffer.GetBufferSize(pSendData, nDecLen);
	char* pSendBuff = new char[nBuffSize];
	//将数据串转换成二进制串
	objConvertBuffer.Convertstr2charArray(pSendData, strlen(pSendData), (unsigned char*)pSendBuff, nBuffSize);
	delete[] pSendData;



        //开始发送数据
    if(strServerIP == wxT("") || nPort == 0)
    {
        wxMessageBox( wxT("请输入完整的服务器IP地址和端口信息。"), wxT("错误信息"), wxOK | wxICON_INFORMATION, this);
        return  ;
    }

	if(pNetOperation->Conn(strServerIP,nPort,1000) == false)
	{
        wxMessageBox(wxT("连接远程服务器失败。"), wxT("错误信息"), wxOK | wxICON_INFORMATION, this);
        return  ;
	}

	Send_Data(pSendBuff, nBuffSize);

	Close();

	delete[] pSendBuff;

}
