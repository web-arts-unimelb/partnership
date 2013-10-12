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
			jQuery('form#views-exposed-form-organisation-page-find-partner #edit-reset').once('reset_find_partner_func', function(){
        jQuery('form#views-exposed-form-organisation-page-find-partner #edit-reset').on('click', function(){
          // Reset all value
          jQuery('form#views-exposed-form-organisation-page-find-partner input#edit-title').prop('value', '');
          jQuery("form#views-exposed-form-organisation-page-find-partner option:selected").prop("selected", false);
          jQuery("form#views-exposed-form-organisation-page-find-partner .form-type-bef-checkbox input").prop('checked', false);
      
          // Trigger click
          jQuery("form#views-exposed-form-organisation-page-find-partner input#edit-submit-organisation").trigger('click');
        });
      });


			// Reset button on view exposed filter on find partner relationship page.
			jQuery('form#views-exposed-form-organisation-find-partner-relationship #edit-reset').once('reset_find_partner_relation_func', function(){
        jQuery('form#views-exposed-form-organisation-find-partner-relationship #edit-reset').on('click', function(){
          // Reset all value
          jQuery("form#views-exposed-form-organisation-find-partner-relationship option:selected").prop("selected", false);
          jQuery("form#views-exposed-form-organisation-find-partner-relationship input#edit-combine").prop('value', '');
          jQuery("form#views-exposed-form-organisation-find-partner-relationship input#edit-field-depar-name-tid").prop('value', '');

          // Trigger click
          jQuery("form#views-exposed-form-organisation-find-partner-relationship input#edit-submit-organisation").trigger('click');
        });
      });
	
			// Export csv button on find partner relationship page
			var form_id = 'form#views-exposed-form-organisation-find-partner-relationship';
			jQuery('form#views-exposed-form-organisation-find-partner-relationship #edit-export-csv').once('export_find_partner_relation_func', function(){
				jQuery('form#views-exposed-form-organisation-find-partner-relationship #edit-export-csv').on('click', function(){
					var partner_search = jQuery('form#views-exposed-form-organisation-find-partner-relationship #edit-combine').prop('value');
					var sector_search = jQuery('form#views-exposed-form-organisation-find-partner-relationship #edit-field-par-sector-tid').val();
					var career_search = jQuery('form#views-exposed-form-organisation-find-partner-relationship #edit-field-par-career-destination-tid').val();
					var deparment_search = jQuery('form#views-exposed-form-organisation-find-partner-relationship #edit-field-internal-department-tid').val();
					var type_search = jQuery('form#views-exposed-form-organisation-find-partner-relationship #edit-field-par-type-tid').val();

					var url = '/find-partner-relationship/file.csv?' + 
						'combine=' + partner_search + '&' + 
						'field_par_sector_tid=' + sector_search + '&' + 
						'field_par_career_destination_tid=' + career_search + '&' + 
						'field_internal_department_tid=' + deparment_search + '&' +
						'field_par_type_tid=' + type_search;				
	
					//console.log(url);
					window.location = url;	
				});
			});

			
		}
  }
}(jQuery));

