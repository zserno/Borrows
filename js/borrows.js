// Handle user interactions on calendar.
(function ($, Drupal) {
  Drupal.behaviors.borrowsCalendar = function(context) {
    $("#calendar .week input.form-checkbox", context).click(function() {
      $("#calendar", context).block({
        // @TODO make path dynamic.
        message: '<img src="' + Drupal.settings.basePath + 'sites/all/modules/borrows/images/loader.gif" /><h2>' + Drupal.t("Checking availability...") + '</h2>',
        css: {width: 'auto', padding: '5px'}
      });
      var dataToSend = {'nid': '12'};
      borrowsAjax(dataToSend);
    });
  }
})(jQuery, Drupal)

function borrowsAjax(dataToSend) {
  $.getJSON(Drupal.settings.basePath + 'borrows_ajax/' + dataToSend.nid, function(data) {
  });
}
