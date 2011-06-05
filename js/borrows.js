// Handle user interactions on calendar.
(function ($, Drupal) {
  Drupal.behaviors.borrowsCalendar = function(context) {
    $("#calendar .week input.form-checkbox", context).click(function() {
      $("#calendar", context).block({
        // @TODO make path dynamic.
        message: '<img src="' + Drupal.settings.basePath + 'sites/all/modules/borrows/images/loader.gif" />',
      });
    });
  }
})(jQuery, Drupal)
