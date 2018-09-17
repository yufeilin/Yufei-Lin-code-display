<html>
<body>
<?php 
	session_start();
	$account=$_POST['account'];
	$pwd_old=$_POST['pwd_old'];
	$pwd_new=$_POST['pwd_new'];
	$pwd_again=$_POST['pwd_again'];
	if (!$account || !$pwd_old || !$pwd_new || !$pwd_again) {
		echo 'You have not entered all the required details.<br />Please go back and try again.';
		exit;
	}
	if (!get_magic_quotes_gpc()) {
		$account=addslashes($account);
		$pwd_old=addslashes($pwd_old);
		$pwd_new=addslashes($pwd_new);
		$pwd_again=addslashes($pwd_again);
	}
	@ $db=new mysqli('localhost','root','gdhygjzx7','db_class');

	if (mysqli_connect_errno()) {
		echo 'Error:Could not connect to database.Please try again later.';
		exit;
	}

	$query="select passwd from account where account_name='".$account."';";
	$result=$db->query($query);
	//echo $result;
	$row=mysqli_fetch_assoc($result);
	echo $row['passwd'];
	if($row['passwd']==$pwd_old){
		$query="update account set passwd='".$pwd_new."' where account_name='".$account."';";
		$result=$db->query($query);
		if($result){
			echo "Succeed!<br>";
		}
		else{
			echo "Fail!<br>";
		}
	}
	else{
		echo "Old Password Error!";
	}
	$db->close();	

?>
</body>
</html>
