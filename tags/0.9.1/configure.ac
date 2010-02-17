#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(lux, 0.9.1, hiroyu@users.sourceforge.net)
AC_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE(lux, 0.9.1)

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_LANG(C++)

# Checks for libraries.
AC_PROG_LIBTOOL


# Checks for header files.
AC_HEADER_STDC
AC_HEADER_DIRENT
AC_HEADER_STAT
AC_HEADER_TIME

AC_CXX_HAVE_STL

AC_CHECK_HEADERS([sys/time.h])
AC_CHECK_HEADERS([algorithm functional map vector string set],,
                 [AC_MSG_ERROR([A required C++ header file is not found.])])

AC_CHECK_HEADERS([tr1/memory])
AC_CHECK_HEADERS([boost/shared_ptr.hpp],, [
  if test $ac_cv_header_tr1_memory = yes; then
    :
  else
    AC_MSG_ERROR([either tr1/memory or boost/shared_ptr.hpp is required for lux.])
  fi])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_HEADER_TIME
AC_SYS_LARGEFILE
AC_FUNC_FSEEKO
AC_TYPE_OFF_T

# Checks for library functions.
AC_HEADER_STDC
AC_CHECK_FUNCS([gettimeofday mkdir strstr])

# Checks for libraries
AC_CHECK_LIB(luxio, main, [],[echo "libluxio is not found"; exit 1])
LUXIO_CFLAGS=""
LUXIO_LIBS="-lluxio"

AC_CHECK_LIB(mecab, mecab_new, [],[echo "libmecab is not found"; exit 1])
MECAB_CFLAGS=""
MECAB_LIBS="-lmecab -lstdc++"
#AC_CHECK_LIB(qdbm, dpopen, [],[echo "libqdbm is not found"; exit 1])
#QDBM_CFLAGS=""
#QDBM_LIBS="-lqdbm"

AC_CHECK_LIB(protobuf, main, [],[echo "libprotobuf is not found"; exit 1])
PROTOBUF_CFLAGS=""
PROTOBUF_LIBS="-lprotobuf"

# zlib
AC_ARG_ENABLE(zlib,
  AC_HELP_STRING([--enable-zlib], [use zlib for data compression.]))
if test "$enable_zlib" = "yes"
then
  AC_CHECK_LIB(z, main)
  AC_DEFINE(USE_ZLIB, [1], [using zlib])
  ZLIB_LIBS="-lz"
fi

LUX_LIBS="$LUXIO_LIBS $MECAB_LIBS $PROTOBUF_LIBS $QDBM_LIBS $ZLIB_LIBS"
AC_SUBST(LUX_LIBS)

AC_OUTPUT(Makefile lux/Makefile)
