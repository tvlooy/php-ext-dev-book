/* hello.c */

/* include PHP API */
#include "php.h"
#include "php_ini.h"

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
  hello_functions,        /* Function entries */
  PHP_MINIT(hello),       /* Module init */
  PHP_MSHUTDOWN(hello),   /* Module shutdown */
  NULL,                   /* Request init */
  NULL,                   /* Request shutdown */
  NULL,                   /* Module information */
  PHP_HELLO_VERSION,
  STANDARD_MODULE_PROPERTIES
};

PHP_INI_BEGIN()
PHP_INI_ENTRY("hello.yell", "0", PHP_INI_ALL, NULL)
PHP_INI_END()

PHP_MINIT_FUNCTION(hello) {
  REGISTER_INI_ENTRIES();
  return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(hello) {
  UNREGISTER_INI_ENTRIES();
  return SUCCESS;
}

/* install module */
ZEND_GET_MODULE(hello)

/* function hello(string $name): bool */
PHP_FUNCTION(hello) {
  int i = 0;
  zend_string *name;

  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_STR(name)
  ZEND_PARSE_PARAMETERS_END();

  if (INI_BOOL("hello.yell") == 1) {
    while (name->val[i]) {
      name->val[i] = toupper(name->val[i]);
      i++;
    }
    php_printf("HELLO %s!\n",  ZSTR_VAL(name));
  } else {
    php_printf("Hello %s\n",  ZSTR_VAL(name));
  }

  RETURN_TRUE;
}
