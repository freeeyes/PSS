@echo off

echo "Check Vs Path..."
REM echo %VS140COMNTOOLS%
set VSFind="false"
set VSPath=%VS140COMNTOOLS%
if defined VSPath ( 
	echo "Find VS2015"
	set VSFind="true"
) else (
	echo "No Find VS2015, You must install vs2015" 
)

if %VSFind%=="true" (
echo "Check Vs OK"
)
echo "Check ACE Path..."
REM echo %VS140COMNTOOLS%
set ACEFind="false"
set ACEPath=%ACE_ROOT%
if defined ACEPath ( 
	echo "Find ACE"
	set VSFind="true"
) else (
	echo "No Find ACE, begin download ace..."
	powershell -Command "Invoke-WebRequest http://download.dre.vanderbilt.edu/previous_versions/ACE-6.4.0.zip -OutFile ACE-src-6.4.0.zip"
	echo "download ace ok"
	echo "download ace expand zip begin..."
	powershell -Command "Expand-Archive 'ACE-6.4.0.zip' '.\'"
	echo "download ace expand zip end..."
	echo "create ace config.h begin..."
	powershell -Command "New-Item .\ACE_wrappers\ace\config.h -type file -force"
	powershell -Command "'#define ACE_HAS_STANDARD_LIBRARY 1' >> '.\ACE_wrappers\ace\config.h'"
	powershell -Command "'#define ACE_HAS_IPV6' >> '.\ACE_wrappers\ace\config.h'"
	powershell -Command "'#include \"ace/config-win32.h\"' >> '.\ACE_wrappers\ace\config.h'"
	echo "create ace config.h end..."\
	echo "compile ace begin..."
	cmake -G "Visual Studio 15 Win64" ..
	"%VSPath%..\IDE\Devenv" .\ACE_wrappers\ace\ace_vc14.sln /build "Debug|x64" /project .\ACE_wrappers\ace\ACE_vc14.vcxproj
	echo "compile ace end..."	
	echo "set environment ACE_ROOT begin..."	
	set CurrPath=%cd%\ACE_wrappers\
	echo %CurrPath%
	setx /M ACE_ROOT "%CurrPath%"
	echo "set environment ACE_ROOT end..."	
	echo "set environment PATH begin..."
	wmic ENVIRONMENT where "name='path' and username='<system>'" set VariableValue="%path%;%ACE_ROOT%\bin"
	echo "set environment PATH end..."	
)

if %VSFind%=="true" (
echo "Check ACE OK"
)




