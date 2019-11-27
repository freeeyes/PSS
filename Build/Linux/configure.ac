#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([Patrol_algorithm], [1.0], [https://github.com/freeeyes/Patrol_algorithm])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([../../purenessscopeserver/Common/ACEMemory.cpp])
AC_CONFIG_SRCDIR([../../purenessscopeserver/Common/ThreadLogic.cpp])
AC_CONFIG_SRCDIR([../../purenessscopeserver/Common/TimerNodeList.cpp])
AC_CONFIG_SRCDIR([../../purenessscopeserver/Common/Threading.cpp])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# check ace envement path
AC_ARG_ENABLE([install-ace], 
    AC_HELP_STRING([--enable-install-ace],[Allow configure to download ace for automatic installation]]),
    [enable_ace_auto_install=yes],
    [enable_ace_auto_install=no])
     
AC_MSG_RESULT([checking enable_ace_auto_install=${enable_ace_auto_install} ...])

AC_MSG_RESULT([checking path=${ACE_ROOT} ...])
AS_IF([test "x${ACE_ROOT}" = x"" ], [ace_path_exist=no], [ace_path_exist=yes])

AS_IF([test "x$ace_path_exist" = "xno"],
[AS_IF([test "x$enable_ace_auto_install" = "xyes"], [sh ./install_ace.sh], [AC_MSG_RESULT([checking no ace path, you need add --add-ace-install ace install])])],
[AC_MSG_RESULT([checking ace path is exist])])

# Checks for libraries.
LDFLAGS="$LDFLAGS -L${ACE_ROOT}/lib"
AC_CHECK_LIB([ACE], [_ZTv0_n64_NK23ACE_POSIX_Asynch_Result5errorEv], [AC_MSG_RESULT([checking ace lib is OK])], [
	AS_IF([test "x$enable_ace_auto_install" = "xyes"], 
	[AC_MSG_RESULT([install ace start ...])],
	AC_MSG_ERROR([Error! You need to have ACE installed!]))
])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.
#AC_CHECK_FUNCS([pow sqrt])
AC_CONFIG_FILES([Makefile])
sh ./runlinuxmpc.sh
AC_OUTPUT
