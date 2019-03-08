-- wxChartDirector configuration file for premake5
--
-- Copyright (C) 2018 Ulrich Telle <ulrich@telle-online.de>
--
-- This file is covered by the same licence as the entire wxchartdir package. 

dofile "premake/wxwidgets.lua"

BUILDDIR = _OPTIONS["builddir"] or "build"

workspace "wxchartdir"
  configurations { "Debug", "Release", "Debug wxDLL", "Release wxDLL", "DLL Debug", "DLL Release" }
  platforms { "Win32", "Win64" }
  location(BUILDDIR)

  if (is_msvc) then
    local wks = workspace()
    wks.filename = "wxchartdir_" .. vc_with_ver
  end

  defines {
    "_WINDOWS",
    "_CRT_SECURE_NO_WARNINGS",
    "_CRT_SECURE_NO_DEPRECATE",
    "_CRT_NONSTDC_NO_DEPRECATE"
  }

  init_filters()

-- wxChartViewer library
project "wxchartviewer"
  location(BUILDDIR)
  language "C++"

  if (is_msvc) then
    local prj = project()
    prj.filename = "wxchartdir_" .. vc_with_ver .. "_chartviewer"
  end
  if wxMonolithic then
    local prj = project()
    prj.filename = "wxchartdir_mono"
  end

  links { "chartdir60" }
  filter { "platforms:Win32" }
    libdirs { "../lib/win32" }
  filter { "platforms:Win64" }
    libdirs { "../lib/win64" }
  filter {}

  make_filters( "CHARTDIR", "wxchartdir", "core,xml" )

  files { "common/*.cpp", "common/*.h", "common/*.rc" }
  vpaths {
    ["Header Files"] = { "**.h" },
    ["Source Files"] = { "**.cpp", "**.rc" }
  }
  includedirs { "common", "../include" }
  characterset "Unicode"

  defines {
    "CHARTDIR_HIDE_OBSOLETE"
  }

-- wxChartDir helloworld
project "helloworld"
  location(BUILDDIR)
  language "C++"
  kind "WindowedApp"

  if (is_msvc) then
    local prj = project()
    prj.filename = "wxchartdir_" .. vc_with_ver .. "_helloworld"
  end
  if wxMonolithic then
    local prj = project()
    prj.filename = "helloworld_mono"
  end

  use_filters( "CHARTDIR", "helloworld", "adv,html,richtext,core,xml" )

  files { "helloworld/*.cpp", "helloworld/*.h", "helloworld/*.rc" }
  vpaths {
    ["Header Files"] = { "**.h" },
    ["Source Files"] = { "**.cpp", "**.rc" }
  }
  includedirs { "helloworld", "common", "../include" }
  characterset "Unicode"
  flags { "WinMain" }  
  links { "wxchartviewer" }

-- wxChartDir crosshair
project "crosshair"
  location(BUILDDIR)
  language "C++"
  kind "WindowedApp"

  if (is_msvc) then
    local prj = project()
    prj.filename = "wxchartdir_" .. vc_with_ver .. "_crosshair"
  end
  if wxMonolithic then
    local prj = project()
    prj.filename = "crosshair_mono"
  end

  use_filters( "CHARTDIR", "crosshair", "adv,html,richtext,core,xml" )

  files { "crosshair/*.cpp", "crosshair/*.h", "crosshair/*.rc" }
  vpaths {
    ["Header Files"] = { "**.h" },
    ["Source Files"] = { "**.cpp", "**.rc" }
  }
  includedirs { "crosshair", "common", "../include" }
  characterset "Unicode"
  flags { "WinMain" }  
  links { "wxchartviewer" }

-- wxChartDir trackaxis
project "trackaxis"
  location(BUILDDIR)
  language "C++"
  kind "WindowedApp"

  if (is_msvc) then
    local prj = project()
    prj.filename = "wxchartdir_" .. vc_with_ver .. "_trackaxis"
  end
  if wxMonolithic then
    local prj = project()
    prj.filename = "trackaxis_mono"
  end

  use_filters( "CHARTDIR", "trackaxis", "adv,html,richtext,core,xml" )

  files { "trackaxis/*.cpp", "trackaxis/*.h", "trackaxis/*.rc" }
  vpaths {
    ["Header Files"] = { "**.h" },
    ["Source Files"] = { "**.cpp", "**.rc" }
  }
  includedirs { "trackaxis", "common", "../include" }
  characterset "Unicode"
  flags { "WinMain" }  
  links { "wxchartviewer" }

