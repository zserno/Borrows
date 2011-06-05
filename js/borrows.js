// Handle user interactions on calendar.
(function ($, Drupal) {
  Drupal.behaviors.borrowsCalendar = function(context) {
    $("#calendar .week input.form-checkbox", context).click(function() {
      $("#calendar", context).block({
        message: "<h1>Processing...</h1>",
      });
    });
  }
})(jQuery, Drupal)
