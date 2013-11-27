(function ($) {
  Drupal.behaviors.rubik_subtheme = {
    attach:function(context, settings) {
			_type_to_hide_show_program();
		}
	}

	function _type_to_hide_show_program() {
		// Because the option is dynamically added. 'Ready' is not going to work, so use waitUntilExists plugin
		// https://gist.github.com/buu700/4200601
		$('form#partnership-node-form #edit-field-par-type select.shs-select-level-1 option').waitUntilExists(function() {
			// Initial
			var init_selected_text = jQuery('form#partnership-node-form #edit-field-par-type select.shs-select-level-1').find('option:selected').text(); 
			if(init_selected_text == 'Teaching and Learning' || init_selected_text == 'Engagement') {
      	jQuery('form#partnership-node-form #edit-field-par-program').show();
      }
      else {
        jQuery('form#partnership-node-form #edit-field-par-program').hide();
      }

			// Change
			// 
      jQuery('form#partnership-node-form #edit-field-par-type select.shs-select-level-1').change(function(){
				var selected_text = jQuery(this).find('option:selected').text();
				if(selected_text == 'Teaching and Learning' || selected_text == 'Engagement') {
					jQuery('form#partnership-node-form #edit-field-par-program').show();
				}
				else {
					jQuery('form#partnership-node-form #edit-field-par-program').hide();
				} 
      });
    });
  }

}(jQuery));
