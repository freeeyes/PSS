/*
** Name:        randomwave.h
** Purpose:     A Random series generator using the random walk method
** Copyright:   (C) 2016, Advanced Software Engineering Limited (QT based implementation)
**              (C) 2018, Ulrich Telle (Adjustments for wxWidgets)
**
**              You may use and modify the code in this file in your application,
**              provided the code and its modifications are used only in conjunction
**              with ChartDirector. Usage of this software is subjected to the terms
**              and condition of the ChartDirector license.
*/

#ifndef RANDOMWAVE_HDR
#define RANDOMWAVE_HDR

#include <wx/thread.h>

//
// A Random series generator using the random walk method
//

class RandomWave : public wxThread
{
public:
  typedef void DataHandler(void *param, double elapsedTime, double series0, double series1);

  RandomWave(DataHandler *handler, void *param);
  virtual ~RandomWave();

  // The thread of the random series generator
  virtual ExitCode Entry();

private :
  // Disable copying and assignment
  RandomWave& operator=(const RandomWave&);
  RandomWave(const RandomWave&);

  // The callback function to handle the generated data
	DataHandler* m_handler;
	void*        m_param;
};

#endif
