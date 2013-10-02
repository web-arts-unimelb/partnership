(function ($) {
  Drupal.behaviors.partnership = {
    attach:function(context, settings) {
			// User login form
			jQuery('form#user-login input#edit-name').once('user_login_form_func', function(){
				jQuery('form#user-login input#edit-name').attr('size', '60');
      	jQuery('form#user-login input#edit-name').removeClass('fluid');

			});

			// Reset button on view exposed filter on find partner page.
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


			// Reset button on view exposed filter on find partner relationship page.
			var form_id = 'form#views-exposed-form-organisation-find-partner-relationship';
			jQuery(form_id + ' input#edit-reset').once('reset_find_partner_relation_func', function(){
        jQuery(form_id + ' input#edit-reset').prop('type', 'button');
        jQuery(form_id + ' input#edit-reset').on('click', function(){
          // Reset all value
          jQuery(form_id + " option:selected").prop("selected", false);
					jQuery(form_id + " input#edit-combine").prop('value', '');
					jQuery(form_id + " input#edit-field-depar-name-tid").prop('value', '');	
	
          // Trigger click
          jQuery(form_id + " input#edit-submit-organisation").trigger('click');
        });
      });
	
			// Export csv button on find partner relationship page
			var form_id = 'form#views-exposed-form-organisation-find-partner-relationship';
			jQuery(form_id + ' #edit-export-csv').once('export_find_partner_relation_func', function(){
				jQuery(form_id + ' #edit-export-csv').on('click', function(){
					var partner_search = jQuery(form_id + ' #edit-combine').prop('value');
					var sector_search = jQuery(form_id + ' #edit-field-par-sector-tid').val();
					var career_search = jQuery(form_id + ' #edit-field-par-career-destination-tid').val();
					var deparment_search = jQuery(form_id + ' #edit-field-depar-name-tid').prop('value');
					var type_search = jQuery(form_id + ' #edit-field-par-type-tid').val();

					var url = '/find-partner-relationship-csv?' + 
						'combine=' + partner_search + '&' + 
						'field_par_sector_tid=' + sector_search + '&' + 
						'field_par_career_destination_tid=' + career_search + '&' + 
						'field_depar_name_tid=' + deparment_search + '&' +
						'field_par_type_tid=' + type_search;				
	
					//console.log(url);
					jQuery(form_id + ' #edit-export-csv').prop('href', url);
				});
			});

			
		}
  }
}(jQuery));

