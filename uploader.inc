<?PHP
	$logpath = "./logs";
/*
Uploader Class 
if ($_FILES["file"]["error"] > 0)
{
	echo "Error: " . $_FILES["file"]["error"] . "<br>";
}
else
{
	echo "Upload: " . $_FILES["file"]["name"] . "<br>";
	echo "Type: " . $_FILES["file"]["type"] . "<br>";
	echo "Size: " . ($_FILES["file"]["size"] / 1024) . " kB<br>";
	echo "Stored in: " . $_FILES["file"]["tmp_name"];
}
if (file_exists("upload/" . $_FILES["file"]["name"]))
{
	echo $_FILES["file"]["name"] . " already exists. ";
}
else
{
	move_uploaded_file($_FILES["file"]["tmp_name"],
	"upload/" . $_FILES["file"]["name"]);
	echo "Stored in: " . "upload/" . $_FILES["file"]["name"];
}
*/ 

/*
 
Need to detect 
problem-report-bundle.zip.enc 
and unencode into deestination 
 
java -jar plexus-encryptor-1.0-cli.jar -d -k private.key -i nexus-error-bundle.ezip -o bundle.zip
*/
class Uploader {
	protected $cobj;

	public function __construct($COBJ){
		$this->cobj = $COBJ;
		$afile = $COBJ["filedata"];

//		set_error( __FILE__, __LINE__, var_export($afile,1) );
		if( $afile["error"] ) {
			set_error( __FILE__, __LINE__, $afile["tmp_name"] );
			set_error( __FILE__, __LINE__, "File upload had error ".$afile["error"]  );
		}

	}

	public function proc_upload() {
		global $logpath;

		$isenc = 0;
		$lpath =  $logpath."/".gVE("logname",$this->cobj );
		$files = gVE("filedata",$this->cobj );

		if( !mkdir($lpath) ) {
			set_msg("Unable to create [$lpath]");
			return 0;
		}
        $tmp_name = $files["tmp_name"];
//		$real_name = $lpath."/".$files['name'];
		//Change names. Makes it easier to determine when other files exist.
		$real_name = $lpath."/logfile";

		//Is this an encrypted file?
		if( preg_match("/[enc|ezip]$/", $files['name'])  ) {
			$real_name = "$lpath/encryptedfile.enc";
			$isenc = 1;

		}
		if( !move_uploaded_file($tmp_name,$real_name )) {
			set_msg("Unable to move logfile from $tmp_name to $real_name");
			return 0;
		}
		if( $isenc ) {
			$result = shell_exec("java -jar ./decode/plexus-encryptor-1.0-cli.jar -d -k ./decode/private.key -i $real_name -o $lpath/bundle.zip ");
			shell_exec( "cd $lpath;unzip bundle.zip");
			shell_exec("mv $lpath/nexus.log $lpath/logfile");
		}
		return 1;
	}
}

?>

