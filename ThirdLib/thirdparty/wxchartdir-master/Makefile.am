# Process this file with automake to create Makefile.in for wxchartdir library
#
# Copyright (C) 2018 Ulrich Telle <ulrich@telle-online.de>
#
# This file is covered by the same licence as the entire wxChartDir package.

ACLOCAL_AMFLAGS = -I admin/m4

# Flags used for compiling all the targets and linking all the executables
# (libraries use LIBADD which is set for lib@WXCHARTDIR_LIBNAME@.la only).
AM_CXXFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/wxdemo/common $(WX_CXXFLAGS)
AM_CXXFLAGS += -DCHARTDIR_HIDE_OBSOLETE

LDADD = lib@WXCHARTDIR_LIBNAME@.la -lchartdir $(WX_LIBS)

# Target-specific assignments are GNU Make-specific, but there doesn't seem to
# be any other way to define different compilation optiosn when building
# shared and static libraries with libtool.
if USE_SHARED
%.lo: AM_CXXFLAGS += -DWXMAKINGDLL_WXCHARTVIEWER
%.o: AM_CXXFLAGS += -DWXUSINGDLL_WXCHARTVIEWER
else
%.lo: AM_CXXFLAGS += -DWXMAKINGLIB_WXCHARTVIEWER
%.o: AM_CXXFLAGS += -DWXUSINGLIB_WXCHARTVIEWER
endif

if HAS_CHARTDIR_PATH
LD_CHARTDIR = -L@CHARTDIR_PATH@
RPATH_CHARTDIR = -rpath @CHARTDIR_PATH@
else
if USE_GTK
if OS_IS64BIT
LD_CHARTDIR = -L$(realpath $(top_srcdir))/lib/linux64
RPATH_CHARTDIR = -rpath $(realpath $(top_srcdir))/lib/linux64
else
LD_CHARTDIR = -L$(realpath $(top_srcdir))/lib/linux32
RPATH_CHARTDIR = -rpath $(realpath $(top_srcdir))/lib/linux32
endif
endif
if USE_MAC
LD_CHARTDIR = -L$(realpath $(top_srcdir))/lib/mac
RPATH_CHARTDIR = -rpath $(realpath $(top_srcdir))/lib/mac
endif
endif
LDFLAGS_SAMPLES = -no-install $(LD_CHARTDIR) $(RPATH_CHARTDIR)

# Define the directory where the library headers are installed.
includewxdir = $(includedir)/wx

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = wxchartdir.pc

# The main library. Notice that it uses dynamic, i.e. determined when running
# configure, name because it depends on the version of wxWidgets used. This is
# rather unusual, but Automake doesn't seem to mind.
lib_LTLIBRARIES = lib@WXCHARTDIR_LIBNAME@.la

lib@WXCHARTDIR_LIBNAME@_la_SOURCES = \
    wxdemo/common/wxchartviewer.cpp

includewx_HEADERS = \
    wxdemo/common/wxchartviewer.h \
    wxdemo/common/wxchartviewer_defs.h \
    wxdemo/common/wxchartviewer_version.h

# TODO: Do we need to list all headers from samples?
# noinst_HEADERS = \
#    wxdemo/*.h

lib@WXCHARTDIR_LIBNAME@_la_LDFLAGS = -no-undefined $(LD_CHARTDIR) $(AM_LDFLAGS)
lib@WXCHARTDIR_LIBNAME@_la_LIBADD = -lchartdir $(WX_LIBS)

# Samples (don't need to be installed).
noinst_PROGRAMS = \
    cppdemo/cppdemo \
    wxdemo/helloworld/helloworld \
    wxdemo/crosshair/crosshair \
    wxdemo/trackaxis/trackaxis \
    wxdemo/trackbox/trackbox \
    wxdemo/trackfinance/trackfinance \
    wxdemo/tracklabel/tracklabel \
    wxdemo/tracklegend/tracklegend \
    wxdemo/wxdemo/wxdemo \
    wxdemo/simplezoomscroll/simplezoomscroll \
    wxdemo/viewportcontroldemo/viewportcontroldemo \
    wxdemo/xyzoomscroll/xyzoomscroll \
    wxdemo/zoomscrolltrack/zoomscrolltrack \
    wxdemo/zoomscrolltrack2/zoomscrolltrack2 \
    wxdemo/realtimedemo/realtimedemo \
    wxdemo/realtimetrack/realtimetrack \
    wxdemo/realtimezoomscroll/realtimezoomscroll \
    wxdemo/realtimesweep/realtimesweep \
    wxdemo/realtimemultithread/realtimemultithread \
    wxdemo/realtimemeasure/realtimemeasure \
    wxdemo/realtimeviewport/realtimeviewport \
    wxdemo/financedemo/financedemo


