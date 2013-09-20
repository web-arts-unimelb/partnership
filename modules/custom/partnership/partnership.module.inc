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
	if($form_id == 'views_exposed_form') {
		_add_javascript();	

	}
	elseif($form_id == 'user_login') {
		_add_javascript();
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

/**
 * Implements hook_preprocess_status_messages().
 *
 * Remove the login warning message, anon users can only see the login form anyway.
 */
function partnership_preprocess_status_messages(&$vars) {
  $messages = drupal_get_messages('error');
  if (!empty($messages['error'])) {
    $errors = $messages['error'];
    foreach ($errors as $error) {
      if ($error == t('Access denied. You may need to login below or register to access this page.')) {
        continue;
      }
      drupal_set_message($error, 'error');
    }
  }
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


function _add_javascript() {
	$scripts = drupal_add_js();
  $script_path = drupal_get_path('module', 'partnership') . '/js/partnership.js';

  if(!isset($scripts[$script_path])) {
		drupal_add_js($script_path);
  }
}
