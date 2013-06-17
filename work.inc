<?php
	require_once("logger.inc");

/*
Code for work area moved here. 
There is no direct communication within php between the seperate frames 
So any variable name duplicates will be distinct variables. 
 
*/


$curlog = "";					//If a log is selected, keep track of name
$curlogdata = Array(
		"logfilecount" => 0,			//The main logs line count
		"linecount" => 0,				//number of lines in the logfile
		"curfile" => "",				//When searching the index file in use
		"startline" => 1,		  		  //What line in logfile to start loading
		"dsplines" => 200,				//Number of lines to display
		"usethread_hl" => 1,				//Whether to highlight different thread lines.
		"dspopts" => 0x7f,				//Bits 
													//  0x01 = time dsp
													//  0x02 = thread dsp
													//  0x04 = log level
													//  0x08 = class
													//	0x10 = msg body
		"ex_lines" => 1,			   //Will be added if viewing exceptions
		"th_lines" => 0				//will be added if viewing threads
	);

$curcmd = "";					//The last command executed (will be written into the work.php hidden form)
$subcmd = "";
//Work are has to accept commands from the controls frame (javascript calls to the embedded form) AND the work frame.
function process_work_php() {
	global $curlog;
	global $curlogdata;
	global $curcmd;
	global $subcmd;
	global $LI;

	dump_post();
	if( array_key_exists("logname", $_POST ) ) {
		$curlog = $_POST["logname"];
	}
	if( array_key_exists("dsplines",$_POST) ) {
		$curlogdata['dsplines'] = $_POST["dsplines"];
	}
	if( array_key_exists("cmd",$_POST)) {
		$curcmd = $_POST["cmd"];
	}
	if( array_key_exists("dspopts",$_POST)) {
		$curlogdata["dspopts"] = $_POST["dspopts"];
	}
	if( array_key_exists("subcmd", $_POST)) {
		$subcmd = $_POST["subcmd"];
	}
	if( array_key_exists("linecount", $_POST) ) {
		$curlogdata["linecount"] = $_POST["linecount"];
	}
	if( array_key_exists("curfile", $_POST) ) {
		$curlogdata["curfile"] = $_POST["curfile"];
	}
	//If this is a clear event then we are done.
	// Only need to output to the view.
	if( $curcmd == "clearview") {
		$curlogdata["subcmd"] = "";
		$curlogdata["curfile"] = "";
		$curlogdata["linecount"] = "";
		return;
	}
	init_log_index();
	$LI->loadIndex();
	load_counts();

	//Due to the command sequence for performing search;
	//Need to preprocess the command.
	//then replace curcmd & subcmd with new values.
	if( $curcmd == "performsearch") {
		$LSI = new LogSearchIndexer($curlog);
		$_POST["curfile"] = $LSI->createNewSearch($subcmd);
		$curcmd = "viewsearch";
	}

	if( array_key_exists("startline", $_POST)) {
		$curlogdata["startline"] = $_POST["startline"];
		$curlogdata["ex_startline"] = $_POST["startline"];
		$curlogdata["th_startline"] = $_POST["startline"];
		$curlogdata["cl_startline"] = $_POST["startline"];
		$curlogdata["se_startline"] = $_POST["startline"];
	}
	else{
		$curlogdata["startline"] = 1;
		$curlogdata["ex_startline"] = 1;
		$curlogdata["th_startline"] = 1;
		$curlogdata["cl_startline"] = 1;
		$curlogdata["se_startline"] = 1;
	}

	//This needs to be modified to support other viewings
	//Need Exceptions
	if( $subcmd != "" ) {
		switch( $subcmd) {
		case "resetstart":
			$curlogdata["startline"] = 1;
			$curlogdata["ex_startline"] = 1;
			$curlogdata["th_startline"] = 1;
			$curlogdata["cl_startline"] = 1;
			$curlogdata["se_startline"] = 1;
			break;
		case "resettoend":
			reset_end_line();
			break;
		}
	}

	switch( $curcmd ) {
	case "viewthreads":
		scroll_threads();
		break;
	case "viewclass":
		scroll_class();
		break;
	case "viewlogdata":
		scroll_log();
		break;
	case "viewexceptions":
		scroll_exceptions();
		break;
	case "viewsearch":
		scroll_search();
		break;
	}
	return 0;
}

