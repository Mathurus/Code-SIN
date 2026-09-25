// This is a code for station mode



#include <WiFi.h> 
#include <WebServer.h>

const char* ssid = "**********"; 
const char* passwd = "**********";

WebServer server(80);

void handleRoot() {
    int adcValue = analogRead(PIN_ANALOG_IN);                       
    double voltage = (float)adcValue / 4095.0 * 3.3;                
    double Rt = 10 * voltage / (3.3 - voltage);                     
    double tempK = 1 / (1 / (273.15 + 25) + log(Rt / 10) / 3950.0); 
    double temp = tempK - 273.15;
    String html = "<html><body><h1>ESP CAPTEUR</h1>";
    html += "<p>Temperature : " + String(temp) + " °C </p>";
    html += "</body></html>";
    server.send(200, "text/html", html);
}

void handleData() {
    int adcValue = analogRead(PIN_ANALOG_IN);                       
    double voltage = (float)adcValue / 4095.0 * 3.3;                
    double Rt = 10 * voltage / (3.3 - voltage);                     
    double tempK = 1 / (1 / (273.15 + 25) + log(Rt / 10) / 3950.0); 
    double temp = tempK - 273.15;
    int json = {"Temperature" : temp};
    String json = "{\"Temperature\":" + String(temp, 2) + "}";
    server.send(200, "application/json", json);
}

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, passwd);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.println("\nConnect to the wifi : " + ssid);
    Serial.println(WiFi.localIP());
    server.on("/", handleRoot);
    server.on("/data", handleData);
    server.begin();
}

void loop() {
    int adcValue = analogRead(PIN_ANALOG_IN);                       
    double voltage = (float)adcValue / 4095.0 * 3.3;                
    double Rt = 10 * voltage / (3.3 - voltage);                     
    double tempK = 1 / (1 / (273.15 + 25) + log(Rt / 10) / 3950.0); 
    double temp = tempK - 273.15;                                  
    Serial.printf("ADC value : %d,\tVoltage : %.2fV, \tTemperature : %.2fC\n", adcValue, voltage, temp);
    server.handleClient();
}
