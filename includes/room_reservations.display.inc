<?php
/**
 * Theme the room reservation calendar page.
 *
 * @global object $user
 *   Drupal user object.
 * @global type $base_url
 *   Application base url.
 *
 * @param array $dates
 *   An array containing information about all the possible days for which a
 *   reservtion can be made.
 * @param array $categories
 *   An array of all the room categories.
 * @param array $reservations
 *   An array representing all the reservations that have been made for the
 *   day that is being displayed on the calendar.
 * @param array $hours
 *   An array representing every half hour time slot in a single day.
 * @param array $building_hours
 *   A two dimensional array, with the first dimension representing a single
 *   day for which a reservation can be made, and the second dimension
 *   representing information about the facility's open hours for that day, such
 *   as whether the facility is open that day, the number of open shifts,
 *   open and close hours, and a string that can be used to display the hours
 *   in a user friendly way.
 * @param string $building_hours_display
 *   A user friendly string of the open hours for the day being displayed
 *   on the calendar.
 * @param array $rooms
 *   An array representing all of the rooms that can be reserved.
 * @param string $selected_category
 *   The current category the user has selected, if any.
 * @param array $user_reservations
 *   An array representing the current user's reservations for the allowable
 *   reservation period.
 *
 * @return string
 *   The html for displaying the page.
 */
function room_reservations_preprocess_room_reservations(&$variables) {
  global $user;
  global $base_url;
  global $base_secure_url;
  global $base_root;
    
  // to support minimum adv booking per room; let's get available dates per room
  $datesperroom = _room_reservations_dates(null, null, true);
  // pass date format to JS
  $format = strtolower(variable_get('room_reservations_picker_format', 'y/m/d'));
  drupal_add_js(array('room_reservations' => array('dateformat' => $format)), 'setting');
  
  $dates = $variables['dates'];
  $categories = array();
  // Reindex $categories by tid
  foreach($variables['categories'] as $term) {
    $term->rooms = array();
    $term->{'#theme'} = 'room_category';
    $categories[$term->tid] = (array) $term;
  }
  $variables['categories'] = $categories;
  $reservations = $variables['reservations'];
  $hours = $variables['hours'];
  $building_hours = $variables['building_hours'];
  $building_hours_display = $variables['building_hours_display'];
  foreach( $variables['rooms'] as $nid => $room) {
    foreach($room['field_reservations_room_category'][LANGUAGE_NONE] as $value) {
//      $room['#theme'] = 'room_reservations__room';
      $variables['categories'][$value['tid']]['rooms'][$nid] = $room;
    }
  }
  $user_reservations = $variables['user_reservations'];

  // User information.
  $full_access = FALSE;
  $user_login_name = NULL;
  if ($user->uid) {
    $user_login_name = $user->name;
    $full_access = user_access('administer room reservations system') || user_access('edit any room reservation');
  }
  
  // Determine which date has been selected by the user.
  foreach ($dates as $day) {
    if ($day['selected'] == TRUE) {
      $day_of_week = $day['day-of-week'];
      $month_number = $day['month-number'];
      $month = $day['month'];
      $xday = $day['day'];
      $year = $day['year'];
      $yyyymmdd = $day['yyyymmdd'];
    }
  }
  
  $calendar_text = check_markup(_room_reservations_get_variable('calendar_text'));
  $reserve_room_instructions_text = check_markup(_room_reservations_get_variable('reserve_instructions'));
  if (!$reserve_room_instructions_text) {
    $reserve_room_instructions_text = t('To reserve a room, click on the desired time/day in the calendar below. You will be asked to login.');
  }
  // Calendar date.
  $variables['date'] = format_date(strtotime($month . ' ' . $xday . ', ' . $year), 'custom', 'l, F d, Y'); 

  // add new Day Selector as popup calendar since we now allow going out up to 6 months rather than 15 days.
  $variables['form'] = drupal_get_form('room_reservations_admin_date_picker', $yyyymmdd);
  
  // Reservation calendar grid:
  //
  // Each block on the grid is assigned one (or more) of the following classes:
  //
  // closed - the building is closed;
  // booked - the building is open and the time slot has been reserved;
  // open - the building is open, the time slot has not been reserved, but the user must login to reserve the time slot;
  // reservable - the building is open, the time slot has not been reserved and the user is able to reserve the time slot.
  // setup - buffer zones added before/after bookings to allow for setup/takedown (per category)
  // 
  
  
  // If the user is logged in, the class for each unbooked time slot is 'reservable'.  If the user is not logged in, the class is 'open'.
  // Only logged in users can click a block of time to open the reservation form.
  $unbooked_class = ($user->uid) ? 'reservable' : 'open';
 
}

function room_reservations_preprocess_room_reservations__room(&$variables) {
  file_put_contents('/var/drupalfiles/agronomy_files/tmp/drupal_debug.txt', print_r($variables, TRUE), FILE_APPEND);
  
}

