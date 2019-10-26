<?php
$labels = array();
foreach ($item->show_full_columns(TRUE) as $col) {
	if ($show_state_options && $col['Field'] == 'state') {
		$current_state = $data[$col['Field']];
		continue;
	}
	$labels[$col['Field']] = $col['Comment'];
}
?>