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
int servo1Pos = 90;
int servo2Pos = 90;
int servo3Pos = 90;
int servo4Pos = 90;
int servo5Pos = 90;
int servo6Pos = 90;

// Creare server web
WebServer server(80);

// Pagina HTML cu slider-uri, afișaj unghiuri și buton de oprire
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>Control Brat Robotic</title>
  <style>
    .slider { width: 100%; }
    .angle { font-weight: bold; }
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
  </style>
</head>
<body>
  <h1>Control Brat Robotic <button class="stop-button" onclick="stopRobot()">OFF</button></h1>
  <p>Servo 1: <input type="range" min="0" max="360" value="90" class="slider" id="servo1"> <span class="angle" id="angle1">90</span>°</p>
  <p>Servo 2: <input type="range" min="0" max="180" value="100" class="slider" id="servo2"> <span class="angle" id="angle2">90</span>°</p>
  <p>Servo 3: <input type="range" min="0" max="180" value="57" class="slider" id="servo3"> <span class="angle" id="angle3">90</span>°</p>
  <p>Servo 4: <input type="range" min="0" max="180" value="170" class="slider" id="servo4"> <span class="angle" id="angle4">90</span>°</p>
  <p>Servo 5: <input type="range" min="0" max="360" value="90" class="slider" id="servo5"> <span class="angle" id="angle5">90</span>°</p>
  <p>Servo 6: <input type="range" min="0" max="180" value="22" class="slider" id="servo6"> <span class="angle" id="angle6">90</span>°</p>
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
    // Mută servomotoarele în pozițiile specificate cu delay
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