<?php

	require_once("rmv_db.inc");
	require_once("uploader.inc");
/* 
Terms 
ProcessName -> When uploading, the name used to identify the log
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- 
-------------------------------------------------

Initializing Sonatype Nexus Professional 2.1.2

-------------------------------------------------
2012-12-13 10:33:12 DEBUG [host] Scheduler] - org.apache.activemq.broker.region.Queue - queue://nexus.task expiring messages done.
2012-12-13 10:33:39 DEBUG [H2DBDeleter    ] - com.sonatype.nexus.licensing.api.access.h2.H2DatabaseManager - IPTracker purged 0 rows, starting from date Thu Dec 06 10:33:39 EST 2012
2012-12-13 10:33:40 DEBUG [ectionManager-1] - com.sonatype.nexus.plugins.smartproxy.internal.connection.HandshakeTask - Running 
2012-12-13 09:39:12 INFO  [y-main-thread-1] - org.sonatype.sisu.velocity.Velocity - Creating default instance of VelocityEngine
2012-12-13 09:39:14 DEBUG [y-main-thread-1] - eu.medsea.mimeutil.MimeUtil2 - Getting MIME types for file name [file:/usr/local/nexus-professional-2.1.2/nexus/WEB-INF/plugin-repository/nexus-procurement-plugin-2.1.2/nexus-procurement-plugin-2.1.2.jar!/static/style/procurement.css]. 
Activation data 
2012-12-13 09:39:38 INFO  [y-main-thread-1] - org.sonatype.nexus.DefaultNexus - Plugin manager request "ACTIVATE" on plugin "com.sonatype.nexus.plugin:nexus-archive-browser-plugin:2.1.2" was successful. 
The following plugins were processed:
... com.sonatype.nexus.plugin:nexus-archive-browser-plugin:2.1.2 :: action=ACTIVATE result=ACTIVATED

       Detailed report about plugin "com.sonatype.nexus.plugin:nexus-archive-browser-plugin:2.1.2":

         Source: "null"

         Custom repository types:

         Static resources:
         * Content type "application/x-javascript", to be published at path "/static/js/nexus-archive-browser-plugin-all.js"
         * Content type "text/css", to be published at path "/static/css/archiveBrowsingPanel.css"

         Imported plugins:

        Resolved plugins:
 
Stop sequence 
2012-12-13 13:27:57 DEBUG [ectionManager-1] - com.sonatype.nexus.plugins.smartproxy.internal.connection.ConnectionMonitor - Scheduling handshake retry (454) in: 30 seconds
2012-12-13 13:27:57 DEBUG [ectionManager-1] - com.sonatype.nexus.plugins.smartproxy.internal.connection.HandshakeTask - Stopped
2012-12-13 13:28:00 DEBUG [ner_stop_runner] - org.sonatype.plexus.appevents.AbstractSimpleEventMulticaster - Notifying 29 EventListener about event org.sonatype.nexus.proxy.events.NexusStoppingEvent fired (org.sonatype.nexus.proxy.events.NexusStoppingEvent@abe06e)
2012-12-13 13:28:00 INFO  [ner_stop_runner] - com.sonatype.nexus.plugins.smartproxy.internal.messaging.StopConnectorsEventInspector - Stopping all connectors
2012-12-13 13:28:00 INFO  [ner_stop_runner] - com.sonatype.nexus.plugins.smartproxy.internal.connection.ConnectionManager - Stopping all connections
2012-12-13 13:28:01 DEBUG [ner_stop_runner] - org.sonatype.sisu.goodies.eventbus.internal.DefaultEventBus - Event 'org.sonatype.nexus.proxy.events.NexusStoppingEvent@abe06e' fired
2012-12-13 13:28:01 INFO  [ner_stop_runner] - com.sonatype.nexus.plugins.smartproxy.event.internal.SuspendEventComponentsEventInspector - Suspending event components
2012-12-13 13:28:01 DEBUG [ner_stop_runner] - com.sonatype.nexus.plugins.smartproxy.event.internal.EventPublisherImpl - Suspending
2012-12-13 13:28:01 DEBUG [ner_stop_runner] - com.sonatype.nexus.plugins.smartproxy.event.internal.EventSubscriberImpl - Suspending
2012-12-13 13:28:01 DEBUG [ner_stop_runner] - com.sonatype.nexus.plugins.smartproxy.internal.connection.camel.LocalJmsComponentFactory - Suspending
2012-12-13 13:28:01 DEBUG [ner_stop_runner] - com.sonatype.nexus.plugins.smartproxy.internal.connection.camel.LocalJmsComponentFactory - Stopping connection: ActiveMQConnection {id=ID:rob-VirtualBox-41080-1355409582527-3:1,clientId=ID:rob-VirtualBox-41080-1355409582527-2:1,started=true}
 
The word Exception can be used to locate error lines. 
2012-12-11 08:32:05 DEBUG [roxy-3-thread-3] - org.sonatype.nexus.proxy.storage.remote.httpclient.HttpClientRemoteStorage - HEAD method failed, will attempt GET. Exception: Transport error while executing HEAD method [repositoryId="ops4j", requestPath="/", remoteUrl="http://repository.ops4j.org/maven2/"]

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------  
Uploaded Logs are placed in /var/www/logger/logs 
Processed logs will be stored in /var/www/logger/logs/ProcessName 
 
Fields 
Time:		2012-12-13 10:33:12 
D Level:   DEBUG 
Thread:		[host] Scheduler]  (Note the error in the field. Somehow have to process this) 
Dash:		" - "
Class:		  org.apache.activemq.broker.region.Queue 
Dash:		" - "
Msg:		queue://nexus.task expiring messages done. 
 
Class Field Breakdown 
Really need to track down the actual names for these items 
org.apache.activemq.broker.region.Queue   
classpath org.apache.activemq 
Subclass array [broker,region,Queue] 
 
	org.sonatype.nexus
	org.sonatype.sisu
	eu.medsea.mimeutil
	org.sonatype.plexus
	org.apache.activemq
	org.apache.kahadb
	org.apache.shiro
	net.sf.ehcache
	org.apache.camel
	org.apache.commons
	org.apache.http
 
 
*/

