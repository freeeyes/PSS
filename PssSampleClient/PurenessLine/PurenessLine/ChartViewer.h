///////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2012 Advanced Software Engineering Limited
//
// You may use and modify the code in this file in your application, provided the code and
// its modifications are used only in conjunction with ChartDirector. Usage of this software
// is subjected to the terms and condition of the ChartDirector license.
///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "chartdir.h"
#include <afxmt.h>

//
// Utility to convert from UTF8 string to MFC TCHAR string.
//
class UTF8toTCHAR
{
public :
	UTF8toTCHAR(const char *utf8_string) : t_string(0), needFree(false)
	{
		if (0 == utf8_string)
			t_string = 0;
		else if (0 == *utf8_string)
			t_string = _T("");
		else if ((sizeof(TCHAR) == sizeof(char)) && isPureAscii(utf8_string))
			// No conversion needed for pure ASCII text
			t_string = (TCHAR *)utf8_string;
		else
		{
			// Either TCHAR = Unicode (2 bytes), or utf8_string contains non-ASCII characters.
			// Needs conversion
			needFree = true;

			// Convert to Unicode (2 bytes)
			int string_len = (int)strlen(utf8_string);
			wchar_t *buffer = new wchar_t[string_len + 1];
			MultiByteToWideChar(CP_UTF8, 0, utf8_string, -1, buffer, string_len + 1);
			buffer[string_len] = 0;
					
#ifdef _UNICODE
			t_string = buffer;
#else
			// TCHAR is MBCS - need to convert back to MBCS
			t_string = new char[string_len * 2 + 2];
			WideCharToMultiByte(CP_ACP, 0, buffer, -1, t_string, string_len * 2 + 1, 0, 0);
			t_string[string_len * 2 + 1] = 0;
			delete[] buffer;
#endif
		}

	}

	operator const TCHAR*()
	{
		return t_string;
	}

	~UTF8toTCHAR()
	{
		if (needFree)
			delete[] t_string;
	}

private :
	TCHAR *t_string;
	bool needFree;

	//
	// helper utility to test if a string contains only ASCII characters
	//
	bool isPureAscii(const char *s)
	{
		while (*s != 0) { if (*(s++) & 0x80) return false; }
		return true;
	}

	//disable assignment
	UTF8toTCHAR(const UTF8toTCHAR &rhs);
	UTF8toTCHAR &operator=(const UTF8toTCHAR &rhs);
};

//
// Utility to convert from MFC TCHAR string to UTF8 string
//
class TCHARtoUTF8
{
public :
	TCHARtoUTF8(const TCHAR *t_string) : utf8_string(0), needFree(false)
	{
		if (0 == t_string)
			utf8_string = 0;
		else if (0 == *t_string)
			utf8_string = "";
		else if ((sizeof(TCHAR) == sizeof(char)) && isPureAscii((char *)t_string))
			// No conversion needed for pure ASCII text
			utf8_string = (char *)t_string;
		else
		{
			// TCHAR is non-ASCII. Needs conversion.
	
			needFree = true;
			int string_len = (int)_tcslen(t_string);

#ifndef _UNICODE
			// Convert to Unicode if not already in unicode.
			wchar_t *w_string = new wchar_t[string_len + 1];
			MultiByteToWideChar(CP_ACP, 0, t_string, -1, w_string, string_len + 1);
			w_string[string_len] = 0;
#else
			wchar_t *w_string = (wchar_t*)t_string;
#endif

			// Convert from Unicode (2 bytes) to UTF8
			utf8_string = new char[string_len * 3 + 1];
			WideCharToMultiByte(CP_UTF8, 0, w_string, -1, utf8_string, string_len * 3 + 1, 0, 0);
			utf8_string[string_len * 3] = 0;
					
			if (w_string != (wchar_t *)t_string)
				delete[] w_string;
		}

	}

	operator const char*()
	{
		return utf8_string;
	}

