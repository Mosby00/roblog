<?php
	require_once("rmv_db.inc");

	$WARN;
	$username = "logger_user";
	$password = "logpass";
	$def_db = "logger";

	init_log();


function setup_session($uname,$uid){
    global $username;
    $userchanged = "";

	//It is critical to test to see if the user has any kind of session

//    set_error(__FILE__,__LINE__, $_SERVER['SERVER_NAME'], 1);

	if( !$uid ) {
		//No user id yet.
		if( isset($_SESSION['UserID']) ) {
			//Have a user id. so use it.
			$uid = $_SESSION['UserID'];
		}
		else{
			//Create a new  user id for him
			$uid = session_id();
		}
	}

    //Does user have name yet?
    if( !isset($_SESSION['UserName'] ) ) {
        //No, so put the name in Session storage.
        $_SESSION['UserName'] = $uname;
        $_SESSION['UserID'] = $uid;
        LogIt($uid, "$uname has logged in SESSION ID=(".session_id().")" );
    }
    //Has the user changed names from last time here
    else if( $_SESSION['UserName'] !== $uname ) {
        //Yes. So make changes
        //Before making these changes it will be important to make sure anything the user might have been doing as guest gets preserved.
        $userchanged = $_SESSION['UserName'];
        $olduserid = $_SESSION['UserID'];

        $_SESSION['UserName'] = $uname;
        $_SESSION['UserID'] = $uid;
    }
    else{
        if( isset( $_POST['COMMAND'] )) {
            LogIt($uid,"$uname has executed {$_POST['COMMAND']}");
        }
    }
    $username = $_SESSION['UserName'];
    $userid = $_SESSION['UserID'];
}

function validate_account($user,$pass) {
	return 1;
}

function is_guest(){
	return ($_SESSION['UserName'] == "Guest");
}

function print_post(){
    foreach($_POST as $key => $value ){
		set_error( __FILE__,__LINE__, "POST: [$key ($value)]", 1);
    }
}

function output_warning(){
	global $WARN;
	echo "$WARN";
}

function warning( $str ){
	global $WARN;
	$WARN .= $str."<BR>";
}

?>