function reset_end_line(){
	global $curlog;
	global $curlogdata;
	global $curcmd;

	$label = "";
	$file = "";
	$linecount = 0;

	switch( $curcmd ) {
	case "viewlogdata":
		$curlogdata["startline"] = $curlogdata["logfilecount"] - $curlogdata["dsplines"];
		$curlogdata["linecount"] = $curlogdata["logfilecount"];
		return;
	case "viewthreads":
		$label = "th_startline";
		$file = $subcmd;
		break;
	case "viewclass":
		$label = "cl_startline";
		$file = $subcmd;
		break;
	case "viewexceptions":
		$label = "ex_startline";
		$file = $subcmd;
		break;
	case "viewsearch":
		$label = "se_startline";
		$file = $curlogdata["curfile"];
		break;
	}
	if( array_key_exists("linecount",$curlogdata) && ($curlogdata["linecount"] != 0) ) {
		$linecount = $curlogdata["linecount"];
	}
	else{
		$linecount = get_line_count($curlog, $file, 9);
		$curlogdata["linecount"] = $linecount;
	}
	if(  $linecount <  $curlogdata["dsplines"]) {
		$curlogdata[$label] = 1;
	}
	else{
		$curlogdata[$label] = $linecount - $curlogdata["dsplines"]; 
	}
}

function output_work_area(){
	global $curcmd;

	//The work area is access using get commands only.
	//Work are has to accept commands from the controls frame AND the work frame.
	if( $curcmd != "" ) {
		switch( $curcmd ) {
		case "viewthreads":
			view_threads();
			break;
		case "viewlogdata":
			view_log_data();
			break;
		case "viewexceptions":
			view_exceptions();
			break;
		case "viewclass":
			view_class();
			break;
		case "viewsearch":
			view_search();
			break;
		case "clearview":
			clear_view();
			break;
		}
	}
	return 0;
	//This is the control page for the log
}

function get_start_line(){
	global $curlogdata;
	global $curcmd;

	switch( $curcmd ) {
	case "viewlogdata":
		return $curlogdata["startline"];
	case "viewexceptions":
		return $curlogdata["ex_startline"];
	case "viewthreads":
		return $curlogdata["th_startline"];
	case "viewclass":
		return $curlogdata["cl_startline"];
	case "viewsearch":
		return $curlogdata["se_startline"];
	default:
		return $curlogdata["startline"];
	}
	//If all else fails use the logfile start line;
	return $curlogdata["startline"];
}

function scroller($startline,$dsplines,$linecount){
	global $curlog;
	global $curlogdata;

//	$startline = $curlogdata["startline"];
//	$dsplines = $curlogdata["dsplines"];
//	$totallines = $curlogdata["linecount"];
	set_error(__FILE__,__LINE__, "Start [$startline] Dsp [$dsplines] Cnt[$linecount] [F:{$_POST['forward']} | B:{$_POST['backward']}");
	$inc = 0;

	//No scrolling when line available is less than lines requested be displayed
	if( $dsplines > $linecount ) {
		return 1;
	}
	if( $_POST["forward"] != "0" ) {
		set_error(__FILE__,__LINE__, "Forward");
		if( $_POST["increment"] == "line" ) {
			set_error(__FILE__,__LINE__, "Line");
			$inc = intval($_POST["forward"]);
		}
		else {
			set_error(__FILE__,__LINE__, "Page");
			$inc = intval($_POST["forward"]) * $dsplines;
		}

		if( ($inc + $startline ) > $linecount ) {
			$startline = $linecount - $dsplines;
		}
		else if( ($inc + $startline ) > ($linecount - $dsplines) ) {
			$startline = $linecount - $dsplines; 
		}
		else {
			$startline += $inc;
		}
	}
	else if( $_POST["backward"] != "0" ) {
		set_error(__FILE__,__LINE__, "Backward");
		if( $_POST["increment"] == "line" ) {
			set_error(__FILE__,__LINE__, "Line");
			$inc = intval($_POST["backward"]);
		}
		else {
			set_error(__FILE__,__LINE__, "Page");
			$inc = intval($_POST["backward"]) * $dsplines;
		}
		if( $inc > $startline) {
			$startline = 1;
		}
		else {
			$startline -= $inc;
		}
	}
	set_error(__FILE__,__LINE__, "New start line [$startline]");
	return $startline;
}

