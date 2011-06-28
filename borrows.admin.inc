<?php

/**
 * @file
 * Administration UI.
 */

/**
 * Admin form definition.
 */
function borrows_admin_form(&$form_state) {

  $form['borrows_prebooking'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of pre-booking days'),
    '#description' => t('You can specify a maximum number of days for future pre-bookings. <br />
      <em>E.g. 60 means that users are allowed to book a resource only in the following 60 days.</em>'),
    '#default_value' => variable_get('borrows_prebooking', BORROWS_PREBOOKING_DAYS),
  );

  $form['borrows_holidays'] = array(
    '#type' => 'textarea',
    '#title' => t('Public holidays'),
    '#description' => t('Define which weekdays are public holidays.<br/>
      Enter one day per line in the following format: YYYY-MM-DD, e.g. !date', array('!date' => date('Y-m-d'))),
    '#default_value' => variable_get('borrows_holidays', NULL),
    '#rows' => 8,
  );

  $form['borrows_weekends'] = array(
    '#type' => 'textarea',
    '#title' => t('Working days on weekends'),
    '#description' => t('Define which weekend days are working days.<br/>
      Enter one day per line in the following format: YYYY-MM-DD, e.g. !date', array('!date' => date('Y-m-d'))),
    '#default_value' => variable_get('borrows_weekends', NULL),
    '#rows' => 8,
  );

  $form['borrows_instructions'] = array(
    '#type' => 'textarea',
    '#title' => t('Booking instructions'),
    '#description' => t('This text will appear next to the calendar.'),
    '#default_value' => variable_get('borrows_instructions', ''),
  );

  $form['borrows_admin_mail'] = array(
    '#type' => 'textfield',
    '#title' => t('Admin e-mail addresses'),
    '#description' => t("Enter e-mail addresses separated by ',' (comma)."),
    '#default_value' => variable_get('borrows_admin_mail', 'csalan@csalan.hu, tanacsadas@csalan.hu'),
  );

  $form['borrows_mail_templates'] = array(
    '#type' => 'fieldset',
    '#title' => t('E-mail templates'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['borrows_mail_templates']['borrows_user_booking_template'] = array(
    '#type' => 'textarea',
    '#title' => t('E-mail template for user'),
    '#default_value' => variable_get('borrows_user_booking_template', ''),
    '#rows' => 12,
  );
  $form['borrows_mail_templates']['borrows_user_cancellation_template'] = array(
    '#type' => 'textarea',
    '#title' => t('E-mail template for cancellation as sent to user'),
    '#default_value' => variable_get('borrows_user_cancellation_template', ''),
    '#rows' => 12,
  );
  $form['borrows_mail_templates']['borrows_admin_booking_template'] = array(
    '#type' => 'textarea',
    '#title' => t('E-mail template for admin'),
    '#default_value' => variable_get('borrows_admin_booking_template', ''),
    '#rows' => 12,
  );
  $form['borrows_mail_templates']['borrows_admin_cancellation_template'] = array(
    '#type' => 'textarea',
    '#title' => t('E-mail template for cancellation as sent to admin'),
    '#default_value' => variable_get('borrows_admin_cancellation_template', ''),
    '#rows' => 12,
  );

  $form['borrows_mail_templates']['help'] = array(
    '#type' => 'fieldset',
    '#title' => t('Replacement patterns'),
    '#collapsible' => TRUE,
    '#default_value' => variable_get('borrows_user_booking_template', ''),
    '#collapsed' => TRUE,
  );
  $form['borrows_mail_templates']['help']['borrows_token_help'] = array(
    '#value' => theme('borrows_token_help'),
  );

  return system_settings_form($form);
}

/**
 * Bookings form.
 *
 * @param array $form_state
 * @param array $booking
 * @return array
 */
function borrows_booking_edit_form(&$form_state, $booking = array()) {
  include_once('./'. drupal_get_path('module', 'bookingsapi') .'/bookingsapi.forms.inc');
  $form = bookingsapi_booking_form($booking);

  $form['basic']['resource_id']['#disabled'] = TRUE;
  $form['basic']['name']['#access'] = FALSE;
  $form['basic']['description']['#access'] = FALSE;
  $form['basic']['rrule']['#access'] = FALSE;
  $form['basic']['priority']['#access'] = FALSE;
  $form['basic']['status']['#access'] = FALSE;
  $form['delete']['#access'] = FALSE;

  // Hide time part.
  $form['basic']['start']['#date_format'] = 'YYYY-mm-dd';
  $form['basic']['end']['#date_format'] = 'YYYY-mm-dd';

  // Client infos.
  $client = borrows_client_load($booking['record_id']);
  $form_state['values']['borrows_comment'] = $client['comments'];
  $form_state['values']['borrows_phone'] = $client['phone'];
  $client_form = drupal_retrieve_form('borrows_review_form', $form_state);

  $form['client'] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#title' => t('Client informations'),
  );
  $form['client']['borrows_phone'] = $client_form['borrows_phone'];
  $form['client']['borrows_comment'] = $client_form['borrows_comment'];

  $form['#submit'] = array('borrows_booking_edit_form_submit');

  return $form;
}

/**
 * Submit handler of edit form.
 */
function borrows_booking_edit_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  list($node, $start, $end, $phone, $comment) = array(node_load($resource_id), $values['start'], $values['end'], $values['borrows_phone'], $values['borrows_comment']);
  // Record booking.
  // @TODO Fix conflict issues. Somehow bookingsapi thinks edited booking is conflicting w/ itself.
  $result = borrows_booking($node, $start, $end, $phone, $comment, $values['record_id']);

  // If booking was successful show a message
  // and send notification emails.
  if ($result['success'] === TRUE) {
    borrows_send_notifications($node, $start, $end, $phone, $comment);
    drupal_set_message(t('Booking was successful.'));
  }
  else {
    drupal_set_message(t('Booking failed. Reason: @reason.', array('@reason' => $result['message'])));
  }

  $form_state['redirect'] = 'node/' . $node->nid;
}

/**
 * Load client related informations for a given booking record.
 */
function borrows_client_load($record_id) {
  if ($record_id < 1) {
    return FALSE;
  }
  $query = 'SELECT * FROM {borrows_clients} WHERE record_id=%d';
  return db_fetch_array(db_query($query, $record_id));
}

/**
 * Booking delete confirmation page.
 * @param array $booking
 */
function borrows_booking_delete($booking) {
  return drupal_get_form('borrows_booking_delete_confirm', $booking);
}

/**
 * Booking delete confirmation form.
 */
function borrows_booking_delete_confirm(&$form_state, $booking) {
  $form['#item'] = $booking;
  $desc = t('Booking will be deleted.');
  return confirm_form($form, t('Are you sure you want to delete this booking?'), '<front>', $desc, t('Delete'));
}

/**
 * Booking delete.
 */
function borrows_booking_delete_confirm_submit($form, &$form_state) {
  bookingsapi_record_delete($form['#item']);
  drupal_set_message(t('The booking has been deleted.'));
  $form_state['redirect'] = '<front>';
}
