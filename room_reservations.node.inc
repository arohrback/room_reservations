<?php
/**
 * @file
 * Arrays for fields and instances on custom content types.
 *
 * @author Bob Humphrey, Web and Applications Developer, UNCW Randall Library
 * @author Andy Rohrback, Systems Analyst, Iowa State University Agronomy Department
 */


/**
 * Define the fields for our content types.
 * @return array
 *   An associative array specifying the fields we wish to add to our
 *   new node type.
 */
function _room_reservations_installed_fields() {
  return array(
    'field_reservation_approved' => array(
      'active' => 1,
      'cardinality' => 1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_reservation_approved',
      'field_permissions' => array(
        'type' => 2,
      ),
      'indexes' => array(
        'value' => array(
          0 => 'value',
        ),
      ),
      'label' => 'Requires approval',
      'locked' => 0,
      'module' => 'list',
      'required' => FALSE,
      'settings' => array(
        'allowed_values' => array(
          0 => 'No',
          1 => 'Yes',
        ),
        'allowed_values_function' => '',
      ),
      'translatable' => 0,
      'type' => 'list_boolean',
      'widget' => array(
        'settings' => array(
          'display_label' => TRUE,
        ),
        'type' => 'options_onoff',
      ),
    ),

    'field_reservation_contact_email' => array(
      'active' => 1,
      'cardinality' => 1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_reservation_contact_email',
      'field_permissions' => array(
        'type' => 2,
      ),
      'indexes' => array(),
      'locked' => 0,
      'module' => 'email',
      'settings' => array(),
      'translatable' => 0,
      'type' => 'email',
    ),

    'field_reservation_date' => array(
      'active' => 1,
      'cardinality' => 1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_reservation_date',
      'indexes' => array(),
      'locked' => 0,
      'module' => 'date',
      'settings' => array(
        'cache_count' => 4,
        'cache_enabled' => 0,
        'granularity' => array(
          'day' => 'day',
          'hour' => 0,
          'minute' => 0,
          'month' => 'month',
          'second' => 0,
          'year' => 'year',
        ),
        'timezone_db' => '',
        'todate' => '',
        'tz_handling' => 'none',
      ),
      'translatable' => 0,
      'type' => 'datetime',
    ),

    'field_reservation_equipment' => array(
      'active' => 1,
      'cardinality' => -1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_reservation_equipment',
      'field_permissions' => array(
        'type' => 0,
      ),
      'indexes' => array(
        'tid' => array(
          0 => 'tid',
        ),
      ),
      'locked' => 0,
      'module' => 'taxonomy',
      'settings' => array(
        'allowed_values' => array(
          0 => array(
            'vocabulary' => 'conference_room_equipment',
            'parent' => 0,
          ),
        ),
      ),
      'translatable' => 0,
      'type' => 'taxonomy_term_reference',
    ),

    'field_reservation_food' => array(
      'active' => 1,
      'cardinality' => 1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_reservation_food',
      'field_permissions' => array(
        'type' => 0,
      ),
      'indexes' => array(
        'value' => array(
          0 => 'value',
        ),
      ),
      'locked' => 0,
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          0 => 'No',
          1 => 'Yes',
        ),
        'allowed_values_function' => '',
      ),
      'translatable' => 0,
      'type' => 'list_boolean',
    ),

    'field_reservation_length' => array(
      'active' => 1,
      'cardinality' => 1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_reservation_length',
      'indexes' => array(
        'value' => array(
          0 => 'value',
        ),
      ),
      'locked' => 0,
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          30 => '30 minutes',
          60 => '60 minutes',
          90 => '90 minutes',
          120 => '2 hours',
          150 => '2.5 hours',
          180 => '3 hours',
          210 => '3.5 hours',
          240 => '4 hours',
          270 => '4.5 hours',
          300 => '5 hours',
          330 => '5.5 hours',
          360 => '6 hours',
          390 => '6.5 hours',
          420 => '7 hours',
          450 => '7.5 hours',
          480 => '8 hours',
          510 => '8.5 hours',
          540 => '9 hours',
          600 => '10 hours',
          720 => '12 nours',
        ),
        'allowed_values_function' => '',
      ),
      'translatable' => 0,
      'type' => 'list_integer',
    ),

    'field_reservation_private' => array(
      'active' => 1,
      'cardinality' => 1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_reservation_private',
      'indexes' => array(
        'value' => array(
          0 => 'value',
        ),
      ),
      'locked' => 0,
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          0 => '',
          1 => '',
        ),
        'allowed_values_function' => '',
      ),
      'translatable' => 0,
      'type' => 'list_boolean',
    ),

    'field_reservation_repeat_type' => array(
      'active' => 1,
      'cardinality' => 1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_reservation_repeat_type',
      'indexes' => array(
        'value' => array(
          0 => 'value',
        ),
      ),
      'locked' => 0,
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          1 => 'No repeat',
          2 => 'Repeat all days until',
          3 => 'Repeat this day of the week until',
          4 => 'Repeat this day every other week until',
          5 => 'Repeat this day of this week of every month until',
        ),
        'allowed_values_function' => '',
      ),
      'translatable' => 0,
      'type' => 'list_integer',
    ),

    'field_reservation_repeat_until' => array(
      'active' => 1,
      'cardinality' => 1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_reservation_repeat_until',
      'indexes' => array(),
      'locked' => 0,
      'module' => 'date',
      'settings' => array(
        'cache_count' => 4,
        'cache_enabled' => 0,
        'granularity' => array(
          'day' => 'day',
          'hour' => 0,
          'minute' => 0,
          'month' => 'month',
          'second' => 0,
          'year' => 'year',
        ),
        'timezone_db' => '',
        'todate' => '',
        'tz_handling' => 'none',
      ),
      'translatable' => 0,
      'type' => 'datetime',
    ),

    'field_reservation_room' => array(
      'active' => 1,
      'cardinality' => 1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_reservation_room',
      'indexes' => array(
        'target_id' => array(
          0 => 'target_id',
        ),
      ),
      'locked' => 0,
      'module' => 'entityreference',
      'settings' => array(
        'handler' => 'base',
        'handler_settings' => array(
          'behaviors' => array(
            'views-select-list' => array(
              'status' => 0,
            ),
          ),
          'sort' => array(
            'type' => 'none',
          ),
          'target_bundles' => array(
            'room_reservations_room' => 'room_reservations_room',
          ),
        ),
        'target_type' => 'node',
      ),
      'translatable' => 0,
      'type' => 'entityreference',
    ),

    'field_reservation_seriesid' => array(
      'active' => 1,
      'cardinality' => 1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_reservation_seriesid',
      'indexes' => array(),
      'locked' => 0,
      'module' => 'number',
      'settings' => array(),
      'translatable' => 0,
      'type' => 'number_integer',
    ),

    'field_reservation_time' => array(
      'active' => 1,
      'cardinality' => 1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_reservation_time',
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => 0,
      'module' => 'text',
      'settings' => array(
        'max_length' => 4,
      ),
      'translatable' => 0,
      'type' => 'text',
    ),

    'field_reservations_equipment' => array(
      'active' => 1,
      'cardinality' => -1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_reservations_equipment',
      'field_permissions' => array(
        'type' => 0,
      ),
      'indexes' => array(
        'tid' => array(
          0 => 'tid',
        ),
      ),
      'locked' => 0,
      'module' => 'taxonomy',
      'settings' => array(
        'allowed_values' => array(
          0 => array(
            'vocabulary' => 'conference_room_equipment',
            'parent' => 0,
          ),
        ),
      ),
      'translatable' => 0,
      'type' => 'taxonomy_term_reference',
    ),

    'field_reservations_minadvbooking' => array(
      'active' => 1,
      'cardinality' => 1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_reservations_minadvbooking',
      'field_permissions' => array(
        'type' => 0,
      ),
      'indexes' => array(),
      'locked' => 0,
      'module' => 'number',
      'settings' => array(
        'default_widget' => 'text',
        'max_length' => 255,
      ),
      'translatable' => 0,
      'type' => 'number_integer',
    ),

    'field_reservations_postbuffer' => array(
      'active' => 1,
      'cardinality' => 1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_reservations_postbuffer',
      'field_permissions' => array(
        'type' => 0,
      ),
      'indexes' => array(
        'value' => array(
          0 => 'value',
        ),
      ),
      'locked' => 0,
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          0 => 'none',
          30 => '30 minutes',
          60 => '1 hour',
          90 => '1.5 hours',
          120 => '2 hours',
        ),
        'allowed_values_function' => '',
      ),
      'translatable' => 0,
      'type' => 'list_integer',
    ),

    'field_reservations_prebuffer' => array(
      'active' => 1,
      'cardinality' => 1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_reservations_prebuffer',
      'field_permissions' => array(
        'type' => 0,
      ),
      'indexes' => array(
        'value' => array(
          0 => 'value',
        ),
      ),
      'locked' => 0,
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          0 => 'none',
          30 => '30 minutes',
          60 => '1 hour',
          90 => '1.5 hours',
          120 => '2 hours',
        ),
        'allowed_values_function' => '',
      ),
      'translatable' => 0,
      'type' => 'list_integer',
    ),

    'field_reservations_room_approval' => array(
      'active' => 1,
      'cardinality' => 1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_reservations_room_approval',
      'field_permissions' => array(
        'type' => 0,
      ),
      'indexes' => array(
        'value' => array(
          0 => 'value',
        ),
      ),
      'locked' => 0,
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          0 => '',
          1 => '',
        ),
        'allowed_values_function' => '',
      ),
      'translatable' => 0,
      'type' => 'list_boolean',
    ),

    'field_reservations_room_capacity' => array(
      'active' => 1,
      'cardinality' => 1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_reservations_room_capacity',
      'field_permissions' => array(
        'type' => 0,
      ),
      'indexes' => array(),
      'locked' => 0,
      'module' => 'number',
      'settings' => array(
        'default_widget' => 'text',
        'max_length' => 255,
      ),
      'translatable' => 0,
      'type' => 'number_integer',
    ),

    'field_reservations_room_category' => array(
      'active' => 1,
      'cardinality' => -1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_reservations_room_category',
      'field_permissions' => array(
        'type' => 0,
      ),
      'indexes' => array(
        'tid' => array(
          0 => 'tid',
        ),
      ),
      'locked' => 0,
      'module' => 'taxonomy',
      'settings' => array(
        'allowed_values' => array(
          0 => array(
            'vocabulary' => 'conference_room_type',
            'parent' => 0,
          ),
        ),
      ),
      'translatable' => 0,
      'type' => 'taxonomy_term_reference',
    ),

    'field_reservations_room_food_ok' => array(
      'active' => 1,
      'cardinality' => 1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_reservations_room_food_ok',
      'field_permissions' => array(
        'type' => 0,
      ),
      'indexes' => array(
        'value' => array(
          0 => 'value',
        ),
      ),
      'locked' => 0,
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          0 => 'No',
          1 => 'Yes',
        ),
        'allowed_values_function' => '',
      ),
      'translatable' => 0,
      'type' => 'list_boolean',
    ),

  );
}
 