cppdemo_cppdemo_SOURCES = \
    cppdemo/cppdemo.cpp \
    cppdemo/cppdemo_gen.h \
    cppdemo/cppdemo_inc.h

cppdemo_cppdemo_LDADD = $(LDADD) 
cppdemo_cppdemo_LDFLAGS = $(LDFLAGS_SAMPLES)


wxdemo_helloworld_helloworld_SOURCES = \
    wxdemo/helloworld/helloworld.cpp \
    wxdemo/helloworld/helloworldapp.cpp \
    wxdemo/helloworld/helloworld.h \
    wxdemo/helloworld/helloworldapp.h

wxdemo_helloworld_helloworld_LDADD = $(WX_LIBS_CHARTDIR_SAMPLE) $(LDADD)
wxdemo_helloworld_helloworld_LDFLAGS = $(LDFLAGS_SAMPLES)


wxdemo_crosshair_crosshair_SOURCES = \
    wxdemo/crosshair/crosshair.cpp \
    wxdemo/crosshair/crosshairapp.cpp \
    wxdemo/crosshair/crosshair.h \
    wxdemo/crosshair/crosshairapp.h

wxdemo_crosshair_crosshair_LDADD = $(WX_LIBS_CHARTDIR_SAMPLE) $(LDADD)
wxdemo_crosshair_crosshair_LDFLAGS = $(LDFLAGS_SAMPLES)


wxdemo_trackaxis_trackaxis_SOURCES = \
    wxdemo/trackaxis/trackaxis.cpp \
    wxdemo/trackaxis/trackaxisapp.cpp \
    wxdemo/trackaxis/trackaxis.h \
    wxdemo/trackaxis/trackaxisapp.h

wxdemo_trackaxis_trackaxis_LDADD = $(WX_LIBS_CHARTDIR_SAMPLE) $(LDADD)
wxdemo_trackaxis_trackaxis_LDFLAGS = $(LDFLAGS_SAMPLES)


wxdemo_trackbox_trackbox_SOURCES = \
    wxdemo/trackbox/trackbox.cpp \
    wxdemo/trackbox/trackboxapp.cpp \
    wxdemo/trackbox/trackbox.h \
    wxdemo/trackbox/trackboxapp.h

wxdemo_trackbox_trackbox_LDADD = $(WX_LIBS_CHARTDIR_SAMPLE) $(LDADD)
wxdemo_trackbox_trackbox_LDFLAGS = $(LDFLAGS_SAMPLES)


wxdemo_trackfinance_trackfinance_SOURCES = \
    wxdemo/trackfinance/trackfinance.cpp \
    wxdemo/trackfinance/trackfinanceapp.cpp \
    wxdemo/trackfinance/trackfinance.h \
    wxdemo/trackfinance/trackfinanceapp.h

wxdemo_trackfinance_trackfinance_LDADD = $(WX_LIBS_CHARTDIR_SAMPLE) $(LDADD)
wxdemo_trackfinance_trackfinance_LDFLAGS = $(LDFLAGS_SAMPLES)


wxdemo_tracklabel_tracklabel_SOURCES = \
    wxdemo/tracklabel/tracklabel.cpp \
    wxdemo/tracklabel/tracklabelapp.cpp \
    wxdemo/tracklabel/tracklabel.h \
    wxdemo/tracklabel/tracklabelapp.h

wxdemo_tracklabel_tracklabel_LDADD = $(WX_LIBS_CHARTDIR_SAMPLE) $(LDADD)
wxdemo_tracklabel_tracklabel_LDFLAGS = $(LDFLAGS_SAMPLES)


wxdemo_tracklegend_tracklegend_SOURCES = \
    wxdemo/tracklegend/tracklegend.cpp \
    wxdemo/tracklegend/tracklegendapp.cpp \
    wxdemo/tracklegend/tracklegend.h \
    wxdemo/tracklegend/tracklegendapp.h

wxdemo_tracklegend_tracklegend_LDADD = $(WX_LIBS_CHARTDIR_SAMPLE) $(LDADD)
wxdemo_tracklegend_tracklegend_LDFLAGS = $(LDFLAGS_SAMPLES)


wxdemo_wxdemo_wxdemo_SOURCES = \
    wxdemo/wxdemo/wxdemo.cpp \
    wxdemo/wxdemo/wxdemoapp.cpp \
    wxdemo/wxdemo/democharts.cpp \
    wxdemo/wxdemo/hotspotdialog.cpp \
    wxdemo/wxdemo/wxdemo.h \
    wxdemo/wxdemo/wxdemoapp.h \
    wxdemo/wxdemo/democharts.h \
    wxdemo/wxdemo/hotspotdialog.h

