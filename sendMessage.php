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

echo $sender .  "\n";
echo $subject . "\n";
echo $message . "\n";
$message = $sender . "\r\n" . $message;
$headers = "Content-type: text/html; charset=iso-8859-1" . "\r\n";
$headers .= "From: Personal Website <will.borie1@gmail.com>" . "\r\n";
$headers .= "Reply-To: Personal Website <will.borie1@gmail.com>" . "\r\n";
$headers .= "X-Mailer: PHP/" . "\r\n";
echo $headers;

if (mail('will.borie@gmail.com', $subject, $message, $headers)) {
    exit("success");
} else exit("failure");

?>
