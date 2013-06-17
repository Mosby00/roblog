<?php
    session_name('EEADMIN');
    session_start();

	$debug = 1;
	require_once("rmv_db.inc");
	require_once("work.inc");
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<HTML>
<HEAD>
<meta charset="UTF-8">
<TITLE></TITLE>
<LINK HREF="controls.css" REL="stylesheet" TYPE="text/css" />
<SCRIPT LANGUAGE='JavaScript1.2' SRC='work.js' TYPE='text/javascript'></SCRIPT>
<SCRIPT LANGUAGE='JavaScript1.2' TYPE='text/javascript'>
<?php process_work_php();  ?>
//This call is placed here incase the php code needs to output javascript code
</SCRIPT>
</HEAD>
<BODY onload="setup();" ID="workbody">
<form name="WFORM" action="work.php" method="post" enctype="multipart/form-data"> 
	<input type="hidden" name="cmd" value="<?PHP echo $curcmd; ?>">
	<input type="hidden" name="subcmd" value="">
	<input type="hidden" name="logname" value="<?PHP echo $curlog; ?>">
	<input type="hidden" name="curfile" value="<?PHP echo $curlogdata['curfile']; ?>">
	<input type="hidden" name="startline" value="<?PHP echo get_start_line(); ?>">
	<input type="hidden" name="forward" value="0">
	<input type="hidden" name="backward" value="0">
	<input type="hidden" name="increment" value="0">
	<input type="hidden" name="linecount" value="<?PHP echo $curlogdata['linecount']; ?>">
	<input type="hidden" name="dsplines" value="<?PHP echo $curlogdata['dsplines']; ?>">
	<input type="hidden" name="dspopts" value="<?PHP echo $curlogdata['dspopts']; ?>">
</form>
<DIV ID="WORKAREA"><?PHP output_work_area(); ?></DIV>
</BODY>
</HTML>
