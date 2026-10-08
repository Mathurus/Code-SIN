// This is a code for station mode



#include <WiFi.h> 
#include <WebServer.h>

const char* ssid = "******************"; //configuration de la connexion au wifi 
const char* passwd = "****************";

WebServer server(80); //démarrage du server wbe sur le port 80 

void handleRoot() {                           //fonction sur le html en local 
    int adcValue = analogRead(1);                       //lecture analogique 
    double voltage = (float)adcValue / 4095.0 * 3.3;    //conversion pour trouver la tension                 
    double Rt = 10 * voltage / (3.3 - voltage);         //conversion pour trouver le nombre                      
    double tempK = 1 / (1 / (273.15 + 25) + log(Rt / 10) / 3950.0);   //conversion pour trouver la température en kelvin 
    double temp = tempK - 273.15;     //soustraction pour remettre en °C
    String html = R"rawliteral(       
<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">

    <title>ESP Capteur</title>

    <style>
        * {
            box-sizing: border-box;
            margin: 0;
            padding: 0;
        }

        body {
            font-family: Arial, Helvetica, sans-serif;
            min-height: 100vh;
            display: flex;
            justify-content: center;
            align-items: center;

            background: linear-gradient(
                135deg,
                #0f172a,
                #1e3a8a,
                #2563eb
            );

            color: white;
        }

        .container {
            width: 90%;
            max-width: 450px;
        }

        .card {
            background: rgba(255, 255, 255, 0.12);
            backdrop-filter: blur(12px);
            -webkit-backdrop-filter: blur(12px);

            border: 1px solid rgba(255, 255, 255, 0.2);
            border-radius: 25px;

            padding: 35px;

            text-align: center;

            box-shadow: 0 20px 50px rgba(0, 0, 0, 0.3);
        }

        .icon {
            font-size: 50px;
            margin-bottom: 15px;
        }

        h1 {
            font-size: 30px;
            margin-bottom: 8px;
        }

        .subtitle {
            color: #cbd5e1;
            font-size: 15px;
            margin-bottom: 30px;
        }

        .temperature {
            background: rgba(255, 255, 255, 0.12);
            border-radius: 20px;
            padding: 25px;
            margin-bottom: 25px;
        }

        .temperature-label {
            color: #cbd5e1;
            font-size: 14px;
            text-transform: uppercase;
            letter-spacing: 2px;
            margin-bottom: 10px;
        }

        .temperature-value {
            font-size: 55px;
            font-weight: bold;
            color: #ffffff;
        }

        .unit {
            font-size: 25px;
            color: #93c5fd;
        }

        .status {
            display: inline-flex;
            align-items: center;
            gap: 8px;

            background: rgba(34, 197, 94, 0.15);
            color: #86efac;

            padding: 8px 15px;
            border-radius: 50px;

            font-size: 14px;
        }

        .status-dot {
            width: 9px;
            height: 9px;
            background: #22c55e;
            border-radius: 50%;

            box-shadow: 0 0 10px #22c55e;
        }

        footer {
            margin-top: 25px;
            color: #94a3b8;
            font-size: 12px;
        }

        @media (max-width: 500px) {
            .card {
                padding: 25px;
            }

            h1 {
                font-size: 26px;
            }

            .temperature-value {
                font-size: 45px;
            }
        }
    </style>
</head>

<body>

    <div class="container">

        <div class="card">

            <div class="icon">🌡️</div>

            <h1>ESP CAPTEUR</h1>

            <p class="subtitle">
                Surveillance de la température
            </p>

            <div class="temperature">

                <div class="temperature-label">
                    Température actuelle
                </div>

                <div class="temperature-value">
                    )rawliteral";

html += String(temp);

html += R"rawliteral(
                    <span class="unit">°C</span>
                </div>

            </div>

            <div class="status">
                <span class="status-dot"></span>
                Capteur connecté
            </div>

            <footer>
                ESP • Monitoring en temps réel
            </footer>

        </div>

    </div>

</body>
</html>
)rawliteral";       //Page html en local 

    server.send(200, "text/html", html);    //envoie de la page html
}

void handleData() {
    int adcValue = analogRead(1);                       //lecture analogique 
    double voltage = (float)adcValue / 4095.0 * 3.3;    //conversion pour trouver la tension              
    double Rt = 10 * voltage / (3.3 - voltage);         //conversion pour trouver le nombre            
    double tempK = 1 / (1 / (273.15 + 25) + log(Rt / 10) / 3950.0);     //conversion pour trouver la température en kelvin 
    double temp = tempK - 273.15;                        //soustraction pour remettre en °C
    String json = "{\"Temperature\":" + String(temp, 2) + "}";     //configuration du json 
    server.send(200, "application/json", json);         //envoie du json 
}

void setup() {
    Serial.begin(115200);         //configuration de la borne passante 
    WiFi.begin(ssid, passwd);     //connection au wifi 
    while (WiFi.status() != WL_CONNECTED) {   //vérification si le wifi est bien connecter 
        Serial.print(".");                    //affichage d'un point si il n'est pas connecter 
        delay(500);
    }
    Serial.println(String("\nConnect to the wifi : ") + ssid);      //information du wifi 
    Serial.println(WiFi.localIP());                       //affichage dans le moniteur de série de l'ip 
    server.on("/", handleRoot);                           //création de la page d'acceil 
    server.on("/data", handleData);                       //création de la page /data
    server.begin();                                       //lancement du serveur
}

void loop() {
    int adcValue = analogRead(1);                       
    double voltage = (float)adcValue / 4095.0 * 3.3;                
    double Rt = 10 * voltage / (3.3 - voltage);                     
    double tempK = 1 / (1 / (273.15 + 25) + log(Rt / 10) / 3950.0); 
    double temp = tempK - 273.15;                                  
    Serial.printf("ADC value : %d,\tVoltage : %.2fV, \tTemperature : %.2fC\n", adcValue, voltage, temp);      //afficher les informations dans le moniteurs de série 
    server.handleClient();                                                                                    //lancer le serveur 
    delay(1000);      //mettre un delay 
}
