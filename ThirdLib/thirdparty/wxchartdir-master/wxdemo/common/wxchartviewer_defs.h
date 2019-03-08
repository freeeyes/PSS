/*
** Name:        wxchartviewer_defs.h
** Purpose:     wxChartViewer defines
** Author:      Ulrich Telle
** Created:     2018-07-09
** Copyright:   (C) 2018, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

/// \file wxchartviewer_defs.h Compile time switches for the \b wxChartViewer component

#ifndef WXCHARTDIR_WXCHARTVIEWER_DEFS_H
#define WXCHARTDIR_WXCHARTVIEWER_DEFS_H

// Unfortunately we can't always just rely on WXEXPORT because it wasn't
// defined correctly when using ELF visibility for symbol hiding in wx 2.8 that
// we still support, so we have to use our own symbol. This part, as well as
// the definition of WXCHARTVIEWER_HAVE_VISIBILITY in configure, should be dropped
// when wx 2.8 is not supported any longer.
#ifdef WXCHARTVIEWER_HAVE_VISIBILITY
    #define WXCHARTVIEWER_EXPORT __attribute__ ((visibility("default")))
#else
    #define WXCHARTVIEWER_EXPORT WXEXPORT
#endif

#if defined(WXMAKINGDLL_WXCHARTVIEWER)
  #define WXDLLIMPEXP_WXCHARTVIEWER WXCHARTVIEWER_EXPORT
  #define WXDLLIMPEXP_DATA_WXCHARTVIEWER(type) WXCHARTVIEWER_EXPORT type
#elif defined(WXUSINGDLL_WXCHARTVIEWER)
  #define WXDLLIMPEXP_WXCHARTVIEWER WXIMPORT
  #define WXDLLIMPEXP_DATA_WXCHARTVIEWER(type) WXIMPORT type
#else // not making nor using DLL
  #define WXDLLIMPEXP_WXCHARTVIEWER
  #define WXDLLIMPEXP_DATA_WXCHARTVIEWER(type) type
#endif

// GCC warns about using __declspec on forward declarations
// while MSVC complains about forward declarations without
// __declspec for the classes later declared with it. To hide this
// difference a separate macro for forward declarations is defined:
#if defined(HAVE_VISIBILITY) || (defined(__WINDOWS__) && defined(__GNUC__))
  #define WXDLLIMPEXP_FWD_WXCHARTVIEWER
#else
  #define WXDLLIMPEXP_FWD_WXCHARTVIEWER WXDLLIMPEXP_WXCHARTVIEWER
#endif

#endif // WXCHARTDIR_WXCHARTVIEWER_DEFS_H
