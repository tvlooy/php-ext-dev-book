# Your first simple extension

Extensions should follow the PHP coding standards. You can find these at [php-src CODING_STANDARDS](https://github.com/php/php-src/blob/master/CODING_STANDARDS).

## Hello world

Make a directory ```hello``` and put these files in it:

```m4
dnl config.m4

PHP_ARG_ENABLE(hello, whether to enable hello extension,
[  --enable-hello          Enable hello extension])

if test "$PHP_HELLO" != "no"; then
  PHP_NEW_EXTENSION(hello, hello.c, $ext_shared)
fi
```

The ```config.m4``` file enables an argument ```--enable-hello``` to build the
extension with PHP. You have to align the flag or it will not be in line when
you ```./configure --help```. In these examples I will always build extensions
separately.

```c
/* php_hello.h */

#define PHP_HELLO_EXTNAME "hello"
#define PHP_HELLO_VERSION "0.1"

extern zend_module_entry hello_module_entry;
#define phpext_hello_ptr &check_hello_entry

PHP_FUNCTION(hello);
```

The ```extern``` and ```define``` are needed to make the module available if you
would like to compile it into PHP with ```--enable-hello``` instead of as a module.

```c
/* hello.c */

/* include PHP API */
#include "php.h"

/* this module's header file */
#include "php_hello.h"

/* provide info for the reflection API */
ZEND_BEGIN_ARG_INFO(arginfo_hello, 0)
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

Or, you can not install it yet and just load it from the build directory:

```bash
php -a -dextension=modules/hello.so
php > hello();
Hello world!
```

Or you can test your extension with ```-r```, I'll show a variation:

```bash
php -dextension_dir=modules \
    -dextension=hello.so \
    -n \
    -r 'hello();'
Hello world!
```

The optional ```-n``` flag makes sure you don't load the default ini file.

I you are done with your development and want to commit your code to Git, you will
notice that the directory is full of build files you don't need. Clean up these with:

```phpize --clean```

## More functions

Adding more functions is a matter of adding them to the hello_functions, writing
the actual function and exporting it in the header.

```c
/* hello.c */

/* provide info for the reflection API */
ZEND_BEGIN_ARG_INFO(arginfo_hi_world, 0)
ZEND_END_ARG_INFO();
ZEND_BEGIN_ARG_INFO(arginfo_bye_world, 0)
ZEND_END_ARG_INFO();

zend_function_entry hello_functions[] = {
  PHP_FE(hi_world, arginfo_hi_world)
  PHP_FE(bye_world, arginfo_bye_world)
  PHP_FE_END
};

PHP_FUNCTION(hi_world) {
  php_printf("Hello world!\n");
}

PHP_FUNCTION(bye_world) {
  php_printf("Goodbye world!\n");
}
```

```c
/* php_hello.h */

PHP_FUNCTION(hi_world);
PHP_FUNCTION(bye_world);
```
