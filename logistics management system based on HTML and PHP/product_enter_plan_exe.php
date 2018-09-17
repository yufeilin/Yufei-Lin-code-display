<?php 
	session_start();
	$product_enter_plan_id=$_POST['product_enter_plan_id'];

	$product_id=$_POST['product_id'];
	$product_name=$_POST['product_name'];
	$enter_count=$_POST['enter_count'];
	$enter_person=$_POST['enter_person'];
	$is_approved=$_POST['is_approved'];
	@ $db=new mysqli('localhost','root','gdhygjzx7','db_class');
	if (mysqli_connect_errno()) {
		echo 'Error:Could not connect to database.Please try again later.';
		exit;
	}
	$query="select * from product_coding where product_id='".$product_id."';";
	$result=$db->query($query);
	$row=mysqli_fetch_assoc($result);
	$product_standard=$row['product_standard'];
	$product_unit=$row['product_unit'];
	$query="insert into product_enter_plan values('".$product_enter_plan_id."','".$product_id."','".$product_name."','".$product_standard."','".$product_unit."','".$enter_count."','".$enter_person."','".$is_approved."');";
	$result=$db->query($query);
	//echo $result;
	if($result){
		echo "Succeesful!";
	}
	else{
		echo "Fail!";
	}
	$query="insert into product_enter_affirm values('".$product_enter_plan_id."','".$product_id."','".	$product_name."','".$product_standard."','".$product_unit."','".$enter_count."','hh',0);";
	$result=$db->query($query);
	if($result){
		echo "insert affirm table successful!";
	}
	else{
		echo "insert fail!";
	}
?>


	

                    