function load_counts(){
	global $LI;
	global $curlog;
	global $curlogdata;

	$log_stats = $LI->loadSubEntries($curlog);

//	set_error( __FILE__, __LINE__, count($log_stats) );

	foreach( $log_stats as $line ){
		if( preg_match("/^Line/", $line )) {
			$linedata = explode(" = ", $line );
			$curlogdata['logfilecount'] = intval($linedata[1]);
		}
	}
}

function clear_view(){
	echo "<span>&nbsp;</span>";
}


$thread_index_matrix = null;

function get_color_for_thread($tname){
	global $logpath;
	global $curlog;
	global $thread_index_matrix;

	if( !$thread_index_matrix ) {
		$str = file_get_contents("$logpath/$curlog/thread.colors");
		if( !$str ) {
			$thread_index_matrix = Array();
		}
		else{
			$thread_index_matrix = unserialize($str);
		}
	}
	//Update only when new entry is added
	if( !array_key_exists($tname,$thread_index_matrix)) {
		$thread_index_matrix[$tname] = count($thread_index_matrix) % 10;
		$str = serialize($thread_index_matrix);
		$fp = fopen( "$logpath/$curlog/thread.colors", "w");
		fwrite($fp,$str);
		fclose($fp);
	}
	return "color_".$thread_index_matrix[$tname];
}

function write_dsp_entries($LCLASS, $curline, $dsplines, $docenter = 1) {
	global $curlogdata;

	$dspopts = $curlogdata['dspopts'];
	$msgcolor = "color_0";
	$dowrap = $dspopts & 0x40 ? " dowrap" : "";

//	set_error(__FILE__,__LINE__, "Start Line [$curline] Dsplines [$dsplines] Dspopts [$dspopts]" );
	while(( ($lent = $LCLASS->getLineData()) != false) && ($dsplines--)) {
//		set_error(__FILE__,__LINE__, "[$lent] Current Line [$curline] Linesleft [$dsplines]" );
		if( $curline % 2 ) {
			echo "<span class='log_dsp_odd'  title='Line: $curline'>";
		}
		else {
			echo "<span class='log_dsp_even'  title='Line: $curline'>";
		}
		if( $docenter ) {
			echo "<a class='centerbutton' href='javascript:center_on_line(".$LCLASS->getLineNumber().");' title='Center on this line in the log[$curline][".$LCLASS->getLineNumber()."]'><img src='center.png' width='10' height='10'></a>";
		}
		if( $lent["l_type"] == 1) {
			if($dspopts & 0x01  ) {
				echo "<span class='time_text'>".$lent["l_timestr"]."</span>\n";
			}
			if( $dspopts & 0x02 ) {
				echo "[<a class='logline' title='View all, this thread' href='javascript:view_threads(\"".$lent["l_thread"]."\");'>".$lent["l_thread"]."</a>] - ";
			}
			if( $dspopts & 0x04 ) {
				echo $lent["l_level"]." ";
			}
			if( $dspopts & 0x08 ) {
				echo "[<a class='logline' title='View all, this class' href='javascript:view_class(\"".$lent["l_class"]."\");'>".$lent["l_class"]."</a>] ";
			}
			if( $dspopts & 0x20 ) {
				$msgcolor = get_color_for_thread($lent["l_thread"]);
			}
			else {
				$msgcolor = "color_0";
			}
			if( $dspopts & 0x10 ) {
				echo "<span class='msg_text $msgcolor $dowrap'>".$lent["l_msg"]."</span>\n";
			}
		}
		else{
			echo "<span class='msg_text $msgcolor $dowrap'>".$lent["l_msg"]."</span>\n";
		}
		echo "</span>";
		$curline++;
	}
}


function view_threads(){
	global $curlog;
	global $curlogdata;

	set_error(__FILE__,__LINE__, "Startline {$curlogdata['th_startline']} Lines Per Page {$curlogdata['dsplines']}" );
	$LT = new LogThreads($curlog,$curlogdata["curfile"],$curlogdata["th_startline"]);
	write_dsp_entries($LT, $curlogdata["th_startline"],
					   $curlogdata['dsplines'] > $curlogdata["th_lines"] ? $curlogdata["th_lines"] : $curlogdata['dsplines'], 
					  1 );
	$LT->setDone();
}

