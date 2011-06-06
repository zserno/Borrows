// Handle user interactions on calendar.
(function ($, Drupal) {
  Drupal.behaviors.borrowsCalendar = function(context) {
    // Initialize Drupal.settings.borrows.
    Drupal.settings.borrows = {};
    $("#calendar .week input.form-checkbox", context).click(function() {
      $("#calendar", context).block({
        // @TODO make path dynamic.
        message: '<img src="' + Drupal.settings.basePath + 'sites/all/modules/borrows/images/loader.gif" /><h2>' + Drupal.t("Checking availability...") + '</h2>',
        css: {width: 'auto', padding: '5px'},
      });
      var dataToSend = {'nid': '12'};
      borrowsAjax(dataToSend, $(this));
    });
  }
})(jQuery, Drupal)

function borrowsAjax(dataToSend, $checkbox) {
  $.getJSON(Drupal.settings.basePath + 'borrows_ajax/' + dataToSend.nid, function(data) {
    // We keep the following allowed_days - 1 number of checkboxes,
    // since current one is already checked.
    var i = data.allowed_days - 1;
    $checkbox.addClass('borrows-available');

    $parent = $checkbox.parents('div.hok');
    // Go through current week.
    $.each($parent.nextAll('div.hok'), function(index) {
      $.each($(this).find('input[type=checkbox]'), function(j) {
        if (i > 0) {
          $(this).addClass('borrows-available');
          i--;
        }
      });
    });

    // Go through remaining weeks.
    $.each($parent.parent('div.week').nextAll('div.week'), function(index) {
      $.each($(this).find('input[type=checkbox]'), function(j) {
        if (i > 0) {
          $(this).addClass('borrows-available');
          i--;
        }
      });
    });

    // Remove all non-available checkboxes.
    $.each($("#calendar input[type=checkbox]:not(.borrows-available)"), function(index) {
      $(this).parent('label').parent('span').html($(this).parent('label').text());
    });

    // Unblock calendar.
    $("#calendar").unblock();
  });
}
