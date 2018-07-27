<div id='rooms-calendar'>
  <div id='tabbedPanels'>
    <h2>Reservation Calendar</h2>
    <p class="alert alert-note">
      <span class="fa fa-info-circle"></span>
      Click in a time slot to reserve a room.
    </p>
    <div class="date-picker-wrap">
      <div class="date-picker-label"><?php print $date ?></div>
      <?php print render($form) ?>
    </div>
  </div>
  <ul class='room-tabs'>
    <?php $i = 0; ?>
    <?php foreach ($categories as $category): ?>
      <?php //  $active = ($i == 0) ? " class='active'" : ""; ?>
      <?php  $id = strtolower(preg_replace('/[^a-zA-Z0-9-]+/', '-', $category['name'])); ?>
      <li>
        <a class="<?php ++$i == 1 && print "active"; ?>" href="#<?php print $id; ?>">
          <?php print $category['name']; ?>
        </a>
      </li>
    <?php endforeach; ?>
  </ul>
  <div class='panelContainer'>
    <div class="alert">
    <pre><?php print_r($categories); ?></pre>
    </div>
    <?php print render($categories); ?>
  </div>
  
  <div class="clear">
  </div>
</div>
