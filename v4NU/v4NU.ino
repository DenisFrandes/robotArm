#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

// Configurare WiFi
const char* ssid = "Tenda_E2CC60";
const char* password = "feelagain755";

// Creare obiecte Servo
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
Servo servo6;

// Pinii pentru servomotoare
int servo1Pin = 0;
int servo2Pin = 1;
int servo3Pin = 2;
int servo4Pin = 3;
int servo5Pin = 4;
int servo6Pin = 10;

// Variabile pentru poziția servomotoarelor
int servo1Pos = 90;   // Poziție neutră pentru servo1
int servo2Pos = 100;  // Poziție inițială pentru servo2
int servo3Pos = 57;   // Poziție inițială pentru servo3
int servo4Pos = 170;  // Poziție inițială pentru servo4
int servo5Pos = 90;   // Poziție neutră pentru servo5
int servo6Pos = 22;   // Poziție inițială pentru servo6

// Creare server web
WebServer server(80);

// Pagina HTML cu slider-uri, afișaj unghiuri și buton de oprire
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>Control Brat Robotic</title>
  <style>
    body {
      margin: 0;
      display: flex;
      flex-direction: column;
      height: 100vh; /* Înălțimea paginii */
    }

    h1 {
      text-align: center;
      padding: 20px;
      background-color: #f0f0f0;
      margin: 0;
    }

    .slider {
      width: 100%;
    }

    .angle {
      font-weight: bold;
    }

    .stop-button {
      background-color: red;
      color: white;
      padding: 10px 20px;
      border: none;
      cursor: pointer;
      font-size: 16px;
    }

    .stop-button:hover {
      background-color: darkred;
    }

    .principale {
      display: flex;
      flex: 1; /* Ocupă spațiul rămas */
    }

    .container-imagine {
      display: flex;
      justify-content: left;
      align-items: center;
      background-color: #e0e0e0;
      height: 88vh;
    }

    .container-imagine img {
      height: 100%; /* Înălțimea completă a containerului */
    }

    .container-sliders {
      display: flex;
      flex-direction: column;
      justify-content: space-evenly; /* Distribuie spațiul uniform */
      padding: 20px;
      flex: 1; /* Ocupă jumătate din spațiu */
      background-color: #f9f9f9;
    }

    .container-sliders p {
      margin: 0; /* Elimină marginile implicite ale paragrafului */
    }

    .control-group {
      margin: 20px;
      padding: 10px;
      border: 1px solid #ccc;
      border-radius: 5px;
    }

    .arrow-button {
      width: 60px;
      height: 60px;
      margin: 5px;
      background-size: contain;
      background-repeat: no-repeat;
      border: none;
      cursor: pointer;
    }

    /* Stil pentru grupul de imagini și text */
    .servo-container {
      display: flex;
      align-items: center;
      justify-content: space-between;
      margin-top: 20px;
    }

    .servo-container img {
      width: 60px; /* Dimensiunea imaginilor */
      height: 60px;
    }

    .servo-container p {
      margin: 0 20px; /* Spațiu între text și imagini */
      font-size: 18px;
      font-weight: bold;
    }
  </style>
