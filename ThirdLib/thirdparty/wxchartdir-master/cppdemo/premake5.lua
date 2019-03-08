-- wxChartDir cppdemo configuration file for premake5
--
-- Copyright (C) 2018 Ulrich Telle <ulrich@telle-online.de>
--
-- This file is covered by the same licence as the entire wxChartDir package. 

BUILDDIR = _OPTIONS["builddir"] or "build"

-- Determine version of Visual Studio action
vc_version = "";
if _ACTION == "vs2003" then
  vc_version = 7
elseif _ACTION == "vs2005" then
  vc_version = 8
elseif _ACTION == "vs2008" then
  vc_version = 9
elseif _ACTION == "vs2010" then
  vc_version = 10
elseif _ACTION == "vs2012" then
  vc_version = 11
elseif _ACTION == "vs2013" then
  vc_version = 12
elseif _ACTION == "vs2015" then
  vc_version = 14
elseif _ACTION == "vs2017" then
  vc_version = 15
end

is_msvc = false
msvc_useProps = false
if ( vc_version ~= "" ) then
  is_msvc = true
  msvc_useProps = vc_version >= 10
  vc_with_ver = "vc"..vc_version
end

-- Activate loading of separate props file
if (msvc_useProps) then
  premake.wxProject = true
end

workspace "cppdemo"
  configurations { "Debug", "Release" }
  platforms { "Win32", "Win64" }
  location(BUILDDIR)

  if (is_msvc) then
    local wks = workspace()
    wks.filename = "cppdemo_" .. vc_with_ver
  end

  defines {
    "_WINDOWS",
    "_CRT_SECURE_NO_WARNINGS",
    "_CRT_SECURE_NO_DEPRECATE",
    "_CRT_NONSTDC_NO_DEPRECATE"
  }

  filter { "platforms:Win32" }
    system "Windows"
    architecture "x32"

  filter { "platforms:Win64" }
    system "Windows"
    architecture "x64"
    targetsuffix "_x64"

  filter { "configurations:Debug*" }
    defines {
      "DEBUG", 
      "_DEBUG"
    }
    symbols "On"

  filter { "configurations:Release*" }
    defines {
      "NDEBUG"
    }
    flags { "Optimize" }  

  filter {}

-- Minimal sample
project "cppdemoall"
  location(BUILDDIR)
  language "C++"
  kind "ConsoleApp"

  if (is_msvc) then
    local prj = project()
    prj.filename = "cppdemo_" .. vc_with_ver .. "_all"
  else
    toolset("gcc")
  end
  makesettings { "include config.gcc" }

  files { "cppdemo.cpp", "cppdemo_*.h" }
  vpaths {
    ["Header Files"] = { "**.h" },
    ["Source Files"] = { "**.cpp", "**.rc" }
  }
  includedirs { "../include" }
  characterset "Unicode"
  flags { "WinMain" }  

  postbuildcommands {
    "{COPY} %{wks.location}/../background/*.gif %{wks.location}/../bin",
    "{COPY} %{wks.location}/../background/*.png %{wks.location}/../bin",
    "{COPY} %{wks.location}/../customsymbolline/*.png %{wks.location}/../bin",
    "{COPY} %{wks.location}/../enhancedarea/*.png %{wks.location}/../bin",
    "{COPY} %{wks.location}/../errline/*.png %{wks.location}/../bin",
    "{COPY} %{wks.location}/../fontpie/*.png %{wks.location}/../bin",
    "{COPY} %{wks.location}/../hboxwhisker/*.png %{wks.location}/../bin",
    "{COPY} %{wks.location}/../iconameter/*.gif %{wks.location}/../bin",
    "{COPY} %{wks.location}/../icondonut/*.png %{wks.location}/../bin",
    "{COPY} %{wks.location}/../iconpie/*.png %{wks.location}/../bin",
    "{COPY} %{wks.location}/../iconpie2/*.png %{wks.location}/../bin",
    "{COPY} %{wks.location}/../linecompare/*.png %{wks.location}/../bin",
    "{COPY} %{wks.location}/../multistackbar/*.png %{wks.location}/../bin",
    "{COPY} %{wks.location}/../patternarea/*.png %{wks.location}/../bin",
    "{COPY} %{wks.location}/../percentarea/*.png %{wks.location}/../bin",
    "{COPY} %{wks.location}/../percentbar/*.png %{wks.location}/../bin",
    "{COPY} %{wks.location}/../polararea/*.png %{wks.location}/../bin",
    "{COPY} %{wks.location}/../polarline/*.png %{wks.location}/../bin",
    "{COPY} %{wks.location}/../scattersymbols/*.png %{wks.location}/../bin",
    "{COPY} %{wks.location}/../splineline/*.png %{wks.location}/../bin",
    "{COPY} %{wks.location}/../texturedonut/*.png %{wks.location}/../bin",
    "{COPY} %{wks.location}/../yzonecolor/*.png %{wks.location}/../bin"
  }

  links { "chartdir60" }
  filter { "platforms:Win32" }
    libdirs { "../lib/win32" }
    debugenvs { "PATH=../../lib/win32;%PATH%" }
  filter { "platforms:Win64" }
    libdirs { "../lib/win64" }
    debugenvs { "PATH=../../lib/win64;%PATH%" }
  filter {}

  -- Intermediate directory
  if (is_msvc) then
    objdir (BUILDDIR .. "/obj/" .. vc_with_ver)
  else
    objdir (BUILDDIR .. "/obj/gcc")
  end

  -- Target directory
  filter { "configurations:Debug*", "platforms:Win32" }
    debugdir "bin"
    if (is_msvc) then
      targetdir ("bin/" .. vc_with_ver .. "/Win32/Debug")
    else
      targetdir "bin/gcc/Win32/Debug"
    end
  filter { "configurations:Debug*", "platforms:Win64" }
    debugdir "bin"
    if (is_msvc) then
      targetdir ("bin/" .. vc_with_ver .. "/Win64/Debug")
    else
      targetdir "bin/gcc/Win64/Debug"
    end
  filter { "configurations:Release*", "platforms:Win32" }
    if (is_msvc) then
      targetdir ("bin/" .. vc_with_ver .. "/Win32/Release")
    else
      targetdir "bin/gcc/Win32/Release"
    end
