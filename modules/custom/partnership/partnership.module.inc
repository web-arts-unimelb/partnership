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
	elseif($form_id == 'views_exposed_form') {
		_add_javascript();

		if($form['#id'] === 'views-exposed-form-organisation-find-partner-relationship') {
			$url = "";
	
			$form['buttons']['export_csv'] = array(
      	'#type'  => 'link',
				'#title' => t('Export'),
				'#href' => $url,
				'#attributes' => array(
					'class' => array('mybutton', 'mybutton-left'),
				),
      );	
		}
	}
	elseif($form_id == 'user_login') {
		_add_javascript();
	}
}

// hook_views_pre_render(&$view)
function partnership_views_pre_render(&$view) {
	global $user;

	//test
	/*
	echo "<pre>";
	print_r($view);
	echo "</pre>";
	die;
	*/

	if(isset($view->current_display)) {
		// On find partner page
    if($view->current_display === 'page_find_partner') {
      $field_name = "nothing"; // Edit button
      if(!in_array('admin', $user->roles) && !in_array('site admin', $user->roles)) {
        unset($view->field[$field_name]); // Hide edit button
      }
    }

		// On find partner relationship page
		if($view->current_display === 'find_partner_relationship') {
			$field_name = "nothing_1"; // Edit button
			if(!in_array('admin', $user->roles) && !in_array('site admin', $user->roles)) {
				unset($view->field[$field_name]); // Hide edit button
			}
		}

		// On view partner page
    if($view->current_display === 'block_1') {
			// add_relationship_button
      $add_relationship_button = "nothing_1"; 
      if(!in_array('admin', $user->roles) && !in_array('site admin', $user->roles)) {
        unset($view->field[$add_relationship_button]); // Hide
      }

			$export_button = 'nothing_2'; 
			if(	
					!in_array('authenticated user', $user->roles) ||
					in_array('admin', $user->roles) ||
					in_array('site admin', $user->roles)
				) 
			{
				unset($view->field[$export_button]); // Hide
			}
    }

		// On view relationship page
		if($view->current_display === 'block_2') {
			// Only admin and site admin are able to see partner's detail
			if(!in_array('admin', $user->roles) && !in_array('site admin', $user->roles)) {
      	// Only admin & site admin can see partner's contact detail
        unset($view->field['field_par_contact_name']);
        unset($view->field['field_par_contact_email']);
        unset($view->field['field_par_contact_phone']);
			}
		}

		// On export partner-relationship-csv page
		if($view->current_display === 'views_data_export_1' || $view->current_display === 'views_data_export_2') {
			if(!in_array('admin', $user->roles) && !in_array('site admin', $user->roles)) {
				// Only admin & site admin can see partner's contact detail
				unset($view->field['field_par_contact_name']);
				unset($view->field['field_par_contact_email']);
				unset($view->field['field_par_contact_phone']);
			}
		}


		// On view partner page
		if($view->current_display === 'partner_attachment') {
      $field_name = "nothing_1"; // Edit button
      if(!in_array('admin', $user->roles) && !in_array('site admin', $user->roles)) {
        unset($view->field[$field_name]); // Hide edit button
      }
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

// hook_menu_local_tasks_alter(&$data, $router_item, $root_path)
function partnership_menu_local_tasks_alter(&$data, $router_item, $root_path) {
	global $user;

	if(in_array('admin', $user->roles) || in_array('site admin', $user->roles)) {
		// If content type == partnership
		if(
			isset($router_item['page_arguments'][0]->type) &&
			$router_item['page_arguments'][0]->type == 'partnership' 
		)
		{
			if( isset($router_item['page_arguments'][0]->field_par_org['und'][0]['target_id']) )
			{
				$partner_id = $router_item['page_arguments'][0]->field_par_org['und'][0]['target_id'];
				$url = 'node/'. $partner_id;
			} else {
				$url = 'node';
			}

  		$data['tabs'][0]['output'][] = array(
    		'#theme' => 'menu_local_task',
    		'#link' => array(
     			'title' => t('View all'),
     			'href' => $url,
     			'localized_options' => array(
       			'attributes' => array(
         			'title' => t('View all relationships'),
       			),
     			),
    		),
  		);

		}
		elseif(
			isset($router_item['page_arguments'][0]->type) &&
      $router_item['page_arguments'][0]->type == 'organisation'
		) {
			$nid = $router_item['page_arguments'][0]->nid;
			$url = "partner-relationship/$nid/file.csv";

			// If content type == partner
			$data['tabs'][0]['output'][] = array(
        '#theme' => 'menu_local_task',
        '#link' => array(
          'title' => t('Export'),
          'href' => $url,
          'localized_options' => array(
            'attributes' => array(
              'title' => t('Export partner'),
            ),
          ),
        ),
      );
	
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
