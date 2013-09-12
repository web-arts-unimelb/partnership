<?php


// hook_init
function partnership_init() {
  drupal_add_css(drupal_get_path('module', 'partnership') . '/css/partnership.css');
}

// hook_form_alter
function partnership_form_alter(&$form, &$form_state, $form_id) {
	if($form_id === 'organisation_node_form') {
		$form['title']['#autocomplete_path'] = 'nodes/org_autocomplete';		

		// Create new node
		if(empty($form['nid']['#value'])) {
			$form['#validate'][] = '_unique_organisation_validator';
		}		
	}
	elseif($form_id == 'user_login') {
		$scripts = drupal_add_js();
  	$script_path = drupal_get_path('module', 'partnership') . '/js/partnership.js';

  	if(!isset($scripts[$script_path])) {
    	drupal_add_js($script_path);
  	}
	}
}

// hook_menu
function partnership_menu() {
  $items['nodes/org_autocomplete'] = array(
		'title' => 'Autocomplete for node title',
		'page callback' => '_nodes_org_autocomplete',
    'access arguments' => array('create partnership content'),
		'acccess callback' => TRUE, 
    'type' => MENU_CALLBACK
	);
  return $items;
}


// hook_preprocess_page
function partnership_preprocess_page(&$variables) {
}

function _nodes_org_autocomplete($string = "") {
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
