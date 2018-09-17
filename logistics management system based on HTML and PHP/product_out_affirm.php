<!DOCTYPE html>
<html>
   <?php
        session_start();
        @ $db=new mysqli('localhost','root','gdhygjzx7','db_class');
        if (mysqli_connect_errno()) {
            echo 'Error:Could not connect to database.Please try again later.';
            exit;
        }
        $query="select * from product_enter_affirm;";
        $result=$db->query($query);
		$product_enter_plan_id=array();
		$product_id=array();
		$product_name=array();
		$enter_count=array();
		$enter_person=array();
		$is_entered=array();
		$i=0;
        while($row=mysqli_fetch_assoc($result)){
			$product_enter_plan_id[$i]=$row['product_enter_plan_id'];
			$product_id[$i]=$row['product_id'];
			$product_name[$i]=$row['product_name'];
			$enter_count[$i]=$row['enter_count'];
			$affirm_person[$i]=$row['affirm_person'];
			$is_entered[$i]=$row['is_entered'];
			$i++;
        }
		
    ?>
    <head>
      <title>管理系统</title>
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <meta charset="utf-8">
      <link href="./bootstrap/css/bootstrap.min.css" rel="stylesheet">
   </head>
   <body>
   <div class="container">
   <div class="row clearfix">
      <div class="col-md-12 column">
         <div class="row clearfix">
            <div class="col-md-10 column">
               <h3 align="center">
                  高铁动车转向架生产线MES系统物理管理子系统
               </h3>
            </div>
            <div class="col-md-2 column">
               <div class="row clearfix">
                  <div class="col-md-6 column">
                      <button type="button" class="btn btn-default btn-warning" onclick="window.location='pwd_change.html'">修改密码</button>
                  </div>
                  <div class="col-md-6 column">
                      <button type="button" class="btn btn-default btn-danger">退出</button>
                  </div>
               </div>
            </div>
         </div>
         <div class="row clearfix">
            <div class="col-md-2 column" >
                 <button type="button" class="btn btn-default btn-primary" onclick="window.location='purchase_plan.php'" >采购计划管理</button>
                <p></p>
                <button type="button" class="btn btn-default btn-primary" onclick="window.location='purchase_affirm.php'">采购记录管理</button> 
               <p></p>
                <button type="button" class="btn btn-default btn-primary" onclick="window.location='product_enter_plan.php'" >产品入库计划管理</button>
                <p></p>
                <button type="button" class="btn btn-default btn-primary" onclick="window.location='product_enter_affirm.php'">产品入库记录管理</button> 
               <p></p>
                <button type="button" class="btn btn-default btn-primary" onclick="window.location='product_out_plan.php'" >产品出库计划管理</button>
                <p></p>
                <button type="button" class="btn btn-default btn-primary" onclick="window.location='product_out_affirm.php'">产品出库记录管理</button> 
               <p></p>
                <button type="button" class="btn btn-default btn-primary" onclick="window.location='component_enter_plan.php'">零件入库计划管理</button>
                 <p></p>
                <button type="button" class="btn btn-default btn-primary" onclick="window.location='component_enter_affirm.php'">零件入库记录管理</button>
                <p></p>
                  <button type="button" class="btn btn-default btn-primary" onclick="window.location='component__plan.php'">零件出库计划管理</button>
                 <p></p>
                <button type="button" class="btn btn-default btn-primary" onclick="window.location='component_out_affirm.php'">零件出库记录管理</button>
                <p></p>
                <button type="button" class="btn btn-default btn-primary" onclick="window.location='product_sell_plan.php'">产品销售计划管理</button>
                <p></p>
                <button type="button" class="btn btn-default btn-primary" onclick="window.location='product_sell_affirm.php'">产品销售记录管理</button>
                <p></p>
                <button type="button" class="btn btn-default btn-primary" onclick="check()">确认采购计划</button>
                <p></p>
            </div>
            <div class="col-md-10 column">
            <h3>product_out_affirm_table</h3>
               <table class="table table-bordered table-hover" id="plan_table">
                  <thead>
                     <tr>
                        <th>
                        	product_out_plan_id
                        </th>
                        <th>
                           product_id
                        </th>
                        <th>
                           product_name
                        </th>
                        <th>
                           out_count
                        </th>
                        <th>
                           affirm_person
                        </th>
                        <th>
                           is_out
                        </th>
                     </tr>
                  </thead>
				<?php 
					echo '<tbody>';
					for($j=0;$j<$i;$j++){
						echo "<tr><th>".$product_enter_plan_id[$j]."</th>";
						echo "<th>".$product_id[$j]."</th>";
						echo "<th>".$product_name[$j]."</th>";
						echo "<th>".$enter_count[$j]."</th>";
						echo "<th>".$affirm_person[$j]."</th>";
						echo "<th>".$is_entered[$j]."</th>";
						echo "<th><input type='checkbox'/></th></tr>";
					}
					echo '</tbody>';
				?>
               </table>
    
            </div>
         </div>
      </div>
   </div>
</div>
 
      <!-- jQuery (Bootstrap 的 JavaScript 插件需要引入 jQuery) -->
      <script src="./jquery.js"></script>
      <!-- 包括所有已编译的插件 -->
      <script src="./bootstrap/js/bootstrap.min.js"></script>
   </body>

   <script>
  
// JavaScript Document

    //传入参数：请求的目的地址，请求的参数，响应后的回调函数
	function check(){
		var tab=document.getElementById("plan_table");
     	var rows=tab.rows;
		for(var i=1;i<rows.length;i++)
		{
			var box = rows[i].cells[6].childNodes[0];
			if(box.checked==true){
				rows[i].cells[5].innerHTML="yes";
				postPara="product_enter_plan_id="+rows[i].cells[0].innerHTML;
				AJAX('product_enter_affirm_exe.php',postPara,function(str){
					alert(str);		
				});
			}
		}
	}
    function AJAX(php,para,callback)
    {
        var xmlhttp;
        //首先，可以对参数进行特殊处理
        //如果没有回调函数，回调函数为空
        if(!callback)
        {
            callback=function(){}
        }
        //创建 XMLHttpRequest 请求
        if(window.XMLHttpRequest)
        {
            xmlhttp=new XMLHttpRequest();
        }
        else
        {
            xmlhttp=new ActiveXObject("Microsoft.XMLHTTP");
        }
        xmlhttp.onreadystatechange=function()
        {
            if(xmlhttp.readyState==4 && xmlhttp.status==200)
            {
                callback(xmlhttp.responseText);
            }
        }
        xmlhttp.open("POST",php,true);
        xmlhttp.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
		//alert(para);
        xmlhttp.send(para);
    }
    </script>
</html>
