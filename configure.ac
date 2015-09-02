AC_PREREQ([2.60])
AC_INIT([nagios-python-broker], [0.1], [david.jose.delassus@gmail.com])

AM_INIT_AUTOMAKE([1.10 foreign subdir-objects])

AC_CONFIG_SRCDIR([src])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CC
AC_PROG_LIBTOOL

# Checks for libraries
PKG_CHECK_MODULES(PYTHON, [python])
AC_PYTHON_MODULE(jinja2)

CFLAGS="$CFLAGS $PYTHON_CFLAGS -I include"
LDFLAGS="$LDFLAGS $PYTHON_LDFLAGS -module -avoid-version -shared"

AC_CONFIG_FILES([
    Makefile
])

AC_OUTPUT