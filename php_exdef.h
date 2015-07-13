/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2015 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_EXDEF_H
#define PHP_EXDEF_H

extern zend_module_entry exdef_module_entry;
#define phpext_exdef_ptr &exdef_module_entry

#define PHP_EXDEF_VERSION "0.1.0"

#ifdef PHP_WIN32
#	define PHP_EXDEF_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_EXDEF_API __attribute__ ((visibility("default")))
#else
#	define PHP_EXDEF_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif


#ifdef EXDEF_DEBUG
#undef EXDEF_DEBUG
#define EXDEF_DEBUG(m) fprintf(stderr, "%s\n", m);
#else
#define EXDEF_DEBUG(m)
#endif


PHP_MINIT_FUNCTION(exdef);
PHP_RINIT_FUNCTION(exdef);
PHP_MSHUTDOWN_FUNCTION(exdef);
PHP_RSHUTDOWN_FUNCTION(exdef);
PHP_MINFO_FUNCTION(exdef);



/* {{{ zend_exdef_globals */
ZEND_BEGIN_MODULE_GLOBALS(exdef)

char *ini_file;
int   parse_err;
int   gm_number;

ZEND_END_MODULE_GLOBALS(exdef)
/* }}} */

/* {{{ extern exdef_globals */
ZEND_EXTERN_MODULE_GLOBALS(exdef)
/* }}} */


#ifdef ZTS
#define EXDEF_G(v) TSRMG(exdef_globals_id, zend_exdef_globals *, v)
#else
#define EXDEF_G(v) (exdef_globals.v)
#endif

#ifdef ZTS
#define EXDEF_G(v) ZEND_TSRMG(exdef_globals_id, zend_exdef_globals *, v)
#ifdef COMPILE_DL_EXDEF
ZEND_TSRMLS_CACHE_EXTERN();
#endif
#else
#define EXDEF_G(v) (exdef_globals.v)
#endif

#endif	/* PHP_EXDEF_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
