<?php
/**
 * @file
 * Classes and general Room Reservations helper functions.
 */

/**
 * Retrieve all the rooms from the database.
 *
 * @return array
 *   An array with each element representing a room that can be reserved.
 */
function _room_reservations_rooms() {
  $rooms = array();
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'room_reservations_room')  
    ->propertyCondition('status', NODE_PUBLISHED);
  $result = $query->execute();
  if (isset($result['node'])) {
    $rids = array_keys($result['node']);
    $room_objs = node_load_multiple($rids);
    foreach ($room_objs as $room) {
      $rooms[$room->nid] = (array) $room;  
    }
  }
  return $rooms;
}

/**
 * Determine the list ofmonths as far out as we have set Extended advanced booking.
 *
 * @return array
 *   Each element represents a month.
 */
function _room_reservations_current_months() {
  // How far out do we want to set allowed hours for?
 
  // build a list of the months we need to look at
  $advance_days = variable_get('room_reservations_advance_extended', 180);
  $found = false;
  for ($x = 0; $x < $advance_days; $x++) { 
    if (date('m', mktime(0, 0, 0, date("m"), date("d") + $x, date("Y"))) != $found) {
      $months[date('Y-m', mktime(0, 0, 0, date("m"), date("d") + $x, date("Y")))]['m'] = date('n', mktime(0, 0, 0, date("m"), date("d") + $x, date("Y")));   
      $months[date('Y-m', mktime(0, 0, 0, date("m"), date("d") + $x, date("Y")))]['mm'] = date('m', mktime(0, 0, 0, date("m"), date("d") + $x, date("Y")));
      $months[date('Y-m', mktime(0, 0, 0, date("m"), date("d") + $x, date("Y")))]['y'] = date('Y', mktime(0, 0, 0, date("m"), date("d") + $x, date("Y")));
      $found = date('m', mktime(0, 0, 0, date("m"), date("d") + $x, date("Y")));
    } 
  }
  
  // Note:  Month names are translated when they are displayed.
  $names = array(
    'Unused',
    'January',
    'February',
    'March',
    'April',
    'May',
    'June',
    'July',
    'August',
    'September',
    'October',
    'November',
    'December',
  );
  
  foreach ($months as $month) {
    $item['display'] = $names[$month['m']] . ' ' . $month['y'];
    $item['year'] = $month['y'];
    $item['month'] = $month['m'];
    $item['MM'] = $month['mm'];
    $item['YYYY_MM'] = $month['y'] . '_' . $month['mm'];
    $results[] = $item;
  }
  return $results;
}

/**
 * Store daily open hours in the database.
 *
 * Initialize a record to store all of the daily open hours for a
 * single month, and save the record in the room_reservations_variables
 * table.
 *
 * @param int $year
 *   The year of the month being represented.
 * @param int $month
 *   The month number of the month being represented.
 * @param string $yyyy_mm
 *   The year and month of the month being represented in format YYYY_MM.
 * @param int $array
 *    if TRUE return mo_hours array rather than creating db record
 *
 * @return object
 *   A database query result resource, or FALSE if the query was not executed
 *   correctly.
 */
function _room_reservations_create_mo_hours($year, $month, $yyyy_mm, $array = false) {
  $mo_hours = array();
  // Days in the month.
  $days = date('t', mktime(0, 0, 0, $month, 1, $year));
  $default_hours = unserialize(_room_reservations_get_variable('default_hours'));
  if (!$default_hours) {
    for ($x = 0; $x < $days; $x++) {
      $mo_hours[] = 'D';
      $mo_hours[] = '9999';
      $mo_hours[] = '9999';
      $mo_hours[] = '9999';
      $mo_hours[] = '9999';
    }
  }
  else {
    for ($x = 0; $x < $days; $x++) {
      // Day of week.
      $dow = date('w', mktime(0, 0, 0, $month, $x + 1, $year));
      $mo_hours[] = 'D';
      $mo_hours[] = $default_hours[$dow * 4];
      $mo_hours[] = $default_hours[($dow * 4) + 1];
      $mo_hours[] = $default_hours[($dow * 4) + 2];
      $mo_hours[] = $default_hours[($dow * 4) + 3];
    }
  }
  $name = 'monthly_hours_' . $yyyy_mm;
  if ($array) {
    return $mo_hours;
  }
  else {
    $result = _room_reservations_set_variable($name, serialize($mo_hours));
    return $result;
  }
}

/**
 * Create reservations dates array.
 *
 * Create an array containing pertinent information about all the
 * possible days for which a reservation can be made.
 * 
 * 7.x-1.3 REV:
 *  - start of adding features on a per Category basis
 *  - for now let's just key this date's array by Cat ID
 *
 * @param int $selected_month
 *   The month of the day currently selected by the user
 * @param int $selected_day
 *   The day of the month of the day currently selected by the user.
 *
 * @return array
 *   Information about each day for which a reservation can be made, including
 *   display name, day of the week, month name and number, day of the month,
 *   date in the format YYYY-MM-DD, whether the day is currently selected by
 *   the user, and whether the day is today.
 */
