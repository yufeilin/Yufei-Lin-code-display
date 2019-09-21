<?php 
	session_start();
	$provider_name=$_POST['provider_name'];
	$purchase_id=$_POST['purchase_id'];
	$component_id=$_POST['component_id'];
	$purchase_count=$_POST['purchase_count'];
	$buyer=$_POST['buyer'];
	$is_approved=$_POST['is_approved'];
	@ $db=new mysqli('localhost','root','gdhygjzx7','db_class');
	if (mysqli_connect_errno()) {
		echo 'Error:Could not connect to database.Please try again later.';
		exit;
	}
	$query="select * from component where component_id='".$component_id."';";
	$result=$db->query($query);
	$row=mysqli_fetch_assoc($result);
	$component_name=$row['component_id'];
	$component_standard=$row['component_standard'];
	$component_unit=$row['$component_unit'];
	$query="insert into purchase_plan values('".$provider_name."','".$purchase_id."','".$component_name."','".$component_id."','".$component_standard."','".$component_unit."','".$purchase_count."','".$buyer."','".$is_approved."');";
	$result=$db->query($query);
	//echo $result;
	if($result){
		echo "Succeesful!";
	}
	else{
		echo "Fail!";
	}
	$query="insert into purchase_affirm values('".$provider_name."','".$purchase_id."','".$component_id."','".$component_name."','".$component_standard."','".$component_unit."','".$purchase_count."','".$buyer."','hh',0);";
	$result=$db->query($query);
	if($result){
		echo "insert affirm table successful!";
	}
	else{
		echo "insert fail!";
	}
?>


	

                    
