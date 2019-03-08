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
    	FrameIcon.CopyFromBitmap(wxBitmap(wxImage(_T("/data/develop/Lib/Network/PSS-master/Icon.jpg"))));
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


    InitializeSocketEnvironment();

    m_sckClient =new CSockWrap(SOCK_STREAM);



}

PSSProxyClientFrame::~PSSProxyClientFrame()
{
    //(*Destroy(PSSProxyClientFrame)
    //*)
}

void PSSProxyClientFrame::OnQuit(wxCommandEvent& event)
{
    if (m_sckClient != NULL)
    {
        m_sckClient->Close();
        delete m_sckClient;
    }
}

void PSSProxyClientFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, wxT("Welcome to..."));
}




bool PSSProxyClientFrame::Conn(const char* pIP, int nPort)
{

    std::string    strServerIP    =std::string(pIP);

    if(strServerIP == wxT("") || nPort == 0)
    {
        wxString msg = wxT("请输入完整的服务器IP地址和端口信息。");
        wxMessageBox(msg, wxT("错误信息"), wxOK | wxICON_INFORMATION, this);
        return  false;
    }

    m_sckClient->SetAddress(strServerIP.c_str(), nPort);
    m_sckClient->Reopen(true);
    if  (GetLastSocketError() !=0)
    {
        wxString msg = wxT("连接远程服务器失败。");
        wxMessageBox(msg, wxT("错误信息"), wxOK | wxICON_INFORMATION, this);
        return  false;
    }

	return true;

}

void PSSProxyClientFrame::Close()
{
	//closesocket(m_sckClient);
	//m_sckClient = INVALID_SOCKET;
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
	while(true)
	{
                //nCurrSendLen = send(m_sckClient, pSendBuff + nBeginSend, nTotalSendLen, 0);

                //int nSend = m_sckServer.Send(szSendDate, nAllLen + 4);
                transresult_t   rt  =m_sckClient->Send((void*)pSendBuff + nBeginSend, nTotalSendLen);
                nCurrSendLen    =rt.nbytes;

		if(nCurrSendLen <= 0)
		{
                        int dwError = GetLastSocketError();
                        wxMessageBox(wxT("远程服务器发送数据失败"), wxT("错误信息"), wxOK | wxICON_INFORMATION, this);

			return;
		}
		else
		{
			nTotalSendLen -= nCurrSendLen;
			if(nTotalSendLen == 0)
			{
				//发送完成
				blSendFlag = true;
				break;
			}
			else
			{
				nBeginSend += nCurrSendLen;
			}
		}
	}

	//先接收四字节的数据包长度
	char szRecvLength[30] = {'\0'};
        //nCurrRecvLen = recv(m_sckClient, (char* )szRecvLength, 30, 0);

      transresult_t  rt  =m_sckClient->Recv( (char* )szRecvLength, 30);
        nCurrRecvLen    =rt.nbytes;

	if(nCurrRecvLen != 30)
	{
                int dwError = GetLastSocketError();
                //MessageBox(_T("远程服务器接收数据失败"), _T("错误信息"), MB_OK);
                wxMessageBox(wxT("远程服务器接收数据失败"), wxT("错误信息"), wxOK | wxICON_INFORMATION, this);
		return;
	}

	int nRecvLength = 0;
        memcpy(&nRecvLength,  &szRecvLength[4], sizeof(int));
	char* pRecvBuff = new char[nRecvLength];
	int nRecvBegin  = 0;

	while(true)
	{
		if(nRecvLength - nRecvBegin == 0)
		{
			break;
		}

		//如果发送成功了，则处理接收数据
                //nCurrRecvLen = recv(m_sckClient, (char* )pRecvBuff + nRecvBegin, nRecvLength - nRecvBegin, 0);

                rt  =m_sckClient->Recv( (char* )pRecvBuff + nRecvBegin, nRecvLength - nRecvBegin);
                nCurrRecvLen    =rt.nbytes;
		if(nCurrRecvLen <= 0)
		{
                        int dwError = GetLastSocketError();
                        //MessageBox(_T("远程服务器发送数据失败"), _T("错误信息"), MB_OK);
                        wxMessageBox(wxT("远程服务器发送数据失败"), wxT("错误信息"), wxOK | wxICON_INFORMATION, this);
			return;
		}
		else
		{
			nRecvBegin += nCurrRecvLen;
		}
	}

        //MessageBox(_T("接收数据成功"), _T("提示信息"), MB_OK);
        wxMessageBox(wxT("接收数据成功"), wxT("提示信息"), wxOK | wxICON_INFORMATION, this);
	return;
}


void PSSProxyClientFrame::OnButton_StartTestClick(wxCommandEvent& event)
{

// TODO: 在此添加控件通知处理程序代码
//	CString strData;
//	m_txtServerIP.GetWindowText(strData);

//	char szServerIP[MAX_BUFF_20] = {'\0'};
//	int nSrcLen = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
//	int nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, szServerIP, MAX_BUFF_20, NULL,NULL);
//	szServerIP[nDecLen] = '\0';

//	m_txtServerPort.GetWindowText(strData);
//	int nServerPort = _ttoi((LPCTSTR)strData);

//	m_txtSendData.GetWindowText(strData);

//	nSrcLen = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
//	int nBufferSize = WideCharToMultiByte(CP_ACP, 0, strData, -1, NULL, 0, NULL, NULL);
//	char* pSendData = new char[nBufferSize];
//	nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, pSendData, nBufferSize, NULL,NULL);
//	pSendData[nDecLen] = '\0';

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
//	if(m_sckClient != INVALID_SOCKET)
//	{
//		Close();
//	}

        if(Conn(strServerIP.c_str(), nPort) == false)
	{
		return;
	}

	Send_Data(pSendBuff, nBuffSize);

	Close();

	delete[] pSendBuff;

}
