<!DOCTYPE html>

<html>
<head>
  <title>Server-Side Include Test</title>
  <link rel="stylesheet" href="styles.css" />
</head>

<body>
  <div class="Header">
    <h1>Templates Rule!</h1>
  </div>

  <div class="MenuPanel">
<?php include 'menu.htm'; ?>
    </div>

  <div class="Content">
    <p>This is the welcome page. Just above this text is the handy menu
    for this site.</p>
  </div>
</body>
</html>