function _room_reservations_dates($selected_month = NULL, $selected_day = NULL, $keyed = false) {
  // Determine date information (month, day, year, etc.) for each of these days.
  $rooms = _room_reservations_rooms();
  $dates = array();
  foreach ($rooms as $room) {
    $advancedaysmax = user_access('create room reservations extended') 
      ? variable_get('room_reservations_advance_extended', 180) 
      : variable_get('room_reservations_advance_standard', 14);
    $advancedaysmin = user_access('bypass minimum advance booking') ? 0 : $room['field_reservations_minadvbooking'][LANGUAGE_NONE][0]['value'];
    for ($j = $advancedaysmin; $j < $advancedaysmax; $j++) {
      $day = array();
      $day['display'] = date("l, n/j", strtotime("now + " . $j . " days"));
      $day['day-of-week'] = date("l", strtotime("now + " . $j . " days"));
      $day['month'] = date("F", strtotime("now + " . $j . " days"));
      $month_number = date("n", strtotime("now + " . $j . " days"));
      $day['month-number'] = $month_number;
      $sday = date("j", strtotime("now + " . $j . " days"));
      $day['day'] = $sday;
      $year = date("Y", strtotime("now + " . $j . " days"));
      $day['year'] = $year;
      // Determine the date selected by the user.  If none selected, default to the first day.
      if (($j == 0) && (!$selected_month) && (!$selected_day)) {
        $day['selected'] = TRUE;
      }
      elseif (($selected_month == $month_number) && ($selected_day == $sday)) {
        $day['selected'] = TRUE;
      }
      else {
        $day['selected'] = FALSE;
      }
      // The date in YYYY-MM-DD format.
      if ($month_number < 10) {
        $month_number = str_pad($month_number, 2, '0', STR_PAD_LEFT);
      }
      if ($sday < 10) {
        $sday = str_pad($sday, 2, '0', STR_PAD_LEFT);
      }
      $day['yyyymmdd'] = $year . "-" . $month_number . "-" . $sday;
      $day['today'] = (($day['month-number'] == date('m')) &&
        ($day['day'] == date('j'))) ? TRUE : FALSE;
      if ($keyed) {
        $dates[$room['nid']][$day['yyyymmdd']] = $day;
      }
      else {
        $dates[$room['nid']][] = $day;
      }
    }
  }
  return $dates;
}

/**
 * Create an array representing every half hour time slot in a single day.
 *
 * @param string $option
 *   If set to 'limited', only include time slots in the array that are
 *   later in the day than the current time minus the longest possible
 *   reservation length.
 *
 * @return array
 *   An array representing reservable time slots in a single day.
 */
function _room_reservations_hours($option = NULL) {
  //watchdog('rr', 'START');
  $hours = array();
  $x = 0;
  $y = 0;
  while ($x <= 23) {
    $hours_entry = array();
    $hour = ($x < 10) ? '0' . $x : $x;
    if ($x == 0) {
      $display_hour = 12;
    }
    elseif ($x <= 12) {
      $display_hour = $x;
    }
    else {
      $display_hour = $x - 12;
    }
    $minutes = ($y % 2) ? '30' : '00';
    $time = $hour . $minutes;
    $ampm = ($y < 24) ? t('AM') : t('PM');
    if ($y == 0) {
      // these shouldn't be wrapped in t() since we are just about to do a date() with them anyway
      $display = 'Midnight';
    }
    elseif ($y == 24) {
      $display = 'Noon';
    }
    else {
      $display = $display_hour . ':' . $minutes . ' ' . $ampm;
    }
    
    // convert display to 24:00 format if required
    if (variable_get('room_reservations_hour_format', 0)) {
      $display = date('H:i', strtotime($display));
    }
    
    $class = ($y % 2) ? 'even' : 'odd';
    $hours_node_time = $display_hour . ':' . $minutes . $ampm;
    $hours_entry['time'] = $time;
    $hours_entry['display'] = $display;
    $hours_entry['hours node time'] = $hours_node_time;
    $hours_entry['class'] = $class;
    $hours_entry['open'] = TRUE;
    $hours[] = $hours_entry;
    if ($y % 2) {
      $x++;
    }
    $y++;
  }

  // Only return time slots that are greater than the current time minus
  // the maximum reservation length.
  if ($option == 'limited') {
    $max_length = user_access('create room reservations extended length') 
      ? variable_get('room_reservations_max_length_extended', 120) 
      : variable_get('room_reservations_max_length_standard', 120);
    $margin_time = ($max_length / 60) * 100;
    if ($max_length % 60) {
      $margin_time += 30;
    }
    $str_current_time = date('H') . date('i');
    $int_current_time = intval($str_current_time);
    $cutoff_time = $int_current_time - $margin_time;
    $cutoff_time = ($cutoff_time < 0) ? 0 : $cutoff_time;
    $limited_hours = array();
    foreach ($hours as $time_slot) {
      $time_slot_time = intval($time_slot['time']);
      if ($time_slot_time > $cutoff_time) {
        $limited_hours[] = $time_slot;
      }
    }
    return $limited_hours;
  }
  else {
    return $hours;
  }
}

