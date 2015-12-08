# Objects

Now let's see how objects are built. We'll build a ```Human``` object and
put it in the ```Earth\Animal``` namespace. Declare a class entry and initialize
it in the PHP_MINIT_FUNCTION.

```c
zend_class_entry *php_animal_human_ce;

PHP_MINIT_FUNCTION(hello) {
    REGISTER_INI_ENTRIES();

    /* namespace Earth\Animal { class Human {} } */
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, ZEND_NS_NAME("Earth\\Animal", "Human"), NULL);
    php_animal_human_ce = zend_register_internal_class(&ce TSRMLS_CC);

    return SUCCESS;
}
```

Use the hello function that we already added as a method of the Human class.
If you remember correctly, the function was added in ```hello_functions```.

```c
PHP_MINIT_FUNCTION(hello) {
    REGISTER_INI_ENTRIES();

    /* namespace Earth\Animal { class Human {} } */
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, ZEND_NS_NAME("Earth\\Animal", "Human"), hello_functions);
    php_animal_human_ce = zend_register_internal_class(&ce TSRMLS_CC);

    return SUCCESS;
}
```
