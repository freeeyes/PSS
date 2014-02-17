
// PurenessLineDlg.h : 头文件
//

#pragma once

#include "ChartViewer.h"
#include <stdlib.h>
#include <time.h>
#include "afxwin.h"
#include <afxsock.h>

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

// CPurenessLineDlg 对话框
class CPurenessLineDlg : public CDialog
{
// 构造
public:
	CPurenessLineDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PURENESSLINE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg void OnViewPortChanged();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

	CEdit m_txtServerIP;
	CEdit m_txtServerPort;
	afx_msg void OnBnClickedButton1();

private:
	void shiftData(double *data, int len, double newValue);
	void drawChart();
	void InitView();
	BOOL GetServerStateInfo(_ServerRunInfo& objServerRunInfo);

private:
	CChartViewer m_cvCPU;                     //CPU使用量
	CChartViewer m_cvMemory;                  //内存使用量
	double       m_dbCPU[MAX_DATA_COUNT];
	double       m_dbMemory[MAX_DATA_COUNT];
	CSocket      m_sckServer;                 //链接远程Server服务器
	bool         m_blState;                   //链接状态

};
