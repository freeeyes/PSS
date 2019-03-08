/*
** Name:        wxdoublebufferedqueue.h
** Purpose:     Header only implementation of double buffered queue
** Copyright:   (C) 2016, Advanced Software Engineering Limited (QT based implementation)
**              (C) 2018, Ulrich Telle (Adjustments for wxWidgets)
**
**              You may use and modify the code in this file in your application,
**              provided the code and its modifications are used only in conjunction
**              with ChartDirector. Usage of this software is subjected to the terms
**              and condition of the ChartDirector license.
*/

#ifndef WXDOUBLEBUFFEREDQUEUE_HDR
#define WXDOUBLEBUFFEREDQUEUE_HDR

#include <wx/thread.h>

template <class T>
class wxDoubleBufferedQueue
{
public:
  // Constructor - construct a queue with the given buffer size
  wxDoubleBufferedQueue(int bufferSize = 10000)
    : m_bufferLen(0), m_bufferSize(bufferSize)
  { 
    m_buffer0 = m_buffer = new T[m_bufferSize]; 
    m_buffer1 = new T[m_bufferSize];
  }

  // Destructor
  ~wxDoubleBufferedQueue()
  {
    delete[] m_buffer0;
    delete[] m_buffer1;
  }

  // Add an item to the queue. Returns true if successful, false if the buffer is full.
  bool Put(const T& datum)
  {
    wxMutexLocker lock(m_mutex);
    bool canWrite = m_bufferLen < m_bufferSize;
    if (canWrite) m_buffer[m_bufferLen++] = datum;
    return canWrite;
  }

  // Get all the items in the queue. The T** argument should be the address of a variable to
  // receive the pointer to the item array. The return value is the size of the array.
  int Get(T** data)
  {
    wxMutexLocker lock(m_mutex);
    *data = m_buffer;
    int ret = m_bufferLen;
    m_buffer = (m_buffer == m_buffer0) ? m_buffer1 : m_buffer0;
    m_bufferLen = 0;
    return ret;
	}

private:
  // Disable copying and assignment
  wxDoubleBufferedQueue& operator=(const wxDoubleBufferedQueue&);
  wxDoubleBufferedQueue(const wxDoubleBufferedQueue&);

  T* m_buffer0;
  T* m_buffer1;
  T* m_buffer;
  int m_bufferLen;
  int m_bufferSize;
  wxMutex m_mutex;
};

#endif
