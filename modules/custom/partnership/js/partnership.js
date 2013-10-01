(function ($) {
  Drupal.behaviors.partnership = {
    attach:function(context, settings) {
			// User login form
			jQuery('form#user-login input#edit-name').once('user_login_form_func', function(){
				jQuery('form#user-login input#edit-name').attr('size', '60');
      	jQuery('form#user-login input#edit-name').removeClass('fluid');

			});

			// Reset button on view exposed filter in find partner page.
			// https://drupal.org/node/1264316
			// form_alter is not working for it, not sure why
			var form_id = 'form#views-exposed-form-organisation-page-find-partner';
			jQuery(form_id + ' input#edit-reset').once('reset_find_partner_func', function(){
				jQuery(form_id + ' input#edit-reset').prop('type', 'button');
	      jQuery(form_id + ' input#edit-reset').on('click', function(){
					// Reset all value
					jQuery(form_id + ' input#edit-title').prop('value', '');
					jQuery(form_id + " option:selected").prop("selected", false);

					// Trigger click
					jQuery(form_id + " input#edit-submit-organisation").trigger('click');		
    	  });
			});


			// Reset button on view exposed filter in find partner relationship page.
			var form_id_1 = 'form#views-exposed-form-organisation-find-partner-relationship';
			jQuery(form_id_1 + ' input#edit-reset').once('reset_find_partner_relation_func', function(){
        jQuery(form_id_1 + ' input#edit-reset').prop('type', 'button');
        jQuery(form_id_1 + ' input#edit-reset').on('click', function(){
          // Reset all value
          jQuery(form_id_1 + " option:selected").prop("selected", false);
					jQuery(form_id_1 + " input#edit-combine").prop('value', '');
					jQuery(form_id_1 + " input#edit-field-depar-name-tid").prop('value', '');	
	
          // Trigger click
          jQuery(form_id_1 + " input#edit-submit-organisation").trigger('click');
        });
      });
		
		}
  }
}(jQuery));

