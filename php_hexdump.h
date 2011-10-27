#ifndef PHP_HEXDUMP_H
#define PHP_HEXDUMP_H

#define PHP_HEXDUMP_VERSION "1.0.0"

extern zend_module_entry hexdump_module_entry;
#define phpext_hexdump_ptr &hexdump_module_entry;

#if defined(PHP_WIN32) && defined(HEXDUMP_EXPORTS)
#define PHP_HEXDUMP_API __declspec(dllexport)
#else
#define PHP_HEXDUMP_API PHPAPI
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(hexdump);
PHP_MSHUTDOWN_FUNCTION(hexdump);
PHP_RINIT_FUNCTION(hexdump);
PHP_RSHUTDOWN_FUNCTION(hexdump);
PHP_MINFO_FUNCTION(hexdump);

PHP_FUNCTION(hex_dump);

#endif /* PHP_HEXDUMP_H */
