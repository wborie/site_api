<?php  // ECHOing/VARDUMPing will cause json read issues in front end, put everything in the return

header('Access-Control-Allow-Origin: *');
header('Access-Control-Allow-Headers: *');
// var_dump($_SERVER);
$postBody = file_get_contents('php://input');
$postBody = json_decode($postBody);
if (!(isset($postBody) && isset($postBody->nodes) && isset($postBody->edges) && isset($postBody->startNodeName))) {
	exit(json_encode("Bad input"));
}
$nodes = json_encode($postBody->nodes);
$edges = json_encode($postBody->edges);
$startNodeName = json_encode($postBody->startNodeName);

// for now, get rid of all '|', later on, make sure nodes can only be entered with a name that doesn't contain | (in dijkstra's handle submit, strip '|')
// do the same for '-' b/c that is used in the name of the edges !!!!!
// '|' is used in edges to separate name from weight and '||' will be used in sending nodes and edges to the C++ backend
// | - [ ]
// !!!!! Will have to insert into incoming and outgoing edges using Graph.h !!!!

// var_dump($nodes);
// var_dump($edges);
// var_dump($startNodeName);
// var_dump(json_encode($nodes));
// var_dump(json_encode($edges));
// var_dump(json_encode($startNodeName));
exec("./graphDijkstra $nodes $edges $startNodeName", $output);
$nodes = explode(",", $output[0]);
$parents = explode(",", $output[1]);
$distances = explode(",", $output[2]);
$traversalOrder1 = explode(",", $output[3]);
$traversalOrder2 = array(); // Since undirected graph, using both possible edge IDs (A-B in traversalOrder1 and B-A in traversalOrder2)
$parentOverrideTracker1 = explode(",", $output[4]);
$parentOverrideTracker2 = array();
for($i = 0; $i < count($traversalOrder1); $i++) {
	$separator = strpos($traversalOrder1[$i], "-");
	$length = strlen($traversalOrder1[$i]);
	array_push($traversalOrder2, substr($traversalOrder1[$i], $separator + 1, $length) . "-" . substr($traversalOrder1[$i], 0, $separator));
}

for($i = 0; $i < count($parentOverrideTracker1); $i++) {
	$separator = strpos($parentOverrideTracker1[$i], "-");
	$length = strlen($parentOverrideTracker1[$i]);
	array_push($parentOverrideTracker2, substr($parentOverrideTracker1[$i], $separator + 1, $length) . "-" . substr($parentOverrideTracker1[$i], 0, $separator));
}

$resultObject = new stdClass();
$resultObject->nodes = $nodes;
$resultObject->parents = $parents;
$resultObject->distances = $distances;
$resultObject->traversalOrder1 = $traversalOrder1;
$resultObject->traversalOrder2 = $traversalOrder2;
$resultObject->parentOverrideTracker1 = $parentOverrideTracker1;
$resultObject->parentOverrideTracker2 = $parentOverrideTracker2;

exit(json_encode($resultObject));


// exec("./test firstParam", $output);

// // var_dump($output);

// $test = json_encode($output);
// exit($test);

?>
