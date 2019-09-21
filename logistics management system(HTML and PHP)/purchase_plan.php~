<!DOCTYPE html>
<html>
   <?php
        session_start();
        @ $db=new mysqli('localhost','root','gdhygjzx7','db_class');
        if (mysqli_connect_errno()) {
            echo 'Error:Could not connect to database.Please try again later.';
            exit;
        }
        $provider_query="select provider_name from provider;";
        $provider_result=$db->query($provider_query);
        $component_id="select component_id from component;";
        $component_id_result=$db->query($component_id);
        $buyer="select name from account where dpt_id=2;";
        $buyer_result=$db->query($buyer);
        echo "<script>var provider_name=new Array(); var i=0;
            var component_id=new Array();
            var buyer=new Array();</script>";
        while($provider_row=mysqli_fetch_assoc($provider_result)){
            echo "<script> 
                provider_name[i]='".$provider_row['provider_name']."';
                i++;
                </script>";
        }
        echo "<script>i=0;</script>";
        while($component_id_row=mysqli_fetch_assoc($component_id_result)){
            echo "<script> 
                component_id[i]='".$component_id_row['component_id']."';
                i++;</script>";
        }
        echo "<script>i=0;</script>";
        while($buyer_row=mysqli_fetch_assoc($buyer_result)){
            echo "<script> 
                buyer[i]='".$buyer_row['name']."';
                i++;</script>";
        }
        echo "<script>i=0;</script>";
        $provider_result=$db->query($provider_query);
        $component_id_result=$db->query($component_id);
        $buyer_result=$db->query($buyer);
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
                  <button type="button" class="btn btn-default btn-primary" onclick="window.location='component_out_plan.php'">零件出库计划管理</button>
                 <p></p>
                <button type="button" class="btn btn-default btn-primary" onclick="window.location='component_out_affirm.php'">零件出库记录管理</button>
                <p></p>
                <button type="button" class="btn btn-default btn-primary" onclick="window.location='product_sell_plan.php'">产品销售计划管理</button>
                <p></p>
                <button type="button" class="btn btn-default btn-primary" onclick="window.location='product_sell_affirm.php'">产品销售记录管理</button>
                <p></p>
                <button type="button" class="btn btn-default btn-primary" onclick="add()">编制采购计划</button> 
                <p></p>
                <button type="button" class="btn btn-default btn-primary" onclick="edit()">编辑采购计划</button> 
                <p></p>
                <button type="button" class="btn btn-default btn-danger">删除采购计划</button> 
                <p></p>
                <button type="button" class="btn btn-default btn-primary">批准采购计划</button>
                <p></p>
                <button type="submit" class="btn btn-default btn-primary" onclick="confirm()">confirm</button>
            </div>
            <div class="col-md-10 column">
            <h3>purchase_plan_table</h3>
               <table class="table table-bordered table-hover" id="plan_table">
                  <thead>
                     <tr>
                        <th>
                           provider_name
                        </th>
                        <th>
                           purchase_id
                        </th>
                        <th>
                           component_id
                        </th>
                        <th>
                           purchase_count
                        </th>
                        <th>
                           buyer
                        </th>
                        <th>
                           is_approved
                        </th>
                     </tr>
                  </thead>
                  <tbody>
                  <tr>
                     <th>
                     <select id="first">
                    <?php
                        while($provider_row=mysqli_fetch_assoc($provider_result)){
                            echo "<option value='".$provider_row['provider_name']."'>".$provider_row['provider_name']."</option>";
                        }
                    ?>
                    </select>
                    </th>
                    <th>
                     <select>
						<?php
							for($i=0;$i<100;$i++){
								echo "<option value='".$i."'>".$i."</option>";
							}
						?>
                    </select>
                     </th>
                      <th>
                     <select>
                        <?php
                            while($component_id_row=mysqli_fetch_assoc($component_id_result)){
                            echo "<option value='".$component_id_row['component_id']."'>".$component_id_row['component_id']."</option>";
                            }
                        ?>
                    </select>
                     </th>
                      <th>
                     <select>
                           <?php
							for($i=0;$i<10000;$i+=100){
								echo "<option value='".$i."'>".$i."</option>";
							}
						?>
                    </select>
                     </th>
                    <th>
                     <select>
                      <?php
                        while($buyer_row=mysqli_fetch_assoc($buyer_result)){
                            echo "<option value='".$buyer_row['name']."'>".$buyer_row['name']."</option>";
                        }   
                        ?>
                    </select>
                     </th>
                    <th>
                     <select>
                       <option value ="yes">yes</option>
                       <option value ="no">no</option>
                    </select>
                     </th>
                  </tr>
                  </tbody>
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
   function confirm(){
     var tab=document.getElementById("plan_table");
     var rows=tab.rows;

     for(var i=1;i<rows.length;i++)
     {
        var postPara = '';
        for(var j=0;j<rows[i].cells.length;j++)
        {
            var selectd = rows[i].cells[j].childNodes[1];
            var index = selectd.selectedIndex; // 选中索引
            var value = selectd.options[index].value; // 选中值
            rows[i].cells[j].innerHTML=value;
            if(j!=0) postPara+='&';
            postPara += ($.trim(rows[0].cells[j].innerHTML) + '=' + value);
			$.trim(postPara);
        }
		//alert(postPara);
    	AJAX('purchase_plan_exe.php',postPara,function(str){
			alert(str);		
		});
    	//window.location.href="purchase_plan_exe.php";
     }
   }
   function edit(){
         var tab=document.getElementById("plan_table");
         var rows=tab.rows;
         // alert(rows.length);
         for(var i=1;i<rows.length;i++)
         {
            for(var j=0;j<rows[i].cells.length;j++)
            {
                var cell=rows[i].cells[j];
                var select=document.createElement("select");
                if(j==0){
                   // alert(provider_name.length());
                    for(var k=0; k<provider_name.length; k++){
                        var option=document.createElement('option');
                        select.options.add(option);
                        option.value=provider_name[k];
                        option.text=provider_name[k];
                    }
                }
                if(j==1){
                    for(var k=0;k<100;k++){
                        var option=document.createElement('option');
                        select.options.add(option);
                        option.value=k;
                        option.text=k;      
                    }
                }
                if(j==2){
                    for(var k=0;k<component_id.length;k++){
                        var option=document.createElement('option');
                        select.options.add(option);
                        option.value=component_id[k];
                        option.text=component_id[k];        
                    }
                }
                if(j==3){
                    for(var k=0;k<100;k++){
                        var option=document.createElement('option');
                        select.options.add(option);
                        option.value=k*100;
                        option.text=k*100;      
                    }
                }
                if(j==4){
                    for(var k=0;k<buyer.length;k++){
                        var option=document.createElement('option');
                        select.options.add(option);
                        option.value=buyer[k];
                        option.text=buyer[k];       
                    }
                }
                if(j==5){
                    for(var k=0;k<2;k++){
                        var option=document.createElement('option');
                        select.options.add(option);
                        if(k==0){
                            option.value='yes';
                            option.text='yes';
                        }
                        else{
                            option.value='no';
                            option.text='no';
                        }
                    }
                }
                var tmp=document.createElement("text");
                var now=cell.childNodes[0];
                cell.removeChild(now);
                cell.appendChild(tmp);
                cell.appendChild(select);
            }
        }
    }
   function add(){
        var tab=document.getElementById("plan_table");
        var row_cnt=tab.rows;
        var tr=tab.insertRow();
        for(var i=0;i<6;i++){
            var th=tr.insertCell();
            var tmp=document.createElement('text');
            th.appendChild(tmp);
            var select=document.createElement('select');
            if(i==0){
                for(var j=0; j<provider_name.length; j++){
                    var option=document.createElement('option');
                    select.options.add(option);
                    option.value=provider_name[j];
                    option.text=provider_name[j];
                }
            }
            if(i==1){
                for(var k=0;k<5;k++){
                    var option=document.createElement('option');
                    select.options.add(option);
                    option.value=k;
                    option.text=k;      
                }
            }
            if(i==2){
                for(var k=0;k<component_id.length;k++){
                    var option=document.createElement('option');
                    select.options.add(option);
                    option.value=component_id[k];
                    option.text=component_id[k];        
                }
            }
            if(i==3){
                for(var k=0;k<4;k++){
                    var option=document.createElement('option');
                    select.options.add(option);
                    option.value=k*100;
                    option.text=k*100;      
                }
            }
            if(i==4){
                for(var k=0;k<buyer.length;k++){
                    var option=document.createElement('option');   
                    select.options.add(option);
                    option.value=buyer[k];
                    option.text=buyer[k];       
                }
            }
            if(i==5){
                for(var k=0;k<2;k++){
                    var option=document.createElement('option');
                    select.options.add(option);
                    if(k==0){
                        option.value='yes';
                        option.text='yes';
                    }
                    else{
                        option.value='no';
                        option.text='no';
                    }
                }
            }
            th.appendChild(select);
        }
   }
// JavaScript Document

    //传入参数：请求的目的地址，请求的参数，响应后的回调函数
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
