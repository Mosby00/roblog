<html>
<head>
	<title>Login to system</title>
	<link href="controls.css" rel="stylesheet" type="text/css" />
</head>
<body>

<?php if( isset($_SESSION['UserName']) ){
    $_SESSION = array(); 
}
?>

<p><center><h1>Please login to system</h1></center>
<form action="index.php" method="post">
<INPUT TYPE="HIDDEN" NAME="COMMAND" VALUE="LOGIN">
<table align="center" border="0">
	<tr>
		<td>User Id:</td>
		<td><input type="text" size="30" name="UserName"></td>
	</tr>
	<tr>
		<td>Password:</td>
		<td><input type="password" size="30" name="Password"></td>
	</tr>
</table>
<center><input type="submit" value="Login"></center>
</form>
</body>
</html>

