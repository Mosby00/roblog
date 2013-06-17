<?php
$debugdataset = 0;
$main_errmsg = "";

// return entry or empty
//
function gVE($value,$COBJ) {
	if( array_key_exists($value,$COBJ) ) {
		return $COBJ[$value];
	}
	return "";
}

// return entry or null
//
function gVN($value,$COBJ) {
	if( array_key_exists($value,$COBJ) ) {
		return $COBJ[$value];
	}
	return null;
}

function site_exists($dbname){
	$dbcon = new mysqli("localhost", "root", "2kedryn2", "information_schema" );
}


abstract class DB {
   protected static $dcon = null;	//every single database call will go to the same database
   protected $dbname;
   protected $uname;
   protected $password;
   protected $errno = 0;	//hold last errno
   protected $lastid = 0;	//hold last db id created
   protected $result;		//mysqli result object
   protected $ret;			//returned data
   protected $cobj;

   public function __construct($COBJ){
      $this->uname = gVE("uname",$COBJ);
	  $this->password = gVE("password",$COBJ);
	  $this->dbname = gVE("dbname",$COBJ);

	  $this->cobj = $COBJ;
   }

   public function __destruct(){
	   if( self::$dcon ){
		   self::$dcon->close();
	   }
   }

   protected function open(){
	   set_error(__FILE__,__LINE__, "Open Called ");
	   if( !self::$dcon ) {
		   set_error(__FILE__,__LINE__, "Open: ".$this->uname."|".$this->password."|".$this->dbname );
		   self::$dcon = new mysqli( "localhost", $this->uname, $this->password, $this->dbname );

		   if( ($this->errno = self::$dcon->connect_errno) != 0 ){
			   set_error(__FILE__,__LINE__, "OPEN ERROR:".self::$dcon->connect_error );
			   self::$dcon = null;
			   return FALSE;
		   }
		   else{
			   set_error(__FILE__,__LINE__, "Connection OPEN" );
		   }
	   }
	   return TRUE;
   }

   public function get($STR){
	   set_error(__FILE__,__LINE__, "Get Called " );
	   if( $this->open() ) {
		   set_error(__FILE__,__LINE__, $STR );
		   $this->result = self::$dcon->query($STR);
		   if( ($this->errno = self::$dcon->errno) != 0 ) {
			   set_error(__FILE__,__LINE__, self::$dcon->error );
			   $this->error = self::$dcon->error;
			   return null;
		   }
		   else{
			   $this->ret = $this->result->fetch_all(MYSQLI_NUM);
			   return $this->ret;
		   }
	   }
   }

   public function set($STR){
	   set_error(__FILE__,__LINE__, "Set Called ".$STR );
	   if( $this->open() ) {
		   $dbcon = self::$dcon;
		   set_error(__FILE__,__LINE__, $STR );
		   $this->result = $dbcon->query($STR);
		   if( ($this->errno = $dbcon->errno) != 0 ) {
			   set_error(__FILE__,__LINE__, $dbcon->error );
			   $this->error = $dbcon->error;
			   return false;
		   }
		   else{
			   //insert id will be 0 for problem.
			   if( self::$dcon->insert_id ) {
				   $this->lastid = self::$dcon->insert_id;
			   }
			   return $this->lastid;
		   }
	   }
   }

   public function get_errno(){
	   return $this->errno;
   }

   public function create( $dbname = "" ){
	   set_error(__FILE__,__LINE__, "Create Called " );

	   if( $dbname == "" ) {
           $dbname = $this->dbname;
       }
	   return $this->set("CREATE DATABASE IF NOT EXISTS ".$dbname );
   }

   protected function get_dcon(){
	   return self::$dcon;
   }

   abstract public function get_cmd_code();
}

class DBtable extends DB {
	private $table;
	private $idx;
	private $tbl_data;

	public function __construct($COBJ){
		$this->table = gVE("table",$COBJ);
		parent::__construct($COBJ);
	}

	protected function clean_up($str){
		return $str;
	}

	public function count(){
		return count($this->ret);
	}

