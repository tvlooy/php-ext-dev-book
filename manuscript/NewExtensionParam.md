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
RETURN_FALSE macros (without parentheses). Besides that there are also these
return types available: ```RESOURCE```, ```ARRAY``` (or ```HASH```) and ```OBJECT```.

## Input argument

Now that the function returns something. Let's accept an input argument. Change
the function entry to this:

```c
ZEND_BEGIN_ARG_INFO(arginfo_hello, 0)
  ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO();

zend_function_entry hello_functions[] = {
  PHP_FE(hello, arginfo_hello)
  PHP_FE_END
};
```

In the function, parse the parameter. I will give you 2 examples. The first is
the old way, that is still works and is in use in a lot of places. The second
is the new way.

Old way:

```c
/* function hello(string $name): bool */
PHP_FUNCTION(hello) {
  char *name = NULL;
  size_t name_len = 0;

  if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &name, &name_len) == FAILURE) {
    return;
  }

  php_printf("Hello %s\n", name);

  RETURN_TRUE;
}
```

New way:

```c
/* function hello(string $name): bool */
PHP_FUNCTION(hello) {
  zend_string *name;

  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_STR(name)
  ZEND_PARSE_PARAMETERS_END();

  php_printf("Hello %s\n", ZSTR_VAL(name));

  RETURN_TRUE;
}
```

The ```macro ZEND_PARSE_PARAMETERS_START``` has 2 parameters. The minumum
and maximum number of arguments the function accepts.

Note that the ```arginfo_hello``` is used by the reflection API. You could omit
it but that would mean reflection would not work on your function. You can dump
reflection information of your own module with:

```bash
php -dextension=modules/hello.so --re hello
```

This would output something like:

```
Extension [ <persistent> extension #48 hello version 0.1 ] {

  - Functions {
    Function [ <internal:hello> function hello ] {

      - Parameters [1] {
        Parameter #0 [ <required> $name ]
      }
    }
  }
}
```
