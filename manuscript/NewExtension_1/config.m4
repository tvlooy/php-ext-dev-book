dnl config.m4

PHP_ARG_ENABLE(hello, whether to enable hello extension,
[  --enable-hello          Enable hello extension])

if test "$PHP_HELLO" != "no"; then
  PHP_NEW_EXTENSION(hello, hello.c, $ext_shared)
fi

