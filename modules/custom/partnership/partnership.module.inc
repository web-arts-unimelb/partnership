<?php


// hook_init
function partnership_init() {
  drupal_add_css(drupal_get_path('module', 'partnership') . '/css/partnership.css');
}

// hook_form_alter
function partnership_form_alter(&$form, &$form_state, $form_id) {
	global $user;

	if($form_id === 'organisation_node_form') {
		$form['title']['#autocomplete_path'] = 'nodes/org_autocomplete';		

		// Create new node
		if(empty($form['nid']['#value'])) {
			$form['#validate'][] = '_unique_organisation_validator';
		}		
	}
	elseif($form_id === 'partnership_node_form') {
		$form['field_par_contact_full_name']['#attributes']['class'][] = 'hide';
	}
	elseif($form_id === 'views_exposed_form') {
		_add_javascript();

		if($form['#id'] === 'views-exposed-form-organisation-page-find-partner') {

		}

		if($form['#id'] === 'views-exposed-form-organisation-find-partner-relationship') {
			/*
			$form['buttons']['export_csv'] = array(
      	'#type'  => 'button', // It doesn't work as a button, it becomes submit
				'#title' => t('Export'),
				'#value' => t('Export'),
				'#attributes' => array(
					'class' => array('mybutton', 'mybutton-left'),
				),
      );
			*/

			// Reset search by contact name
			if(!in_array('admin', $user->roles) && !in_array('site admin', $user->roles)) {	
				$fieldName = 'field_par_contact_full_name_value';
				$info_key = 'filter-' . $fieldName;
				unset($form[$fieldName], $form['#info'][$info_key], $form_state['view']->display_handler->options['filters'][$fieldName], $form_state['view']->display_handler->handlers['filter'][$fieldName], $form_state['view']->filter[$fieldName]);	
			}
		}
	}
	elseif($form_id == 'user_login') {
		_add_javascript();

		drupal_set_title(t('Faculty of Arts Partnership Database'));

		$form['name']['#title'] = t('University username');
		$form['pass']['#title'] = t('University password');
	}
}

