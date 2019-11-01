--TEST--
Check if tonyenc is loaded
--SKIPIF--
<?php
if (!extension_loaded('tonyenc')) {
	echo 'skip';
}
?>
--FILE--
<?php
echo 'The extension "tonyenc" is available';
?>
--EXPECT--
The extension "tonyenc" is available
