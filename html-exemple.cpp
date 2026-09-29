#include <WiFi.h>
#include <WebServer.h>

// Vos identifiants Wi-Fi
const char* ssid = "VOTRE_NOM_WIFI";
const char* password = "VOTRE_MOT_DE_PASSE";

WebServer server(80);

// --- TOUTE VOTRE PAGE WEB EST STOCKÉE ICI EN MÉMOIRE FLASH ULTRA-RAPIDE ---
const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <title>ESP32 Ultra Rapide</title>
    <style>
        /* Le CSS est intégré ici : Pas de fichier séparé */
        body { font-family: Arial, sans-serif; background: #f0f2f5; text-align: center; margin-top: 50px; }
        .card { background: white; padding: 300px; display: inline-block; border-radius: 10px; box-shadow: 0 4px 8px rgba(0,0,0,0.1); }
        .btn { background: #007bff; color: white; border: none; padding: 10px 20px; font-size: 16px; border-radius: 5px; cursor: pointer; }
        .btn:hover { background: #0056b3; }
    </style>
</head>
<body>

    <div class="card">
        <h1>Mon ESP32 Local</h1>
        <p>Cette page se charge instantanément car elle est en mémoire Flash.</p>
        <button class="btn" onclick="maAction()">Lancer une requête</button>
    </div>

    <script>
        // Le JavaScript est intégré ici : Pas de fichier séparé
        function maAction() {
            alert("Requête envoyée instantanément !");
            // C'est ici que vous mettriez un fetch() ou un XMLHttpRequest si besoin
        }
    </script>

</body>
</html>
)rawliteral";

// --- GESTION DES REQUÊTES EN C++ ---

void handleRoot() {
  // server.send_P est spécifique pour envoyer les données depuis la mémoire Flash (PROGMEM) à vitesse maximale
  server.send_P(200, "text/html", INDEX_HTML);
}

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println("\nConnecté ! Adresse IP : " + WiFi.localIP().toString());

  // Quand vous tapez l'adresse IP, l'ESP32 exécute la fonction handleRoot
  server.on("/", handleRoot);
  
  server.begin();
}

void loop() {
  server.handleClient();
}
