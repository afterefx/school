<?php

header("Cache-Control: no-cache, must-revalidate"); // HTTP/1.1
header("Expires: Sat, 26 Jul 1997 05:00:00 GMT"); // Date in the past

$page =<<<HTML
<html>
<head>
<title>Test</title>
<link rel="stylesheet" href="style.css" type="text/css" />
</head>
<body>
<div id="title"></div>
<div id="menu"></div>
<div id="gallery">
    <div class="row">
        <span class="image">
        text
        </span>
        <span class="image">
        text
        </span>
        <span class="image">
        text
        </span>
        <span class="image">
        text
        </span>
    </div>
    <div class="row">
        <span class="image">
        </span>
        <span class="image">
        </span>
        <span class="image">
        </span>
        <span class="image">
        </span>
    </div>
    <div class="row">
        <span class="image">
        </span>
        <span class="image">
        </span>
        <span class="image">
        </span>
        <span class="image">
        </span>
    </div>
    <div class="row">
        <span class="image">
        </span>
        <span class="image">
        </span>
        <span class="image">
        </span>
        <span class="image">
        </span>
    </div>
</div>

</body>
</html>
HTML;
echo $page;
?>
