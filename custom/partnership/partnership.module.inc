<?php

// hook_form_alter
function partnership_form_alter(&$form, &$form_state, $form_id) {
	// When create new organisation
	if( $form_id === 'organisation_node_form' && empty($form['nid']['#value']) ) {
		$form['title']['#autocomplete_path'] = 'nodes/autocomplete';
		$form['#validate'][] = '_unique_organisation_validator';	
	}
}

// hook_menu
function partnership_menu() {
  $items['nodes/autocomplete'] = array(
		'title' => 'Autocomplete for node title',
		'page callback' => '_nodes_autocomplete',
    'access arguments' => array('create partnership content'),
		'acccess callback' => TRUE, 
    'type' => MENU_CALLBACK
	);
  return $items;
}

function _nodes_autocomplete($string = "") {
  $matches = array();
  $result = db_select('node', 'n')
    ->fields('n', array('nid', 'title'))
    ->condition('title', '%' . db_like($string) . '%', 'LIKE')
		->condition('type', 'organisation')
    ->execute();

  // save the query to matches
  foreach ($result as $row) {
    $matches[$row->title] = check_plain($row->title);
  }

  // Return the result to the form in json
  drupal_json_output($matches);
}


function _unique_organisation_validator($form, &$form_state) {
	if($form_state['values']['title']) {
		$submitted_title = strtolower($form_state['values']['title']);	
		
		$query = db_select('node', 'n')->fields('n', array('nid', 'title'));
		$query->where('LOWER(title) = :title', array(':title' => $submitted_title));

   	$result = $query->condition('type', 'organisation')->execute();

		$msg = "";	
		if($result) {
			foreach($result as $row) {
				$nid = $row->nid;
				$url = "/node/$nid";
				$msg = "Partner already exists! ". "<a href=\"$url\">Please view and edit it</a>."; 
				break;	
			}
				
			if(!empty($msg)) {
				form_set_error('organisation', $msg);
			}
		}
		else {

		}
  }
}