	~TCHARtoUTF8()
	{
		if (needFree)
			delete[] utf8_string;
	}

private :
	char *utf8_string;
	bool needFree;

	//
	// helper utility to test if a string contains only ASCII characters
	//
	bool isPureAscii(const char *s)
	{
		while (*s != 0) { if (*(s++) & 0x80) return false; }
		return true;
	}
	
	//disable assignment
	TCHARtoUTF8(const TCHARtoUTF8 &rhs);
	TCHARtoUTF8 &operator=(const TCHARtoUTF8 &rhs);
};

/////////////////////////////////////////////////////////////////////////////
// CRectCtrl window

//
// A rectangle with a background color. Use as thick edges for the selection
// rectangle.
//

class CRectCtrl : public CStatic
{
public:
// Public interface
	BOOL Create(CWnd* pParentWnd, COLORREF c);
	void SetColor(COLORREF c);

// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRectCtrl)
	//}}AFX_VIRTUAL

protected:
// Generated message map functions
	//{{AFX_MSG(CRectCtrl)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private :
	CBrush m_Color;
};

/////////////////////////////////////////////////////////////////////////////
// CChartViewer window

//
// Mouse usage mode constants
//
namespace Chart
{
	enum 
	{ 
		MouseUsageDefault = 0,
		MouseUsageScroll = 1,
		MouseUsageZoomIn = 3,
		MouseUsageZoomOut = 4
	};
}

//
// Event message ID
//
#define CVN_ViewPortChanged	1000			// View port has changed
#define CVN_MouseMoveChart 1001				// Mouse moves over the chart
#define CVN_MouseLeaveChart 1002			// Mouse leaves the chart
#define CVN_MouseMovePlotArea 1003			// Mouse moves over the extended plot area
#define CVN_MouseLeavePlotArea 1004			// Mouse leaves the extended plot area


class CChartViewer : public CStatic, public ViewPortManager
{
public:
	CChartViewer();

	//
	// CChartViewer properties
	//

	virtual void setChart(BaseChart *c);
	virtual BaseChart *getChart();

	virtual void setImageMap(const char *imageMap);
	virtual ImageMapHandler *getImageMapHandler();

	virtual void setDefaultToolTip(LPCTSTR text);
	virtual CToolTipCtrl *getToolTipCtrl();

	virtual void setSelectionBorderWidth(int width);
	virtual int getSelectionBorderWidth();

	virtual void setSelectionBorderColor(COLORREF c);
	virtual COLORREF getSelectionBorderColor();
	
	virtual void setMouseUsage(int mouseUsage);
	virtual int getMouseUsage();

	virtual void setZoomDirection(int direction);
	virtual int getZoomDirection();
	
	virtual void setScrollDirection(int direction);
	virtual int getScrollDirection();

	virtual void setZoomInRatio(double ratio);
	virtual double getZoomInRatio();

	virtual void setZoomOutRatio(double ratio);
	virtual double getZoomOutRatio();

	virtual void setMinimumDrag(int offset);
	virtual int getMinimumDrag();

	virtual void setUpdateInterval(int interval);
	virtual int getUpdateInterval();

	virtual bool needUpdateChart();
	virtual bool needUpdateImageMap();

	virtual bool isMouseOnPlotArea();
	virtual bool isInMouseMoveEvent();

	//
	// CChartViewer methods
	//

	// Trigger the ViewPortChanged event
	virtual void updateViewPort(bool needUpdateChart, bool needUpdateImageMap);
	
	// Request CChartViewer to redisplay the chart
	virtual void updateDisplay();

	// Set the message used to remove the dynamic layer
	virtual void removeDynamicLayer(int msg);
	
	// Get the mouse coordinates
	virtual int getChartMouseX();
	virtual int getChartMouseY();

	// Get the mouse coordinates bounded to the plot area
	virtual int getPlotAreaMouseX();
	virtual int getPlotAreaMouseY();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChartViewer)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

protected:

	// Generated message map functions
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDelayedMouseMove();
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