-- wxChartDir trackbox
project "trackbox"
  location(BUILDDIR)
  language "C++"
  kind "WindowedApp"

  if (is_msvc) then
    local prj = project()
    prj.filename = "wxchartdir_" .. vc_with_ver .. "_trackbox"
  end
  if wxMonolithic then
    local prj = project()
    prj.filename = "trackbox_mono"
  end

  use_filters( "CHARTDIR", "trackbox", "adv,html,richtext,core,xml" )

  files { "trackbox/*.cpp", "trackbox/*.h", "trackbox/*.rc" }
  vpaths {
    ["Header Files"] = { "**.h" },
    ["Source Files"] = { "**.cpp", "**.rc" }
  }
  includedirs { "trackbox", "common", "../include" }
  characterset "Unicode"
  flags { "WinMain" }  
  links { "wxchartviewer" }

-- wxChartDir trackfinance
project "trackfinance"
  location(BUILDDIR)
  language "C++"
  kind "WindowedApp"

  if (is_msvc) then
    local prj = project()
    prj.filename = "wxchartdir_" .. vc_with_ver .. "_trackfinance"
  end
  if wxMonolithic then
    local prj = project()
    prj.filename = "trackfinance_mono"
  end

  use_filters( "CHARTDIR", "trackfinance", "adv,html,richtext,core,xml" )

  files { "trackfinance/*.cpp", "trackfinance/*.h", "trackfinance/*.rc" }
  vpaths {
    ["Header Files"] = { "**.h" },
    ["Source Files"] = { "**.cpp", "**.rc" }
  }
  includedirs { "trackfinance", "common", "../include" }
  characterset "Unicode"
  flags { "WinMain" }  
  links { "wxchartviewer" }

-- wxChartDir tracklabel
project "tracklabel"
  location(BUILDDIR)
  language "C++"
  kind "WindowedApp"

  if (is_msvc) then
    local prj = project()
    prj.filename = "wxchartdir_" .. vc_with_ver .. "_tracklabel"
  end
  if wxMonolithic then
    local prj = project()
    prj.filename = "tracklabel_mono"
  end

  use_filters( "CHARTDIR", "tracklabel", "adv,html,richtext,core,xml" )

  files { "tracklabel/*.cpp", "tracklabel/*.h", "tracklabel/*.rc" }
  vpaths {
    ["Header Files"] = { "**.h" },
    ["Source Files"] = { "**.cpp", "**.rc" }
  }
  includedirs { "tracklabel", "common", "../include" }
  characterset "Unicode"
  flags { "WinMain" }  
  links { "wxchartviewer" }

-- wxChartDir tracklegend
project "tracklegend"
  location(BUILDDIR)
  language "C++"
  kind "WindowedApp"

  if (is_msvc) then
    local prj = project()
    prj.filename = "wxchartdir_" .. vc_with_ver .. "_tracklegend"
  end
  if wxMonolithic then
    local prj = project()
    prj.filename = "tracklegend_mono"
  end

  use_filters( "CHARTDIR", "tracklegend", "adv,html,richtext,core,xml" )

  files { "tracklegend/*.cpp", "tracklegend/*.h", "tracklegend/*.rc" }
  vpaths {
    ["Header Files"] = { "**.h" },
    ["Source Files"] = { "**.cpp", "**.rc" }
  }
  includedirs { "tracklegend", "common", "../include" }
  characterset "Unicode"
  flags { "WinMain" }  
  links { "wxchartviewer" }