/**
 * Create open hours array.
 *
 * Create an array of open hours information for each day for which a
 * reservation can be made.
 * 
 * NOTE - D6 version included only current month and next month since we could only book 15 days in advance
 * since we can now book up to almost a year in advance; we need to redo how this is done
 *
 * @return array
 *   A two dimensional array, with the first dimension representing a single
 *   day for which a reservation can be made, and the second dimension
 *   representing information about the facility's open hours for that day, such
 *   as whether the facility is open that day, the number of open shifts,
 *   open and close hours, and a string that can be used to display the hours
 *   in a user friendly way.
 */
function _room_reservations_facility_hours($reset = FALSE) {
  static $building_hours;
  if (!isset($building_hours) || $reset) {
    $building_hours = array();
    if (user_access('create room reservations extended')) {
      $advancedays = variable_get('room_reservations_advance_extended', 180);
    }
    else {
      $advancedays = variable_get('room_reservations_advance_standard', 14);
    }
    
    // build a list of the months we need to look at
    for ($x = 0; $x < $advancedays; $x++) { 
      $months[] = date('Y_m', mktime(0, 0, 0, date("m"), date("d") + $x, date("Y")));
    }
    $months = array_unique($months);
    
    //$today = date('Y-m-d');
    
    $mo_hours = array();
    foreach ($months as $month) {
      $mo_hours_this = unserialize(_room_reservations_get_variable('monthly_hours_' . $month));    
      if ($mo_hours_this) {
        $mo_hours = array_merge($mo_hours, $mo_hours_this);
      }
      else {
        $m = intval(drupal_substr($month, 5));
        $year = drupal_substr($month, 0, 4);
        $mo_hours_this = _room_reservations_create_mo_hours($year, $m, $month, true); 
        $mo_hours = array_merge($mo_hours, $mo_hours_this);
      }
    }
    $start = (date('j') - 1) * 5;
    for ($x = 0; $x < $advancedays; $x++) {
      $yyyymmdd  = date('Y-m-d', mktime(0, 0, 0, date("m"), date("d") + $x, date("Y")));
      $first_shift_open = $mo_hours[($start + ($x * 5) + 1)];
      $first_shift_close = $mo_hours[($start + ($x * 5) + 2)];
      $second_shift_open = $mo_hours[($start + ($x * 5) + 3)];
      $second_shift_close = $mo_hours[($start + ($x * 5) + 4)];
      if (($first_shift_open == '9999') && ($first_shift_close == '9999') && ($second_shift_open == '9999') && ($second_shift_close == '9999')) {
        $open = FALSE;
      }
      else {
        $open = TRUE;
      }
      if (($open) && ($first_shift_open == '0000') && ($first_shift_close == '2400')) {
        $open_24_hours = TRUE;
      }
      else {
        $open_24_hours = FALSE;
      }
      if (!$open) {
        $shifts = 0;
      }
      elseif ($open_24_hours) {
        $shifts = 1;
      }
      elseif (($open) && ($second_shift_open == '9999') && ($second_shift_close == '9999')) {
        $shifts = 1;
      }
      else {
        $shifts = 2;
      }
      $day_hours = array(
        $first_shift_open,
        $first_shift_close,
        $second_shift_open,
        $second_shift_close,
      );
      $display = _room_reservations_hours_display($day_hours);
      $hours_data = array(
        'open' => $open,
        'open_24_hours' => $open_24_hours,
        'shifts' => $shifts,
        'first_shift_open' => $first_shift_open,
        'first_shift_close' => $first_shift_close,
        'second_shift_open' => $second_shift_open,
        'second_shift_close' => $second_shift_close,
        'display' => $display,
      );
      $building_hours[($yyyymmdd)] = $hours_data;
    }
  }
  return $building_hours;
}

/**
 * Creates a string for displaying the open hours for a single day.
 *
 * @param array $day_hours
 *   An array that represents the openning and closing hours for two
 *   separate shifts in a single day
 *
 * @return string
 *   A string that can be used to display the hours for a single day, such as
 *   'Open 24 Hours' or 'Noon - 6:00 PM'.
 */
