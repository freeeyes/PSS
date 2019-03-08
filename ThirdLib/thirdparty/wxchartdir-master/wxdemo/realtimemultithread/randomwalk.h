/*
** Name:        randomwalk.h
** Purpose:     A Random series generator using the random walk method
** Copyright:   (C) 2016, Advanced Software Engineering Limited (QT based implementation)
**              (C) 2018, Ulrich Telle (Adjustments for wxWidgets)
**
**              You may use and modify the code in this file in your application,
**              provided the code and its modifications are used only in conjunction
**              with ChartDirector. Usage of this software is subjected to the terms
**              and condition of the ChartDirector license.
*/

#ifndef RANDOMWALK_HDR
#define RANDOMWALK_HDR

#include <wx/thread.h>

// A Random series generator using the random walk method
class RandomWalk : public wxThread
{
public:
  typedef void DataHandler(void* param, double elapsedTime, double series0, double series1);

	RandomWalk(DataHandler* handler, void* param);
  virtual ~RandomWalk();

  // The thread of the random series generator
  virtual ExitCode Entry();

private :
  // Disable copying and assignment
  RandomWalk& operator=(const RandomWalk&);
  RandomWalk(const RandomWalk&);

  // Flag to stop the flag
  bool stopThread;

  // The callback function to handle the generated data
	DataHandler* m_handler;
	void*        m_param;
};

#endif
