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
