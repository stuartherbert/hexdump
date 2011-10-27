dnl $Id$
dnl config.m4 for extension hexdump

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(hexdump, for hexdump support,
dnl Make sure that the comment is aligned:
dnl [  --with-hexdump             Include hexdump support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(hexdump, whether to enable hexdump support,
dnl Make sure that the comment is aligned:
[  --enable-hexdump           Enable hexdump support])

if test "$PHP_HEXDUMP" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-hexdump -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/hexdump.h"  # you most likely want to change this
  dnl if test -r $PHP_HEXDUMP/$SEARCH_FOR; then # path given as parameter
  dnl   HEXDUMP_DIR=$PHP_HEXDUMP
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for hexdump files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       HEXDUMP_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$HEXDUMP_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the hexdump distribution])
  dnl fi

  dnl # --with-hexdump -> add include path
  dnl PHP_ADD_INCLUDE($HEXDUMP_DIR/include)

  dnl # --with-hexdump -> check for lib and symbol presence
  dnl LIBNAME=hexdump # you may want to change this
  dnl LIBSYMBOL=hexdump # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $HEXDUMP_DIR/lib, HEXDUMP_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_HEXDUMPLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong hexdump lib version or lib not found])
  dnl ],[
  dnl   -L$HEXDUMP_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(HEXDUMP_SHARED_LIBADD)

  PHP_NEW_EXTENSION(hexdump, hexdump.c, $ext_shared)
fi
