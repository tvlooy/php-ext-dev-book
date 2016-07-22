/* php_hello.h */

#define PHP_HELLO_EXTNAME "hello"
#define PHP_HELLO_VERSION "0.1"

extern zend_module_entry hello_module_entry;
#define phpext_hello_ptr &check_hello_entry

PHP_FUNCTION(hi_world);
PHP_FUNCTION(bye_world);
