<?php
  $n1 = $_POST['1'];
  $n2 = $_POST['2'];
  $op = $_POST['operateur'];
  
  function puissance($x,$y){
	  $res = 1;
	  for ($i=0;$i<$y;$i++)
		  $res *= $x;
	  return $res;
  }
  
  if(isset($n1) && isset($n2)){
  
	  if($n1 != NULL && $n2 != NULL){
		  if($op == 'division' AND $n2 == 0){
			  echo 'Impossible de diviser par 0, veuillez entrer une autre valeur';
		  }else{
			  switch($op){
			  case 'multiplication':
				  $result = $n1 * $n2;
				  break;  
  
			  case 'addition':
				  $result = $n1 + $n2;
				  break;
  
			  case 'division':
				  $result = $n1 / $n2;
				  break; 
			  case 'soustraction':
				  $result = $n1 - $n2;
				  break;
  
			  case 'puissance':
				  $result = puissance($n1,$n2);
				  break;
			  }
			  echo 'Result: ' . $result;
		  }
	  }
  }
  else{
	  echo 'Error';
  }
  ?>