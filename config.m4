dnl $Id$
dnl config.m4 for extension tonyenc

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(tonyenc, for tonyenc support,
dnl Make sure that the comment is aligned:
dnl [  --with-tonyenc             Include tonyenc support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(tonyenc, whether to enable tonyenc support,
Make sure that the comment is aligned:
[  --enable-tonyenc           Enable tonyenc support])

if test "$PHP_TONYENC" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-tonyenc -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/tonyenc.h"  # you most likely want to change this
  dnl if test -r $PHP_TONYENC/$SEARCH_FOR; then # path given as parameter
  dnl   TONYENC_DIR=$PHP_TONYENC
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for tonyenc files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       TONYENC_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$TONYENC_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the tonyenc distribution])
  dnl fi

  dnl # --with-tonyenc -> add include path
  dnl PHP_ADD_INCLUDE($TONYENC_DIR/include)

  dnl # --with-tonyenc -> check for lib and symbol presence
  dnl LIBNAME=tonyenc # you may want to change this
  dnl LIBSYMBOL=tonyenc # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $TONYENC_DIR/$PHP_LIBDIR, TONYENC_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_TONYENCLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong tonyenc lib version or lib not found])
  dnl ],[
  dnl   -L$TONYENC_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(TONYENC_SHARED_LIBADD)

  PHP_NEW_EXTENSION(tonyenc, tonyenc.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
