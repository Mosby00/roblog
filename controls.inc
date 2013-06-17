<?php
	require_once("logger.inc");

/*
	Required Routines 
	Present Time Period 
	Present Thread List 
	Present Classes List 
	Select Time Period to view 
*/

$curlog = "";					//If a log is selected, keep track of name
$curlogdata = Array();

function process_control_php() {
	//Start the index module now.
	init_log_index();

	if( array_key_exists("cmd",$_POST)) {
		switch( $_POST["cmd"] ) {
		case "upload":
			process_upload();
			break;
		case "viewlog":
			view_log();
			break;
		case "initlog":
			initialize_log();
			break;
		}
	}
	return 0;
}

function output_controls(){
	$hasdata = 0;

	echo "<table ID='control_table'>";
	echo "<tr style='white-space: nowrap'>"; 
		echo "<th id='menu_header_1'>Add Log</th>"; 
		echo "<th id='menu_header_2'>Previous Uploads</th>"; 
		echo "<th id='menu_header_3'>Log Statistics</th>"; 
		echo "<th id='menu_header_4'>Log Options</th>"; 
		echo "<th id='menu_header_5'>Search</th>"; 
		echo "<th id='menu_header_6'>View Options</th>"; 
	echo "</tr>";
	 
	echo "<tr>"; 
		echo "<td id='menu_item_1'>"; 
			echo "<form name='AFORM' action='controls.php' method='post' enctype='multipart/form-data'>"; 
			echo "<input type='hidden' name='cmd' value='upload'>"; 
			echo "<input type='file' name='logfname' value='file'><br>"; 
			echo "Label for the log entry<br>"; 
			echo "<input type='text' name='logname' value=''><br>"; 
			echo "<input type='submit' name='Upload' value='Upload' onclick=''><br>"; 
			echo "</form>"; 
		echo "</td>"; 
		echo "<td id='menu_item_2'>";
		output_log_list();
		echo "</td>";
		echo "<td id='menu_item_3'>";
		$hasdata = output_log_stats($hasdata);
		echo "</td>"; 
		echo "<td id='menu_item_4'>";
		output_sub_options($hasdata);
		echo "</td>"; 
		echo "<td id='menu_item_5'>";
		output_search_options($hasdata);
		echo "</td>"; 
		echo "<td id='menu_item_6'>";
		output_view_options($hasdata);
		echo "</td>"; 
	echo "</tr>";
	if( $hasdata ) {
		echo "<tr><td colspan='6'>";
		put_log_controls();
		echo "</td></tr>";
	}
	echo "</table>";
}

function put_log_controls(){
	global $curlog;

	echo "<div ID='PAGE_CONTROL_CONTAINER'>";
	echo "<form name='CTRLFORM'>";
	echo "<span class='scroller_container'><input type='button' onClick='javascript:scroll_start();' value='Start'></span>";
	echo "<span class='scroller_container'><input type='button' onClick='javascript:scroll_back_line(1);' value='< 1L'></span>";
	echo "<span class='scroller_container'><input type='button' onClick='javascript:scroll_back_line(5);' value='< 5L'></span>";
	echo "<span class='scroller_container'><input type='button' onClick='javascript:scroll_back_line(10);' value='< 10L'></span>";
	echo "<span class='scroller_container'><input type='button' onClick='javascript:scroll_back(1);' value='< 1P'></span>";
	echo "<span class='scroller_container'><input type='button' onClick='javascript:scroll_back(5);' value='< 5P'></span>";
	echo "<span class='scroller_container'><input type='button' onClick='javascript:scroll_back(10);' value='< 10P'></span>";
	echo "<span class='scroller_container'><input type='button' onClick='javascript:scroll_forward(10);' value='10P >'></span>";
	echo "<span class='scroller_container'><input type='button' onClick='javascript:scroll_forward(5);' value='5P >'></span>";
	echo "<span class='scroller_container'><input type='button' onClick='javascript:scroll_forward(1);' value='1P >'></span>";
	echo "<span class='scroller_container'><input type='button' onClick='javascript:scroll_forward_line(10);' value='10L >'></span>";
	echo "<span class='scroller_container'><input type='button' onClick='javascript:scroll_forward_line(5);' value='5L >'></span>";
	echo "<span class='scroller_container'><input type='button' onClick='javascript:scroll_forward_line(1);' value='1L >'></span>";
	echo "<span class='scroller_container'><input type='button' onClick='javascript:scroll_end();' value='File End'></span>";
	echo "<span class='scroller_container declare_text'>&nbsp;Current Log  : <b>$curlog</b>&nbsp;</span>";
	echo "<span class='scroller_container declare_text'>&nbsp;Current Line : <input type='text' name='startline' value='' size='8'>&nbsp;</span>";
	echo "<span class='scroller_container declare_text'>&nbsp;Total Lines : <input type='text' name='linecount' value='' size='8'>&nbsp;</span>";
	echo "</form>";
	echo "</div>";
}