function get_file_list($logname){
	global $logpath;

	set_error(__FILE__,__LINE__, "$logpath/$logname" );

	$flist = Array();
	$dlfp = opendir("$logpath/$logname");
	
	while( ($entry = readdir($dlfp)) ) {
		 if ( $entry == "." ||  $entry == ".." || $entry == "bundle.zip" || $entry == "encryptedfile.enc" ) continue;
		 if( is_dir("$logpath/$logname/$entry") ) {
			 $tflist = get_file_list("$logname/$entry");
			 foreach($tflist as $fname){
				 $flist[] = "$entry/$fname";
			 }
		 }
		 else{
			 $flist[] = $entry;
		 }
	}
	closedir($dlfp);
	return $flist;
}

class  LogIndexer {
	protected $lindex = null;
	protected $cob = null;

	public function __construct() {
	}

	public function loadIndex(){
		global $logpath;
		//initialize array
		$this->lindex = Array();

		if( !($dh = opendir($logpath)) ){
			set_error(__FILE__,__LINE__, "Unable to open [$logpath]" );
			return;
		}
		while( $entry = readdir($dh) ) {
			 if ( $entry == "." ||  $entry == ".." ) continue;
			$this->lindex[$entry] = $this->loadSubEntries($entry);
		}
		closedir($dh);
	}

	public function loadSubEntries($logent){
		global $logpath;
		$result = Array();

		if(  is_file($logpath."/".$logent."/log.stats" )  ) {
			$lfh = fopen($logpath."/".$logent."/log.stats", "r" );
			while( ($line = fgets($lfh)) ) {
				$result[] = $line;
			}
			fclose($lfh);
		}
		return $result;
	}

	public function hasEntry($ename) {
		if( ! $this->lindex ) {
			$this->loadIndex();
		}
		return array_key_exists($ename,$this->lindex);
	}

	public function getLogEntries(){
		if( ! $this->lindex ) {
			$this->loadIndex();
		}
		return $this->lindex;
	}
}

$LI;

function init_log_index() {
	global $LI;

	$LI = new LogIndexer();
}


class LogParser {
	protected $logfname;

