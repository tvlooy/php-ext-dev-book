/* php_hello.h */

#define PHP_HELLO_EXTNAME "hello"
#define PHP_HELLO_VERSION "0.1"

PHP_MINIT_FUNCTION(hello);
PHP_MSHUTDOWN_FUNCTION(hello);

PHP_FUNCTION(hello);
