dnl $Id$
dnl config.m4 for extension exdef

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(exdef, for exdef support,
dnl Make sure that the comment is aligned:
dnl [  --with-exdef             Include exdef support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(exdef, whether to enable exdef support,
Make sure that the comment is aligned:
[  --enable-exdef           Enable exdef support])

if test "$PHP_EXDEF" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-exdef -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/exdef.h"  # you most likely want to change this
  dnl if test -r $PHP_EXDEF/$SEARCH_FOR; then # path given as parameter
  dnl   EXDEF_DIR=$PHP_EXDEF
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for exdef files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       EXDEF_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$EXDEF_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the exdef distribution])
  dnl fi

  dnl # --with-exdef -> add include path
  dnl PHP_ADD_INCLUDE($EXDEF_DIR/include)

  dnl # --with-exdef -> check for lib and symbol presence
  dnl LIBNAME=exdef # you may want to change this
  dnl LIBSYMBOL=exdef # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $EXDEF_DIR/$PHP_LIBDIR, EXDEF_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_EXDEFLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong exdef lib version or lib not found])
  dnl ],[
  dnl   -L$EXDEF_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  PHP_SUBST(EXDEF_SHARED_LIBADD)

  PHP_NEW_EXTENSION(exdef, exdef.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
