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
				jQuery('form#views-exposed-form-organisation-page-find-partner input#edit-reset').prop('type', 'reset');
	      jQuery('form#views-exposed-form-organisation-page-find-partner input#edit-reset').on('click', function(){
  	      history.go(0);
    	  });
			});
		}
  }
}(jQuery));

