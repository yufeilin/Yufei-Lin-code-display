<html>
<body>
<?php 
	$account=$_POST['account'];
	$pwd=$_POST['pwd'];
	$name=$_POST['name'];
	$department=$_POST['department'];
	$dpt_id=$_POST['dpt_id'];
	$is_root=$_POST['is_root'];
	//if (!$name || !$department || !$dpt_id || !$is_root || !$account || !$pwd) {
	//	echo 'You have not entered all the required details.<br />Please go back and try again.';
	//	exit;
	//}

	if (!get_magic_quotes_gpc()) {
		$name=addslashes($name);
		$department=addslashes($department);
		$dpt_id=addslashes($dpt_id);
		$is_root=addslashes($is_root);
		$account=addslashes($account);
		$pwd=addslashes($pwd);
	}
	@ $db=new mysqli('localhost','root','gdhygjzx7','db_class');
	if (mysqli_connect_errno()) {
		echo 'Error:Could not connect to database.Please try again later.';
		exit;
	}
	$query="insert into account values
		('".$account."','".$name."','".$pwd."','".$department."','".$dpt_id."','".$is_root."')";
	
	$result=$db->query($query);
	if ($result){
		echo "Successful inserted into database.";
	}
	else {
		echo "An error has occurred.The item was not added.";
	}
	$db->close();
	echo "<script>location.href='login.html';</script>";
?>
</body>
</html>
