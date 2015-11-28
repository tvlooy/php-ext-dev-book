/* hello.c */

/* include PHP API */
#include "php.h"

/* this module's header file */
#include "php_hello.h"

/* define the function we want to add */
zend_function_entry hello_functions[] = {
  PHP_FE(hello, NULL)
  { NULL, NULL, NULL }
};

/* "hello_functions" refers to the struct defined above */
zend_module_entry hello_module_entry = {
  STANDARD_MODULE_HEADER,
  PHP_HELLO_EXTNAME,
  hello_functions,
  NULL, NULL, NULL, NULL, NULL,
  PHP_HELLO_VERSION,
  STANDARD_MODULE_PROPERTIES
};

/* install module */
ZEND_GET_MODULE(hello)

PHP_FUNCTION(hello) {
  /* php_printf is PHP's version of printf */
  php_printf("Hello world!\n");
}