	protected $threadnames = Array();
	protected $exceptionlines = Array();
	protected $stats = Array();
	protected $classes = Array();
	protected $linenum = 0;

	public function __construct($logfname) {
		$this->logfname = $logfname;
	}

	//Loop
	//write file pointer position to index
	//read line 
	//parse line for line type
	//write type to types file
	public function runInitializeLog(){
		global $logpath;

		$this->doEncryptList();
		$this->linenum = 1;
		$ll = new LogLine();
		$this->stats["timelines"] = 0;

		$l_fh = fopen($logpath."/".$this->logfname."/logfile", "r" );
		$li_fh = fopen($logpath."/".$this->logfname."/log.index", "w");
		$lt_fh = fopen($logpath."/".$this->logfname."/log.types", "w");
		$lti_fh = fopen($logpath."/".$this->logfname."/log.times", "w");

		$fpos = 0;
		while( ($line = fgets($l_fh) ) !== false  ) {
			$line = rtrim($line);
			if( $line == "" ) {
					$fpos =  ftell($l_fh);
					continue;
			}
			$l_ent = $ll->deconstruct(rtrim($line),$this->linenum);
			fprintf( $li_fh, "%08x\n", $fpos );
			fprintf( $lt_fh, "%d\n", $l_ent["l_type"] );
			fprintf( $lti_fh, "%08x\n", $l_ent["l_time"] );

			if( $l_ent["l_thread"] != ""  ) {
				if( !array_key_exists($l_ent["l_thread"],$this->threadnames)) {
					$this->threadnames[$l_ent["l_thread"]] = Array();
				}
				$this->threadnames[$l_ent["l_thread"]][] = $this->linenum;
			}
			//Was this an error line?
			if( preg_match("/Exception/", $l_ent["l_msg"]) ) {
				$this->exceptionlines[] = $this->linenum;
			}
			//Is this a non-normal line?
			if( $l_ent['l_type'] == 2 ) {
				$this->doStatChecks( $l_ent['l_msg'] );
				//Run test for various important log entries.
			}
			else if( $l_ent['l_type'] == 1 ) {
				$this->stats["timelines"]++;
				if( ! array_key_exists($l_ent['l_class'], $this->classes) ) {
					$this->classes[$l_ent["l_class"]] = Array();
				}
				$this->classes[$l_ent["l_class"]][] = $this->linenum;
				/*
				foreach( $this->specialclasses as $aclass => $tlines ) {
					if( preg_match( "/{$l_ent['l_class']}/", $aclass ) ) {
						$this->specialthreads[$aclass][] = $this->linenum;
					}
				} 
				*/  
			}
			$fpos =  ftell($l_fh);
			$this->linenum++;
		}
		fclose($lti_fh);
		fclose($lt_fh);
		fclose($li_fh);
		fclose($l_fh);
		$this->writeThreads();
		$this->writeExceptions();
		$this->writeStats();
		$this->writeClasses();
	}

	//If this was an encrypted upload 
	//Need to create a file with list of files that were included
	public function doEncryptList(){
		global $logpath;
		$flist =null;

		if( is_file("$logpath/$this->logfname/encryptedfile.enc")) {
			$flist = get_file_list($this->logfname);
		}
		if( $flist && count($flist) ) {
			$fh = fopen("$logpath/$this->logfname/log.files", "w");
			foreach($flist as $fname) {
				fprintf( $fh, "%s\n", $fname );
			}
			fclose($fh);
		}
	}

	public function writeThreads(){
		global $logpath;

		$threadcnt = 1;
		if( count( $this->threadnames ) ) {

			$lt_fh = fopen($logpath."/".$this->logfname."/log.threads", "w" );
			foreach( $this->threadnames as $name => $linearray ) {
				$fname = "thread.$threadcnt";
				fprintf( $lt_fh, "%15s = $fname\n", $name );

				$tfh = fopen($logpath."/".$this->logfname."/".$fname, "w");
				foreach ( $linearray as $tline ){
					fprintf( $tfh, "%08x\n", $tline );
				}
				fclose($tfh);
				$threadcnt++;
			}
			fclose($lt_fh);
		}
	}