function output_log_list(){
	global $LI;
	global $curlog;

	$llist = $LI->getLogEntries();
	ksort($llist);
	if( count($llist)) {
		echo "<form name='LFORM'>";
		echo "<select name='logent' onChange='javascript:view_log_alt(this);'>";
		echo "<option value='select'>select</option>";
	}
	foreach( $llist as $logent => $logval ) {
		if( $logent == $curlog ) {
			echo "<option value='$logent' selected>$logent</option>";
//			echo "<a class='menusel'  href='javascript:view_log(\"$logent\");' title='View this previous upload'>$logent</a><br>";
		}
		else{
			echo "<option value='$logent'>$logent</option>";
//			echo "<a class='menu'  href='javascript:view_log(\"$logent\");' title='View this previous upload'>$logent</a><br>";
		}
		/*
		if( $logent == $curlog ) {
			echo "<a class='menusel'  href='javascript:view_log(\"$logent\");' title='View this previous upload'>$logent</a><br>";
		}
		else{
			echo "<a class='menu'  href='javascript:view_log(\"$logent\");' title='View this previous upload'>$logent</a><br>";
		}
	*/ 
	}
	if( count($llist)) {
		echo "</select>";
		echo "</form>";
	}
}

function output_log_stats(){
	global $curlog;
	global $LI;
	$hasdata = 0;

	$exceptcnt = 0;
	$llist = $LI->getLogEntries();

	if( $curlog != "" ) {
		$stats = $llist[$curlog];
		if( count($stats) ) {
			//if stats exists then initialization has been run.
			$hasdata = 1;
		}
		foreach( $stats as $line ){
			if( preg_match("/Exception/", $line)) {
				$match = Array();
				preg_match("/: (\d+)/", $line,$match);
				$exceptcnt = intval($match[1]);
			}
		}
		foreach( $stats as $line ){
			echo "$line<br>";
		}
	}
	return $hasdata;
}

function output_view_options($hasdata){
	//  0x01 = time dsp
	//  0x02 = thread dsp
	//  0x04 = log level
	//  0x08 = class
	//	0x10 = msg body
	//	0x20 = Colorize
	//	0x40 = Wrap Lines

	$dspopts = 127;			//0x7f

	if( !$hasdata ) {
		echo "&nbsp;";
		return;
	}
	
	if( array_key_exists("dspopts", $_POST)) {
		if( $_POST["dspopts"] != "" ) {
//  			set_error(__FILE__,__LINE__, "In post ???? {$_POST['dspopts']}" );
			$dspopts = intval($_POST["dspopts"] );
		}
	}

//	set_error(__FILE__,__LINE__, "DSPOPTS = $dspopts" );
	echo "<form name='DOFORM'><table id='ControlsTable'>";
	echo "<tr>";
	$checked = ($dspopts & 0x01) ? " checked='checked' " : "";
	echo "<td style='width:30%'><input type='checkbox' name='check1' value='0x01' $checked>Time</td>";
	$checked = ($dspopts & 0x02) ? " checked='checked' " : "";
	echo "<td style='width:30%'><input type='checkbox' name='check2' value='0x02' $checked>Threads</td>";
	$checked = ($dspopts & 0x04) ? " checked='checked' " : "";
	echo "<td style='width:30%'><input type='checkbox' name='check4' value='0x04' $checked>Log Level</td>";
	echo "</tr>";

	echo "<tr>";
	$checked = ($dspopts & 0x08) ? " checked='checked' " : "";
	echo "<td style='width:30%'><input type='checkbox' name='check8' value='0x08' $checked>Classes</td>";
	$checked = ($dspopts & 0x10) ? " checked='checked' " : "";
	echo "<td style='width:30%'><input type='checkbox' name='check10' value='0x10' $checked>Message</td>";
	/*
	$checked = ($dspopts & ) ? " checked='checked' " : "";
	echo "<td style='width:30%'><input type='checkbox' name='' value='' $checked></td>";
	*/ 
	echo "<td style='width:30%'>&nbsp;</td>";
	echo "</tr>";

	echo "<tr>";
	$checked = ($dspopts & 0x20) ? " checked='checked' " : "";
	echo "<td style='width:30%'><input type='checkbox' name='check20' value='0x20' $checked>Colorize</td>";
	$checked = ($dspopts & 0x40) ? " checked='checked' " : "";
	echo "<td style='width:30%'><input type='checkbox' name='check40' value='0x40' $checked>Wrap</td>";
	/*
	$checked = ($dspopts & ) ? " checked='checked' " : "";
	echo "<td style='width:30%'><input type='checkbox' name='' value='' $checked></td>";
	*/ 
	echo "<td style='width:30%'>&nbsp;</td>";
	echo "</tr>";


	echo "<tr>";
	echo "<td style='width:30%'><b>Display lines</b></td>";
	echo "<td style='width:30%''><select name='dsplines'>";
	for( $idx = 10; $idx < 500; ($idx += 10) ) {
		if( $idx != 200 ) {	//default lines to view
			echo "<option value='$idx'>$idx</option>";
		}
		else{
			echo "<option value='$idx' selected>$idx</option>";
		}
	}
	echo "</select></td>";
	echo "<td><input type='button' name='Update View' value='Update View' onClick='javascript:update_display()'></td>";
	echo "</tr>";
	echo "</table>";
	echo "</form>";
}

