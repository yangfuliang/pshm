/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2016 The PHP Group                                |
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

#include <sys/types.h>
#include <sys/ipc.h>  
#include <sys/shm.h>

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_pshm.h"

/* If you declare any globals in php_pshm.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(pshm)
*/

/* True global resources - no need for thread safety here */
static int le_pshm;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("pshm.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_pshm_globals, pshm_globals)
    STD_PHP_INI_ENTRY("pshm.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_pshm_globals, pshm_globals)
PHP_INI_END()
*/
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_pshm_compiled(string arg)
   Return a string to confirm that the module is compiled in */

/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/


/* {{{ php_pshm_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_pshm_init_globals(zend_pshm_globals *pshm_globals)
{
	pshm_globals->global_value = 0;
	pshm_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(pshm)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(pshm)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(pshm)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(pshm)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(pshm)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "pshm support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

ZEND_FUNCTION(shm_write)
{
	long key, buf_size;
	int shmid, value_len;
	char *shmptr, *value;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lls",&key, &buf_size, &value, &value_len) == FAILURE)
	{
		RETURN_NULL();
	}

	shmid = shmget(key, buf_size, IPC_CREAT);
	if (shmid == -1) RETURN_FALSE;

	shmptr = shmat(shmid, 0, 0);
	if (shmptr == (void *)-1) RETURN_FALSE;

	strcpy(shmptr, value);
	shmdt(shmptr);

	RETURN_TRUE;
}

ZEND_FUNCTION(shm_read)
{
	long key;
	int shmid;
	char *shmptr;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l",&key) == FAILURE)
	{
		RETURN_NULL();
	}

	shmid = shmget(key, 0, 0);
	if (shmid == -1) RETURN_FALSE;

	shmptr = shmat(shmid, 0, 0);
	if (shmptr == (void *)-1) RETURN_FALSE;

	RETURN_STRINGL(shmptr, strlen(shmptr), 1);
}

ZEND_FUNCTION(shm_clear)
{
	long key;
	int shmid;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l",&key) == FAILURE)
	{
		RETURN_NULL();
	}

	shmid = shmget(key, 0, 0);
	if (shmid == -1) RETURN_FALSE;

	if (shmctl(shmid, IPC_RMID, 0) == 0)
	{
		RETURN_TRUE;
	}

	RETURN_FALSE;
}

/* {{{ pshm_functions[]
 *
 * Every user visible function must have an entry in pshm_functions[].
 */
const zend_function_entry pshm_functions[] = {
	PHP_FE(shm_write,	NULL)
	PHP_FE(shm_read,	NULL)
	PHP_FE(shm_clear,	NULL)

	PHP_FE_END	/* Must be the last line in pshm_functions[] */
};
/* }}} */

/* {{{ pshm_module_entry
 */
zend_module_entry pshm_module_entry = {
	STANDARD_MODULE_HEADER,
	"pshm",
	pshm_functions,
	PHP_MINIT(pshm),
	PHP_MSHUTDOWN(pshm),
	PHP_RINIT(pshm),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(pshm),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(pshm),
	PHP_PSHM_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_PSHM
ZEND_GET_MODULE(pshm)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