function _room_reservations_hours_display($day_hours) {
  $hours = _room_reservations_hours();
  $first_shift_open = $day_hours[0];
  $first_shift_close = $day_hours[1];
  $second_shift_open = $day_hours[2];
  $second_shift_close = $day_hours[3];
  // Closed.
  if (($first_shift_open == '9999') && 
    ($first_shift_close == '9999') && 
    ($second_shift_open == '9999') && 
    ($second_shift_close == '9999')) {
    return 'Closed';
  }
  // Open 24 hours.
  if (($first_shift_open == '0000') && 
    ($first_shift_close == '2400')) {
    return 'Open 24 Hours';
  }
  // One shift.
  if (($second_shift_open == '9999') && 
    ($second_shift_close == '9999')) {
    $first_shift_open_display
      = _room_reservations_display_time($first_shift_open);
    $first_shift_close_display
      = _room_reservations_display_time($first_shift_close);
    return $first_shift_open_display . ' - ' .
      $first_shift_close_display;
  }
  // Two shifts.
  $first_shift_open_display
    = _room_reservations_display_time($first_shift_open);
  $first_shift_close_display
    = _room_reservations_display_time($first_shift_close);
  $second_shift_open_display
    = _room_reservations_display_time($second_shift_open);
  $second_shift_close_display
    = _room_reservations_display_time($second_shift_close);
  return $first_shift_open_display . ' - ' .
    $first_shift_close_display . ' and ' .
    $second_shift_open_display . ' - ' .
    $second_shift_close_display;
}

/**
 * Create time slot array.
 *
 * Create an array with each element representing one of the 48 half hour time
 * slots that make up a day.
 *
 * @return array
 *   An array with each element representing a half hour time slot.
 */
function _room_reservations_times() {
  $times = array();
  $hours = _room_reservations_hours();
  foreach ($hours as $hour) {
    $times[] = $hour['time'];
  }
  return $times;
}

/**
 * Return time in display format.
 *
 * This function returns the time in display format (Midnight, 12:30 AM,
 * 1:00 AM, etc.)  for any time slot of the day given in military time
 * format (0000, 0030, 0100, etc.).
 *
 * @param string $military_time
 *   Time of day represented in four digit military time.
 *
 * @return string
 *   Time of day represented as HH:MM AM.
 */
function _room_reservations_display_time($military_time) {
  $hours = _room_reservations_hours();
  $hours[] = array(
    'time' => '2400',
    'display' => 'Midnight',
  );
  foreach ($hours as $hour) {
    $time = $hour['time'];
    if ($time == $military_time) {
      return $hour['display'];
    }
  }
  return '';
}

/**
 * Determine closing times.
 *
 * Determines which half hour time slots represent the last ones before the
 * building closes.
 *
 * @param string $yyyymmdd
 *   The date for which close times are being determine, in the format
 *   YYYY-MM-DD.
 *
 * @return array
 *   An array representing the time slots just before closing.  The array can
 *   contain 0, 1, or 2 items.
 */
function _room_reservations_close_times($yyyymmdd) {
  $closing_times = array();
  $_room_reservations_building_hours = _room_reservations_facility_hours();
  $building_hours_day = $_room_reservations_building_hours[$yyyymmdd];
  // 24 hours.
  if ($building_hours_day['open_24_hours']) {
    $next_day = date('Y-m-d', strtotime("$yyyymmdd +1 days"));
    $next_day_first_time_slot_open
      = _room_reservations_first_slot_open($next_day);
    if (!$next_day_first_time_slot_open) {
      $closing_times[] = '2330';
    }
    return $closing_times;
  }
  // First shift ends at midnight.
  if ($building_hours_day['first_shift_close'] === '2400') {
    $next_day = date('Y-m-d', strtotime("$yyyymmdd +1 days"));
    $next_day_first_time_slot_open
      = _room_reservations_first_slot_open($next_day);
    if (!$next_day_first_time_slot_open) {
      $closing_times[] = '2330';
    }
    return $closing_times;
  }
  // First shift does not end at midnight.
  $time = $building_hours_day['first_shift_close'];
  $hours = _room_reservations_hours();
  $time_found = FALSE;
  while (!$time_found) {
    $time_slot = array_pop($hours);
    if ($time_slot['time'] == $time) {
      $time_found = TRUE;
    }
  }
  $time_slot = array_pop($hours);
  $int_second_shift_close = intval($building_hours_day['second_shift_close']);
  $closing_times[] = $time_slot['time'];
  // Second shift ends at midnight.
  if (($building_hours_day['shifts'] == 2) && 
    ($int_second_shift_close == 2400)) {
    $next_day = date('Y-m-d', strtotime("$yyyymmdd +1 days"));
    $next_day_first_time_slot_open
      = _room_reservations_first_slot_open($next_day);
    if (!$next_day_first_time_slot_open) {
      $closing_times[] = '2330';
    }
  }
  // Second shift does not end at midnight.
  if (($building_hours_day['shifts'] == 2) && 
  ($int_second_shift_close < 2400)) {
    $time = $building_hours_day['second_shift_close'];
    $hours = _room_reservations_hours();
    $time_found = FALSE;
    while (!$time_found) {
      $time_slot = array_pop($hours);
      if ($time_slot['time'] == $time) {
        $time_found = TRUE;
      }
    }
    $time_slot = array_pop($hours);
    $closing_times[] = $time_slot['time'];
  }
  return $closing_times;
}