	public function writeExceptions(){
		global $logpath;

		if( count( $this->exceptionlines ) ) {
			$lt_fh = fopen($logpath."/".$this->logfname."/log.exceptions", "w" );
			foreach( $this->exceptionlines as $value ) {
				fprintf( $lt_fh, "%08x\n", $value );
			}
			fclose($lt_fh);
		}
	}

	public function writeClasses(){
		global $logpath;
		if( count( $this->classes ) ) {
			foreach( $this->classes as $cname => $linearray){
				$fp = fopen( "$logpath/$this->logfname/$cname", "w" );
				foreach ($linearray as $tline ){
					fprintf( $fp, "%08x\n", $tline );
				}
				fclose($fp);
			}
		}
	}

	public function writeStats(){
		global $logpath;
		if( count( $this->stats ) ) {
			$lt_fh = fopen($logpath."/".$this->logfname."/log.stats", "w" );
			if( $this->stats ) {
				if( array_key_exists( "initialized", $this->stats ) ) {
					fprintf( $lt_fh, "Nexus Started %d times.\n", $this->stats["initialized"]["count"] );
					fprintf( $lt_fh, "Nexus Version %s\n", $this->stats["initialized"]["version"]  );
				}
				$fs = filesize($logpath."/".$this->logfname."/logfile");
				fprintf( $lt_fh, "Log Size [".number_format($fs)."] bytes\n");
				fprintf( $lt_fh, "Lines = ".$this->linenum."\n");
				fprintf( $lt_fh, "Timestamped lines: ".number_format($this->stats['timelines'])."\n");
				fprintf( $lt_fh, "Exceptions Detected: ".number_format(count($this->exceptionlines))."\n" );
				if( is_file("$logpath/$this->logfname/encryptedfile.enc") ) {
					fprintf($lt_fh, "From encrypted file: true");
				}
			}
			fclose($lt_fh);
		}
	}

	public function doStatChecks($msg) {
//		set_error( __FILE__, __LINE__, "Testing [$msg]");
		if( preg_match("/Initializing Sonatype Nexus/",$msg)) {
//			set_error( __FILE__, __LINE__, "Found Initialize");
			if( !array_key_exists( "initialized", $this->stats ) ) {
				$this->stats["initialized"] = Array( "count" => 0, "Version" => ""  );
			}
			$this->stats["initialized"]["count"]++;
			$mary = Array();
			if( preg_match( "/(\d+\.\d+\.\d+)\s*$/", $msg, $mary )  ){
//				set_error( __FILE__, __LINE__, "Version = {$mary[1]}");
				$this->stats["initialized"]["version"] = $mary[1];
			}
		}
	}

}

/*
Fields 
Time:		2012-12-13 10:33:12 
D Level:   DEBUG 
Thread:		[host] Scheduler]  (Note the error in the field. Somehow have to process this) 
Dash:		" - "
Class:		  org.apache.activemq.broker.region.Queue 
Dash:		" - "
Msg:		queue://nexus.task expiring messages done. 
*/ 
	//Class for new log line entry
class LogLine {
	//l_type values
	//0 = unknown
	//1 = normal line
	//2 = extra info line
	//3 = non-normal exception line (the lines following an exception)

	protected $lasttime = 0;

	public function __construct() {
		//need to decode line right here.
	}