</head>
<body>
  <h1>Control Brat Robotic <button class="stop-button" onclick="stopRobot()">OFF</button></h1>
  <div class="principale">
    <div class="container-imagine">
      <img src="brat.png" alt="">
    </div>
    <div class="container-sliders">
      <p>Servo 6: <input type="range" min="9" max="85" value="22" class="slider" id="servo6"> <span class="angle" id="angle6">22</span>°</p>
      

      <div class="control-group">
        <div class="servo-container">
          <!-- Imagini în stânga -->
          <div>
            <img src="stanga1.png" alt="Stânga 1" 
                 onmousedown="controlServo5(5, 'fast_left')" 
                 onmouseup="controlServo(5, 'stop')">
            <img src="stanga2.png" alt="Stânga 2" 
                 onmousedown="controlServo5(5, 'slow_left')" 
                 onmouseup="controlServo(5, 'stop')">
          </div>
          <!-- Text în mijloc -->
          <p>Servo 5</p>
          <!-- Imagini în dreapta -->
          <div>
            <img src="dreapta1.png" alt="Dreapta 1" 
                 onmousedown="controlServo(5, 'slow_right')" 
                 onmouseup="controlServo5(5, 'stop')">
            <img src="dreapta2.png" alt="Dreapta 2" 
                 onmousedown="controlServo(5, 'fast_right')" 
                 onmouseup="controlServo5(5, 'stop')">
          </div>
        </div>
      </div>
      <p>Servo 4: <input type="range" min="0" max="180" value="170" class="slider" id="servo4"> <span class="angle" id="angle4">170</span>°</p>
      <p>Servo 3: <input type="range" min="0" max="180" value="57" class="slider" id="servo3"> <span class="angle" id="angle3">57</span>°</p>
      <p>Servo 2: <input type="range" min="0" max="180" value="100" class="slider" id="servo2"> <span class="angle" id="angle2">100</span>°</p>
      <div class="control-group">
        <div class="servo-container">
          <!-- Imagini în stânga -->
          <div>
            <img src="stanga1.png" alt="Stânga 1" 
                 onmousedown="controlServo(1, 'fast_left')" 
                 onmouseup="controlServo(1, 'stop')">
            <img src="stanga2.png" alt="Stânga 2" 
                 onmousedown="controlServo(1, 'slow_left')" 
                 onmouseup="controlServo(1, 'stop')">
          </div>
          <!-- Text în mijloc -->
          <p>Servo 1</p>
          <!-- Imagini în dreapta -->
          <div>
            <img src="dreapta1.png" alt="Dreapta 1" 
                 onmousedown="controlServo(1, 'slow_right')" 
                 onmouseup="controlServo(1, 'stop')">
            <img src="dreapta2.png" alt="Dreapta 2" 
                 onmousedown="controlServo(1, 'fast_right')" 
                 onmouseup="controlServo(1, 'stop')">
          </div>
        </div>
      </div>

    </div>
  </div>

  <script>
    // Funcție pentru a actualiza unghiurile
    function updateAngles() {
      fetch("/angles")
        .then(response => response.json())
        .then(data => {
          document.getElementById("angle1").textContent = data.servo1;
          document.getElementById("angle2").textContent = data.servo2;
          document.getElementById("angle3").textContent = data.servo3;
          document.getElementById("angle4").textContent = data.servo4;
          document.getElementById("angle5").textContent = data.servo5;
          document.getElementById("angle6").textContent = data.servo6;
        });
    }

    // Actualizează unghiurile la fiecare secundă
    setInterval(updateAngles, 1000);

    // Funcție pentru butonul de oprire
    function stopRobot() {
      fetch("/stop")
        .then(response => response.text())
        .then(data => {
          console.log(data); // Afișează răspunsul serverului în consolă
        });
    }

    // Funcții pentru a trimite comenzi la server
    document.getElementById("servo1").oninput = function() {
      fetch("/servo1?value=" + this.value);
    }
    document.getElementById("servo2").oninput = function() {
      fetch("/servo2?value=" + this.value);
    }
    document.getElementById("servo3").oninput = function() {
      fetch("/servo3?value=" + this.value);
    }
    document.getElementById("servo4").oninput = function() {
      fetch("/servo4?value=" + this.value);
    }
    document.getElementById("servo5").oninput = function() {
      fetch("/servo5?value=" + this.value);
    }
    document.getElementById("servo6").oninput = function() {
      fetch("/servo6?value=" + this.value);
    }

    // Funcție pentru controlul servomotorului
    function controlServo(servoNr, direction) {
      let speedValue;
      switch(direction) {
        case 'slow_left': speedValue = 80; break;  // Viteză mică spre stânga
        case 'fast_left': speedValue = 0; break;   // Viteză mare spre stânga
        case 'slow_right': speedValue = 100; break; // Viteză mică spre dreapta
        case 'fast_right': speedValue = 180; break; // Viteză mare spre dreapta
        case 'stop': speedValue = 90; break;       // Oprire
      }
      fetch("/servo" + servoNr + "?value=" + speedValue);
    }
    function controlServo5(servoNr, direction) {
      let speedValue;
      switch(direction) {
        case 'slow_left': speedValue = 80; break;  // Viteză mică spre stânga
        case 'fast_left': speedValue = 0; break;   // Viteză mare spre stânga
        case 'slow_right': speedValue = 100; break; // Viteză mică spre dreapta
        case 'fast_right': speedValue = 180; break; // Viteză mare spre dreapta
        case 'stop': speedValue = 90; break;       // Oprire
      }
      fetch("/servo" + servoNr + "?value=" + speedValue);
    }
  </script>