function _room_reservations_installed_instances_room() {
  return array(
    'field_reservations_equipment' => array(
      'default_value' => NULL,
      'deleted' => 0,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'taxonomy',
          'settings' => array(),
          'type' => 'taxonomy_term_reference_link',
          'weight' => 9,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_reservations_equipment',
      'label' => 'Equipment available',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_buttons',
        'weight' => 6,
      ),
    ),

    // Exported field_instance:
    // 'field_reservations_minadvbooking'.
    'field_reservations_minadvbooking' => array(
      'default_value' => array(
        0 => array(
          'value' => 0,
        ),
      ),
      'deleted' => 0,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'number',
          'settings' => array(
            'decimal_separator' => '.',
            'prefix_suffix' => TRUE,
            'scale' => 0,
            'thousand_separator' => '',
          ),
          'type' => 'number_integer',
          'weight' => 7,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_reservations_minadvbooking',
      'label' => 'Minimum advance booking',
      'placeholder' => '',
      'required' => 0,
      'settings' => array(
        'max' => '',
        'min' => '',
        'prefix' => '',
        'suffix' => 'day|days',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'number',
        'settings' => array(),
        'type' => 'number',
        'weight' => 9,
      ),
    ),

    // Exported field_instance:
    // 'field_reservations_postbuffer'.
    'field_reservations_postbuffer' => array(
      'default_value' => NULL,
      'deleted' => 0,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
          'weight' => 6,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_reservations_postbuffer',
      'label' => 'Room takedown buffer',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => 8,
      ),
    ),

    // Exported field_instance:
    // 'field_reservations_prebuffer'.
    'field_reservations_prebuffer' => array(
      'default_value' => NULL,
      'deleted' => 0,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
          'weight' => 5,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_reservations_prebuffer',
      'label' => 'Room setup buffer',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => 7,
      ),
    ),

    // Exported field_instance:
    // 'field_reservations_room_approval'.
    'field_reservations_room_approval' => array(
      'default_value' => NULL,
      'deleted' => 0,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
          'weight' => 4,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_reservations_room_approval',
      'label' => 'Approval required',
      'required' => 0,
      'settings' => array(
        'allowed_values' => array(
          0 => 'No',
          1 => 'Yes',
        ),
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(
          'display_label' => 1,
        ),
        'type' => 'options_onoff',
        'weight' => 4,
      ),
    ),

    // Exported field_instance:
    // 'field_reservations_room_capacity'.
    'field_reservations_room_capacity' => array(
      'default_value' => NULL,
      'deleted' => 0,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'number',
          'settings' => array(
            'decimal_separator' => '.',
            'prefix_suffix' => TRUE,
            'scale' => 0,
            'thousand_separator' => '',
          ),
          'type' => 'number_integer',
          'weight' => 2,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_reservations_room_capacity',
      'label' => 'Capacity',
      'placeholder' => '',
      'required' => 1,
      'settings' => array(
        'max' => '',
        'min' => '',
        'prefix' => '',
        'suffix' => 'person|people',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => 15,
        ),
        'type' => 'text_textfield',
        'weight' => 3,
      ),
    ),

    // Exported field_instance:
    // 'field_reservations_room_category'.
    'field_reservations_room_category' => array(
      'default_value' => NULL,
      'deleted' => 0,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'taxonomy',
          'settings' => array(),
          'type' => 'taxonomy_term_reference_link',
          'weight' => 8,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_reservations_room_category',
      'label' => 'Categories',
      'placeholder' => '',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'taxonomy',
        'settings' => array(
          'autocomplete_path' => 'taxonomy/autocomplete',
          'size' => 60,
        ),
        'type' => 'taxonomy_autocomplete',
        'weight' => 10,
      ),
    ),

    // Exported field_instance:
    // 'field_reservations_room_food_ok'.
    'field_reservations_room_food_ok' => array(

      'default_value' => array(
        0 => array(
          'value' => 0,
        ),
      ),
      'deleted' => 0,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
          'weight' => 10,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_reservations_room_food_ok',
      'label' => 'Food allowed',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(
          'display_label' => 1,
        ),
        'type' => 'options_onoff',
        'weight' => 5,
      ),
    ),
  );
}