/**
 * Determine if facility is open from midnight to 12:30 AM.
 *
 * Determines if the facility is open during the first half hour of the day,
 * from midnight to 12:30 AM.  This information is needed when determining if
 * any particular half hour time slot is the last one before the building
 * closes.
 *
 * @param string $yyyymmdd
 *   The date being examined, in the format YYYY-MM-DD.
 *
 * @return bool
 *   TRUE - The facility is open during the first half hour of the day.
 *   FALSE - The facility is not open during the first half hour of the day.
 */
function _room_reservations_first_slot_open($yyyymmdd) {
  $_room_reservations_building_hours = _room_reservations_facility_hours();
  $building_hours_day = $_room_reservations_building_hours[$yyyymmdd];
  if (!$building_hours_day['open']) {
    return FALSE;
  }
  if ($building_hours_day['open_24_hours']) {
    return TRUE;
  }
  if ($building_hours_day['first_shift_open'] == '0000') {
    return TRUE;
  }
  return FALSE;
}

/**
 * Determine reservation start conflicts.
 *
 * Determines if a new reservation room, date and start time conflicts with a
 * previously existing reservation.
 *
 * @param string $room
 *   The room that is being reserved.
 * @param string $yyyymmdd
 *   The date of the start time for the reservation, in the format 'yyyy-mm-dd'.
 * @param string $time
 *   The start time for the reservation, in military time.  9:00 AM is
 *   represented as '0900', and 9:00 PM is represented as '2100'.
 *
 * @return bool
 *   TRUE - A scheduling conflict was found.
 *   FALSE - A scheduling conflict was not found.
 */
function _room_reservations_start_conflicts($room, $yyyymmdd, $time) {
  // Previous and next days.
  $previous_day = date('Y-m-d', strtotime("$yyyymmdd -1 days"));
  // Start times of other reservations that could conflict with this one.
  // Since reservations are limited to 2 hours, we are interested in times that
  // are less than or equal to the reservation time and greater than or equal
  // to 90 minutes before the reservation time.
      $max_length = user_access('create room reservations extended length') ? variable_get('room_reservations_max_length_extended', 120) : variable_get('room_reservations_max_length_standard', 120);
  $max_slots = $max_length / 30;
  $search_items = array();
  for ($x = 0; $x < 8; $x++) {
    $search_items = array(
      'date' => '1999-01-01',
      'start_time' => '9999',
      'length' => 999,
    );
  }
  $day = $yyyymmdd;
  $hours = _room_reservations_hours();
  $time_found = FALSE;
  $time_slot = NULL;
  while (!$time_found) {
    $time_slot = array_pop($hours);
    if ($time_slot['time'] == $time) {
      $time_found = TRUE;
    }
  }
  for ($x = 0; $x < $max_slots; $x++) {
    if ($time_slot == NULL) {
      $day = $previous_day;
      $hours = _room_reservations_hours();
      $time_slot = array_pop($hours);
    }
    $search_item['date'] = $day;
    $search_item['start_time'] = $time_slot['time'];
    $search_item['length'] = 30 * $x;
    $search_items[] = $search_item;
    $time_slot = array_pop($hours);
  }
  $sql = "
    SELECT id FROM {room_reservations}
    WHERE (
    (deleted = 'N' AND room = :room AND date = :date0 AND	time = :time0 AND length > :length0)
    OR
    (deleted = 'N' AND room = :room AND date = :date1 AND	time = :time1 AND length > :length1)
    OR
    (deleted = 'N' AND room = :room AND date = :date2 AND	time = :time2 AND length > :length2)
    OR
    (deleted = 'N' AND room = :room AND date = :date3 AND	time = :time3 AND length > :length3)
    OR
    (deleted = 'N' AND room = :room AND date = :date4 AND	time = :time4 AND length > :length4)
    OR
    (deleted = 'N' AND room = :room AND date = :date5 AND	time = :time5 AND length > :length5)
    OR
    (deleted = 'N' AND room = :room AND date = :date6 AND	time = :time6 AND length > :length6)
    OR
    (deleted = 'N' AND room = :room AND date = :date7 AND	time = :time7 AND length > :length7)
    )
  ";
  $conflicts_found = FALSE;
  $conflicts_found = db_query($sql, array(
    ':room' => $room,
    ':date0' => $search_items[0]['date'], ':time0' => $search_items[0]['start_time'], ':length0' => $search_items[0]['length'], 
    ':date1' => $search_items[1]['date'], ':time1' => $search_items[1]['start_time'], ':length1' => $search_items[1]['length'],
    ':date2' => $search_items[2]['date'], ':time2' => $search_items[2]['start_time'], ':length2' => $search_items[2]['length'],
    ':date3' => $search_items[3]['date'], ':time3' => $search_items[3]['start_time'], ':length3' => $search_items[3]['length'],
    ':date4' => $search_items[4]['date'], ':time4' => $search_items[4]['start_time'], ':length4' => $search_items[4]['length'],
    ':date5' => $search_items[5]['date'], ':time5' => $search_items[5]['start_time'], ':length5' => $search_items[5]['length'],
    ':date6' => $search_items[6]['date'], ':time6' => $search_items[6]['start_time'], ':length6' => $search_items[6]['length'],
    ':date7' => $search_items[7]['date'], ':time7' => $search_items[7]['start_time'], ':length7' => $search_items[7]['length'])
  )->rowCount();
  return $conflicts_found;
}

