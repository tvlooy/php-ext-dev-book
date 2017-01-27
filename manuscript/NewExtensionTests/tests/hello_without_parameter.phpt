--TEST--
hello() function - basic test for hello() without parameter
--FILE--
<?php
hello();
?>
--EXPECTF--
Warning: hello() expects exactly 1 parameter, 0 given in %s on line %d