</body>
</html>
)rawliteral";

void setup() {
  // Inițializare Serial
  Serial.begin(115200);

  // Conectare la WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectare la WiFi...");
  }
  Serial.println("Conectat la WiFi");
  Serial.println(WiFi.localIP());

  // Atasare servomotoare la pini
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  servo3.attach(servo3Pin);
  servo4.attach(servo4Pin);
  servo5.attach(servo5Pin);
  servo6.attach(servo6Pin);

  // Setare poziții inițiale
  servo1.write(servo1Pos);
  servo2.write(servo2Pos);
  servo3.write(servo3Pos);
  servo4.write(servo4Pos);
  servo5.write(servo5Pos);
  servo6.write(servo6Pos);

  // Configurare rute server web
  server.on("/", HTTP_GET, []() {
    server.send_P(200, "text/html", index_html);
  });

  server.on("/servo1", HTTP_GET, []() {
    if (server.hasArg("value")) {
      servo1Pos = server.arg("value").toInt();
      servo1.write(servo1Pos);
    }
    server.send(200, "text/plain", "OK");
  });

  server.on("/servo2", HTTP_GET, []() {
    if (server.hasArg("value")) {
      servo2Pos = server.arg("value").toInt();
      servo2.write(servo2Pos);
    }
    server.send(200, "text/plain", "OK");
  });

  server.on("/servo3", HTTP_GET, []() {
    if (server.hasArg("value")) {
      servo3Pos = server.arg("value").toInt();
      servo3.write(servo3Pos);
    }
    server.send(200, "text/plain", "OK");
  });

  server.on("/servo4", HTTP_GET, []() {
    if (server.hasArg("value")) {
      servo4Pos = server.arg("value").toInt();
      servo4.write(servo4Pos);
    }
    server.send(200, "text/plain", "OK");
  });

  server.on("/servo5", HTTP_GET, []() {
    if (server.hasArg("value")) {
      servo5Pos = server.arg("value").toInt();
      servo5.write(servo5Pos);
    }
    server.send(200, "text/plain", "OK");
  });

  server.on("/servo6", HTTP_GET, []() {
    if (server.hasArg("value")) {
      servo6Pos = server.arg("value").toInt();
      servo6.write(servo6Pos);
    }
    server.send(200, "text/plain", "OK");
  });

  // Rută pentru a obține unghiurile curente
  server.on("/angles", HTTP_GET, []() {
    String json = "{";
    json += "\"servo1\":" + String(servo1.read()) + ",";
    json += "\"servo2\":" + String(servo2.read()) + ",";
    json += "\"servo3\":" + String(servo3.read()) + ",";
    json += "\"servo4\":" + String(servo4.read()) + ",";
    json += "\"servo5\":" + String(servo5.read()) + ",";
    json += "\"servo6\":" + String(servo6.read());
    json += "}";
    server.send(200, "application/json", json);
  });

  // Rută pentru butonul de oprire
  server.on("/stop", HTTP_GET, []() {
    // Mută servomotoarele în pozițiile de oprire cu delay
    servo2.write(100);
    delay(500); // Delay de 500ms
    servo3.write(57);
    delay(500); // Delay de 500ms
    servo4.write(170);
    delay(500); // Delay de 500ms
    servo6.write(22);
    delay(500); // Delay de 500ms

    server.send(200, "text/plain", "Robot oprit");
  });

  // Pornire server
  server.begin();
}

void loop() {
  server.handleClient();
}