function output_search_options($hasdata){
	global $logpath;
	global $curlog;

	if( $hasdata ) {
		echo "<form name='SFORM' action='controls.php' method='post' enctype='multipart/form-data'>";
		echo "<input type ='text' name='searchstr' value=''><br>";
		echo "<input type='button' name='cmd' value='Search' onClick='javascript:perform_search(\"$curlog\");'><br>";

		$LSI = new LogSearchIndexer($curlog);
		$sdata = $LSI->getSearchIndexData();
		if( count( $sdata )) {
			echo "<select name='sterm' onChange='javascript:view_search(\"$curlog\");'>";
			echo "<option value='select'>select</option>";
			foreach ($sdata as $asearch ){
				echo "<option value='{$asearch['file']}'>{$asearch['term']}</option>";
			}
			echo "</select><br>";
		}
		$LSI = null;

		if( is_file( "$logpath/$curlog/log.files" )) {
			echo "View Included File";
			echo "<select name='incfile' onChange='javascript:view_file();'>";
			echo "<option value='select'>select</option>";
			$f_fp = fopen("$logpath/$curlog/log.files", "r");
			while( $line = fgets($f_fp) ) {
				$line = rtrim($line);
				echo "<option value='$logpath/$curlog/$line'>$line</option>";
			}
			fclose($f_fp);
			echo "</select>";
		}
		echo "</form>";
	}
	else{
		echo "&nbsp;";
	}
}

function output_sub_options($hasdata){
	global $curlog;
	global $LI;

	$exceptcnt = 0;
	$llist = $LI->getLogEntries();
	if( $curlog != "" ) {
		$stats = $llist[$curlog];
		foreach( $stats as $line ){
			if( preg_match("/Exception/", $line)) {
				$match = Array();
				preg_match("/: (\d+)/", $line,$match);
				$exceptcnt = intval($match[1]);
			}
		}

		echo "<a class='menu' href='javascript:initialize(\"$curlog\");'>Run initial scan</a><br>";
		if( count($stats) ) {
			echo "<a class='menu' href='javascript:view_log_data(\"$curlog\");'>View this log</a><br>"; 
			if( $exceptcnt ) {
				echo "<a class='menu' href='javascript:view_exceptions(\"$curlog\");'>Show Exceptions</a><br>";
			}
		}
	}
}

//Need file
//Need logname
function process_upload(){
	global $LI;

	if( $_POST["logname"] ) {
		if( $_POST["logname"] == "" ) {
			set_msg("Please enter a name for this log entry" );
			return;
		}

		if( $LI->hasEntry( $_POST["logname"] )  ) {
			set_msg("{$_POST['logname']} is already in use" );
			return;
		}
//		set_error( __FILE__, __LINE__, var_export($_FILES,1) );
		$upl = new Uploader( 
			Array( 
				"logname" => $_POST["logname"],
				"filedata" => $_FILES['logfname']
			  )
		 );
		if( !$upl->proc_upload() ){
			set_error( __FILE__,__LINE__, "Problem with file upload", 1 );
		}
		$LI->loadIndex();
	}
}

function view_log( ){
	global $curlog;

	if( array_key_exists("logname", $_POST ) ) {
		$curlog = $_POST["logname"];
	}

}

function initialize_log(){
	global $curlog;

	if( array_key_exists("logname", $_POST ) ) {
		$curlog = $_POST["logname"];
	}
	$LP = new LogParser($curlog);
	$LP->runInitializeLog();
	$LP = null;

	$defsrch = Array( "org.sonatype.nexus.proxy", "org.sonatype.nexus.DefaultNexus", "com.sonatype.nexus.plugins", "Added repository" );
	$LSI = new LogSearchIndexer($curlog);
	foreach($defsrch as $sterm){
		$LSI->createNewSearch($sterm);
	}
}


?>