	public function deconstruct($line,$linenum){
		$l_fields = Array( 
			"l_type" => 0,
			"l_lnum" => 0,
			"l_time" => 0,
			"l_timestr" => "",
			"l_level" => "",
			"l_thread" => "",
			"l_class" => "",
			"l_msg" => ""
		);
		if( preg_match("/^jvm/", $line ) ) {
				$tarray = Array();
				if( preg_match("/^(jvm \d+\s+\|\s+)(.*)$/", $line, $tarry ) ) {
//					set_error(__FILE__,__LINE__, "Match: {$tarry[2]}" );
					$line = $tarry[2];
				}
		}
		$l_fields["l_lnum"] = $linenum;
		//If this does not start with a timestamp 
		if( ! preg_match( '/^\d{4}/' , $line ) ) {
			$l_fields["l_type"] = 2;
			$l_fields["l_msg"] = $line;
			$l_fields["l_time"] = $this->lasttime;
		}
		else {
			// Added to filter wrapper.log versus nexus.log
			//2012-12-13 10:33:12 DEBUG [host] Scheduler] - org.apache.activemq.broker.region.Queue - queue://nexus.task expiring messages done.
			//has timestamp
//		    $sparse = '/^(\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}) (TRACE|INFO|DEBUG|ERROR|FATAL|WARN)\s+\[(.{15})\]\s+-\s+([^\s]+)\s+-(\s+.*)/';
		    $sparse = '/^(\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}) (TRACE|INFO|DEBUG|ERROR|FATAL|WARN)\s+\[(.{15})\]\s+-\s+([^\s]+)(.*)$/';
			$mary = Array();
//			set_error(__FILE__,__LINE__, "LINE:: $line" );
//			error_log("LINE:: $line");
			$result = preg_match($sparse,$line,$mary);
			//mary 0 = all matched 1 = DateTime 2 = TRACE LEVEL 3 = thread id 4 = classpath 5 = message 
			if( $result === false ) {
				set_error(__FILE__,__LINE__, "error parsing time string" );
				return $l_fields;
			}
			$l_fields["l_type"] = 1;
			if( count($mary) < 1 ){
				set_error(__FILE__,__LINE__, "Unable to parse line $linenum");
				$l_fields["l_type"] = 0;
			}
			else{
				$l_fields["l_timestr"] = $mary[1];
				$l_fields["l_time"] = $this->getTimeStamp($this->lasttime,$mary[1]);
				$l_fields["l_level"] = $mary[2];
				$l_fields["l_thread"] = $mary[3];
				$l_fields["l_class"] = $mary[4];
				$l_fields["l_msg"] = $mary[5];
				$this->lasttime = $l_fields["l_time"];
			}
		}
		return $l_fields;
	}

	//Pass in the last verified time to have it returned
	//Alternately pass in whatever you want returned if timestamp is not found
	public function getTimeStamp($lastime, $timestr){

		$format = 'Y-m-d H:i:s';
		$date = DateTime::createFromFormat($format, $timestr);
		if( ! $date ) {
			set_error( __FILE__, __LINE__, "Failed to create data object from [$timestr]");
			return 0;
		}
		$tstamp = $date->getTimeStamp();
		return $tstamp;
	}

	public function getMsg(){
		return gVE("l_msg",$this->cobj );
	}

	public function getType(){
		return gVE("l_msg",$this->cobj );
	}
}

class LogDisplay {
	protected $logfile;
	protected $linecount;
	protected $lfp;
	protected $ldxfp;
	protected $LL;
	protected $curlogline;


	public function __construct($logfile,$startline) {
		global $logpath;

		$this->logfile = $logfile;
		$this->linecount = get_line_count($logfile,"log.index", 9);
		if( !is_dir("$logpath/$logfile") || !is_file( "$logpath/$logfile/log.index" ) ) {
			set_error(__FILE__,__LINE__, "You will need to run Initialization command first");
		}
		$this->lfp = fopen("$logpath/{$this->logfile}/logfile", "r" );
		$this->lidxfp = fopen("$logpath/{$this->logfile}/log.index", "r" ); 
		$this->moveToLine($startline);

	}

	public function __destruct() {
		if( $this->lfp ) {
			fclose($this->lfp);
		}
		if( $this->lidxfp ) {
			fclose( $this->lidxfp );
		}
	}

	public function getRealLogOffset( $linenum ) {
		global $logpath;

		$logoffset = 1;
		$indexofs = (($linenum - 1) * (8 + 1) );	//add one becuase file seek offset starts at 1

		if( !fseek($this->lidxfp,$indexofs) ) {
			$str = rtrim( fgets ( $this->lidxfp ) );
			//set_error(__FILE__,__LINE__, "record $str intval = ".intval( "0x$str", 16 ) );
			$logoffset = intval( "0x$str", 16 );
		}
		//set_error(__FILE__,__LINE__, "Startline {$newlinenum} log index [$this->logindex]" );
		return $logoffset;
	}