private:

	//
	// CChartViewer configurable properties
	//

	BaseChart *m_currentChart;			// Current BaseChart object
	ImageMapHandler *m_hotSpotTester;	// ImageMapHander representing the image map
	CString m_defaultToolTip;			// Default tool tip text
	CToolTipCtrl m_ToolTip;				// CToolTipCtrl for managing tool tips
	bool m_toolTipHasAttached;			// CToolTipCtrl has attached to CChartViewer
	COLORREF m_selectBoxLineColor;		// Selectiom box border color
	int m_selectBoxLineWidth;			// Selectiom box border width
	int m_mouseUsage;					// Mouse usage mode
	int m_zoomDirection;				// Zoom direction
	int m_scrollDirection;				// Scroll direction
	double m_zoomInRatio;				// Zoom in ratio
	double m_zoomOutRatio;				// Zoom out ratio
	int m_minDragAmount;				// Minimum drag amount
	int m_updateInterval;				// Minimum interval between chart updates
	bool m_needUpdateChart;				// Has pending chart update request
	bool m_needUpdateImageMap;			// Has pending image map udpate request

	//
	// Keep track of mouse states
	//

	int m_currentHotSpot;				// The hot spot under the mouse cursor.
	bool m_isClickable;					// Mouse is over a clickable hot spot.
	bool m_isOnPlotArea;				// Mouse is over the plot area.
	bool m_isPlotAreaMouseDown;			// Mouse left button is down in the plot area.
	bool m_isDragScrolling;				// Is drag scrolling the chart.
	bool m_isMouseTracking;				// Is tracking mouse leave event.
    bool m_isInMouseMove;				// Is in mouse moeve event handler

	//
	// Dragging support
	//

	int m_plotAreaMouseDownXPos;		// The starting x coordinate of the mouse drag.
	int m_plotAreaMouseDownYPos;		// The starting y coordinate of the mouse drag.
	bool isDrag(int direction, CPoint point);				// Check if mouse is dragging
	void OnPlotAreaMouseDrag(UINT nFlags, CPoint point);	// Process mouse dragging

	//
	// Selection rectangle
	//

	CRectCtrl m_LeftLine;				// Left edge of selection rectangle
	CRectCtrl m_RightLine;				// Right edge of selection rectangle
	CRectCtrl m_TopLine;				// Top edge of selection rectangle
	CRectCtrl m_BottomLine;				// Bottom edge of selection rectangle
	
	void initRect();					// Initialize selection rectangle edges
	void drawRect(int x, int y, int width, int height);		// Draw selection rectangle
	void setRectVisible(bool b);		// Show/hide selection rectangle

	//
	// Chart update rate control
	//

	bool m_holdTimerActive;				// Delay chart update to limit update frequency
	
	int m_delayUpdateChart;			// Delay chart update until the mouse event is completed
	BaseChart *m_delayedChart;
	void commitUpdateChart();			// Commit updating the chart.

	unsigned int m_lastMouseMove;		// The timestamp of the last mouse move event.
	bool m_hasDelayedMouseMove;			// Delay the mouse move event to allow other updates
	UINT m_delayedMouseMoveFlag;		// The mouse key flags of the delayed mouse move event.
	CPoint m_delayedMouseMovePoint;		// The mouse coordinates of the delayed mouse move event.
	void commitMouseMove(UINT nFlags, CPoint point);    // Raise the delayed mouse move event.

	//
	// Track Cursor support
	//
	int m_currentMouseX;				// Get the mouse x-pixel coordinate
	int m_currentMouseY;				// Get the mouse y-pixel coordinate
	int m_isInMouseMovePlotArea;		// flag to indicate if is in a mouse move plot area event.

	//
	// Dynamic Layer support
	//
	int m_autoHideMsg;					// The message that will trigger removing the dynamic layer.
	void applyAutoHide(int msg);		// Attempt to remove the dynamic layer with the given message.
};

