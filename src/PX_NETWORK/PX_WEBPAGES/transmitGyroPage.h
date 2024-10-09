#pragma once

const char *const transmitGyroHTML = R"=====(

  <!DOCTYPE html>
  <html>
  <head>
    <title>Jamminnnn'</title>
    <style>
      h1 {text-align: center;}
      p {text-align: center;}
      form {text-align: center;}
      {text-align: center;}
    </style>
    <script>
      function fetchValue() {
        fetch('/getValue')
          .then(response => response.text())
          .then(data => {
            const values = data.split(',');  // Split the comma-separated response
            document.getElementById('pitch').innerHTML = values[0];
            document.getElementById('roll').innerHTML = values[1];
            document.getElementById('yaw').innerHTML = values[2]
            document.getElementById('direction').innerHTML = values[3];
            document.getElementById('battery').innerHTML = values[4];
          })
          .catch(error => console.error('Error fetching values:', error));
      }
      setInterval(fetchValue, 16); // Fetch every 16 milliseconds (~60Hz)
    </script>
  </head>
  <body>
    <h1>~~~~~ Jam the object ~~~~~</h1>
    <p>
      <a href=/startX\">Start</a>/<a href=/stopX\">Stop</a>/<a href=/soloX\">Solo</a> X transmition<br>
      <a href=/startY\">Start</a>/<a href=/stopY\">Stop</a>/<a href=/soloY\">Solo</a> Y transmition<br>
      <a href=/startZ\">Start</a>/<a href=/stopZ\">Stop</a>/<a href=/soloZ\">Solo</a> Z transmition<br>
      <a href=/enableAll\">Enable</a> all transmitions<br>
      <a href=/disableAll\">Disable</a> all transmitions<br>
      <a href=/save\">Save</a> preset<br>
      <a href=/load\">Load</a> preset<br>
    </p>

	<p>
      Pitch: <b><span id="pitch">Loading...</span></b><br> <!-- Insert float here -->
      Roll: <b><span id="roll">Loading...</span></b><br> <!-- Insert float here -->
      Yaw: <b><span id="yaw">Loading...</span></b><br> <!-- Insert float here -->
      Direction: <b><span id="direction">Loading...</span></b><br> <!-- Insert float here -->
      Battery: <b><span id="battery">Loading...</span></b><br> <!-- Insert float here -->
    </p>
  </body>
  </html>

)=====";