	public function moveToLine($newlinenum){
//		set_error(__FILE__,__LINE__, "Move to $newlinenum");
		if( $newlinenum > $this->linecount ) {
			set_error(__FILE__,__LINE__, "Attempt to exceed max lines [$newlinenum]" );
			return 0;
		}
		$this->curlogline = $newlinenum;
		$offset = $this->getRealLogOffset($newlinenum);
//		set_error(__FILE__,__LINE__, "Offset $offset");
		if( $this->lfp ) {
			fseek( $this->lfp,  $offset);
		}
		return 1;
	}

	//Moved to here because it is a common routine
	//Don't forget to add one to count for newlines.
	protected function getRealLogLine( $fp, $cnt, $startline ){
		global $logpath;
		$logstartline;

//		set_error( __FILE__,__LINE__, "[$fp] [$cnt] [$startline] ");
		$indexofs = (($startline - 1) * $cnt );	//add one becuase file seek offset starts at 1
//		set_error(__FILE__,__LINE__, "exceptstartline = $exceptstartline IndexOfs = $this->indexofs" );
		if( !fseek($fp,$indexofs) ) {
			$str = rtrim( fgets ( $fp ) );
			$logstartline = intval( "0x$str", 16 );
//			set_error(__FILE__,__LINE__, "gRLL:: record $str intval = $logstartline" );
		}
		return $logstartline;
	}

	public function getLine(){
		$line = fgets($this->lfp);
		if(  $line  != false) {
			return rtrim($line);
		}
		return $line;
	}

	public function getLineData(){
//		set_error(__FILE__,__LINE__,"LD::getLineData" );
		if( !$this->LL ) {
			$this->LL= new LogLine();
		}
		$line = $this->getLine();
		if(  $line  != false ) {
			return $this->LL->deconstruct($line,$this->curlogline);
		}
		return $line;
	}

	public function setDone(){
		if( $this->lfp ) {
			fclose($this->lfp);
			$this->lfp = null;
		}
	}

	public function getLineNumber(){
		return $this->curlogline;
	}
}

class LogExceptions extends LogDisplay {
	protected $ei_fp;
	protected $ei_curline;
	protected $curlogline;
	
	public function __construct($logfile,$exceptstartline) {
		global $logpath;
		$this->ei_fp = fopen("$logpath/$logfile/log.exceptions", "r" );
		$this->ei_curline = $exceptstartline;
		parent::__construct($logfile,$this->getRealLogLine($this->ei_fp, 9, $exceptstartline) );
	}

	public function getLineData(){
		parent::moveToLine($this->getRealLogLine($this->ei_fp, 9,$this->ei_curline++));
		return parent::getLineData();
	}
}


class LogThreads  extends LogDisplay {
	protected $ti_fp;
	protected $ti_curline;
	protected $threadfile;
	
	public function __construct($logfile,$threadname, $threadstartline) {
		global $logpath;

		$this->ti_curline = $threadstartline;

		$fp = fopen("$logpath/$logfile/log.threads", "r" );
		while( ( $line = fgets($fp) ) ) {
			$tdata = explode(" = ", rtrim($line) );
			if( $tdata[0] == $threadname) {
				$this->threadfile = $tdata[1];
				break;
			}
		}
		fclose($fp);
		$this->ti_fp = fopen("$logpath/$logfile/$this->threadfile","r");
		parent::__construct($logfile, parent::getRealLogLine($this->ti_fp, 9, $threadstartline) );
	}

	public function getLineData(){
//		set_error(__FILE__,__LINE__,"LT::getLineData" );
		parent::moveToLine(parent::getRealLogLine($this->ti_fp, 9, $this->ti_curline++));
		return parent::getLineData();
	}
}

class LogFull  extends LogDisplay {
	protected $fi_curline;
	
	public function __construct($logfile, $startline) {
		global $logpath;

		$this->fi_curline = $startline;
		parent::__construct($logfile, $startline );
	}

	public function getLineData(){
//		set_error(__FILE__,__LINE__,"LC::getLineData" );
		parent::moveToLine($this->fi_curline++);
		return parent::getLineData();
	}
}

class LogClasses  extends LogDisplay {
	protected $ci_fp;
	protected $ci_curline;
	protected $cname;
	
