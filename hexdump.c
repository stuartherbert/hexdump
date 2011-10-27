#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "php_smart_str.h"
#include "ext/standard/info.h"
#include "php_hexdump.h"

zend_function_entry hexdump_functions[] = {
	PHP_FE(hex_dump, NULL)
	{NULL, NULL, NULL}
};

zend_module_entry hexdump_module_entry = {
	STANDARD_MODULE_HEADER,
	"hexdump",
	hexdump_functions,
	PHP_MINIT(hexdump),
	PHP_MSHUTDOWN(hexdump),
	NULL,
	NULL,
	PHP_MINFO(hexdump),
	PHP_HEXDUMP_VERSION,
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_HEXDUMP
ZEND_GET_MODULE(hexdump)
#endif

PHP_MINIT_FUNCTION(hexdump)
{
	return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(hexdump)
{
	return SUCCESS;
}

PHP_RINIT_FUNCTION(hexdump)
{
	return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(hexdump)
{
	return SUCCESS;
}

PHP_MINFO_FUNCTION(hexdump)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "hexdump support", "enabled");
	php_info_print_table_row(2, "extension version", PHP_HEXDUMP_VERSION);
	php_info_print_table_end();
}

PHPAPI int php_hexdump(char *src, int src_len, char **dest)
{
	char *sp; /* pointer to walk through the src */
	char *dp; /* pointer to walk through the dest */
	int i = 0; /* keep count of where we are */
	int asciiBufferSize = 16 + sizeof(PHP_EOL) + 1;
	char asciiBuffer[asciiBufferSize]; /* helper buffer */

	/* make sure the buffer is NULL-terminated!! */
	asciiBuffer[asciiBufferSize - 1] = 0;

	/* 
	 * converted length is ...
	 *
	 * * 3 for the bytes
	 * +16 for the ascii display per line
	 * +7 for the address display per line
	 * +2 for the extra whitespace mid line, per line
	 * +2 for the line ending per line
	 * +1 at the very end for NULL termination
	 */

	int line_len = 72 + sizeof(PHP_EOL);
	int dest_len = (src_len / 16) * line_len;
	if (src_len % 16 > 0)
	{
		dest_len += line_len;
	}
	dest_len++;
	dp = *dest = emalloc((size_t) dest_len);
	
	/* line by line conversion */

	sp = src;
	for(i = 0; i < src_len; i++, sp++)
	{
		/* step 1:
		 *
		 * are we at the start of the line?
		 */
		if (0 == i % 16)
		{
			sprintf(dp, "[%.4x] ", i);
			dp += 7;
		}

		/* step 2:
		 *
		 * work out the ASCII to show
		 */
		if (*sp < 32 || *sp > 127)
		{
			asciiBuffer[i % 16] = '.';
		}
		else
		{
			asciiBuffer[i % 16] = *sp;
		}

		/* step 3:
		 *
		 * we need the hex, next
		 */
		sprintf(dp, "%02X ", *sp);
		dp+=3;

		/* step 4:
		 *
		 * are we in the middle of a line?
		 */
		if (7 == i % 16)
		{
			sprintf(dp, "  ");
			dp+=2;
		}

		/* step 4:
		 *
		 * are we at the end of a line?
		 */
		if (15 == i % 16)
		{
			/* yes ... append the ascii buffer! */
			sprintf(dp, "%s%s", &asciiBuffer[0], PHP_EOL);
			dp+=sizeof(asciiBuffer);
		}
	}

	/* finally ... deal with any partial line */
	if (0 != i % 16)
	{
		for (; i%16 != 0; i++)
		{
			asciiBuffer[i%16] = ' ';
			*dp = ' ';
			dp++;
			*dp = ' ';
			dp++;
			*dp = ' ';
			dp++;
		}

		sprintf(dp, "%s%s", &asciiBuffer[0], PHP_EOL);
		dp += sizeof(asciiBuffer);
	}

	printf("%s\n", *dest);
	printf("len: %d; alloc'd size: %d\n", dp - *dest, dest_len);

	/* return the final length of the buffer */
	return dp - *dest;
}

PHP_FUNCTION(hex_dump)
{
	char *arg1 = NULL;
	char *ret1 = NULL;
	int arg1_len;
	int ret1_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg1, &arg1_len) == FAILURE || arg1_len < 1)
	{
		RETURN_NULL();
	}

	ret1_len = php_hexdump(arg1, arg1_len, &ret1);

	RETURN_STRINGL(ret1, ret1_len, 0);
}
