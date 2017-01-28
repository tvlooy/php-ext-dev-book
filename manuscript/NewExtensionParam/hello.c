/* hello.c */

/* include PHP API */
#include "php.h"

/* this module's header file */
#include "php_hello.h"

/* provide info for the reflection API */
ZEND_BEGIN_ARG_INFO(arginfo_hello, 0)
  ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO();

/* define the function we want to add */
zend_function_entry hello_functions[] = {
  PHP_FE(hello, arginfo_hello)
  PHP_FE_END
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

/* function hello(string $name): bool */
PHP_FUNCTION(hello) {
  zend_string *name;

  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_STR(name)
  ZEND_PARSE_PARAMETERS_END();

  php_printf("Hello %s\n", ZSTR_VAL(name));

  RETURN_TRUE;
}
