<?php
    session_name('EEADMIN');
    session_start();

	$debug = 1;
	require_once("rmv_db.inc");
	require_once("controls.inc");
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<HTML>
<HEAD>
<meta charset="UTF-8">
<TITLE></TITLE>
<LINK HREF="controls.css" REL="stylesheet" TYPE="text/css" />
<SCRIPT LANGUAGE='JavaScript1.2' SRC='logger.js' TYPE='text/javascript'></SCRIPT>
<SCRIPT LANGUAGE='JavaScript1.2' TYPE='text/javascript'>
<?php process_control_php();  ?>
//This call is placed here incase the php code needs to output javascript code
</SCRIPT>
</HEAD>
<BODY onload="setup();">
<form name="ZFORM" action="controls.php" method="post" enctype="multipart/form-data"> 
	<input type="hidden" name="cmd" value="">
	<input type="hidden" name="logname" value="">
	<input type="hidden" name="param1" value="">
	<input type="hidden" name="param2" value="">
	<input type="hidden" name="param3" value="">
	<input type="hidden" name="dspopts" value=127">
</form>
<DIV ID="CONTROLS_CONTAINER">
	<?PHP output_controls(); ?>
</DIV>
</BODY>
</HTML>
