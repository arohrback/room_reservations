<?php
    $room_view = node_view((object) $room, 'reservation_calendar');
    $room_header = drupal_render($room_view);
?>
  <table class="reservation-grid">
    <tr>
    <td class="room-info">Room</td>
    <td class="room-info">
      <?php print $room_header; ?>
    </td>
    </tr>
  </table>