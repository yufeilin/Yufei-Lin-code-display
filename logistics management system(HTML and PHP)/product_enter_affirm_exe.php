<?php 
	session_start();
	$product_enter_plan_id=$_POST['product_enter_plan_id'];
	echo $product_enter_plan_id;
	@ $db=new mysqli('localhost','root','gdhygjzx7','db_class');
	if (mysqli_connect_errno()) {
		echo 'Error:Could not connect to database.Please try again later.';
		exit;
	}
	$query="update product_enter_affirm set is_entered=1 where product_enter_plan_id='".$product_enter_plan_id."';";
	$result=$db->query($query);
	if($result){
		echo "Succeesful!";
	}
	else{
		echo "Fail!";
	}
?>


	

                    