function view_log_data(){
	global $curlog;
	global $curlogdata;

	set_error(__FILE__,__LINE__, "Startline {$curlogdata['startline']} Lines Per Page {$curlogdata['dsplines']}" );
	$LF = new LogFull($curlog,$curlogdata["startline"]);
	write_dsp_entries($LF, $curlogdata["startline"], 
					  $curlogdata['dsplines'] > $curlogdata["logfilecount"] ? $curlogdata["logfilecount"] : $curlogdata['dsplines'], 
					  0 );
	$LF->setDone();
}

function view_exceptions(){
	global $curlog;
	global $curlogdata;

	set_error(__FILE__,__LINE__, "Startline {$curlogdata['ex_startline']} Lines Per Page {$curlogdata['dsplines']}" );
	$LE = new LogExceptions($curlog,$curlogdata["ex_startline"]);
	write_dsp_entries($LE, $curlogdata["ex_startline"], 
					  $curlogdata['dsplines'] > $curlogdata["ex_lines"] ? $curlogdata["ex_lines"] : $curlogdata['dsplines'], 
					  1 );
	$LE->setDone();
}

function view_class(){
	global $curlog;
	global $curlogdata;

	set_error(__FILE__,__LINE__, "Startline {$curlogdata['cl_startline']} Lines Per Page {$curlogdata['dsplines']}" );
	$LC = new LogClasses($curlog, $curlogdata["curfile"], $curlogdata["cl_startline"]);
	write_dsp_entries($LC, $curlogdata["cl_startline"], 
					  $curlogdata['dsplines'] > $curlogdata["cl_lines"] ? $curlogdata["cl_lines"] : $curlogdata['dsplines'], 
					  1 );
	$LC->setDone();
}

function view_search(){
	global $curlog;
	global $curlogdata;

	set_error(__FILE__,__LINE__, "Startline {$curlogdata['se_startline']} Lines Per Page {$curlogdata['dsplines']}" );
	$LS = new LogSearch($curlog, $curlogdata["curfile"], $curlogdata["se_startline"]);
	write_dsp_entries($LS, $curlogdata["se_startline"], 
					  $curlogdata['dsplines'] > $curlogdata["se_lines"] ? $curlogdata["se_lines"] : $curlogdata['dsplines'], 
					  1 );
	$LS->setDone();
}

function scroll_threads(){
	global $curlog;
	global $curlogdata;

	//9 8 characters for numeric value + 1 for newline

	$curlogdata["th_lines"] = get_thread_line_count($curlog, $curlogdata["curfile"] );
	$curlogdata["linecount"] = $curlogdata["th_lines"];
	$curlogdata["th_startline"] = scroller($curlogdata["th_startline"],$curlogdata["dsplines"],$curlogdata["th_lines"] );
}

function scroll_search(){
	global $curlog;
	global $curlogdata;

	//9 8 characters for numeric value + 1 for newline
	set_error(__FILE__,__LINE__, "Scroll Search {$curlogdata['curfile']}" );
	$curlogdata["se_lines"] = get_line_count($curlog, $curlogdata["curfile"], 9 );
	$curlogdata["linecount"] = $curlogdata["se_lines"];
	$curlogdata["se_startline"] = scroller($curlogdata["se_startline"],$curlogdata["dsplines"],$curlogdata["se_lines"] );
}

function scroll_class(){
	global $curlog;
	global $curlogdata;

	//9 8 characters for numeric value + 1 for newline
	$curlogdata["cl_lines"] = get_line_count($curlog,$curlogdata["curfile"], 9 );
	$curlogdata["linecount"] = $curlogdata["cl_lines"];
	$curlogdata["cl_startline"] = scroller($curlogdata["cl_startline"],$curlogdata["dsplines"],$curlogdata["cl_lines"] );
}

function scroll_exceptions(){
	global $curlog;
	global $curlogdata;

	//9 8 characters for numeric value + 1 for newline
	$curlogdata["ex_lines"] = get_line_count($curlog,"log.exceptions", 9 );
	$curlogdata["linecount"] = $curlogdata["ex_lines"];
	$curlogdata["ex_startline"] = scroller($curlogdata["ex_startline"],$curlogdata["dsplines"],$curlogdata["ex_lines"] );
}

function scroll_log(){
	global $curlogdata;
	$curlogdata["linecount"] = $curlogdata["logfilecount"];
	$curlogdata["startline"] = scroller($curlogdata["startline"],$curlogdata["dsplines"],$curlogdata["logfilecount"] );
}

?>


