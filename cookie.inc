<?php 
    require_once("session.inc");
	
	if( isset( $_POST['COMMAND'])  && $_POST['COMMAND'] === "LOGIN" ) {
		$UserID = validate_account( $_POST['UserName'], $_POST['Password']);
		if( !$UserID ){
			header("Location: ./login.php");
		}
		else{
			setup_session($_POST['UserName'],$UserID );
			header("Location: index.php");
		}
		//script called itself
	}
    else if( !isset( $_SESSION['UserName'] )){
        header("Location: ./login.php");
    }
    else{
        setup_session($_SESSION['UserName'], $_SESSION['UserID']);
    }
?>