-- wxChartDir wxdemo
project "wxdemo"
  location(BUILDDIR)
  language "C++"
  kind "WindowedApp"

  if (is_msvc) then
    local prj = project()
    prj.filename = "wxchartdir_" .. vc_with_ver .. "_wxdemo"
  end
  if wxMonolithic then
    local prj = project()
    prj.filename = "wxdemo_mono"
  end

  use_filters( "CHARTDIR", "wxdemo", "adv,html,richtext,core,xml" )

  files { "wxdemo/*.cpp", "wxdemo/*.h", "wxdemo/*.rc" }
  vpaths {
    ["Header Files"] = { "**.h" },
    ["Source Files"] = { "**.cpp", "**.rc" }
  }
  includedirs { "wxdemo", "common", "../include" }
  characterset "Unicode"
  flags { "WinMain" }  
  links { "wxchartviewer" }

-- simplezoomscroll
project "simplezoomscroll"
  location(BUILDDIR)
  language "C++"
  kind "WindowedApp"

  if (is_msvc) then
    local prj = project()
    prj.filename = "wxchartdir_" .. vc_with_ver .. "_simplezoomscroll"
  end
  if wxMonolithic then
    local prj = project()
    prj.filename = "simplezoomscroll_mono"
  end

  use_filters( "CHARTDIR", "simplezoomscroll", "adv,html,richtext,core,xml" )

  files { "simplezoomscroll/*.cpp", "simplezoomscroll/*.h", "simplezoomscroll/*.rc" }
  vpaths {
    ["Header Files"] = { "**.h" },
    ["Source Files"] = { "**.cpp", "**.rc" }
  }
  includedirs { "simplezoomscroll", "common", "../include" }
  characterset "Unicode"
  flags { "WinMain" }  
  links { "wxchartviewer" }

-- viewportcontroldemo
project "viewportcontroldemo"
  location(BUILDDIR)
  language "C++"
  kind "WindowedApp"

  if (is_msvc) then
    local prj = project()
    prj.filename = "wxchartdir_" .. vc_with_ver .. "_viewportcontroldemo"
  end
  if wxMonolithic then
    local prj = project()
    prj.filename = "viewportcontroldemo_mono"
  end

  use_filters( "CHARTDIR", "viewportcontroldemo", "adv,html,richtext,core,xml" )

  files { "viewportcontroldemo/*.cpp", "viewportcontroldemo/*.h", "viewportcontroldemo/*.rc" }
  vpaths {
    ["Header Files"] = { "**.h" },
    ["Source Files"] = { "**.cpp", "**.rc" }
  }
  includedirs { "viewportcontroldemo", "common", "../include" }
  characterset "Unicode"
  flags { "WinMain" }  
  links { "wxchartviewer" }

-- xyzoomscroll
project "xyzoomscroll"
  location(BUILDDIR)
  language "C++"
  kind "WindowedApp"

  if (is_msvc) then
    local prj = project()
    prj.filename = "wxchartdir_" .. vc_with_ver .. "_xyzoomscroll"
  end
  if wxMonolithic then
    local prj = project()
    prj.filename = "xyzoomscroll_mono"
  end

  use_filters( "CHARTDIR", "xyzoomscroll", "adv,html,richtext,core,xml" )

  files { "xyzoomscroll/*.cpp", "xyzoomscroll/*.h", "xyzoomscroll/*.rc" }
  vpaths {
    ["Header Files"] = { "**.h" },
    ["Source Files"] = { "**.cpp", "**.rc" }
  }
  includedirs { "xyzoomscroll", "common", "../include" }
  characterset "Unicode"
  flags { "WinMain" }  
  links { "wxchartviewer" }

-- zoomscrolltrack
project "zoomscrolltrack"
  location(BUILDDIR)
  language "C++"
  kind "WindowedApp"

  if (is_msvc) then
    local prj = project()
    prj.filename = "wxchartdir_" .. vc_with_ver .. "_zoomscrolltrack"
  end
  if wxMonolithic then
    local prj = project()
    prj.filename = "zoomscrolltrack_mono"
  end

  use_filters( "CHARTDIR", "zoomscrolltrack", "adv,html,richtext,core,xml" )

  files { "zoomscrolltrack/*.cpp", "zoomscrolltrack/*.h", "zoomscrolltrack/*.rc" }
  vpaths {
    ["Header Files"] = { "**.h" },
    ["Source Files"] = { "**.cpp", "**.rc" }
  }
  includedirs { "zoomscrolltrack", "common", "../include" }
  characterset "Unicode"
  flags { "WinMain" }  
  links { "wxchartviewer" }

