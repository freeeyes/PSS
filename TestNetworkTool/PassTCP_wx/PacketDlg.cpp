#include "PacketDlg.h"
#include "tinyxml.h"


//(*InternalHeaders(CPacketDlg)
#include <wx/intl.h>
#include <wx/string.h>
//*)
#include <wx/msgdlg.h>
#include <wx/string.h>

#define PACKETDATA_FILENAME "PacketData.xml"


//(*IdInit(CPacketDlg)
const long CPacketDlg::ID_STATICBOX1 = wxNewId();
const long CPacketDlg::ID_STATICBOX2 = wxNewId();
const long CPacketDlg::ID_STATICTEXT2 = wxNewId();
const long CPacketDlg::ID_TEXTCTRL_PacketData = wxNewId();
const long CPacketDlg::ID_BUTTON_AddPacket = wxNewId();
const long CPacketDlg::ID_RADIOBOX_PacketType = wxNewId();
const long CPacketDlg::ID_STATICTEXT3 = wxNewId();
const long CPacketDlg::ID_TEXTCTRL_RecvLength = wxNewId();
const long CPacketDlg::ID_BUTTON_SavePacketToFile = wxNewId();
const long CPacketDlg::ID_BUTTON_LoadPacketFile = wxNewId();
const long CPacketDlg::ID_BUTTON4 = wxNewId();
const long CPacketDlg::ID_BUTTON_PreviewSendResult = wxNewId();
const long CPacketDlg::ID_BUTTON_CloseDialog = wxNewId();
const long CPacketDlg::ID_LISTBOX_PacketList = wxNewId();
const long CPacketDlg::ID_LISTBOX_PacketPreviewList = wxNewId();
const long CPacketDlg::ID_PANEL1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(CPacketDlg,wxDialog)
    //(*EventTable(CPacketDlg)
    //*)
END_EVENT_TABLE()