// hook_views_pre_render(&$view)
function partnership_views_pre_render(&$view) {
	global $user;

	if(isset($view->current_display)) {
		// On find partner page
    if($view->current_display === 'page_find_partner') {
			if(!in_array('admin', $user->roles) && !in_array('admin 1', $user->roles) && !in_array('site admin', $user->roles)) {
        unset($view->field['nothing']); // Hide edit button
      }

      if(!in_array('admin 1', $user->roles) && !in_array('site admin', $user->roles)) {
        unset($view->field['nothing_1']); // Hide delete button
      }

			// Loop through each entry
			if(isset($view->result) && count($view->result) > 0) {
				foreach($view->result as $entry) {
					if(isset($entry->field_field_org_flag)) {
						if(count($entry->field_field_org_flag) > 0) {
          		foreach($entry->field_field_org_flag as $key => &$item) {
            		if($item['rendered']['#markup'] == 'Restricted contact') {
              		$item['rendered']['#markup'] = '<span class="restricted-contact-flag">'. $item['rendered']['#markup']. '</span>';
            		}
          		}
        		}
					}
				}
			}

    }

		// On find partner relationship page
		if($view->current_display === 'find_partner_relationship') {
      if(!in_array('admin', $user->roles) && !in_array('admin 1', $user->roles) && !in_array('site admin', $user->roles)) {
      	unset($view->field['nothing']); // Hide edit button
			}

			if(!in_array('admin 1', $user->roles) && !in_array('site admin', $user->roles)) {
        unset($view->field['nothing_2']); // Hide delete button
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

			// Add html tag to 'Restricted contact'
			// It should have only 1 result
			if(isset($view->result[0]->field_field_org_flag)) {
				if(count($view->result[0]->field_field_org_flag) > 0) {
					foreach($view->result[0]->field_field_org_flag as $key => &$item) {
						if($item['rendered']['#markup'] == 'Restricted contact') {
							$item['rendered']['#markup'] = '<span class="restricted-contact-flag">'. $item['rendered']['#markup']. '</span>';
						}
					}
				}	
			}
    }

		// On view relationship page
		if($view->current_display === 'block_2') {
			// Only admin and site admin are able to see partner's detail
			if(!in_array('admin', $user->roles) && !in_array('site admin', $user->roles)) {
      	// Only admin & site admin can see partner's contact detail
        unset($view->field['nothing_1']); // partner contact full name e.g. Dr pete rad
        unset($view->field['field_par_contact_email']);
        unset($view->field['field_par_contact_phone']);
			}
		}

		// On export partner-relationship-csv page
		if($view->current_display === 'views_data_export_1' || $view->current_display === 'views_data_export_2') {
			if(!in_array('admin', $user->roles) && !in_array('site admin', $user->roles)) {
				// Only admin & site admin can see partner's contact detail
				unset($view->field['nothing']); // partner contact full name e.g. Dr pete rad
				unset($view->field['field_par_contact_email']);
				unset($view->field['field_par_contact_phone']);
			}
		}

		// On view partner page
		if($view->current_display === 'partner_attachment') {
			if(!in_array('admin', $user->roles) && !in_array('admin 1', $user->roles) && !in_array('site admin', $user->roles)) {
        unset($view->field['nothing_1']); // Hide edit button
      }

      if(!in_array('admin 1', $user->roles) && !in_array('site admin', $user->roles)) {
        unset($view->field['nothing_2']); // Hide delete button
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
          'title' => t('CSV'),
          'href' => $url,
          'localized_options' => array(
            'attributes' => array(
              'title' => t('Export partner'),
            ),
          ),
        ),
      );

			$url = "pdf/partner/$nid/partner.pdf";
			$data['tabs'][0]['output'][] = array(
        '#theme' => 'menu_local_task',
        '#link' => array(
          'title' => t('PDF'),
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

function partnership_node_presave($node) {
	if($node->type == 'partnership') {
		$first_name = $node->field_par_contact_first_name['und'][0]['value'];
		$surname = $node->field_par_contact_surname['und'][0]['value'];

		$full_name = $first_name. " ". $surname;
		$node->field_par_contact_full_name['und'][0]['value'] = $full_name;
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

/**
 * Implements hook_ldap_authentication_allowuser_results_alter().
 */
function partnership_ldap_authentication_allowuser_results_alter($ldap_user_entry, $name, &$result) {
	if ($result === FALSE) { // other module has denied user, should not override
  	return;
  }
  elseif ($result === TRUE) { // other module has allowed, maybe override
 		$ldap_dept_nums = _partnership_authentication_attribute_values($ldap_user_entry, 'uomdepartmentnumbers');
  	$allowed_dept_nums = _partnership_allowed_department_nums();
  	$ldap_match = array_intersect($ldap_dept_nums, $allowed_dept_nums);
	
		// White list
		$while_list_array = _partnership_white_list();
	
		if(!empty($ldap_match)) {
			// Normal ldap to allow people from arts
    	$result = TRUE;
  	}
		elseif( in_array($name, $while_list_array) ) {
			// White list, external to arts.
			$result = TRUE;
		}
  	else {
    	$result = FALSE;	
  	} 
  }

}

function _partnership_white_list() {
	return array(
		'acollette',
		'fdickson',
		'cstrong1',
		'robbm',

		'susannahm',
		'lmckay1',
		'rosssr1',
		'martinma',
	); 
}

function _partnership_allowed_department_nums() {
  /*
    100|Arts
    110|Asia Institute
    106|Culture and Communcation
    131|Historical and Philosophical Studies
    119|Languages and Linguistics
    166|Social and Political Sciences
    114|Unknown
    151|Unknown
  */

  return array(
    '100',
    '110',
    '106',
    '131',
    '119',
    '166',
    '114',
    '151'
  );
}

/**
 * Return an array of attribute values from the ldap entry.
 */
function _partnership_authentication_attribute_values($entry, $attribute) {
  $values = array();
  if (!is_array($entry['attr'][$attribute])) {
    return $values;
  }

  foreach ($entry['attr'][$attribute] as $key => $val) {
    if (is_numeric($key)) {
      $val = drupal_strtolower($val);
      $values[$val] = $val;
    }
  }
  return $values;
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