-- zoomscrolltrack2
project "zoomscrolltrack2"
  location(BUILDDIR)
  language "C++"
  kind "WindowedApp"

  if (is_msvc) then
    local prj = project()
    prj.filename = "wxchartdir_" .. vc_with_ver .. "_zoomscrolltrack2"
  end
  if wxMonolithic then
    local prj = project()
    prj.filename = "zoomscrolltrack2_mono"
  end

  use_filters( "CHARTDIR", "zoomscrolltrack2", "adv,html,richtext,core,xml" )

  files { "zoomscrolltrack2/*.cpp", "zoomscrolltrack2/*.h", "zoomscrolltrack2/*.rc" }
  vpaths {
    ["Header Files"] = { "**.h" },
    ["Source Files"] = { "**.cpp", "**.rc" }
  }
  includedirs { "zoomscrolltrack2", "common", "../include" }
  characterset "Unicode"
  flags { "WinMain" }  
  links { "wxchartviewer" }

-- realtimedemo
project "realtimedemo"
  location(BUILDDIR)
  language "C++"
  kind "WindowedApp"

  if (is_msvc) then
    local prj = project()
    prj.filename = "wxchartdir_" .. vc_with_ver .. "_realtimedemo"
  end
  if wxMonolithic then
    local prj = project()
    prj.filename = "realtimedemo_mono"
  end

  use_filters( "CHARTDIR", "realtimedemo", "adv,html,richtext,core,xml" )

  files { "realtimedemo/*.cpp", "realtimedemo/*.h", "realtimedemo/*.rc" }
  vpaths {
    ["Header Files"] = { "**.h" },
    ["Source Files"] = { "**.cpp", "**.rc" }
  }
  includedirs { "realtimedemo", "common", "../include" }
  characterset "Unicode"
  flags { "WinMain" }  
  links { "wxchartviewer" }

-- realtimetrack
project "realtimetrack"
  location(BUILDDIR)
  language "C++"
  kind "WindowedApp"

  if (is_msvc) then
    local prj = project()
    prj.filename = "wxchartdir_" .. vc_with_ver .. "_realtimetrack"
  end
  if wxMonolithic then
    local prj = project()
    prj.filename = "realtimetrack_mono"
  end

  use_filters( "CHARTDIR", "realtimetrack", "adv,html,richtext,core,xml" )

  files { "realtimetrack/*.cpp", "realtimetrack/*.h", "realtimetrack/*.rc" }
  vpaths {
    ["Header Files"] = { "**.h" },
    ["Source Files"] = { "**.cpp", "**.rc" }
  }
  includedirs { "realtimetrack", "common", "../include" }
  characterset "Unicode"
  flags { "WinMain" }  
  links { "wxchartviewer" }

-- realtimezoomscroll
project "realtimezoomscroll"
  location(BUILDDIR)
  language "C++"
  kind "WindowedApp"

  if (is_msvc) then
    local prj = project()
    prj.filename = "wxchartdir_" .. vc_with_ver .. "_realtimezoomscroll"
  end
  if wxMonolithic then
    local prj = project()
    prj.filename = "realtimezoomscroll_mono"
  end

  use_filters( "CHARTDIR", "realtimezoomscroll", "adv,html,richtext,core,xml" )

  files { "realtimezoomscroll/*.cpp", "realtimezoomscroll/*.h", "realtimezoomscroll/*.rc" }
  vpaths {
    ["Header Files"] = { "**.h" },
    ["Source Files"] = { "**.cpp", "**.rc" }
  }
  includedirs { "realtimezoomscroll", "common", "../include" }
  characterset "Unicode"
  flags { "WinMain" }  
  links { "wxchartviewer" }

