<?php

echo '<!DOCTYPE html><html><head><meta http-equiv="Content-Type" content="text/html;charset=utf-8">
<link rel="stylesheet" type="text/css" href="../css/common.css">
<title>test</title></head><body style="margin:7px;">';

echo "<p style='white-space:pre;'>";

/* 
foreach(['en','ru'] as $e){
	$s=file_get_contents("c:/Users/user/git/test/test/p2data$e.txt");
	$c=gzdeflate($s, 9);
	$a=base64_encode($c);
	var_dump(strlen($s),strlen($c),strlen($a));
	file_put_contents("../scripts/p2data$e.js","gd='$a'");
}
exit;
*/

//$c = file_get_contents("c:/Users/user/git/test/test/en29.txt");
$c = file_get_contents("c:/Users/user/git/test/test/ru3235.txt");
$a = explode("\n", $c);
$r = [];
$u=[];
foreach ($a as $e) {
	if(strlen($e)==0){//prevents $en corruption
		continue;
	}
	$s=iconv("CP1251", "UTF-8", $e);
	$en=$s==$e;
	$ra=$en?'a-z':'-а-я';
	if (preg_match("/^\d+\{(.+)\}\{(.+)\}/u", $s, $g)) {
		for ($n = 1; $n < 3; $n++) {
			preg_match_all("/([$ra]+)\s([$ra]+)/u", $g[$n], $m[$n]);
		}
		for ($i[1] = 0; $i[1] < count($m[1][0]); $i[1]++) {
			for ($i[2] = 0; $i[2] < count($m[2][0]); $i[2]++) {
				$a = [];
				for ($n = 1; $n < 3; $n++) {
					for ($j = 1; $j < 3; $j++) {
						$a[] = $m[$n][$j][$i[$n]];
						$u[]=$m[$n][$j][$i[$n]];
					}
				}
				sort($a);
				$r[]=implode(" ", $a);
				// echo implode(" ", $a) . "\n";
			}
		}
	}
}
// echo count($r);
$r=array_unique($r);
sort($r);
echo "<table class='single'>";
$str=$en?'abcdefghijklmnopqrstuvwxyz':'абвгдежзийклмнопрстуфхцчшщъыьэюя';
foreach ($r as $e) {
	$s="";
	for ($j = 0; $j < mb_strlen($str); $j++) {
		$c = mb_substr($str, $j, 1);
		$i=mb_substr_count($e, $c);
		if($i>1){
			$s.=str_repeat($c,$i-1);
		}
	}

	echo "<tr><td>$e<td>$s\n";
}
echo "</table>\n".count($r);
// $r=array_unique($u);
// sort($r);
// echo implode('","',$r);
exit;

echo "<table><tr><td style='vertical-align: top;'>";//top table

$c=file_get_contents("c:/Users/user/git/test/test/outru3.txt");
$a=explode("\n",$c);
$r=[];
foreach($a as $e){
	$s=iconv("CP1251", "UTF-8", $e);
	if(preg_match("/(.+) suml=(\d+)/u",$s,$g)){
		$r[]=array_slice($g,1);
	}
}

usort($r, fn($a, $b) => $a[1]-$b[1] );

$al="абвгдежзийклмнопрстуфхцчшщъыьэюя";
$b=mb_str_split($al);

echo "<table class='single'>";
$i=1;
foreach($r as $e){
	$w=$e[0];
	$a=explode(' ',$w);
	$last=array_pop($a);
	$s='';
	foreach($b as $q){
		$found=false;
		foreach($a as $e1){
			if(str_contains($e1,$q)){
				$found=true;
				break;
			}
		}
		if(!$found){
			$s.=$q;
		}
	}
	echo "<tr><td>$i<td>$w<td>$e[1]";//."<td>".$s."=".mb_strlen($s)." ".mb_strlen($last);
	$i++;
}
echo "</table>";

echo "<td>";//top table

//english
$c=file_get_contents("c:/Users/user/git/test/test/outen3.txt");
$a=explode("\n",$c);
$r=[];
foreach($a as $e){
	$s=$e;
	if(preg_match("/(.+) bits=25 (\w) suml=(\d+)/u",$s,$g)){
		$r[]=array_slice($g,1);
	}
}

usort($r, fn($a, $b) => $a[2]-$b[2] );

echo "<table class='single'>";
$i=1;
foreach($r as $e){
	echo "<tr><td>$i<td>$e[0]<td>$e[2]<td>$e[1]";
	$i++;
}
echo "</table>";

echo "</table>";//top table