/**
 * Determine valid reservation lengths of time.
 *
 * Determines which lengths of time are valid for a reservation for a particular
 * room starting at particular time.  Valid lengths are limited by the
 * following:
 * (1)  Previously scheduled reservations.
 * (2)  Building close times.
 * (3)  Last time slot of the day.  Reservations possibly end 15 minutes before
 * the building closes.
 *
 * @param string $room
 *   The room that is being reserved.
 * @param string $yyyymmdd
 *   The date of the start time for the reservation, in the format 'yyyy-mm-dd'.
 * @param string $time
 *   The start time for the reservation, in military time.
 *   9:00 AM is represented as '0900', and 9:00 PM is represented as '2100'.
 * @param int $id
 *   The id of the reservation being made.
 * @param int $all
 *   By default we do not check the first time slot since it should be true or we wouldn't have been able to pick it on calendar
 *    but, for repeating reserverations we need to check all slots. 
 *
 * @return array
 *   An array with an element for each possible reservation length of time,
 *   and an indicator showing whether that particular length is valid for the
 *   reservation being made.
 */
function _room_reservations_valid_lengths($rid, $yyyymmdd, $time, $id = NULL, $all = FALSE) {
  // let's first ensure this is a valid RID
  $rooms = _room_reservations_rooms();
  if (!isset($rooms[$rid])) {
    return null;  
  }
  
  $max_length = user_access('create room reservations extended length') 
    ? variable_get('room_reservations_max_length_extended', 120) 
    : variable_get('room_reservations_max_length_standard', 120);
  $max_slots = $max_length / 30;
  $valid_lengths = array();
  for ($x = 30; $x <= $max_length; $x += 30) {
    $valid_lengths[] = array(
      'length' => $x,
      'is_valid' => TRUE,
    );
  }
  // Divide the maximum reservation length into 30 minute time slots and determine the start date and time of each of these slots.
  $search_items = array();
  // Time slots for the first day.
  $day = $yyyymmdd;
  $hours = _room_reservations_hours();
  $x = 0;
  $include = FALSE;
  foreach ($hours as $time_slot) {
    if ($time_slot['time'] == $time) {
      $include = TRUE;
    }
    if ($include) {
      $search_item['date'] = $day;
      $search_item['start_time'] = $time_slot['time'];
      $search_items[] = $search_item;
      $x++;
    }
    if ($x == $max_slots) {
      $include = FALSE;
      break;
    }
  }
  // If it's possible for the reservation to span 2 days, get the time slots
  // for the subsequent day.
  if ($x < $max_slots) {
    $include = TRUE;
    $day = date('Y-m-d', strtotime("$yyyymmdd +1 days"));
    $hours = _room_reservations_hours();
    foreach ($hours as $time_slot) {
      if ($include) {
        $search_item['date'] = $day;
        $search_item['start_time'] = $time_slot['time'];
        $search_items[] = $search_item;
        $x++;
      }
      if ($x == $max_slots) {
        $include = FALSE;
        break;
      }
    }
  }
  
  // Determine if the reservation at each possible length would conflict with another reservation.  If so, set is_valid for that length to FALSE.
  // The first time slot has already been validated and does not need to be checked again.
  if ($all) {
    $start = 0;
  }
  else {
    $start = 1;
  }
  for ($x = $start; $x < $max_slots; $x++) {
    $valid_length = $valid_lengths[$x];
    if ($valid_length['is_valid']) {
      $search_item = $search_items[$x];
      $date = $search_item['date'];
      $start_time = $search_item['start_time'];
      
      $conflicts_found = false;
      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', 'node')
        ->entityCondition('bundle', 'room_reservations_reservation')   
        ->fieldCondition('field_reservation_date', 'value', $date, '=')     
        ->fieldCondition('field_reservation_time', 'value', $start_time, '=')         
        ->fieldCondition('field_reservation_room', 'target_id', $rid, '=')
        // EFQ respects access so let's load the reservations as user 1 to make sure we get them all
        ->addMetaData('account', user_load(1));
      // if editing a reservation (series) we need to not include any of the reservations in current series
      if (arg(2) == 'edit') {
        $nid = arg(1);
        $res = node_load($nid);
        if (isset($res->field_reservation_seriesid[LANGUAGE_NONE][0]['value']) && $sid = $res->field_reservation_seriesid[LANGUAGE_NONE][0]['value']) {
          $query->fieldCondition('field_reservation_seriesid', 'value', $sid, '!=');
        }
      }     
      $result = $query->execute();
      if (isset($result['node'])) {
        $conflicts_found = true;
        for ($y = $x; $y < $max_slots; $y++) {
          $valid_lengths[$y]['is_valid'] = FALSE;
        }
      }
    }
  }
  // need to limit these valid lengths by a prebuffer if it exists for the room
  // but only if not permissions to do this
  if (!user_access('book over buffer')) {
    $preslots = $rooms[$rid]['field_reservations_prebuffer'][LANGUAGE_NONE][0]['value'] / 30;
    if ($preslots) {
      foreach ($valid_lengths as $index => $length) {
        if ($length['is_valid'] == false) {
          $last = $index;
          break;
        }
      }
      if (isset($last)) {
        for ($x = $last - $preslots; $x <= $last; $x++) {
          $valid_lengths[$x]['is_valid'] = false;
        }
      }
    }
  }

  // Determine if the reservation at each possible length would conflict with
  // the hours that the building is closed.  If so, set is_valid for that
  // length to FALSE.  The first time slot has already been validated and
  // does not need to be checked again.
  $_room_reservations_building_hours = _room_reservations_facility_hours();
  $conflicts_found = FALSE;
  for ($x = 0; $x < $max_slots; $x++) {
    $valid_length = $valid_lengths[$x];
    if ($valid_length['is_valid']) {
      $search_item = $search_items[$x];
      $date = $search_item['date'];
      $start_time = $search_item['start_time'];
      if (!isset($_room_reservations_building_hours[$date])) {
        $conflicts_found = TRUE;
        break;
      }
      $building_hours_day = $_room_reservations_building_hours[$date];
      // If building is closed, set conflict flag.
      if (!$building_hours_day['open']) {
        $conflicts_found = TRUE;
        break;
      }
      $int_start_time = intval($start_time);
      $int_first_shift_open = intval($building_hours_day['first_shift_open']);
      $int_first_shift_close = intval($building_hours_day['first_shift_close']);
      // One shift.
      if ((!$building_hours_day['open_24_hours']) && ($building_hours_day['shifts'] == 1) && 
      (($int_start_time < $int_first_shift_open) || ($int_start_time >= $int_first_shift_close))) {
        $conflicts_found = TRUE;
        break;
      }
      // Two shifts.
      $int_second_shift_open = intval($building_hours_day['second_shift_open']);
      $int_second_shift_close = intval($building_hours_day['second_shift_close']);
      if ((!$building_hours_day['open_24_hours']) && 
        ($building_hours_day['shifts'] == 2) && 
        (($int_start_time < $int_first_shift_open) ||
          (($int_start_time >= $int_first_shift_close) &&
          ($int_start_time < $int_second_shift_open)) ||
          ($int_start_time >= $int_second_shift_close))
        ) {
        $conflicts_found = TRUE;
        break;
      }
    }

  }

  if ($conflicts_found) {
    for ($y = $x; $y < $max_slots; $y++) {
      $valid_lengths[$y]['is_valid'] = FALSE;
    }
  }

  return $valid_lengths;
}

