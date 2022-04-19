<?php
if (!isset($_COOKIE["visit"])) {
    setcookie("visit", time(), time() +  86400);
    $date_value = time();
}
else
    $date_value = $_COOKIE["visit"];

$waiting_day = strtotime('+1 day', $date_value);
$expire_date = date('m/d/Y H:i:s', $waiting_day);
$first_visit = date('m/d/Y H:i:s', $date_value);
// echo "fist visit: $first_visit <br>";
// echo "expire date: $expire_date <br>";

sleep(1);

$time_left = $waiting_day - time();

$days = floor($time_left / (60*60*24));
$time_left %= (60 * 60 * 24);

$hours = floor($time_left / (60 * 60));
$time_left %= (60 * 60);

$min = floor($time_left / 60);
$time_left %= 60;

$sec = $time_left;


echo "<strong>$hours";
echo "h$min";
echo "m$sec";
echo "s</strong>";
echo "<p>salut</p>";
?>
