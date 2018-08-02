/**
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2017 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Tony  htmln.com   github.com/lihancong/tonyenc               |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_tonyenc.h"

#include "core.h"

/* If you declare any globals in php_tonyenc.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(tonyenc)
*/


/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("tonyenc.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_tonyenc_globals, tonyenc_globals)
    STD_PHP_INI_ENTRY("tonyenc.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_tonyenc_globals, tonyenc_globals)
PHP_INI_END()
*/
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ */
PHP_FUNCTION(tonyenc_encode)
{
    zend_string *strg;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "S", &strg) == FAILURE) {
        return;
    }

    /* If encoded */
    if (!memcmp(ZSTR_VAL(strg), tonyenc_header, sizeof(tonyenc_header))) {
        RETURN_FALSE;
    }

    char *t = emalloc(sizeof(tonyenc_header) + ZSTR_LEN(strg));
    memcpy(t, tonyenc_header, sizeof(tonyenc_header));
    if (ZSTR_LEN(strg) > 0) {
        tonyenc_encode(ZSTR_VAL(strg), ZSTR_LEN(strg));
        memcpy(t + sizeof(tonyenc_header), ZSTR_VAL(strg), ZSTR_LEN(strg));
    }

    RETURN_STR(zend_string_init(t, sizeof(tonyenc_header) + ZSTR_LEN(strg), 0));
}
/* }}} */

/*
PHP_FUNCTION(tonyenc_decode)
{
    zend_string *strg;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "S", &strg) == FAILURE) {
        return;
    }

    if (memcmp(ZSTR_VAL(strg), tonyenc_header, sizeof(tonyenc_header))) {
        RETURN_FALSE;
    }

    size_t len = ZSTR_LEN(strg) - sizeof(tonyenc_header);
    if (ZSTR_LEN(strg) > 0) {
        tonyenc_decode(ZSTR_VAL(strg) + sizeof(tonyenc_header), len);
    }

    RETURN_STR(zend_string_init(ZSTR_VAL(strg) + sizeof(tonyenc_header), len, 0));
}
*/

/* {{{ php_tonyenc_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_tonyenc_init_globals(zend_tonyenc_globals *tonyenc_globals)
{
    tonyenc_globals->global_value = 0;
    tonyenc_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(tonyenc)
{
    /* If you have INI entries, uncomment these lines
    REGISTER_INI_ENTRIES();
    */

    old_compile_file = zend_compile_file;
    zend_compile_file = cgi_compile_file;

    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(tonyenc)
{
    /* uncomment this line if you have INI entries
    UNREGISTER_INI_ENTRIES();
    */

    zend_compile_file = old_compile_file;

    return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(tonyenc)
{
#if defined(COMPILE_DL_TONYENC) && defined(ZTS)
    ZEND_TSRMLS_CACHE_UPDATE();
#endif
    return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(tonyenc)
{
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(tonyenc)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "Tonyenc Support", "enabled");
    php_info_print_table_row(2, "Version", PHP_TONYENC_VERSION);
    php_info_print_table_row(2, "Open Sourced By", "htmln.com");
    php_info_print_table_end();

    /* Remove comments if you have entries in php.ini
    DISPLAY_INI_ENTRIES();
    */
}
/* }}} */

/* {{{ tonyenc_functions[]
 *
 * Every user visible function must have an entry in tonyenc_functions[].
 */
const zend_function_entry tonyenc_functions[] = {
    PHP_FE(tonyenc_encode,	NULL)
    PHP_FE_END	/* Must be the last line in tonyenc_functions[] */
};
/* }}} */

/* {{{ tonyenc_module_entry
 */
zend_module_entry tonyenc_module_entry = {
    STANDARD_MODULE_HEADER,
    "tonyenc",
    tonyenc_functions,
    PHP_MINIT(tonyenc),
    PHP_MSHUTDOWN(tonyenc),
    NULL,   /* Replace with NULL if there's nothing to do at request start */
    NULL,	/* Replace with NULL if there's nothing to do at request end */
    PHP_MINFO(tonyenc),
    PHP_TONYENC_VERSION,
    STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_TONYENC
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(tonyenc)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
