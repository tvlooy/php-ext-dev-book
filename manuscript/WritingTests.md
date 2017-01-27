# Writing tests

## Your first test

PHP is tested with PHPT files. These are files that contain PHP code to check if
functionality behaves as expected. See http://qa.php.net/write-test.php for more info.

Let's write a test for our hello function in the hello extension. At this point
the function is defined like this:

```php
function hello(string $name): bool;
```

Create a file ```tests/hello.phpt``` with this content:

```text
--TEST--
hello() function - basic test for hello()
--FILE--
<?php
hello('world');
?>
--EXPECT--
Hello world
```

To run the test just type ```make test```. It will run all tests and print a report:

```text
---------------------------------------------------------------------
Number of tests :    1                 1
Tests skipped   :    0 (  0.0%) --------
Tests warned    :    0 (  0.0%) (  0.0%)
Tests failed    :    0 (  0.0%) (  0.0%)
Expected fail   :    0 (  0.0%) (  0.0%)
Tests passed    :    1 (100.0%) (100.0%)
---------------------------------------------------------------------
```

If a test would fail, the test results will be dumped in the directory of the test
file. There will be a lot of information:

  - hello.log The test log
  - hello.exp What was expected
  - hello.out The actual result
  - hello.diff The difference between the expected and actual result
  - hello.php A PHP snippet of the failing test
  - hello.sh A script to run the PHP snippet of the failing test

## More tests

You can test more than one case in a PHPT file, or you can create multiple PHPT
files for different features.

```text
--TEST--
hello() function - basic test for hello()
--FILE--
<?php
hello('world');
hello();
?>
--EXPECTF--
Hello world

Warning: hello() expects exactly 1 parameter, 0 given in %s on line %d
```

Note that in the first test example I used ```EXPECT``` and in the second I
use ```EXPECTF``` to support the ```%s``` and ```%d``` variables.

You can get a lot of examples of how to write test cases from the ext directory
of PHP.

If you ever encounter a bug in PHP or an extension, you have to write a specific
test for it. Put the ID of the bug in the name of the text, eg: bug12345.phpt.
This makes it easy for the developers to reproduce the bug and all future versions
of PHP or the extension will keep test for this specific bug, making sure it will
never happen again.

If you don't want the script to ask "Do you want to send this report now? [Yns]:"
run it with ```NO_INTERACTION``` set. Behind the scenes the ```run-tests.php```
that is in the root of the php-scr directory will be used for make test. Look at
that file for all details.

```bash
NO_INTERACTION=1 make test
```

If you have multiple tests but only want to run one or some of them, you can
specify the test like this:

```bash
make test TESTS=tests/hello_with_parameter.phpt
```
