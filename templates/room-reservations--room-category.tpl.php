<?php
  $id = strtolower(preg_replace('/[^a-zA-Z0-9-]+/', '-', $category->name));
  $rooms_per_category = 0;
  foreach ($rooms as $room) {
    $rid = $room['nid'];
    if (!empty($room['field_reservations_room_category'][LANGUAGE_NONE][0]['target_id']) && $room['field_reservations_room_category'][LANGUAGE_NONE][0]['tid'] == $category->tid) {
      $rooms_per_category++;
    }
  }
?>
<div id="<?php print $id; ?>" class="panel <?php print $show; ?>">
  <div class="gcolumns">
    <pre><?php print_r($rooms); ?></pre>
  </div>
</div>