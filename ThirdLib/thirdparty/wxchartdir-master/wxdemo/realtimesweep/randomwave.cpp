/*
** Name:        randomwave.cpp
** Purpose:     A Random series generator using the random walk method
** Copyright:   (C) 2016, Advanced Software Engineering Limited (QT based implementation)
**              (C) 2018, Ulrich Telle (Adjustments for wxWidgets)
**
**              You may use and modify the code in this file in your application,
**              provided the code and its modifications are used only in conjunction
**              with ChartDirector. Usage of this software is subjected to the terms
**              and condition of the ChartDirector license.
*/

#include <wx/stopwatch.h>

#include "randomwave.h"
#include <math.h>

// The period of the data series in milliseconds. This random series implementation just use the 
// windows timer for timing. In many computers, the default windows timer resolution is 1/64 sec,
// or 15.6ms. This means the interval may not be exactly accurate.
const int interval = 100;

// Constructor. The handler is the function to call then new data are generated. The "param" is a
// parameter for passing to the handler function.
RandomWave::RandomWave(RandomWave::DataHandler* handler, void* param)
  : m_handler(handler), m_param(param)
{
}

// Destructor
RandomWave::~RandomWave()
{
}

// The random generator thread
wxThread::ExitCode
RandomWave::Entry()
{
  long currentTime = 0;
  long nextTime = 0;
	
  // Variables to keep track of the timing
  wxStopWatch timer;
  timer.Start();

  while (!TestDestroy())
	{
    // Compute the next data value
    currentTime = timer.Time();
    double p = currentTime / 1000.0 * 4;
    double series0 = 20 + cos(p * 2.2) * 10 + 1 / (cos(p) * cos(p) + 0.01);
    double series1 = 210 + 60 * sin(p / 21.7) * sin(p / 7.8);

    // Call the handler
    m_handler(m_param, currentTime / 1000.0, series0, series1);

    // Sleep until next walk
    if ((nextTime += interval) <= currentTime)
    {
      nextTime = currentTime + interval;
    }

    Sleep((unsigned long)(nextTime - currentTime));
	}

  return (wxThread::ExitCode) 0;     // success
}