function room_reservations_preprocess_room_category(&$variables) {
  file_put_contents('/var/drupalfiles/agronomy_files/tmp/drupal_debug.txt', print_r($variables, TRUE), FILE_APPEND);
  $reservations = $variables['reservations'];
  $hours = $variables['hours'];
  $building_hours = $variables['building_hours'];
  $building_hours_display = $variables['building_hours_display'];
  $rooms = $variables['rooms'];
  $user_reservations = $variables['user_reservations'];

/*  // Show the first tab and hide all others.
  if (empty($variables['selected_category'])) {
    $show = ($i == 0) ? 'show' : 'hide';
    $i++;
  }
  else {
    $show = (!empty($variables['selected_category']) && $category->name == $variables['selected_category']) ? 'show' : 'hide';
  } */
 
  // Available hours column.
  foreach ($hours as $time_slot) {
    $time_display = ($time_slot['class'] == 'odd') ? t($time_slot['display']) : "";
    $table[$r][$c] = "<td class='" . $time_slot['class'] . " timeslot' time='" . $time_slot['time'] . "' " . '>' . $time_display . '</td>';
    $r++;
  }
}

function room_reservations_preprocess_time_slot(&$variables) {
  $time = $time_slot['time'];
  $open = $time_slot['open'];
  
  // lets use slot class from reservation if it is set
  $slotclass = isset($reservations[$rid][$time_slot['time']]['class']) ? $reservations[$rid][$time_slot['time']]['class'] : $time_slot['class'];
  // to support min adv booking per room; let's simply mark all slots as closed for dates not availble to this user for this room
  if (!isset($datesperroom[$rid][$yyyymmdd])) {
    $open = false; 
  }
  
  // Determine if the building is open or closed for this time slot.
  if ($open) {
    $booked_class = ($reservations[$rid][$time]['booked']) ? '' : $unbooked_class;
  }
  else {
    $booked_class = 'closed';
  }
  // The time slot can be reserved by the current user.
  $viewable_class = '';
  $widthclass = '';
  if ($booked_class == 'reservable' && 
    (user_access('create room reservations standard') || user_access('create room reservations extended') || user_access('administer room reservations system'))) {
    $link = l(
      '<img src="' . base_path() . drupal_get_path('module', 'room_reservations') . '/images/clear.png" />',
      'node/add/room-reservations-reservation/' . $month_number . '/' . $xday . '/' . $time_slot['time'] . '/' . $rid,
      array('html' => true)
    );
    $viewable = '';
  }
  // The time slot can be reserved by the current user, but the user must login first.
  elseif ($booked_class == 'open') {

    $options = array(
      'https' => TRUE,
      'query' => array('target' => url($base_secure_url . '/' . variable_get('isushib_login_dir', 'shibboleth') . '/pc', array('query' => array('destination' => 'node/add/room-reservations-reservation/' . $month_number . '/' . $xday . '/' . $time_slot['time'] . '/' . $rid)))),
    );

    $url = url($base_root . '/Shibboleth.sso/Login', $options);

    // url() does not respect https option unless variable_get('https', FALSE)
    // is TRUE, so ensure that link to Shibboleth is an https link.
    $secure_url = str_replace('http://', 'https://', $url);

    $link = l(theme('image', array('path' => drupal_get_path('module', 'room_reservations') . '/images/clear.png')), $secure_url, array('html' => TRUE));

    $viewable = '';
  }
  elseif ($booked_class == 'closed') {
    $link = '';
  }
  else {
    // The time slot has a reservation that can be edited by the current user.
    $reservation = node_load($reservations[$rid][$time]['id']);
    $viewable_class = node_access('update', $reservation) ? 'viewable' : '';
    //$viewable_class = (($full_access) || ($reservations[$rid][$time]['user'] == $user->uid)) ? 'viewable' : '';
    if ($viewable_class == 'viewable') {
      $id = $reservations[$rid][$time]['id'];
      $link = $id ? l($reservations[$rid][$time]['name'], 'node/' . $id . '/edit', 
        array('attributes' => array(
          'title' => $reservations[$rid][$time]['name'],
          'class' => 'booking-span')
        )
      ) : '';
    }
    // The time slot has a reservation that cannot be viewed by the current user. and we are NOT allowed to see the Title
    else if (isset($reservations[$rid][$time]['blocked']) && $reservations[$rid][$time]['blocked']) {
      $link = t('Booked');
    }    
    // The time slot has a reservation that cannot be edited by the current user. but we are allowed to see the Title
    else {
      $link = $reservations[$rid][$time]['name'];
    }
    $slots = isset($reservations[$rid][$time]['slots']) ? $reservations[$rid][$time]['slots'] : '';
    $widthclass = $slots ? 'colspan' . $reservations[$rid][$time]['slots'] : '';
  }
  
  // allow other modules to modify the $link
  drupal_alter('room_reservations_link', $link, $reservations[$rid][$time]); 
  
  // allow other modules adding a custom class to slots
  $custom_class = '';
  drupal_alter('room_reservations_custom_class', $custom_class, $reservations[$rid][$time]);
           
  // add div wrapper to display better
  $link = $link ? '<div class="booking-span">' . $link . '</div>' : '';
  
  // we used book class to determine if linked or not; which we needed for pre/post slots as well as actual reservation slots
  // but we don't want to show booked class now for the slots which are just buffer slots
  if (stristr($slotclass, 'setup')) {
    $booked_class = '';
  }
  $table[$r][$c] = "<td class='reservation-slot " . $slotclass . ' ' . $booked_class . ' ' . $custom_class . ' ' . $viewable_class .  ' ' . $widthclass. "'>" . $link . "</td>";  
}