function _room_reservations_installed_instances_reservation() {
  return array(
    'field_reservation_approved' => array(
      'cardinality' => 1,
      'default_value' => array(
        0 => array(
          'value' => 0,
        ),
      ),
      'deleted' => 0,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
          'weight' => 9,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_reservation_approved',
      'label' => 'Approved',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'type' => 'list_boolean',
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(
          'display_label' => 1,
        ),
        'type' => 'options_onoff',
        'weight' => 3,
      ),
    ),

    // Exported field_instance:
    // 'field_reservation_contact_email'.
    'field_reservation_contact_email' => array(
      'default_value' => NULL,
      'deleted' => 0,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'email',
          'settings' => array(),
          'type' => 'email_default',
          'weight' => 10,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_reservation_contact_email',
      'label' => 'Contact Email',
      'placeholder' => '',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'email',
        'settings' => array(
          'size' => 60,
        ),
        'type' => 'email_textfield',
        'weight' => 1,
      ),
    ),

    // Exported field_instance:
    // 'field_reservation_date'.
    'field_reservation_date' => array(
      'deleted' => 0,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'inline',
          'module' => 'date',
          'settings' => array(
            'format_type' => 'long',
            'fromto' => 'both',
            'multiple_from' => '',
            'multiple_number' => '',
            'multiple_to' => '',
            'show_remaining_days' => FALSE,
          ),
          'type' => 'date_default',
          'weight' => 3,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_reservation_date',
      'label' => 'Date',
      'required' => 0,
      'settings' => array(
        'default_value' => 'now',
        'default_value2' => 'same',
        'default_value_code' => '',
        'default_value_code2' => '',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'date',
        'settings' => array(
          'display_all_day' => 0,
          'increment' => 15,
          'input_format' => 'Y-m-d H:i:s',
          'input_format_custom' => '',
          'label_position' => 'above',
          'text_parts' => array(),
          'year_range' => '-3:+3',
        ),
        'type' => 'date_text',
        'weight' => 6,
      ),
    ),

    // Exported field_instance:
    // 'field_reservation_equipment'.
    'field_reservation_equipment' => array(
      'default_value' => NULL,
      'deleted' => 0,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'taxonomy',
          'settings' => array(),
          'type' => 'taxonomy_term_reference_link',
          'weight' => 11,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_reservation_equipment',
      'label' => 'Equipment needed',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_buttons',
        'weight' => 10,
      ),
    ),

    // Exported field_instance:
    // 'field_reservation_food'.
    'field_reservation_food' => array(
      'default_value' => array(
        0 => array(
          'value' => 0,
        ),
      ),
      'deleted' => 0,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
          'weight' => 12,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_reservation_food',
      'label' => 'Will there be food?',
      'required' => 1,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_buttons',
        'weight' => 5,
      ),
    ),

    // Exported field_instance:
    // 'field_reservation_length'.
    'field_reservation_length' => array(
      'default_value' => NULL,
      'deleted' => 0,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'inline',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
          'weight' => 5,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_reservation_length',
      'label' => 'Length',
      'required' => TRUE,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => 8,
      ),
    ),

    // Exported field_instance:
    // 'field_reservation_private'.
    'field_reservation_private' => array(
      'default_value' => array(
        0 => array(
          'value' => 0,
        ),
      ),
      'deleted' => 0,
      'description' => 'Check this to hide the Group Name for this reservation.',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 1,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_reservation_private',
      'label' => 'Private',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(
          'display_label' => 1,
        ),
        'type' => 'options_onoff',
        'weight' => 2,
      ),
    ),

    // Exported field_instance:
    // 'field_reservation_repeat_type'.
    'field_reservation_repeat_type' => array(
      'default_value' => NULL,
      'deleted' => 0,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'inline',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
          'weight' => 7,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_reservation_repeat_type',
      'label' => 'Repeat Type',
      'required' => 1,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_buttons',
        'weight' => 11,
      ),
    ),

    // Exported field_instance:
    // 'field_reservation_repeat_until'.
    'field_reservation_repeat_until' => array(
      'deleted' => 0,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'inline',
          'module' => 'date',
          'settings' => array(
            'format_type' => 'long',
            'fromto' => 'both',
            'multiple_from' => '',
            'multiple_number' => '',
            'multiple_to' => '',
            'show_remaining_days' => FALSE,
          ),
          'type' => 'date_default',
          'weight' => 8,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_reservation_repeat_until',
      'label' => 'Repeat Until',
      'required' => 0,
      'settings' => array(
        'default_value' => 'now',
        'default_value2' => 'same',
        'default_value_code' => '',
        'default_value_code2' => '',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'date',
        'settings' => array(
          'display_all_day' => 0,
          'increment' => 15,
          'input_format' => 'Y-m-d H:i:s',
          'input_format_custom' => '',
          'label_position' => 'above',
          'text_parts' => array(),
          'year_range' => '-3:+3',
        ),
        'type' => 'date_popup',
        'weight' => 12,
      ),
    ),

    // Exported field_instance:
    // 'field_reservation_room'.
    'field_reservation_room' => array(
      'default_value' => NULL,
      'deleted' => 0,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'inline',
          'module' => 'entityreference',
          'settings' => array(
            'bypass_access' => FALSE,
            'link' => FALSE,
          ),
          'type' => 'entityreference_label',
          'weight' => 2,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_reservation_room',
      'label' => 'Room',
      'required' => 0,
      'settings' => array(
        'nodeaccess_nodereference' => array(
          'all' => array(
            'view' => 0,
          ),
          'all_published' => 0,
          'author' => array(
            'delete' => 0,
            'update' => 0,
            'view' => 0,
          ),
          'author_published' => 0,
          'priority' => 0,
          'referenced' => array(
            'delete' => array(
              'delete' => 0,
              'update' => 0,
              'view' => 0,
            ),
            'published' => 0,
            'update' => array(
              'delete' => 0,
              'update' => 0,
              'view' => 0,
            ),
            'view' => array(
              'delete' => 0,
              'update' => 0,
              'view' => 0,
            ),
          ),
          'unused' => 0,
        ),
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => 4,
      ),
    ),

    // Exported field_instance:
    // 'field_reservation_seriesid'.
    'field_reservation_seriesid' => array(
      'default_value' => NULL,
      'deleted' => 0,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 6,
        ),
        'teaser' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_reservation_seriesid',
      'label' => 'Series ID',
      'required' => 0,
      'settings' => array(
        'max' => '',
        'min' => '',
        'prefix' => '',
        'suffix' => '',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'number',
        'settings' => array(),
        'type' => 'number',
        'weight' => 13,
      ),
    ),

    // Exported field_instance:
    // 'field_reservation_time'.
    'field_reservation_time' => array(
      'default_value' => NULL,
      'deleted' => 0,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'inline',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 4,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_reservation_time',
      'label' => 'Time',
      'required' => 0,
      'settings' => array(
        'better_formats' => array(
          'allowed_formats' => array(
            'filtered_html' => 'filtered_html',
            'full_html' => 'full_html',
            'php_code' => 'php_code',
            'plain_text' => 'plain_text',
          ),
          'allowed_formats_toggle' => 0,
          'default_order_toggle' => 0,
          'default_order_wrapper' => array(
            'formats' => array(
              'filtered_html' => array(
                'weight' => 0,
              ),
              'full_html' => array(
                'weight' => 1,
              ),
              'php_code' => array(
                'weight' => 11,
              ),
              'plain_text' => array(
                'weight' => 10,
              ),
            ),
          ),
        ),
        'text_processing' => 0,
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => 60,
        ),
        'type' => 'text_textfield',
        'weight' => 7,
      ),
    ),
  );

}

