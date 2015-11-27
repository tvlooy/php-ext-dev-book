# Function input and output

Normal functions have input and output, let's add these.

## Return value

First we make our own function return success:

```c
/* php_hello.h */

PHP_FUNCTION(hello) {
  php_printf("Hello world!\n");
  RETURN_BOOL(true);
}
```

You could also return "Hello world" instead of print it. There are functions for
various data types:

```c
RETURN_STRING("Hello world!\n");
RETURN_LONG(42);
RETURN_DOUBLE(3.1415926535);
RETURN_BOOL(true);
RETURN_NULL();
```

Now clean the build and re-compile the module:

```bash
make clean all
make
```

Note that instead of RETURN_BOOL(), you can also use the RETURN_TRUE and
RETURN_FALSE macros (withhout parentheses). Besides that there are also these
return types available: ```RESOURCE```, ```ARRAY``` (or ```HASH```) and ```OBJECT```.

## Input argument

Now that the function returns something. Let's accept an input argument. Change
the function entry to this:

```c
ZEND_BEGIN_ARG_INFO(hello_arginfo, 0)
  ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO();

zend_function_entry hello_functions[] = {
  PHP_FE(hello, hello_arginfo)
  { NULL, NULL, NULL }
};
```

In the function, parse the parameter:

```c
/* function hello(string $name): bool */
PHP_FUNCTION(hello) {
  char *name = NULL;
  size_t name_len = 0;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name, &name_len) == FAILURE) {
      return;
  }

  php_printf("Hello %s\n", name);
  RETURN_TRUE;
}
```
