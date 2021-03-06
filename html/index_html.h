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
  <body class="container">
    <nav class="navbar sticky-top navbar-dark bg-dark">
      <span class="navbar-brand">WiFi-Neopixel <small class="navbar-text">on ESP8266</small></span>
      <div class="form-check navbar-text">
        <input class="form-check-input" type="checkbox" value="" id="verbose">
        <label class="form-check-label" for="verbose">Verbose</label>
      </div>
    </nav>
    <div class="container">
      <div id="output"></div>

      <h2>Available URLs</h2>
      
      <div class="row">
        <div class="col col-lg-4 col-md-6 col-sm-12">
          <div class="card">
            <h3 class="card-header"><code>/clear</code></h3>
            <div class="card-body">
              <p class="card-text">Clear the NeoPixel strip.</p>
              <form id="clear" action="/clear" method="GET">
              <input type="submit" class="btn btn-primary"/>
              </form>
            </div>
          </div>
        </div>
        <div class="col col-lg-4 col-md-6 col-sm-12">
          <div class="card">
            <h3 class="card-header"><code>/wipe</code></h3>
            <div class="card-body">
              <p class="card-text">Wipe a solid colour over the whole strip.</p>

              <form action="/wipe" method="GET">
                <div class="input-group mb-3">
                  <div class="input-group-prepend">
                    <span class="input-group-text">Wait</span>
                  </div>
                  <input class="form-control" type="number" name="wait" value="0" min="0">
                  <div class="input-group-append">
                    <span class="input-group-text">ms</span>
                  </div>
                </div>
                <div class="input-group mb-3">
                  <div class="input-group-prepend">
                    <span class="input-group-text">Colour</span>
                    <span class="input-group-text">#</span>
                  </div>
                  <input class="form-control" type="color" name="colour" maxlength="6" pattern="[0-9a-fA-F]{6,6}" size="6" placeholder="RRGGBB">
                </div>
                <input type="submit" class="btn btn-primary"/>
              </form>
            </div>
          </div>
        </div>
        <div class="col col-lg-4 col-md-6 col-sm-12">
          <div class="card">
            <h3 class="card-header"><code>/sunrise</code></h3>
            <div class="card-body">
              <p class="card-text">Simulate a sunrise. There are 300 steps, with a <code>wait</code> between each.
              For a 30min sunrise, use <code>wait</code> of <code>6000</code>ms.</p>
        
              <form action="/sunrise" method="GET">
                <div class="input-group mb-3">
                  <div class="input-group-prepend">
                    <span class="input-group-text">Wait</span>
                  </div>
                  <input class="form-control" type="number" name="wait" value="0" min="0">
                  <div class="input-group-append">
                    <span class="input-group-text">ms</span>
                  </div>
                </div>
                <input type="submit" class="btn btn-primary"/>
              </form>
            </div>
          </div>
        </div>
        <div class="col col-lg-4 col-md-6 col-sm-12">
          <div class="card">
            <h3 class="card-header"><code>/sunset</code></h3>
            <div class="card-body">
              <p class="card-text">Simulate a sunset (backwards sunrise). There are 300 steps, with a <code>wait</code> between each.
              For a 5min sunset, use <code>wait</code> of <code>1000</code>ms.</p>

              <form action="/sunset" method="GET">
                <div class="input-group mb-3">
                  <div class="input-group-prepend">
                    <span class="input-group-text">Wait</span>
                  </div>
                  <input class="form-control" type="number" name="wait" value="0" min="0">
                  <div class="input-group-append">
                    <span class="input-group-text">ms</span>
                  </div>
                </div>
                <input type="submit" class="btn btn-primary"/>
              </form>
            </div>
          </div>
        </div>
        <div class="col col-lg-4 col-md-6 col-sm-12">
          <div class="card">
            <h3 class="card-header"><code>/rainbow</code></h3>
            <div class="card-body">
              <p class="card-text">Display a cycling rainbow pattern.</p>

              <form action="/rainbow" method="GET">
                <div class="input-group mb-3">
                  <div class="input-group-prepend">
                    <span class="input-group-text">Wait</span>
                  </div>
                  <input class="form-control" type="number" name="wait" value="0" min="0">
                  <div class="input-group-append">
                    <span class="input-group-text">ms</span>
                  </div>
                </div>
                <div class="input-group mb-3">
                  <div class="input-group-prepend">
                    <span class="input-group-text">Cycles</span>
                  </div>
                  <input class="form-control" type="number" name="cycles" value="1" min="1" size="6">
                </div>
              <input type="submit" class="btn btn-primary"/>
              </form>
            </div>
          </div>
        </div>
      </div>
    </div>

    <script src="https://code.jquery.com/jquery-3.3.1.min.js" integrity="sha256-FgpCb/KJQlLNfOu91ta32o/NMZxltwRo8QtmkMRdAu8=" crossorigin="anonymous"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.6/umd/popper.min.js" integrity="sha384-wHAiFfRlMFy6i5SRaxvfOCifBUQy1xHdJ/yoi7FRNXMRBu5WHdZYu1hA6ZOblgut" crossorigin="anonymous"></script>
    <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.2.1/js/bootstrap.min.js" integrity="sha384-B0UglyR+jN6CkvvICOB2joaf5I4l3gm9GU6Hc1og6Ls7i6U/mkkaduKaBhlAXv9k" crossorigin="anonymous"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/jquery.form/4.2.2/jquery.form.min.js" integrity="sha384-FzT3vTVGXqf7wRfy8k4BiyzvbNfeYjK+frTVqZeNDFl8woCbF0CYG6g2fMEFFo/i" crossorigin="anonymous"></script>
    <script>
    /* https://github.com/madmurphy/cookies.js (GPL3) */
    var docCookies={getItem:function(e){return e&&decodeURIComponent(document.cookie.replace(new RegExp("(?:(?:^|.*;)\\s*"+encodeURIComponent(e).replace(/[\-\.\+\*]/g,"\\$&")+"\\s*\\=\\s*([^;]*).*$)|^.*$"),"$1"))||null},setItem:function(e,o,t,n,r,c){if(!e||/^(?:expires|max\-age|path|domain|secure)$/i.test(e))return!1;var s="";if(t)switch(t.constructor){case Number:s=t===1/0?"; expires=Fri, 31 Dec 9999 23:59:59 GMT":"; max-age="+t;break;case String:s="; expires="+t;break;case Date:s="; expires="+t.toUTCString()}return document.cookie=encodeURIComponent(e)+"="+encodeURIComponent(o)+s+(r?"; domain="+r:"")+(n?"; path="+n:"")+(c?"; secure":""),!0},removeItem:function(e,o,t){return!!this.hasItem(e)&&(document.cookie=encodeURIComponent(e)+"=; expires=Thu, 01 Jan 1970 00:00:00 GMT"+(t?"; domain="+t:"")+(o?"; path="+o:""),!0)},hasItem:function(e){return!(!e||/^(?:expires|max\-age|path|domain|secure)$/i.test(e))&&new RegExp("(?:^|;\\s*)"+encodeURIComponent(e).replace(/[\-\.\+\*]/g,"\\$&")+"\\s*\\=").test(document.cookie)},keys:function(){for(var e=document.cookie.replace(/((?:^|\s*;)[^\=]+)(?=;|$)|^\s*|\s*(?:\=[^;]*)?(?:\1|$)/g,"").split(/\s*(?:\=[^;]*)?;\s*/),o=e.length,t=0;t<o;t++)e[t]=decodeURIComponent(e[t]);return e},clear:function(e,o){for(var t=this.keys(),n=t.length,r=0;r<n;r++)this.removeItem(t[r],e,o)}};"undefined"!=typeof module&&void 0!==module.exports&&(module.exports=docCookies);
    </script>
    <script>
      $(function () {
        // load the verbose setting
        var verbose = docCookies.getItem("verbose") == "true"
        $('#verbose').prop('checked', verbose)
        $('#verbose').change('change', function (event) {
          verbose = event.target.checked
          docCookies.setItem('verbose', verbose)
        })

        var o = $("#output")
        function log(level, msg) {
          if (verbose) console.log('log:', msg)
          var a = $(`<div>`).addClass('row').append(
            $('<div role="alert">').addClass(['col-md-auto', 'alert', `alert-${level}`,
                'alert-dismissible', 'fade', 'show']).append(
              $('<pre>').addClass(['pre-scrollable']).append(
                $('<code>').text(msg)
              ).append(
                $('<button type="button" class="close" data-dismiss="alert" aria-label="Close">').html(
                  '<span aria-hidden="true">&times;</span>'
                )
              )
            )
          )
          o.append(a)

          setTimeout(function() {
            a.fadeTo(500, 0).slideUp(500, function(){
              $(this).remove()
            })
          }, 20000)
        }
        
        $("form").submit(function (event) {
          event.preventDefault()

          var q = $(this).formSerialize()
          var u = event.target.action
          if (verbose) {
            log('info', `calling ${u}?${q}`)
          }
          console.log(event, q, u)
          $.get(u, q).done((data) => {
            log('success', data)
          }).fail(function(err) {
            log('danger', `${err.statusText} (see console for details)`)
            console.error(err)
          })
        })
      })
    </script>
  </body>
</html>
)=====";
#endif
