<?php 
	session_start();
	$purchase_id=$_POST['purchase_id'];
	echo $product_enter_plan_id;
	@ $db=new mysqli('localhost','root','gdhygjzx7','db_class');
	if (mysqli_connect_errno()) {
		echo 'Error:Could not connect to database.Please try again later.';
		exit;
	}
	$query="update component_enter_affrim set is_entered=1 where purchase_id='".$purchase_id."';";
	$result=$db->query($query);
	if($result){
		echo "Succeesful!";
	}
	else{
		echo "Fail!";
	}
?>


	

                    