CPacketDlg::CPacketDlg(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(CPacketDlg)
    Create(parent, wxID_ANY, wxT("顺序数据包设置"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
    SetClientSize(wxSize(581,389));
    Panel1 = new wxPanel(this, ID_PANEL1, wxPoint(0,0), wxSize(584,400), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    StaticBox1 = new wxStaticBox(Panel1, ID_STATICBOX1, wxT("添加数据包"), wxPoint(0,0), wxSize(576,224), 0, _T("ID_STATICBOX1"));
    StaticBox2 = new wxStaticBox(Panel1, ID_STATICBOX2, wxT("压测数据包信息"), wxPoint(0,224), wxSize(576,168), 0, _T("ID_STATICBOX2"));
    StaticText1 = new wxStaticText(Panel1, wxID_ANY, wxT("NOTE：添加数据包，其中随机数字的部分请用%d来替代，目前只支持文本协议，未来考虑二进制协议的支持，所有的回车换行请用/r/n明文写出，程序会 自动替换这这些字符。二进制数据包暂不支持随机替换.\\\\r\\\n如果发送文本数据包样例格式为:GET / HTTP/1.1\\\\\\\\r\\\\\\\nAccept: */*\\\\\\\\r\\\\\\\nAccept-Language: zh-cn\\\\\\\\r\\\\\\\n\\\\\\\\r\\\\\\\n\\\\r\\\n文本数据包格式为:0a 00 00 00 00 10 be cd aa 8f 3c 01 00 00"), wxPoint(16,24), wxSize(544,112), 0, _T("wxID_ANY"));
    StaticText2 = new wxStaticText(Panel1, ID_STATICTEXT2, wxT("数据包"), wxPoint(16,160), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    TextCtrl_PacketData = new wxTextCtrl(Panel1, ID_TEXTCTRL_PacketData, wxEmptyString, wxPoint(72,144), wxSize(488,48), 0, wxDefaultValidator, _T("ID_TEXTCTRL_PacketData"));
    Button_AddPacket = new wxButton(Panel1, ID_BUTTON_AddPacket, wxT("添加数据包"), wxPoint(24,190), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON_AddPacket"));
    wxString __wxRadioBoxChoices_1[2] =
    {
        wxT("二进制数据包"),
        wxT("文本数据包")
    };
    RadioBox_PacketType = new wxRadioBox(Panel1, ID_RADIOBOX_PacketType, wxEmptyString, wxPoint(128,178), wxDefaultSize, 2, __wxRadioBoxChoices_1, 1, wxRA_VERTICAL, wxDefaultValidator, _T("ID_RADIOBOX_PacketType"));
    RadioBox_PacketType->SetSelection(0);
    StaticText3 = new wxStaticText(Panel1, ID_STATICTEXT3, wxT("接收字节数"), wxPoint(384,195), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    TextCtrl_RecvLength = new wxTextCtrl(Panel1, ID_TEXTCTRL_RecvLength, wxT("0"), wxPoint(448,192), wxSize(120,25), 0, wxDefaultValidator, _T("ID_TEXTCTRL_RecvLength"));
    Button_SavePacketToFile = new wxButton(Panel1, ID_BUTTON_SavePacketToFile, wxT("保存成文件"), wxPoint(16,352), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON_SavePacketToFile"));
    Button_LoadPacketFile = new wxButton(Panel1, ID_BUTTON_LoadPacketFile, wxT("打开文件"), wxPoint(96,352), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON_LoadPacketFile"));
    Button_DeletePacketItem = new wxButton(Panel1, ID_BUTTON4, wxT("删除条目"), wxPoint(184,352), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON4"));
    Button_PreviewSendResult = new wxButton(Panel1, ID_BUTTON_PreviewSendResult, wxT("预览发送结果"), wxPoint(280,352), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON_PreviewSendResult"));
    Button_CloseDialog = new wxButton(Panel1, ID_BUTTON_CloseDialog, wxT("设置完成"), wxPoint(480,352), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON_CloseDialog"));
    ListBox_PacketList = new wxListBox(Panel1, ID_LISTBOX_PacketList, wxPoint(16,248), wxSize(256,104), 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX_PacketList"));
    ListBox_PacketPreviewList = new wxListBox(Panel1, ID_LISTBOX_PacketPreviewList, wxPoint(280,248), wxSize(288,104), 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX_PacketPreviewList"));

    Connect(ID_BUTTON_AddPacket,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CPacketDlg::OnButton_AddPacketClick);
    Connect(ID_BUTTON_SavePacketToFile,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CPacketDlg::OnButton_SavePacketToFileClick);
    Connect(ID_BUTTON_LoadPacketFile,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CPacketDlg::OnButton_LoadPacketFileClick);
    Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CPacketDlg::OnButton_DeletePacketItemClick);
    Connect(ID_BUTTON_PreviewSendResult,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CPacketDlg::OnButton_PreviewSendResultClick);
    Connect(ID_BUTTON_CloseDialog,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CPacketDlg::OnButton_CloseDialogClick);
    Panel1->Connect(wxEVT_PAINT,(wxObjectEventFunction)&CPacketDlg::OnPanel1Paint,0,this);
    //*)
}

CPacketDlg::~CPacketDlg()
{
    //(*Destroy(CPacketDlg)
    //*)
}


void CPacketDlg::OnPanel1Paint(wxPaintEvent& event)
{
}

void CPacketDlg::OnButton_AddPacketClick(wxCommandEvent& event)
{
    //添加数据包

    if(m_pBaseDataLogic == NULL)
    {
        //MessageBox(_T("必须指定m_pBaseDataLogic指针，指针不能为空。"));
        wxString msg    =wxT("必须指定m_pBaseDataLogic指针，指针不能为空。");
        wxMessageBox(msg, this->GetTitle(), wxOK | wxICON_INFORMATION, this);

        return;
    }

    //获得接收字节数
    wxString    strData =this->TextCtrl_RecvLength->GetValue();
    int nRecvLength = wxAtoi(strData);

    if(nRecvLength <= 0)
    {
        //MessageBox(_T("必须指定接收数据包的长度，否则无法发送。"));
        wxString msg    =wxT("必须指定接收数据包的长度，否则无法发送。");
        wxMessageBox(msg, this->GetTitle(), wxOK | wxICON_INFORMATION, this);

        return;
    }


    //wxString strData;
    char szPacketData[MAX_RANDOM_PACKET] = {'\0'};
    std::string  PacketStrData =std::string(this->TextCtrl_PacketData->GetValue().ToAscii());
    ///m_txtPacketData.GetWindowText(strData);

    //int nSrcLen = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
    //int nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, szPacketData, MAX_RANDOM_PACKET, NULL,NULL);

    strncpy(szPacketData, (const char*)PacketStrData.c_str(), MAX_RANDOM_PACKET);
    int nDecLen =PacketStrData.length();
    szPacketData[nDecLen] = '\0';



    if  ( this->RadioBox_PacketType->GetSelection() == 0)
    {
        //二进制模式2
        m_pBaseDataLogic->InsertRandomPacket(szPacketData, nDecLen, nRecvLength, 2);
    }
    else
    {
        //文本模式1
        string strFormatData = (string)szPacketData;

        //替换\r\n
        string_replace(strFormatData, "\\r", "\r");
        string_replace(strFormatData, "\\n", "\n");

        m_pBaseDataLogic->InsertRandomPacket(strFormatData.c_str(), strFormatData.length(), nRecvLength, 1);
    }

    ShowPacketList();
}

void CPacketDlg::OnButton_SavePacketToFileClick(wxCommandEvent& event)
{
    //保存成XML文件
    FILE* pFile = NULL;

    char szFileName[20];
    sprintf(szFileName, PACKETDATA_FILENAME);
    pFile   =fopen( szFileName, "w");

    if(pFile == NULL)
    {
        //MessageBox(_T("导出顺序数据包出错，文件不存在"), _T("提示信息"), MB_OK);
        wxString msg    =wxT("导出顺序数据包出错，文件不存在");
        wxMessageBox(msg, this->GetTitle(), wxOK | wxICON_INFORMATION, this);

        return;
    }

    char szLogText[MAX_RANDOM_PACKET] = {'\0'};

    sprintf(szLogText,  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);

    sprintf(szLogText,  "<config>\n");
    fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);

    for(int i = 0; i < m_pBaseDataLogic->GetRandomPacketCount(); i++)
    {
        _RandomPacketInfo* pRandomPacketInfo = m_pBaseDataLogic->GettRandomPacket(i);
        sprintf(szLogText,  "<PacketData Len=\"%d\" Type=\"%d\" RecdvLength=\"%d\"><![CDATA[%s]]></PacketData>\n"
                ,pRandomPacketInfo->nLen
                ,pRandomPacketInfo->nType
                ,pRandomPacketInfo->nRecdvLength
                ,pRandomPacketInfo->szPacket);

        fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);
    }

    sprintf(szLogText,  "</config>\n");
    fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);

    fclose(pFile);

    //MessageBox(_T("导出顺序数据包成功"), _T("提示信息"), MB_OK);
    wxMessageBox(wxT("导出顺序数据包成功"), this->GetTitle(), wxOK | wxICON_INFORMATION, this);




}

void CPacketDlg::OnButton_LoadPacketFileClick(wxCommandEvent& event)
{
    m_pBaseDataLogic->ClearRandomPacket();

    //读取XML文件
    TiXmlDocument* pDocument = new TiXmlDocument(PACKETDATA_FILENAME); //tstl可以为文件路径或文件名
    pDocument->LoadFile();

    if(NULL == pDocument)
    {
        //MessageBox(_T("读取顺序数据包xml文件失败"), _T("提示信息"), MB_OK);
        wxMessageBox(wxT("读取顺序数据包xml文件失败"), this->GetTitle(), wxOK | wxICON_INFORMATION, this);
        return;
    }

    TiXmlElement* Root = pDocument->RootElement();//获取根节点<Particls>
    TiXmlElement* Particl = NULL;

    int nLen        = 0;
    int nType       = 0;
    int nRecvLength = 0;

    for(Particl = Root->FirstChildElement(); Particl != NULL; Particl = Particl->NextSiblingElement())
    {
        //得到文档内容
        const char* sztext = Particl->GetText();

        //得到文档属性
        TiXmlAttribute* pAddrAttr = Particl->FirstAttribute();

        if(strcmp(pAddrAttr->Name(), "Len") == 0)
        {
            nLen = atoi(pAddrAttr->Value());
        }
        else if(strcmp(pAddrAttr->Name(), "Type") == 0)
        {
            nType = atoi(pAddrAttr->Value());
        }
        else
        {
            nRecvLength = atoi(pAddrAttr->Value());
        }

        TiXmlAttribute* pAttr1 = pAddrAttr->Next();

        if(strcmp(pAttr1->Name(), "Len") == 0)
        {
            nLen = atoi(pAttr1->Value());
        }
        else if(strcmp(pAttr1->Name(), "Type") == 0)
        {
            nType = atoi(pAttr1->Value());
        }
        else
        {
            nRecvLength = atoi(pAttr1->Value());
        }

        TiXmlAttribute* pAttr2 = pAttr1->Next();

        if(strcmp(pAttr2->Name(), "Len") == 0)
        {
            nLen = atoi(pAttr2->Value());
        }
        else if(strcmp(pAttr2->Name(), "Type") == 0)
        {
            nType = atoi(pAttr2->Value());
        }
        else
        {
            nRecvLength = atoi(pAttr2->Value());
        }

        //添加类型
        m_pBaseDataLogic->InsertRandomPacket(sztext, nLen, nRecvLength, nType);
    }

    ShowPacketList();

    delete pDocument;

}

void CPacketDlg::OnButton_DeletePacketItemClick(wxCommandEvent& event)
{
    //删除选中的条目

    int nIndex =this->ListBox_PacketList->GetSelection();

    if(nIndex >= 0)
    {
        m_pBaseDataLogic->DeleteRandomPacket(nIndex);
    }
}

void CPacketDlg::OnButton_PreviewSendResultClick(wxCommandEvent& event)
{
    //预览数据包
    this->ListBox_PacketPreviewList->Clear();
    //m_lbPreviewList.ResetContent();
    int nRecvLength = 0;

    char szTemp[MAX_RANDOM_PACKET] = {'\0'};

    for(int i = 0; i < m_pBaseDataLogic->GetRandomPacketCount(); i++)
    {
        int nLen = MAX_RANDOM_PACKET;
        m_pBaseDataLogic->GetReview(i, szTemp, nLen, nRecvLength);

        if(0 < nLen)
        {
            //wxString    temp=wxString::FromAscii(szTemp);

            //wchar_t sszTemp[MAX_RANDOM_PACKET] = {'\0'};

            //int nSrcLen = MultiByteToWideChar(CP_ACP, 0, szTemp, -1, NULL, 0);
            //int nDecLen = MultiByteToWideChar(CP_ACP, 0, szTemp, -1, sszTemp, MAX_RANDOM_PACKET);
            //sszTemp[nDecLen] = '\0';

            this->ListBox_PacketPreviewList->Insert(wxString::FromAscii(szTemp),this->ListBox_PacketPreviewList->GetCount());
        }
    }

    this->Destroy();

}

void CPacketDlg::OnButton_CloseDialogClick(wxCommandEvent& event)
{
    this->Close();
}





void CPacketDlg::SetBaseDataLogic(CBaseDataLogic* pBaseDataLogic)
{
    m_pBaseDataLogic = pBaseDataLogic;
    m_pBaseDataLogic->ClearRandomPacket();
    m_pBaseDataLogic->InitSendSize(MAX_RANDOM_PACKET);
}


void CPacketDlg::ShowPacketList()
{
    //显示所有的数据包
    //m_lbRondomPacketList.ResetContent();
    this->ListBox_PacketPreviewList->Clear();

    if(m_pBaseDataLogic == NULL)
    {
        return;
    }

    for(int i = 0; i < m_pBaseDataLogic->GetRandomPacketCount(); i++)
    {
        _RandomPacketInfo* pRandomPacketInfo = m_pBaseDataLogic->GettRandomPacket(i);

        if(NULL != pRandomPacketInfo)
        {
            //wchar_t szTemp[MAX_RANDOM_PACKET] = {'\0'};

            //int nSrcLen = MultiByteToWideChar(CP_ACP, 0, pRandomPacketInfo->szPacket, -1, NULL, 0);
            //int nDecLen = MultiByteToWideChar(CP_ACP, 0, pRandomPacketInfo->szPacket, -1, szTemp, MAX_RANDOM_PACKET);
            //szTemp[nDecLen] = '\0';

            //m_lbRondomPacketList.AddString(szTemp);
            //this->ListBox_PacketPreviewList->Insert(szTemp,this->ListBox_PacketPreviewList->GetCount());

            this->ListBox_PacketPreviewList->Insert(wxString::FromAscii(pRandomPacketInfo->szPacket),this->ListBox_PacketPreviewList->GetCount());

        }
    }
}



