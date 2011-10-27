#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_xdiff.h"

zend_function_entry hexdump_functions[] = {
	PHP_FE(hex_dump, NULL)
	{NULL, NULL, NULL}
};

zend_module_entry hexdump_module_entry = {
	STANDARD_MODULE_HEADER,
	"hexdump",
	hexdump_functions,
	PHP_MINIT(hexdump),
	NULL,
	NULL,
	NULL,
	PHP_MINFO(hexdump),
	PHP_HEXDUMP_VERSION,
	STANDARD_MODULE_PROPERTIES
};

PHP_MINIT_FUNCTION(hexdump)
{
	REGISTER_LONG_CONSTANT("HEXDUMP_VERSION", PHP_HEXDUMP_VERSION, CONST_CS | CONST_PERSISTENT);
	return SUCCESS;
}

PHP_MINFO_FUNCTION(hexdump)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "hexdump support", "enabled");
	php_info_print_table_row(2, "extension version", PHP_HEXDUMP_VERSION);
	php_info_print_table_end();
}

PHP_FUNCTION(hex_dump)
{
	char *string1;
	int size;

	if (ZEND_NUM_ARGS() != 1 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &string1, &size) == FAILURE)
	{
		WRONG_PARAM_COUNT;
	}

	RETVAL_TRUE;

	return;
}
