/**
 * Handle user interactions on calendar.
 */

(function ($, Drupal) {
  Drupal.behaviors.borrowsCalendar = function(context) {
    // Global semaphore to track if a booking has started.
    // E.g. there is at least one active checkbox.
    Drupal.settings.borrows.semaphore = false;
    // Remove submit button, e.g. when changing month.
    $("#borrows-submit").remove();

    $("#calendar .week input.form-checkbox", context).click(function() {
      // We're at the beginnign of the booking process.
      if (Drupal.settings.borrows.semaphore == false) {
        Drupal.settings.borrows.semaphore = true;

        // Block calendar with an overlay.
        $("#calendar", context).block({
          // @TODO Make path dynamic.
          message: '<img src="' + Drupal.settings.basePath + 'sites/all/modules/borrows/images/loader.gif" /><h2>' + Drupal.t("Checking availability...") + '</h2>',
          css: {width: 'auto', padding: '5px'},
        });

        // @TODO Use nid.
        var dataToSend = {'nid': '12'};
        borrowsAjax(dataToSend, $(this));
      }
      // We are in the booking process.
      else {
        // Unchecked a checkbox.
        if ($(this).attr('checked') == false) {
          // Remove all checkboxes after current one.
          var current = $("#calendar .week input.form-checkbox").index(this);
          $("#calendar .week input.form-checkbox:gt(" + current + ")").filter(":checked").attr('checked', false);

          // Check if no more active checkboxes left.
          if ($("#calendar .week input.form-checkbox[checked]").length == 0) {
            // Unset semaphore.
            Drupal.settings.borrows.semaphore = false;
            // Remove submit button.
            $("#borrows-submit").remove();
            // Restore all available checkboxes.
            Drupal.calendar.init();
          }
        }
        // Checked a checkbox.
        else {
          // Let's see if there's any unchecked checkbox between
          // current and the first one, then make them checked.
          var current = $("#calendar .week input.form-checkbox").index(this);
          $("#calendar .week input.form-checkbox:lt(" + current + ")").filter(":not(:checked)").attr('checked', true);
        }
      }

      // Debug.
      //console.log('Sempahore: ' + Drupal.settings.borrows.semaphore);
    });
  }
})(jQuery, Drupal)

function borrowsAjax(dataToSend, $checkbox) {
  $.getJSON(Drupal.settings.basePath + 'borrows_ajax/' + dataToSend.nid, function(data) {
    // We keep the next (allowed_days - 1) number of checkboxes,
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

    // Add submit button.
    $('<input type="submit" id="borrows-submit" value="' + Drupal.t('Submit') + '" />').appendTo($("#block-calendar_block-0")).click(function() {
      borrowsSubmit();
    });
  });
}

function borrowsSubmit() {
  var selectedDays = $("#calendar .week input.form-checkbox[checked]");

  if (borrowsValidate(selectedDays)) {
    selectedDays.each(function(value) {
      console.log($(this).val());
    });
  }
}

// @TODO Implement me.
function borrowsValidate(selectedDays) {
  console.log("Valid.");
  return true;
}
