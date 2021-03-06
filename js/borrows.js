/**
 * Handle user interactions on calendar.
 */

(function ($, Drupal) {
  Drupal.behaviors.borrowsCalendar = function(context) {
    // Global semaphore to track if a booking has started.
    // E.g. there is at least one active checkbox.
    Drupal.settings.borrows.semaphore = false;

    // Apply borrows status classes on days.
    $("#calendar .week span.borrows-booked").parents('div.hok').addClass('borrows-booked');

    // Remove submit button, e.g. when changing month.
    $("#borrows-form").remove();

    $("#calendar .week input.form-checkbox").click(function() {
      // We're at the beginning of the booking process.
      // No checkboxes are checked at this point.
      if (Drupal.settings.borrows.semaphore == false) {
        Drupal.settings.borrows.semaphore = true;

        // Block calendar with an overlay.
        $("#calendar").block({
          message: '<img src="' + Drupal.settings.basePath + 'sites/all/modules/borrows/images/loader.gif" /><h2>' + Drupal.t("Checking availability...") + '</h2>',
          css: {width: 'auto', padding: '5px'},
        });

        var dataToSend = {'nid': Drupal.settings.borrows.nid};
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
            // Block calendar with an overlay.
            $("#calendar").block({
              message: '<img src="' + Drupal.settings.basePath + 'sites/all/modules/borrows/images/loader.gif" /><h2>' + Drupal.t("Checking availability...") + '</h2>',
              css: {width: 'auto', padding: '5px'},
            });
            // Unset semaphore.
            Drupal.settings.borrows.semaphore = false;
            // Remove submit button.
            $("#borrows-form").remove();
            // Restore all available checkboxes.
            Drupal.calendar = new calendar();
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
    }).parents('div.hok').addClass('borrows-available');
  }
})(jQuery, Drupal)

function borrowsAjax(dataToSend, $checkbox) {
  $.getJSON(Drupal.settings.basePath + 'borrows_ajax/' + dataToSend.nid, function(data) {

    // We keep the next [allowed_days - 1] number of checkboxes,
    // since current one is already checked.
    var i = data.allowed_days - 1;
    $checkbox.addClass('borrows-available');

    $parent = $checkbox.parents('div.hok');
    // Go through current week.
    // @TODO consider merging these 2 each() structures into one,
    // e.g. $.each($parent.nextAll('div.hok').find('input[type=checkbox]')
    $.each($parent.nextAll('div.hok'), function(index) {
      $.each($(this).find('input[type=checkbox]'), function(j) {
        if (i > 0) {
          $(this).addClass('borrows-available');
          // Weekend days don't count into allowed days.
          if (!$(this).hasClass('borrows-weekend')) {
            i--;
          }
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
    var submit = '<form id="borrows-form" method="POST" action="' + Drupal.settings.basePath + 'borrows-review">';
    submit += '<input type="submit" id="borrows-submit" value="' + Drupal.t('Save booking') + '" />';
    submit += '<input type="hidden" name="borrows_nid" value="0" />';
    submit += '<input type="hidden" name="borrows_start" value="0" />';
    submit += '<input type="hidden" name="borrows_end" value="0" />';
    submit += '</form>';

    $("#block-calendar_block-0").append($(submit).submit(function() {
      return borrowsSubmit();
    }));
  });
}

function borrowsValidate(start, end) {
  // Check if start or end is weekend day.
  var valid = true;
  valid = valid && !$("#borrowdate-" + start).hasClass('borrows-weekend');
  valid = valid && !$("#borrowdate-" + end).hasClass('borrows-weekend');

  return valid;
}

// Submit callback.
function borrowsSubmit() {
  var start = $("#calendar .week input.form-checkbox[checked]:first").val();
  var end = $("#calendar .week input.form-checkbox[checked]:last").val();
  var nid = Drupal.settings.borrows.nid;

  if (borrowsValidate(start, end)) {
    $("#borrows-form input[name=borrows_nid]").val(Drupal.settings.borrows.nid);
    $("#borrows-form input[name=borrows_start]").val(start);
    $("#borrows-form input[name=borrows_end]").val(end);

    return true;
  }
  else {
    // Nice error message here...
    $('<div class="messages error">' + Drupal.t('Validation error: start or end date cannot be weekend or public holiday.') + '</div>').insertBefore("#block-calendar_block-0").hide().fadeIn();
    return false;
  }
}