	//perform get data queries
	public function search($retfld,$fld,$val,$order = ""){
		//Clear index in case a previous search was done.
		$this->idx = 0;

		$srch = "SELECT $retfld FROM $this->table";
		if( $fld ) {
			$srch .= " WHERE $fld = '$val'";
		}
		if( $order != "" ) {
			$srch .= " ORDER BY $order";
		}
		set_error(__FILE__,__LINE__, $srch);
		$result = parent::get($srch);
		set_error(__FILE__,__LINE__, "Found ".count($result)." elements.");
		if( $result ) {
			return count($result);
		}
		else{
			return null;
		}
	}
	
	//perform row update
	//Hash with set values and where values
	public function update($SOBJ,$WOBJ){
		$set = "";
		$where = "";
		foreach( $SOBJ as $key => $value ) {
			$set .= " ".$key."='".$value."'";
		}
		foreach($WOBJ as $key => $value ){
			$where .= " ".$key."='".$value."'";
		}
		$str = "UPDATE ".$this->table." SET".$set;
		if( $where != "" ) {
			$str .= " WHERE".$where;
		}
		return $this->set($str);
	}
	
	//perform distinct data query
	public function search_distinct(){
	}

	//perform a complex query
	public function search_cmplx(){
	}

	//
	public function get_row(){
		if( $this->ret && $this->ret[$this->idx] ) {
			if( $this->idx < ($this->count() -1 ) ) {
				$retary = $this->ret[$this->idx];
				$this->idx++;
				return $retary;
			}
		}
		return null;
	}

	public function insert($SOBJ){
		$isfirst = 1;
		$str = "INSERT INTO ".$this->table;
		$set = "(";
		$val = " VALUES (";
		foreach( $SOBJ as $key => $value ) {
			if( !$isfirst ) {
				$set .= ",";
				$val .= ",";
			}
			else{
				$isfirst = 0;
			}
			$set .= $key;
			$val .= "'".$value."'";
		}
		$set .= ")";
		$val .= ")";
		$str .= $set.$val;
		return $this->set($str);
	}

	public function clear(){
		return $this->set("DELETE FROM ".$this->table);
	}

	//NOTE: Due to the complexity of the where clause
	//you must pass in the complete where clause as a str
	public function delete($where){
		$str = "DELETE FROM ".$this->table." ".$where;
		return $this->set($str);
	}

	public function get_cmd_code(){
		$did = gVE("id", $this->cobj );
		$dtype = gVE("classtype", $this->cobj ); 
//		echo "logit('id [".$did."]' + ' classtype [".$dtype."]');\n";

		if( ($did != "") && ($dtype != "") ) {
//			echo "logit('cmd_code [".$dtype.".".$did."]');\n";
			return $dtype.".".$did;
		}
		return "";
	}
	
	//Allocate a new entry in table and return the 
	//ID number
	public function get_new_entry(){
	}
}


$main_errmsg = "";

//This class is specifc to the execution of the site's log file
//A different class will be used for process nexus logs
class Log extends DBtable {
	public function __construct($COBJ=null){
		if( $COBJ == null ) {
			$COBJ = Array( 
				"uname" => "logger_user",
				"password" => "logpass",
				"dbname" => "logger",
				"table" => "exec_log"
				);
		}
		parent::__construct( $COBJ );
	}

	public function write($str){
		if( !$this->insert(Array( "exec_time" => "NOW()", "exec_str" => $str)) ) {
		   // set_error(__FILE__,__LINE__, "unable to insert log data",  0 );
		}
	}
}


function dump_post(){
	global $debug;

	if( $debug ) {
		error_log("POST-----------------------------------------");
		foreach ($_POST as $key => $value ){
			error_log( "$key = $value" );
		}
		error_log("POST-----------------------------------------");
	}
}

function set_error($file,$line,$str, $dolog=0){
    global $debug;
	global $main_errmsg;

	if( $debug ) {
		error_log("[".str_replace("/var/www/", "", $file).":{$line}] $str");
	}

	//$main_errmsg .= "[".str_replace("/var/www/", "", $file).":{$line}] $str"."<BR>";
	if( $dolog ) {
//		LogIt("{$file}:{$line}] $str");
	}
}

