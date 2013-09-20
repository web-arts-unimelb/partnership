(function ($) {
  Drupal.behaviors.partnership = {
    attach:function(context, settings) {
			// User login form
			jQuery('form#user-login input#edit-name').once('user_login_form_func', function(){
				jQuery('form#user-login input#edit-name').attr('size', '60');
      	jQuery('form#user-login input#edit-name').removeClass('fluid');

			});

			// Reset button on view exposed filter
			// https://drupal.org/node/1264316
			// form_alter is not working for it, not sure why
			jQuery('form#views-exposed-form-organisation-page-find-partner input#edit-reset').once('reset_exposed_button_func', function(){
				jQuery('form#views-exposed-form-organisation-page-find-partner input#edit-reset').prop('type', 'button');
	      jQuery('form#views-exposed-form-organisation-page-find-partner input#edit-reset').on('click', function(){
					// Reset all value
					jQuery('form#views-exposed-form-organisation-page-find-partner input#edit-title').prop('value', '');
					jQuery("form#views-exposed-form-organisation-page-find-partner option:selected").prop("selected", false);

					jQuery("form#views-exposed-form-organisation-page-find-partner input#edit-submit-organisation").trigger('click');		
    	  });
			});
		
		}
  }
}(jQuery));