/**
 * Determine if the user has exceeded the maximum reservations per day.
 *
 * @global object $user
 *   Drupal user object.
 *
 * @param string $yyyymmdd
 *   The date for which maximum allowable reservations are being checked, in
 *   the format YYYY-MM-DD.
 *
 * @return bool
 *   TRUE - the maximum has been exceeded.
 *   FALSE - the maximum has not been exceeded.
 */
function _room_reservations_daily_max_exceeded($yyyymmdd) {
  $max = variable_get('room_reservations_reservations_per_day', 1);
  if (!$max) {
    return FALSE;
  }
  global $user;
  $record_count = 0;
  if ($user->uid) {
    $sql = "SELECT id FROM {room_reservations} WHERE user_name = :name AND date = :date AND deleted = 'N'";
    $record_count = db_query($sql, array(':name' => $user->name, ':date' => $yyyymmdd))->rowCount();
  }
  if ($record_count < $max) {
    return FALSE;
  }
  else {
    return TRUE;
  }
}

/**
 * Find the current user's reservations for the next 14 days.
 *
 * @global object $user
 *   A Drupal user object.
 *
 * @return array
 *   An array with each element representing one reservation that the user has
 *   made.
 */
function _room_reservations_user_reservations() {
  $user_reservations = array();
  $all_hours = _room_reservations_hours();
  global $user;
  if ($user->uid) {
    $earliest_date = date('Y-m-d', strtotime(date('Y-m-d')));
    $latest_date = date('Y-m-d', strtotime("now +13 days"));
    
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'node')
      ->entityCondition('bundle', 'room_reservations_reservation')   
      ->propertyCondition('uid', $user->uid)
      ->fieldCondition('field_reservation_date', 'value', $earliest_date, '>=')     
      ->fieldCondition('field_reservation_date', 'value', $latest_date, '<=')
      ->fieldOrderBy('field_reservation_date', 'value', 'DESC')  
      ->fieldOrderBy('field_reservation_time', 'value', 'DESC');
    $result = $query->execute();
    if (isset($result['node'])) {
      $result_nids = array_keys($result['node']);
      $results = entity_load('node', $result_nids);
      foreach ($results as $data) {
        $reservation = array();
        $reservation['id'] = $data->nid;
        $unix_timestamp = strtotime($data->field_reservation_date[LANGUAGE_NONE][0]['value']);
        $reservation['date'] = date("l, n/j", $unix_timestamp);
        $reservation['time'] = '';
        foreach ($all_hours as $time_slot) {
          if ($time_slot['time'] == $data->field_reservation_time[LANGUAGE_NONE][0]['value']) {
            $reservation['time'] = $time_slot['display'];
            break;
          }
        }
        $user_reservations[] = $reservation;
      }
    }
  }
  return $user_reservations;
}