function LogIt($msg){
	global $LOG;

//	init_log('datachange');
    $userid = 0;
	$LOG->write($msg);
}

function init_log(){
	global $LOG;
	$LOG = new Log();
}

function postLogToJavascript(){
	global $main_errmsg;

	$lines = split("<BR>",$main_errmsg);
	foreach($lines as $line){
		$prn = str_replace("`","", str_replace("'","|",str_replace("\n","", $line) ) );
		echo "logit('".$prn."');\n";
	}
}

$warnings = "";
//Need routine to write this data to work area.
function set_msg($msg){
	global $warnings;
	global $debug;

	$warnings .= $msg."<br>";
	if( $debug ) {
		set_error(__FILE__,__LINE__, $msg );
	}
	echo "haswarning = 1;";
}



//Databases
// 
// Classes need to perform the following
// read-in from database. This requires the proper class DB ID
// create entry with no data.
// write-out to database.
/*
GRANT ALL ON mydb.mytbl TO 'someuser'@'somehost';
GRANT SELECT, INSERT ON mydb.mytbl TO 'someuser'@'somehost';
 
mysql> INSERT INTO user (Host,User,Password)
    ->     VALUES('localhost','custom',PASSWORD('obscure'));
mysql> INSERT INTO user (Host,User,Password)
    ->     VALUES('host47.example.com','custom',PASSWORD('obscure'));
mysql> INSERT INTO user (Host,User,Password)
    ->     VALUES('server.domain','custom',PASSWORD('obscure'));
 
mysql> INSERT INTO db
    ->     (Host,Db,User,Select_priv,Insert_priv,
    ->     Update_priv,Delete_priv,Create_priv,Drop_priv)
    ->     VALUES('localhost','bankaccount','custom',
    ->     'Y','Y','Y','Y','Y','Y');
mysql> INSERT INTO db
    ->     (Host,Db,User,Select_priv,Insert_priv,
    ->     Update_priv,Delete_priv,Create_priv,Drop_priv)
    ->     VALUES('host47.example.com','expenses','custom',
    ->     'Y','Y','Y','Y','Y','Y');
mysql> INSERT INTO db
    ->     (Host,Db,User,Select_priv,Insert_priv,
    ->     Update_priv,Delete_priv,Create_priv,Drop_priv)
    ->     VALUES('server.domain','customer','custom',
    ->     'Y','Y','Y','Y','Y','Y');
mysql> FLUSH PRIVILEGES;
*/ 	
//
//This is a special routine for creating a database
//The site class was unable to create a data base through the internal dbconnection
//So this was split off for doing it.
//It will login to the database as root using db mysql
//create the database 
//give users permission to use the db
//then close the connection.
//This should be sufficient to allow the DB class to then open the new db
//and begin adding entries.
function createDatabase($dbname){
	//After the database is created. Editor and Acctuser will be given priviledges to modify the database.
	$dbcon = new mysqli("localhost", "root", "2kedryn2", "information_schema" );
	if( !$dbcon->query( "CREATE DATABASE IF NOT EXISTS ".$dbname ) ) {
		set_error(__FILE__,__LINE__,"Unable to create database [".$dbname."]");
		return;
	}
	if( !$dbcon->query("GRANT SELECT,INSERT,UPDATE,DELETE,CREATE on ".$dbname.".* to 'editor'") ){
		set_error(__FILE__,__LINE__,"Unable to grant priviledges to editor");
		return;
	}
	if( !$dbcon->query("GRANT SELECT,INSERT,UPDATE,DELETE,CREATE on ".$dbname.".* to 'admin'") ){
		set_error(__FILE__,__LINE__,"Unable to grant priviledges to admin");
		return;
	}
	//Not sure this one is necessary
	if( !$dbcon->query("GRANT SELECT,INSERT,UPDATE,DELETE,CREATE on ".$dbname.".* to 'acctuser'") ){
		set_error(__FILE__,__LINE__,"Unable to grant priviledges to acctuser");
		return;
	}
	$dbcon->close();
}

?>
