# Your first simple extension

Extensions should follow the PHP codings standards. You can find these at
[https://github.com/php/php-src/blob/master/CODING_STANDARDS](https://github.com/php/php-src/blob/master/CODING_STANDARDS).

## Hello world

Make a directory ```hello``` and put these files in it:

```m4
dnl config.m4

PHP_ARG_ENABLE(hello, whether to enable hello extension,
[  --enable-hello          Enable hello extension])

if test "$PHP_HELLO" != "no"; then
    PHP_NEW_EXTENSION(hello, php_hello.c, $ext_shared)
fi
```

```c
/* php_hello.h */

#define PHP_HELLO_EXTNAME "hello"
#define PHP_HELLO_VERSION "0.1"

PHP_FUNCTION(hello);
```

```c
/* php_hello.c */

/* include PHP API */
#include <php.h>

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
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  PHP_HELLO_VERSION,
  STANDARD_MODULE_PROPERTIES
};

/* install module */
ZEND_GET_MODULE(hello)

PHP_FUNCTION(hello) {
  /* php_printf is PHP's version of printf */
  php_printf("Hello world!\n");
}
```

Build the extension with:

```bash
phpize
./configure
make
```

You can test your extension by installing it and loading if with an ini directive:

```bash
make install
echo "extension=hello.so" > /etc/php7/conf.d/hello.ini
ln -s /etc/php7/conf.d/hello.ini /etc/php7/cli/conf.d/hello.ini
ln -s /etc/php7/conf.d/hello.ini /etc/php7/fpm/conf.d/hello.ini
```

Or, you can not install it and just load it from the build directory:

```bash
php -a -dextension=modules/hello.so
php > hello();
Hello world!
```