/**
 * Replace tokens in messages.
 *
 * Inserts reservation specific information in email and text messages
 * sent as confirmation and reminder of reservations.
 *
 * @param string $text
 *   The email or text message.
 * @param array $parameters
 *   Reservation specific information to be inserted into the message.
 *
 * @return string
 *   The email or text message updated with reservation specific information.
 */
function _room_reservations_replace_tokens($text, $parameters) {
  $tokens = array(
    '%reservation_name' => $parameters['name'],
    '%room' => $parameters['room'],
    '%month_number' => $parameters['month_number'],
    '%month' => $parameters['month'],
    '%day_of_the_week' => $parameters['day_of_week'],
    '%day' => $parameters['day'],
    '%time' => $parameters['time'],
    '%minutes' => $parameters['minutes'],
  );
  foreach ($tokens as $key => $value) {
    $text = str_replace($key, $value, $text);
  }
  return $text;
}

/**
 * Get a variable from the room_reservations_variables table.
 *
 * @param string $key
 *   The key used to identify the variable.
 *
 * @return object
 *   The value of the variable, or FALSE, if a record could not be found with
 *   the supplied $key.
 */
function _room_reservations_get_variable($key) {
  $sql = "SELECT value FROM {room_reservations_variables} WHERE name = :name";
  $result = db_query($sql, array(':name' => $key))->fetchField();
  if ($result) {
    return $result;
  }
  else {
    return FALSE;
  }
}

/**
 * Save a module variable.
 *
 * Set a variable in the room_reservations_variables table.  If a record
 * with the key exists, update it.  If a record with the key does not exist,
 * insert a new record.
 *
 * @param string $key
 *   The key used to identify the variable.
 * @param object $value
 *   The value of the variable.
 *
 * @return object
 *   A database query result resource.
 */
function _room_reservations_set_variable($key, $value) {
  $sql = "SELECT id FROM {room_reservations_variables} WHERE name = :name";
  $id = db_query($sql, array(':name' => $key))->fetchField();
  if ($id) {
    $sql = "UPDATE {room_reservations_variables} SET value = :value WHERE id = :id";
    $result = db_query($sql, array(':value' => $value, ':id' => $id));
    return $result;
  }
  else {
    $sql = "INSERT INTO {room_reservations_variables} (name, value) VALUES (:name, :value)";
    $result = db_query($sql, array(':name' => $key, ':value' => $value));
    return $result;
  }
}

/**
 * Delete a variable in the room_reservations_variables table.
 *
 * @param string $key
 *   The key used to identify the variable.
 *
 * @return object
 *   A database query result resource, or FALSE if the record to be deleted
 *   could not be found in the database.
 */
function _room_reservations_delete_variable($key) {
  $sql = "SELECT id FROM {room_reservations_variables} WHERE name = :name";
  $id = db_query($sql, array(':name' => $key))->fetchField();
  if ($id) {
    $sql = "DELETE FROM {room_reservations_variables} WHERE id = :id";
    $result = db_query($sql, array(':id' =>$id));
    return $result;
  }
  return FALSE;
}

function _room_reservations_is_slot_free($rid, $yyyymmdd, $time, $length) {
  $slots = _room_reservations_valid_lengths($rid, $yyyymmdd, $time, NULL, true);
  foreach ($slots as $slot) {
    if ($slot['length'] == $length && $slot['is_valid']) {
      return true;
    }
  }
  return false;
}

function _room_reservations_yyyymmdd($month, $day) {
  // determine if this year or next year
  $yearnow = date('Y');
  $absdaynow = date('z');
  $absdaydefault = date('z', mktime(0, 0, 0, $month, $day, $yearnow));
  if ($absdaynow > $absdaydefault) {
    $year = $yearnow + 1;
  }
  else {
    $year = $yearnow;
  }
  return date('Y-m-d', strtotime($year . '-' . $month . '-' . $day));
}