wxdemo_wxdemo_wxdemo_LDADD = $(WX_LIBS_CHARTDIR_SAMPLE) $(LDADD)
wxdemo_wxdemo_wxdemo_LDFLAGS = $(LDFLAGS_SAMPLES)


wxdemo_simplezoomscroll_simplezoomscroll_SOURCES = \
    wxdemo/simplezoomscroll/simplezoomscroll.cpp \
    wxdemo/simplezoomscroll/simplezoomscrollapp.cpp \
    wxdemo/simplezoomscroll/simplezoomscroll.h \
    wxdemo/simplezoomscroll/simplezoomscrollapp.h

wxdemo_simplezoomscroll_simplezoomscroll_LDADD = $(WX_LIBS_CHARTDIR_SAMPLE) $(LDADD)
wxdemo_simplezoomscroll_simplezoomscroll_LDFLAGS = $(LDFLAGS_SAMPLES)


wxdemo_viewportcontroldemo_viewportcontroldemo_SOURCES = \
    wxdemo/viewportcontroldemo/viewportcontroldemo.cpp \
    wxdemo/viewportcontroldemo/viewportcontroldemoapp.cpp \
    wxdemo/viewportcontroldemo/viewportcontroldemo.h \
    wxdemo/viewportcontroldemo/viewportcontroldemoapp.h

wxdemo_viewportcontroldemo_viewportcontroldemo_LDADD = $(WX_LIBS_CHARTDIR_SAMPLE) $(LDADD)
wxdemo_viewportcontroldemo_viewportcontroldemo_LDFLAGS = $(LDFLAGS_SAMPLES)


wxdemo_xyzoomscroll_xyzoomscroll_SOURCES = \
    wxdemo/xyzoomscroll/xyzoomscroll.cpp \
    wxdemo/xyzoomscroll/xyzoomscrollapp.cpp \
    wxdemo/xyzoomscroll/hotspotdialog.cpp \
    wxdemo/xyzoomscroll/xyzoomscroll.h \
    wxdemo/xyzoomscroll/xyzoomscrollapp.h \
    wxdemo/xyzoomscroll/hotspotdialog.h

wxdemo_xyzoomscroll_xyzoomscroll_LDADD = $(WX_LIBS_CHARTDIR_SAMPLE) $(LDADD)
wxdemo_xyzoomscroll_xyzoomscroll_LDFLAGS = $(LDFLAGS_SAMPLES)


wxdemo_zoomscrolltrack_zoomscrolltrack_SOURCES = \
    wxdemo/zoomscrolltrack/zoomscrolltrack.cpp \
    wxdemo/zoomscrolltrack/zoomscrolltrackapp.cpp \
    wxdemo/zoomscrolltrack/zoomscrolltrack.h \
    wxdemo/zoomscrolltrack/zoomscrolltrackapp.h

wxdemo_zoomscrolltrack_zoomscrolltrack_LDADD = $(WX_LIBS_CHARTDIR_SAMPLE) $(LDADD)
wxdemo_zoomscrolltrack_zoomscrolltrack_LDFLAGS = $(LDFLAGS_SAMPLES)


wxdemo_zoomscrolltrack2_zoomscrolltrack2_SOURCES = \
    wxdemo/zoomscrolltrack2/zoomscrolltrack2.cpp \
    wxdemo/zoomscrolltrack2/zoomscrolltrack2app.cpp \
    wxdemo/zoomscrolltrack2/zoomscrolltrack2.h \
    wxdemo/zoomscrolltrack2/zoomscrolltrack2app.h

wxdemo_zoomscrolltrack2_zoomscrolltrack2_LDADD = $(WX_LIBS_CHARTDIR_SAMPLE) $(LDADD)
wxdemo_zoomscrolltrack2_zoomscrolltrack2_LDFLAGS = $(LDFLAGS_SAMPLES)


wxdemo_realtimedemo_realtimedemo_SOURCES = \
    wxdemo/realtimedemo/realtimedemo.cpp \
    wxdemo/realtimedemo/realtimedemoapp.cpp \
    wxdemo/realtimedemo/realtimedemo.h \
    wxdemo/realtimedemo/realtimedemoapp.h

wxdemo_realtimedemo_realtimedemo_LDADD = $(WX_LIBS_CHARTDIR_SAMPLE) $(LDADD)
wxdemo_realtimedemo_realtimedemo_LDFLAGS = $(LDFLAGS_SAMPLES)


wxdemo_realtimetrack_realtimetrack_SOURCES = \
    wxdemo/realtimetrack/realtimetrack.cpp \
    wxdemo/realtimetrack/realtimetrackapp.cpp \
    wxdemo/realtimetrack/realtimetrack.h \
    wxdemo/realtimetrack/realtimetrackapp.h

