#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(bcalc, 0.2.6, blambi@chebab.com)
AC_CONFIG_SRCDIR([src/bcalc.c])
AC_CONFIG_HEADER([src/config.h])

# Options
AC_ARG_ENABLE(curses,
  [AC_HELP_STRING([--disable-curses],[Disable the ncurses ui.])],
  enable_curses=$enableval, enable_curses=yes)

AC_ARG_ENABLE(readline,
  [AC_HELP_STRING([--disable-readline],[Disable readline based ui.])],
  enable_readline=$enableval, enable_readline=yes)

AC_ARG_ENABLE(gtk2,
  [AC_HELP_STRING([--disable-gtk2],[Disable GTK 2.x GUI])],
  enable_gtk2=$enableval, enable_gtk2=yes)

AC_ARG_ENABLE(debug,
  [AC_HELP_STRING([--enable-debug],[Enable debug related code.])],
  enable_debug=$enableval, enable_debug=no)

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL

# Checks for libraries.
if test "x$enable_curses" = "xyes"; then
   AC_CHECK_LIB([ncurses], [initscr],
   , enable_curses=no)
fi

if test "x$enable_readline" = "xyes"; then
   AC_CHECK_LIB([readline], [readline],
   , enable_readline=no)
fi

if test "x$enable_gtk2" = "xyes"; then
   # FIXME: a bit ugly but didn't get PKG_CHECK_MODULES to work.
   AC_MSG_CHECKING([if we have GTK+ 2.x])
   if test "$( pkg-config gtk+-2.0 --modversion 2> /dev/null )" != ""; then
      enable_gtk2="yes"
      GTK2_CFLAGS=$( pkg-config gtk+-2.0 --cflags 2> /dev/null )
      GTK2_LIBS=$( pkg-config gtk+-2.0 --libs 2> /dev/null )
   else
      enable_gtk2="no"
   fi
   AC_MSG_RESULT($enable_gtk2)
fi

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_VPRINTF

# options modifying CFLAGS etc and the output of enabled features
AC_MSG_NOTICE([Enabled features:])
if test "x$enable_debug" = "xyes"; then
   AC_DEFINE(DEBUG,, [Define to enable debug code.])
   AC_MSG_NOTICE([ + debug])
fi
if test "x$enable_curses" = "xyes"; then
   AC_DEFINE(UI_CURSES,, [Define to enable curses ui.])
   AC_MSG_NOTICE([ + ncurses ui])
fi
if test "x$enable_readline" = "xyes"; then
   AC_DEFINE(UI_READLINE,, [Define to enable readline ui.])
   AC_DEFINE(DEFAULT_UI, 2, [Since we have readline lets use it.])
   AC_MSG_NOTICE([ + readline ui])
else
   AC_DEFINE(DEFAULT_UI, 0, [Uses normal dull text-ui.])
fi
if test "x$enable_gtk2" = "xyes"; then
   AC_DEFINE(UI_GTK2,, [Define to enable GTK+ 2.x gui.])
   CFLAGS="$GTK2_CFLAGS $CFLAGS"
   LIBS="$GTK2_LIBS $LIBS"
   AC_MSG_NOTICE([ + GTK 2.x ui])
else
   LIBS="-lm $LIBS"
fi

AC_DEFINE(VERSION, PACKAGE_VERSION, [Some more magic])

# Output
AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
