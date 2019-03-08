/***************************************************************
 * Name:      PurenessLineMain.h
 * Purpose:   Defines Application Frame
 * Author:    smith ()
 * Created:   2019-02-22
 * Copyright: smith ()
 * License:
 **************************************************************/

#ifndef PURENESSLINEMAIN_H
#define PURENESSLINEMAIN_H

//(*Headers(PurenessLineFrame)
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/timer.h>
//*)

#include "wxchartviewer.h"
#include "chartdir.h"

#include "sock_wrap.h"


const int sampleSize = 240;
static const int gs_dataInterval = 250;

//服务器运行时信息
struct _ServerRunInfo
{
        int m_nCPU;        //当前CPU占用率
        int m_nMemorySize; //当前内存占用率

        _ServerRunInfo()
        {
                m_nCPU        = 0;
                m_nMemorySize = 0;
        }

        _ServerRunInfo& operator = (const _ServerRunInfo& ar)
        {
                this->m_nCPU        = ar.m_nCPU;
                this->m_nMemorySize = ar.m_nMemorySize;
                return *this;
        }
};

//设置一个随机种子
inline void InitRandom()
{
        srand((int)time(NULL));
};

//从一个值域中获取一个随机值
inline int RandomValue(int nMin, int nMax)
{
        return  nMin + (int) (nMax * (rand() / (RAND_MAX + 1.0)));
};

#define TIMER_ID       1        //定时器ID
#define TIMER_INTERVAL 1000     //定时器间隔
#define MAX_DATA_COUNT 25       //数组最大上限

class PurenessLineFrame: public wxFrame
{
    public:

        PurenessLineFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~PurenessLineFrame();

    private:

        //(*Handlers(PurenessLineFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnButton_StartMonitorClick(wxCommandEvent& event);
        void OnButton_StopMonitorClick(wxCommandEvent& event);
        void OnTimer_dataRateTimerTrigger(wxTimerEvent& event);
        void OnTimer_chartUpdateTimerTrigger(wxTimerEvent& event);
        //*)

        //(*Identifiers(PurenessLineFrame)
        static const long ID_STATICBOX1;
        static const long ID_STATICBOX2;
        static const long ID_STATICTEXT1;
        static const long ID_TEXTCTRL1;
        static const long ID_STATICTEXT2;
        static const long ID_TEXTCTRL2;
        static const long ID_BUTTON1;
        static const long ID_BUTTON2;
        static const long ID_PANEL1;
        static const long ID_TIMER1;
        static const long ID_TIMER2;
        //*)
        static  const long ID_CHARTPANEL_CPU ;
        static  const long ID_CHARTPANEL_MEM ;

        //(*Declarations(PurenessLineFrame)
        wxButton* Button_StartMonitor;
        wxButton* Button_StopMonitor;
        wxPanel* Panel1;
        wxStaticBox* StaticBox1;
        wxStaticBox* StaticBox2;
        wxStaticText* StaticText1;
        wxStaticText* StaticText2;
        wxTextCtrl* TextCtrl_ServerIP;
        wxTextCtrl* TextCtrl_ServerPort;
        wxTimer Timer_chartUpdateTimer;
        wxTimer Timer_dataRateTimer;
        //*)




        DECLARE_EVENT_TABLE()


        void    OnViewPortChanged(wxCommandEvent& event);


private:
	void shiftData(double *data, int len, double newValue);
	void drawChart();
	void Init();
	bool GetServerStateInfo(_ServerRunInfo& objServerRunInfo);

private:
	wxChartViewer *m_cvCPU;                     //CPU使用量
	wxChartViewer *m_cvMemory;                  //内存使用量
	double       m_dbCPU[MAX_DATA_COUNT];
	double       m_dbMemory[MAX_DATA_COUNT];
	CSockWrap      *m_sckServer;                 //链接远程Server服务器
	bool         m_blState;                   //链接状态





       //   wxChartViewer* m_chartViewer_cpu;
    //      wxChartViewer* m_chartViewer_mem;





    private:



};

#endif // PURENESSLINEMAIN_H
