<?php
/**
 * @file
 * Expose booking informations to Views.
 */

/**
 * Implementation of hook_views_data().
 */
function borrows_views_data() {
  $data['borrows_clients']['table']['group']  = t('Booking client');

  $data['borrows_clients']['table']['base'] = array(
    'field' => 'client_id',
    'title' => t('Booking client'),
    'help' => t('Client information.'),
  );

  $data['borrows_clients']['table']['join'] = array(
    'bookings_records' => array(
      'left_field' => 'record_id',
      'field' => 'record_id',
    ),
  );

  $data['borrows_clients']['phone'] = array(
    'title' => t('Phone'),
    'help' => t('Phone number of the client.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => FALSE,
    ),
  );

  $data['borrows_clients']['comments'] = array(
    'title' => t('Comments'),
    'help' => t('Comments by the client.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => FALSE,
    ),
  );

  $data['borrows_clients']['uid'] = array(
    'title' => t('User'),
    'help' => t("Client's user ID."),
    'relationship' => array(
      'base' => 'users',
      'field' => 'uid',
      'handler' => 'views_handler_relationship',
      'label' => t('User'),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_user_current',
      'type' => 'yes-no',
    ),
  );

  $data['bookings_records']['edit_booking'] = array(
    'field' => array(
      'title' => t('Edit booking link'),
      'help' => t('Provide a simple link to edit the booking.'),
      'handler' => 'views_handler_field_borrows_booking_link_edit',
    ),
  );

  $data['bookings_records']['delete_booking'] = array(
    'field' => array(
      'title' => t('Delete booking link'),
      'help' => t('Provide a simple link to delete the booking.'),
      'handler' => 'views_handler_field_borrows_booking_link_delete',
    ),
  );

  return $data;
}

