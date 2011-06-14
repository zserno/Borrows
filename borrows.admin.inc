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
    '#description' => t('Description...'),
    '#default_value' => variable_get('borrows_user_booking_template', ''),
    '#rows' => 12,
  );
  $form['borrows_mail_templates']['borrows_user_cancellation_template'] = array(
    '#type' => 'textarea',
    '#title' => t('E-mail template for cancellation as sent to user'),
    '#description' => t('Description...'),
    '#default_value' => variable_get('borrows_user_cancellation_template', ''),
    '#rows' => 12,
  );
  $form['borrows_mail_templates']['borrows_admin_booking_template'] = array(
    '#type' => 'textarea',
    '#title' => t('E-mail template for admin'),
    '#description' => t('Description...'),
    '#default_value' => variable_get('borrows_admin_booking_template', ''),
    '#rows' => 12,
  );
  $form['borrows_mail_templates']['borrows_admin_cancellation_template'] = array(
    '#type' => 'textarea',
    '#title' => t('E-mail template for cancellation as sent to admin'),
    '#description' => t('Description...'),
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
    '#value' => theme('token_help', 'all'),
  );

  return system_settings_form($form);
}


