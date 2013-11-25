<?php

/**
 * @file
 * The default format for adresses.
 */

$plugin = array(
  'title' => t('My address form (country-specific)'),
  'format callback' => 'addressfield_format_myaddress_generate',
  'type' => 'address',
  'weight' => -80,
);

/**
 * Format callback.
 *
 * @see CALLBACK_addressfield_format_callback()
 */
function addressfield_format_myaddress_generate(&$format, $address, $context = array()) {
  $format['street_block']['thoroughfare'] = array(
    '#title' => t('Street'),
    '#tag' => 'div',
    '#attributes' => array('class' => array('thoroughfare')),
    '#size' => 30,
    // The #required will be automatically set to FALSE when processing.
    '#required' => TRUE,
  );

  $format['street_block']['premise'] = array(
    '#title' => t(''),
    '#tag' => 'div',
    '#attributes' => array('class' => array('premise')),
    '#size' => 30,
  );

  $format['locality_block']['postal_code'] = array(
		'#title' => t('ZIP code'),
    '#size' => 10,
    '#required' => TRUE,
    '#attributes' => array('class' => array('postal-code')),
  );

	if ($context['mode'] == 'form') {
		// Form mode, move the country selector to the bottom of the form.
    $format['country']['#weight'] = 60;		
	}

}
