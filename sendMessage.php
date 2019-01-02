<?php 

header('Access-Control-Allow-Origin: *');
$sender = $_REQUEST["email"];
$subject= $_REQUEST["subject"];
$message= $_REQUEST["message"];
if (!isset($sender)) {
	exit("failNoSender");
} else {
	if (!isset($subject)) $subject = "";
	if (!isset($message)) $message= "";
}

$headers = array('From' => $sender, 'Reply-To' => $sender, 'X-Mailer' => 'PHP/' . phpversion());

mail("will.borie@gmail.com", $subject, $message, $headers);
exit("success");

 ?>