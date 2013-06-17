<?php 
    session_name('EEADMIN');
    session_start();

	$debug = 0;
	require_once("cookie.inc");
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<HTML>
<HEAD>
<meta charset="UTF-8">
<TITLE></TITLE>
<LINK HREF="controls.css" REL="stylesheet" TYPE="text/css" />
<SCRIPT LANGUAGE='JavaScript1.2' TYPE='text/javascript'>
//If haswarning is set. There is a message to be written to WARNINGS container in work area
var haswarning = 0;
//This call is placed here incase the php code needs to output javascript code
</SCRIPT>
</HEAD>
<frameset rows="180,*">
   <frame marginwidth="1" marginheight="1" frameborder="1" scrolling="auto" name="controls" src="controls.php" />
   <frame marginwidth="1" marginheight="1" frameborder="1" scrolling="auto" name="work" src="work.php" />
</frameset>
</HTML>
