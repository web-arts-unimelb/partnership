(function ($) {
  Drupal.behaviors.partnership = {
    attach:function(context, settings) {
			// User login form
			jQuery('form#user-login input#edit-name').once('user_login_form_func', function(){
				jQuery('form#user-login input#edit-name').attr('size', '60');
      	jQuery('form#user-login input#edit-name').removeClass('fluid');

			});
		}
  }
}(jQuery));