-- realtimesweep
project "realtimesweep"
  location(BUILDDIR)
  language "C++"
  kind "WindowedApp"

  if (is_msvc) then
    local prj = project()
    prj.filename = "wxchartdir_" .. vc_with_ver .. "_realtimesweep"
  end
  if wxMonolithic then
    local prj = project()
    prj.filename = "realtimesweep_mono"
  end

  use_filters( "CHARTDIR", "realtimesweep", "adv,html,richtext,core,xml" )

  files { "realtimesweep/*.cpp", "realtimesweep/*.h", "realtimesweep/*.rc" }
  vpaths {
    ["Header Files"] = { "**.h" },
    ["Source Files"] = { "**.cpp", "**.rc" }
  }
  includedirs { "realtimesweep", "common", "../include" }
  characterset "Unicode"
  flags { "WinMain" }  
  links { "wxchartviewer" }

-- realtimemultithread
project "realtimemultithread"
  location(BUILDDIR)
  language "C++"
  kind "WindowedApp"

  if (is_msvc) then
    local prj = project()
    prj.filename = "wxchartdir_" .. vc_with_ver .. "_realtimemultithread"
  end
  if wxMonolithic then
    local prj = project()
    prj.filename = "realtimemultithread_mono"
  end

  use_filters( "CHARTDIR", "realtimemultithread", "adv,html,richtext,core,xml" )

  files { "realtimemultithread/*.cpp", "realtimemultithread/*.h", "realtimemultithread/*.rc" }
  vpaths {
    ["Header Files"] = { "**.h" },
    ["Source Files"] = { "**.cpp", "**.rc" }
  }
  includedirs { "realtimemultithread", "common", "../include" }
  characterset "Unicode"
  flags { "WinMain" }  
  links { "wxchartviewer" }

-- realtimemeasure
project "realtimemeasure"
  location(BUILDDIR)
  language "C++"
  kind "WindowedApp"

  if (is_msvc) then
    local prj = project()
    prj.filename = "wxchartdir_" .. vc_with_ver .. "_realtimemeasure"
  end
  if wxMonolithic then
    local prj = project()
    prj.filename = "realtimemeasure_mono"
  end

  use_filters( "CHARTDIR", "realtimemeasure", "adv,html,richtext,core,xml" )

  files { "realtimemeasure/*.cpp", "realtimemeasure/*.h", "realtimemeasure/*.rc" }
  vpaths {
    ["Header Files"] = { "**.h" },
    ["Source Files"] = { "**.cpp", "**.rc" }
  }
  includedirs { "realtimemeasure", "common", "../include" }
  characterset "Unicode"
  flags { "WinMain" }  
  links { "wxchartviewer" }

-- realtimeviewport
project "realtimeviewport"
  location(BUILDDIR)
  language "C++"
  kind "WindowedApp"

  if (is_msvc) then
    local prj = project()
    prj.filename = "wxchartdir_" .. vc_with_ver .. "_realtimeviewport"
  end
  if wxMonolithic then
    local prj = project()
    prj.filename = "realtimeviewport_mono"
  end

  use_filters( "CHARTDIR", "realtimeviewport", "adv,html,richtext,core,xml" )

  files { "realtimeviewport/*.cpp", "realtimeviewport/*.h", "realtimeviewport/*.rc" }
  vpaths {
    ["Header Files"] = { "**.h" },
    ["Source Files"] = { "**.cpp", "**.rc" }
  }
  includedirs { "realtimeviewport", "common", "../include" }
  characterset "Unicode"
  flags { "WinMain" }  
  links { "wxchartviewer" }

-- financedemo
project "financedemo"
  location(BUILDDIR)
  language "C++"
  kind "WindowedApp"

  if (is_msvc) then
    local prj = project()
    prj.filename = "wxchartdir_" .. vc_with_ver .. "_financedemo"
  end
  if wxMonolithic then
    local prj = project()
    prj.filename = "financedemo_mono"
  end

  use_filters( "CHARTDIR", "financedemo", "adv,html,richtext,core,xml" )

  files { "financedemo/*.cpp", "financedemo/*.h", "financedemo/*.rc" }
  vpaths {
    ["Header Files"] = { "**.h" },
    ["Source Files"] = { "**.cpp", "**.rc" }
  }
  includedirs { "financedemo", "common", "../include" }
  characterset "Unicode"
  flags { "WinMain" }  
  links { "wxchartviewer" }
