#include <WiFi.h>

const char* ssid = "RobotCar";
const char* password = "12345678";

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {

    String request = client.readStringUntil('\r');
    client.flush();

    char cmd = parseCommand(request);
    if (cmd != 0) {
      Serial.write(cmd);   // Send to Arduino
    }

    // ===== SEND WEB PAGE =====
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();

    client.println("<!DOCTYPE html><html><head>");
    client.println("<meta name='viewport' content='width=device-width, initial-scale=1.0'>");

    client.println("<style>");
    client.println("body{display:flex;flex-direction:column;justify-content:center;align-items:center;height:100vh;background:#222;margin:0;color:white;font-family:sans-serif;}");

    // ===== MODE BUTTONS =====
    client.println(".modeBox{display:flex;gap:15px;margin-bottom:30px;}");
    client.println(".modeBtn{padding:12px 20px;border:none;border-radius:12px;font-size:16px;background:#444;color:white;}");
    client.println(".active{background:#00BCD4;}");

    // ===== D-PAD =====
    client.println(".dpad{display:grid;grid-template-columns:110px 110px 110px;");
    client.println("grid-template-rows:110px 110px 110px;gap:10px;}");

    client.println(".btn{width:110px;height:110px;border:none;border-radius:20px;");
    client.println("user-select:none;-webkit-user-select:none;");
    client.println("-webkit-tap-highlight-color:transparent;touch-action:manipulation;}");

    client.println(".F{background:#4CAF50;}");
    client.println(".B{background:#f44336;}");
    client.println(".L{background:#2196F3;}");
    client.println(".R{background:#FF9800;}");
    client.println(".S{background:#9E9E9E;}");

    // ===== SERVO BUTTONS =====
    client.println(".servoBox{margin-top:30px;display:flex;gap:20px;}");
    client.println(".servoBtn{width:70px;height:70px;border:none;border-radius:15px;");
    client.println("background:#673AB7;}");

    client.println("</style></head><body>");

    // ===== MODE SELECTOR =====
    client.println("<div class='modeBox'>");
    client.println("<button class='modeBtn active' onclick=\"setMode('M',this)\">Manual</button>");
    client.println("<button class='modeBtn' onclick=\"setMode('A',this)\">Auto</button>");
    client.println("<button class='modeBtn' onclick=\"setMode('H',this)\">Scan</button>");
    client.println("</div>");

    // ===== D-PAD =====
    client.println("<div class='dpad'>");

    client.println("<div></div><button class='btn F' onpointerdown=\"send('F')\" onpointerup=\"send('S')\"></button><div></div>");

    client.println("<button class='btn L' onpointerdown=\"send('L')\" onpointerup=\"send('S')\"></button>");
    client.println("<button class='btn S' onclick=\"send('S')\"></button>");
    client.println("<button class='btn R' onpointerdown=\"send('R')\" onpointerup=\"send('S')\"></button>");

    client.println("<div></div><button class='btn B' onpointerdown=\"send('B')\" onpointerup=\"send('S')\"></button><div></div>");

    client.println("</div>");

    // ===== SERVO BUTTONS =====
    client.println("<div class='servoBox'>");
    client.println("<button class='servoBtn' onclick=\"send('U')\"></button>");
    client.println("<button class='servoBtn' onclick=\"send('C')\"></button>");
    client.println("<button class='servoBtn' onclick=\"send('D')\"></button>");
    client.println("</div>");

    // ===== JAVASCRIPT =====
    client.println("<script>");
    client.println("function send(c){fetch('/'+c).catch(e=>console.log(e));}");
    client.println("function setMode(c,btn){");
    client.println("document.querySelectorAll('.modeBtn').forEach(b=>b.classList.remove('active'));");
    client.println("btn.classList.add('active');");
    client.println("send(c);");
    client.println("}");
    client.println("</script>");

    client.println("</body></html>");
    client.stop();
  }
}

char parseCommand(String req) {
  int idx = req.indexOf('/');
  if (idx >= 0 && req.length() > idx + 1) {
    char c = req.charAt(idx + 1);
    if (c=='F'||c=='B'||c=='L'||c=='R'||c=='S'||c=='U'||c=='D'||c=='C'||c=='M'||c=='A'||c=='H')
      return c;
  }
  return 0;
}