wxdemo_realtimetrack_realtimetrack_LDADD = $(WX_LIBS_CHARTDIR_SAMPLE) $(LDADD)
wxdemo_realtimetrack_realtimetrack_LDFLAGS = $(LDFLAGS_SAMPLES)


wxdemo_realtimezoomscroll_realtimezoomscroll_SOURCES = \
    wxdemo/realtimezoomscroll/realtimezoomscroll.cpp \
    wxdemo/realtimezoomscroll/realtimezoomscrollapp.cpp \
    wxdemo/realtimezoomscroll/realtimezoomscroll.h \
    wxdemo/realtimezoomscroll/realtimezoomscrollapp.h

wxdemo_realtimezoomscroll_realtimezoomscroll_LDADD = $(WX_LIBS_CHARTDIR_SAMPLE) $(LDADD)
wxdemo_realtimezoomscroll_realtimezoomscroll_LDFLAGS = $(LDFLAGS_SAMPLES)


wxdemo_realtimesweep_realtimesweep_SOURCES = \
    wxdemo/realtimesweep/realtimesweep.cpp \
    wxdemo/realtimesweep/realtimesweepapp.cpp \
    wxdemo/realtimesweep/randomwave.cpp \
    wxdemo/realtimesweep/realtimesweep.h \
    wxdemo/realtimesweep/realtimesweepapp.h \
    wxdemo/realtimesweep/randomwave.h \
    wxdemo/realtimesweep/wxdoublebufferedqueue.h

wxdemo_realtimesweep_realtimesweep_LDADD = $(WX_LIBS_CHARTDIR_SAMPLE) $(LDADD)
wxdemo_realtimesweep_realtimesweep_LDFLAGS = $(LDFLAGS_SAMPLES)


wxdemo_realtimemultithread_realtimemultithread_SOURCES = \
    wxdemo/realtimemultithread/realtimemultithread.cpp \
    wxdemo/realtimemultithread/realtimemultithreadapp.cpp \
    wxdemo/realtimemultithread/randomwalk.cpp \
    wxdemo/realtimemultithread/realtimemultithread.h \
    wxdemo/realtimemultithread/realtimemultithreadapp.h \
    wxdemo/realtimemultithread/randomwalk.h \
    wxdemo/realtimemultithread/wxdoublebufferedqueue.h

wxdemo_realtimemultithread_realtimemultithread_LDADD = $(WX_LIBS_CHARTDIR_SAMPLE) $(LDADD)
wxdemo_realtimemultithread_realtimemultithread_LDFLAGS = $(LDFLAGS_SAMPLES)


wxdemo_realtimemeasure_realtimemeasure_SOURCES = \
    wxdemo/realtimemeasure/realtimemeasure.cpp \
    wxdemo/realtimemeasure/realtimemeasureapp.cpp \
    wxdemo/realtimemeasure/realtimemeasure.h \
    wxdemo/realtimemeasure/realtimemeasureapp.h \
    wxdemo/realtimemeasure/wxdoublebufferedqueue.h

wxdemo_realtimemeasure_realtimemeasure_LDADD = $(WX_LIBS_CHARTDIR_SAMPLE) $(LDADD)
wxdemo_realtimemeasure_realtimemeasure_LDFLAGS = $(LDFLAGS_SAMPLES)


wxdemo_realtimeviewport_realtimeviewport_SOURCES = \
    wxdemo/realtimeviewport/realtimeviewport.cpp \
    wxdemo/realtimeviewport/realtimeviewportapp.cpp \
    wxdemo/realtimeviewport/randomwalk.cpp \
    wxdemo/realtimeviewport/realtimeviewport.h \
    wxdemo/realtimeviewport/realtimeviewportapp.h \
    wxdemo/realtimeviewport/randomwalk.h \
    wxdemo/realtimeviewport/wxdoublebufferedqueue.h

wxdemo_realtimeviewport_realtimeviewport_LDADD = $(WX_LIBS_CHARTDIR_SAMPLE) $(LDADD)
wxdemo_realtimeviewport_realtimeviewport_LDFLAGS = $(LDFLAGS_SAMPLES)


wxdemo_financedemo_financedemo_SOURCES = \
    wxdemo/financedemo/financedemo.cpp \
    wxdemo/financedemo/financedemoapp.cpp \
    wxdemo/financedemo/hotspotdialog.cpp \
    wxdemo/financedemo/financedemo.h \
    wxdemo/financedemo/financedemoapp.h \
    wxdemo/financedemo/hotspotdialog.h

wxdemo_financedemo_financedemo_LDADD = $(WX_LIBS_CHARTDIR_SAMPLE) $(LDADD)
wxdemo_financedemo_financedemo_LDFLAGS = $(LDFLAGS_SAMPLES)
