# INI settings

Let's say we want to configure with an INI setting ```hello.yell``` that defines
whether or not our hello function has to yell.

In the header, add two prototypes to the header:

```c
PHP_MINIT_FUNCTION(hello);
PHP_MSHUTDOWN_FUNCTION(hello);
```

Use a init and shutdown function in the module_entry. These will be called
during the startup (MINIT) of the SAPI and during the shutdown (MSHUTDOWN).

```c
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
```

These two functions look like this:

```c
PHP_MINIT_FUNCTION(hello) {
  REGISTER_INI_ENTRIES();
  return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(hello) {
  UNREGISTER_INI_ENTRIES();
  return SUCCESS;
}
```

Now we are ready to actually register the INI setting:

```c
PHP_INI_BEGIN()
PHP_INI_ENTRY("hello.yell", "0", PHP_INI_ALL, NULL)
PHP_INI_END()
```

And use the INI setting in our function:

```c
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
```