	public function __construct($logfile, $cname, $classstartline) {
		global $logpath;

		$this->ci_curline = $classstartline; ;
		$this->cname = $cname;
		$this->ci_fp = fopen("$logpath/$logfile/$this->cname","r");
		parent::__construct($logfile, parent::getRealLogLine($this->ci_fp, 9, $classstartline) );
	}

	public function getLineData(){
//		set_error(__FILE__,__LINE__,"LC::getLineData" );
		parent::moveToLine(parent::getRealLogLine($this->ci_fp, 9, $this->ci_curline++));
		return parent::getLineData();
	}
}

class LogSearchIndexer {
	private $logfile = "";
	private $s_data = null;

	public function __construct($logfile){
		global $logpath;
		
		$this->logfile = $logfile;
		if( is_file("$logpath/$logfile/log.searches") ) {
			$str = file_get_contents("$logpath/$logfile/log.searches");
			$this->s_data = unserialize($str);
		}
		if( !$this->s_data) {
			$this->s_data = Array();
		}
	}

	public function saveSearchIndex(){
		global $logpath;

		$str = serialize($this->s_data);
		$fp = fopen( "$logpath/$this->logfile/log.searches", "w");
		fwrite($fp,$str);
		fclose($fp);
	}

	public function createNewSearch($sterm){
		$idx = count( $this->s_data );
		set_error(__FILE__,__LINE__, "New Search $idx");
		$this->s_data[$idx] = Array("term" => $sterm, "file" => "search.$idx");
		$this->saveSearchIndex();
		create_search_index($this->logfile,$sterm,"search.$idx");
		return "search.$idx";
	}

	public function getSearchIndexData(){
		return $this->s_data;
	}
}

//This class is used to perform searches 
function create_search_index($logfile,$sterm,$savefile){
	global $logpath;

	set_error(__FILE__,__LINE__, "Create Search file [$logfile][$sterm][$savefile]");

	$linen = 1;
	$LP = new LogDisplay($logfile,$linen);
	$matchcnt = 0;
	$s_fh = fopen("$logpath/$logfile/$savefile", "w");
	while( ($lent = $LP->getLine() ) != false ) {
		if( preg_match("/$sterm/",$lent) ) {
			fprintf( $s_fh, "%08x\n", $linen );
			//set_error(__FILE__,__LINE__, "Matched ".number_format($matched++) );
		}
		if( !$LP->moveToLine(++$linen) ){
			set_error(__FILE__,__LINE__,"moveToLine Failed." );
			break;
		}
	}
	fclose($s_fh);
	$LP->setDone();
}

//This class is used to display previous searches

class LogSearch extends LogDisplay {
	protected $si_fp;
	protected $si_curline;
	protected $sname;

	public function __construct($logfile, $sname, $searchstartline) {
		global $logpath;

		$this->si_curline = $searchstartline; ;
		$this->sname = $sname;
		$this->si_fp = fopen("$logpath/$logfile/$this->sname","r");
		parent::__construct($logfile, parent::getRealLogLine($this->si_fp, 9, $searchstartline) );
	}

	public function getLineData(){
//		set_error(__FILE__,__LINE__,"LC::getLineData" );
		parent::moveToLine(parent::getRealLogLine($this->si_fp, 9, $this->si_curline++));
		return parent::getLineData();
	}
}

function get_thread_line_count($logfile,$threadname){
	global $logpath;

	$tfile = "";

	$fp = fopen("$logpath/$logfile/log.threads", "r" );
	while( ( $line = fgets($fp) ) ) {
		$tdata = explode(" = ", rtrim($line) );
		if( $tdata[0] == $threadname) {
			$tfile = $tdata[1];
			break;
		}
	}
	fclose($fp);
	return get_line_count($logfile,$tfile,9);
}

function get_line_count( $logname, $fname, $divisor ) {
	global $logpath;

	$filename = "$logpath/$logname/$fname";

	if( is_file($filename) ) {
		$fsize = filesize($filename);
		//If the file is really an index file then the remainder should be 0
		if( $fsize % $divisor  ) {
			set_error(__FILE__,__LINE__, "Filesize / divisor did not result in 0" );
		}
		return $fsize / $divisor;
	}
	return 0;
}

?>
