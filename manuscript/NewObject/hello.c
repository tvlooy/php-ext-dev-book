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
  hello_functions,
  PHP_MINIT(hello),
  PHP_MSHUTDOWN(hello),
  NULL, NULL, NULL,
  PHP_HELLO_VERSION,
  STANDARD_MODULE_PROPERTIES
};

PHP_INI_BEGIN()
PHP_INI_ENTRY("hello.yell", "0", PHP_INI_ALL, NULL)
PHP_INI_END()

zend_class_entry *php_animal_human_ce;

PHP_MINIT_FUNCTION(hello) {
    REGISTER_INI_ENTRIES();

    /* namespace Earth\Animal { class Human {} } */
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, ZEND_NS_NAME("Earth\\Animal", "Human"), hello_functions);
    php_animal_human_ce = zend_register_internal_class(&ce);

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
  char *name = NULL;
  size_t name_len = 0;

  if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &name, &name_len) == FAILURE) {
      return;
  }

  if (INI_BOOL("hello.yell") == 1) {
      while (name[i]) {
          name[i] = toupper(name[i]);
           i++;
      }
      php_printf("HELLO %s!\n", name);
  } else {
      php_printf("Hello %s\n", name);
  }

  RETURN_TRUE;
}
