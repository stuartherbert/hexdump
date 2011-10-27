#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
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
	char asciiBuffer[18]; /* helper buffer */

	/* a buffer of hex characters */
	char * hexchars = "0123456789ABCDEF";

	/* make sure the buffer is NULL-terminated!! */
	asciiBuffer[16] = '\n';
	asciiBuffer[17] = 0;

	/* 
	 * converted length is ...
	 *
	 * 77 bytes per line
	 * (
	 *   11 for the address display
	 *   16 * 3 for the hex chars 
	 *    1 for the extra whitespace mid line
	 *   16 for the ascii display
	 *    1 for the line ending
	 * )
	 * multiplied by the number of lines to display
	 * +1 at the very end for NULL termination
	 */

	int line_len = 77;
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
		int asciiBufferIndex = i % 16;

		/* step 1:
		 *
		 * are we at the start of the line?
		 */
		if (0 == asciiBufferIndex)
		{
			sprintf(dp, "[%08x] ", i);
			dp += 11;
		}

		/* step 2:
		 *
		 * work out the ASCII to show
		 */
		if (*sp < 32 || *sp > 126)
		{
			asciiBuffer[asciiBufferIndex] = '.';
		}
		else
		{
			asciiBuffer[asciiBufferIndex] = *sp;
		}

		/* step 3:
		 *
		 * we need the hex, next
		 */
		*dp = hexchars[(*sp >> 4) & 0xF];
		dp++;
		*dp = hexchars[*sp & 0xF];
		dp++;
		*dp = ' ';
		dp++;

		/* step 4:
		 *
		 * are we in the middle of a line?
		 */
		if (7 == asciiBufferIndex)
		{
			*dp = ' ';
			dp++;
		}

		/* step 4:
		 *
		 * are we at the end of a line?
		 */
		if (15 == asciiBufferIndex)
		{
			/* yes ... append the ascii buffer! */
			strncat(dp, &asciiBuffer[0], sizeof(asciiBuffer));
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

		sprintf(dp, "%s\n", &asciiBuffer[0]);
		dp += sizeof(asciiBuffer);
	}

	/*
	printf("%s\n", *dest);
	printf("len: %d; alloc'd size: %d\n", dp - *dest, dest_len);
	 */

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
