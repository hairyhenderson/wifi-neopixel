#ifndef INDEX_HTML
#define INDEX_HTML
const String index_html = R"=====(
<!doctype html>
<html lang="en">
  <head>
    <!-- Required meta tags -->
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">

    <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.2.1/css/bootstrap.min.css" integrity="sha384-GJzZqFGwb1QTTN6wy59ffF1BuGJpLSa9DkKMp0DgiMDm4iYMj70gZWKYbI706tWS" crossorigin="anonymous">
    <title>WiFi NeoPixel</title>
  </head>
  <body>
    <h1>Hello from ESP8266!</h1>
    Available URLs:
    
    <h3><code>/clear</code></h3>
    Clear the NeoPixel strip.
    <form action="/clear" method="GET">
      <input type="submit" class="btn btn-primary"/>
    </form>

    <h3><code>/wipe</code></h3>
    <p>
      Wipe a solid colour over the whole strip.
    </p>

    <form action="/wipe" method="GET">
    <ul>
      <li>Wait: <input type="number" name="wait" value="0" min="0" size="6">ms
      <li>Colour: #<input type="color" name="colour" value="808080" maxlength="6" pattern="[0-9a-fA-F]{6,6}" size="6"> <i>(RRGGBB)</i>
    </ul>
    <input type="submit" class="btn btn-primary"/>
    </form>

    <h3><code>/sunrise</code></h3>

    <p>
      Simulate a sunrise. There are 300 steps, with a <code>wait</code> between each.
      For a 30min sunrise, use <code>wait</code> of <code>6000</code>ms.
    </p>

    <form action="/sunrise" method="GET">
    <ul>
      <li>Wait: <input type="number" name="wait" value="5" min="0" size="6">ms
    </ul>
    <input type="submit" class="btn btn-primary"/>
    </form>

    <h3><code>/sunset</code></h3>
    <p>
      Simulate a sunset (backwards sunrise). There are 300 steps, with a <code>wait</code> between each.
      For a 5min sunset, use <code>wait</code> of <code>1000</code>ms.
    </p>

    <form action="/sunset" method="GET">
    <ul>
      <li>Wait: <input type="number" name="wait" value="5" min="0" size="6">ms
    </ul>
    <input type="submit" class="btn btn-primary"/>
    </form>

    <h3><code>/rainbow</code></h3>

    <p>Display a cycling rainbow pattern.</p>

    <form action="/rainbow" method="GET">
    <ul>
      <li>Wait: <input type="number" name="wait" value="5" min="0" size="6">ms
      <li>Cycles: <input type="number" name="cycles" value="1" min="1" size="6">
    </ul>
    <input type="submit" class="btn btn-primary"/>
    </form>

    <script src="https://code.jquery.com/jquery-3.3.1.slim.min.js" integrity="sha384-q8i/X+965DzO0rT7abK41JStQIAqVgRVzpbzo5smXKp4YfRvH+8abtTE1Pi6jizo" crossorigin="anonymous"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.6/umd/popper.min.js" integrity="sha384-wHAiFfRlMFy6i5SRaxvfOCifBUQy1xHdJ/yoi7FRNXMRBu5WHdZYu1hA6ZOblgut" crossorigin="anonymous"></script>
    <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.2.1/js/bootstrap.min.js" integrity="sha384-B0UglyR+jN6CkvvICOB2joaf5I4l3gm9GU6Hc1og6Ls7i6U/mkkaduKaBhlAXv9k" crossorigin="anonymous"></script>
  </body>
</html>
)=====";
#endif
