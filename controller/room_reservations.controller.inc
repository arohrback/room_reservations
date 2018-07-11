<?php
/**
 * @file
 * Miscellaneous controller functionality.
 */

/**
 * Display a page showing the reservation system policies.
 *
 * @return string
 *   The policies for the reservation system as entered by the administrator
 *   using the Display Text configuration page.
 */
function room_reservations_policies() {
  $output = check_markup(_room_reservations_get_variable('policies'));
  return $output;
}

