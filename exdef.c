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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_exdef.h"


ZEND_DECLARE_MODULE_GLOBALS(exdef)

/* True global resources - no need for thread safety here */
static int le_exdef;


/* {{{ exdef_str_replace_in_subject
 * can only replace none-a-z chars.
 * a-z automaticly replace to A-Z.
 */
static zval *exdef_str_replace_in_subject(char search, char replace, zval *subject)
{
	int len = 0;
	int pos = 0;
	zval *result;

	result = subject;
	zval_copy_ctor(result);

	len = Z_STRLEN_P(result);



	do {
		if (len == 0) {
			break;
		}
		if (Z_STRVAL_P(result)[pos] > 96 && Z_STRVAL_P(result)[pos] < 123) {
			Z_STRVAL_P(result)[pos] = Z_STRVAL_P(result)[pos] - 32;
		}
		else if (Z_STRVAL_P(result)[pos] == search) {
			Z_STRVAL_P(result)[pos] = replace;
		}
	} while(len - pos++);

	return result;
}
/* }}} */



/* {{{ */
static void php_exdef_register_constant(zval *key, zval *value) {

	char *skey;

	skey = estrndup(Z_STRVAL_P(key), Z_STRLEN_P(key));

	switch(Z_TYPE_P(value)) {
		case IS_LONG:
			zend_register_long_constant(skey, strlen(skey), Z_LVAL_P(value), CONST_CS|CONST_PERSISTENT, EXDEF_G(gm_number));
			break;

		case _IS_BOOL:
		case IS_TRUE:
		case IS_FALSE:
			zend_register_bool_constant(skey, strlen(skey), Z_LVAL_P(value), CONST_CS|CONST_PERSISTENT, EXDEF_G(gm_number));
			break;

		case IS_DOUBLE:
			zend_register_double_constant(skey, strlen(skey), Z_DVAL_P(value), CONST_CS|CONST_PERSISTENT, EXDEF_G(gm_number));
			break;

		case IS_NULL:
			zend_register_null_constant(skey, strlen(skey), CONST_CS|CONST_PERSISTENT, EXDEF_G(gm_number));
			break;

		case IS_STRING:
		default:
			zend_register_stringl_constant(skey, strlen(skey), Z_STRVAL_P(value), Z_STRLEN_P(value), CONST_CS|CONST_PERSISTENT, EXDEF_G(gm_number));
			break;
	}


}
/* }}} */

static void php_exdef_simple_parser_cb(zval *key, zval *value, zval *index, int callback_type, void *arg) /* {{{ */ {
	char *skey;
	zval *npkey ;

	char csearch = '.';
	char creplace = '\\';

	skey = estrndup(Z_STRVAL_P(key), Z_STRLEN_P(key));

	if (value == NULL) {
		return;
	}
	if (callback_type == ZEND_INI_PARSER_ENTRY) {

		npkey = exdef_str_replace_in_subject(csearch, creplace, key);
		php_exdef_register_constant(npkey, value);
		efree(skey);
	} else {
	}
}
/* }}} */

static void php_exdef_ini_parser_cb(zval *key, zval *value, zval *index, int callback_type, void *arg) /* {{{ */ {

	if (EXDEF_G(parse_err)) {
		return;
	}

	if (callback_type == ZEND_INI_PARSER_SECTION) {

	} else if (value) {
		php_exdef_simple_parser_cb(key, value, index, callback_type, NULL);
	}
}
/* }}} */



/* {{{ php_exdef_init_globals
 */
static void php_exdef_init_globals(zend_exdef_globals *exdef_globals)
{
	exdef_globals->ini_file = "";
}
/* }}} */



/* {{{ PHP_INI
 */

PHP_INI_BEGIN()
STD_PHP_INI_ENTRY("exdef.ini_file",      "", PHP_INI_SYSTEM, OnUpdateString, ini_file, zend_exdef_globals, exdef_globals)
PHP_INI_END()

/* }}} */




/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(exdef)
{
	zval result;
	int i;
	struct stat sb;
	zend_file_handle fh = {0};

	ZEND_INIT_MODULE_GLOBALS(exdef, php_exdef_init_globals, php_exdef_shutdown_globals);
	REGISTER_INI_ENTRIES();

	EXDEF_G(gm_number) = module_number;

	if (VCWD_STAT(EXDEF_G(ini_file), &sb) == 0) {
		if (S_ISREG(sb.st_mode)) {
			if ((fh.handle.fp = VCWD_FOPEN(EXDEF_G(ini_file), "r"))) {
				fh.filename = EXDEF_G(ini_file);
				fh.type = ZEND_HANDLE_FP;
				EXDEF_G(parse_err) = 0;
				if (zend_parse_ini_file(&fh, 0, 0/* ZEND_INI_SCANNER_NORMAL */
						, php_exdef_ini_parser_cb, (void *)&result) == FAILURE || EXDEF_G(parse_err)) {
					if (!EXDEF_G(parse_err)) {
						php_error(E_WARNING, "Parsing '%s' failed", EXDEF_G(ini_file));
					}
					EXDEF_G(parse_err) = 0;
				}
			}
		}
	} else {
		php_error(E_ERROR, "Could not stat '%s'", EXDEF_G(ini_file));
	}
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(exdef)
{
	UNREGISTER_INI_ENTRIES();

	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(exdef)
{
#if defined(COMPILE_DL_EXDEF) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(exdef)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(exdef)
{
	php_info_print_table_start();
	php_info_print_table_row(2, "exdef support", "enabled");
	php_info_print_table_row(2, "exdef version", PHP_EXDEF_VERSION);
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}
/* }}} */

/* {{{ exdef_functions[]
 *
 * Every user visible function must have an entry in exdef_functions[].
 */
const zend_function_entry exdef_functions[] = {
		PHP_FE_END
};
/* }}} */

/* {{{ exdef_module_entry
 */
zend_module_entry exdef_module_entry = {
		STANDARD_MODULE_HEADER,
		"exdef",
		exdef_functions,
		PHP_MINIT(exdef),
		PHP_MSHUTDOWN(exdef), //PHP_MSHUTDOWN(exdef),
		PHP_RINIT(exdef), //PHP_RINIT(exdef),		/* Replace with NULL if there's nothing to do at request start */
		PHP_RSHUTDOWN(exdef), //PHP_RSHUTDOWN(exdef),	/* Replace with NULL if there's nothing to do at request end */
		PHP_MINFO(exdef), //PHP_MINFO(exdef),
		PHP_EXDEF_VERSION,
		STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_EXDEF
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE();
#endif
ZEND_GET_MODULE(exdef)
#endif




/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
