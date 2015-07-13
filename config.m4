dnl $Id$
dnl config.m4 for extension exdef

PHP_ARG_ENABLE(exdef, whether to enable exdef support,
Make sure that the comment is aligned:
[  --enable-exdef           Enable exdef support])

if test "$PHP_EXDEF" != "no"; then
  PHP_SUBST(EXDEF_SHARED_LIBADD)
  PHP_NEW_EXTENSION(exdef, exdef.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
  PHP_INSTALL_HEADERS([ext/exdef], [php_exdef.h])
fi
