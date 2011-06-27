<?php

class views_handler_field_borrows_record_link extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['record_id'] = 'record_id';
  }

  function query() { // what is this?
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    parent::render($values);
  }
}

class views_handler_field_borrows_booking_link_edit extends views_handler_field_borrows_record_link {
  function render($values) {
    parent::render($values);
    $record_id = $values->{$this->aliases['record_id']};
    return l(t('edit'), "admin/content/borrows/booking/$record_id/edit");
  }
}

class views_handler_field_borrows_booking_link_delete extends views_handler_field_borrows_record_link {
  function render($values) {
    parent::render($values);
    $record_id = $values->{$this->aliases['record_id']};
    return l(t('delete'), "admin/content/borrows/booking/$record_id/delete");
  }
}

