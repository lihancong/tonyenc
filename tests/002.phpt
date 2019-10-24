--TEST--
tonyenc_encode() exists test
--SKIPIF--
<?php
if (!function_exists('tonyenc_encode')) {
	echo 'skip';
}
?>
--FILE--
<?php
	echo "The function tonyenc_encode is exists!";
?>
--EXPECT--
The function tonyenc_encode is exists!