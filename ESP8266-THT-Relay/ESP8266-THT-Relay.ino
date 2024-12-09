#include "ESP8266WiFi.h"
#include "ESPAsyncWebServer.h"
#include <DHT.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"


// Set to true to define Relay as Normally Open (NO)
#define RELAY_NO    true

// Set number of relays
#define NUM_RELAYS  5

// Assign each GPIO to a relay
int relayGPIOs[NUM_RELAYS] = {15, 4, 14, 12, 13};

// Wi-Fi credentials
const char* ssid = "OPPO A73";
const char* password = "estai123";

// Firebase credentials
#define API_KEY "AIzaSyDe4tK1DS3q8MTnbUE3bZXRz3kUwpB652c"
#define DATABASE_URL "https://esp8266-a-8b851-default-rtdb.firebaseio.com/"  // Replace with your RTDB URL
// Firebase Authentication Credentials
#define USER_EMAIL "haninasri14@gmail.com"  // Replace with your Firebase Auth email
#define USER_PASSWORD "estai123"           // Replace with your Firebase Auth password

// Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
String userUID = "KzxtTF6QydTGBI97GGmTwfvUepD3";  // Variable to store user UID

// DHT sensor configuration
#define DHTPIN 2       // Pin connected to the DHT sensor
#define DHTTYPE DHT11  // DHT 11 sensor
DHT dht(DHTPIN, DHTTYPE);

// WiFi initialization
void initWiFi() {
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: " + WiFi.localIP().toString());
}

// Firebase initialization
void initFirebase() {
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.token_status_callback = tokenStatusCallback;  // Defined in TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  if (Firebase.ready()) {
    //userUID = auth.token.uid;
    Serial.println("Firebase authentication successful");
    Serial.println("User UID: " + userUID);
  } else {
    Serial.println("Firebase authentication failed");
  }
}

// Get relay state for web page
String relayState(int numRelay) {
  if(RELAY_NO) {
    if(digitalRead(relayGPIOs[numRelay-1])) {
      return "";
    }
    else {
      return "checked";
    }
  } else {
    if(digitalRead(relayGPIOs[numRelay-1])) {
      return "checked";
    }
    else {
      return "";
    }
  }
}

// Process HTML placeholders
String processor(const String& var) {
  if (var == "BUTTONPLACEHOLDER") {
    String buttons = "";
    for (int i = 1; i <= NUM_RELAYS; i++) {
      String relayStateValue = relayState(i);
      buttons += "<h4>Relay #" + String(i) + " - GPIO " + String(relayGPIOs[i-1]) + "</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"" + String(i) + "\" " + relayStateValue + "><span class=\"slider\"></span></label>";
    }
    return buttons;
  }
  return String();
}

// Send DHT data to Firebase
void sendDHTData() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  String tempPath = "/users/" + userUID + "/temperature";
  String humPath = "/users/" + userUID + "/humidity";

  if (Firebase.RTDB.setFloat(&fbdo, tempPath.c_str(), temperature)) {
    Serial.println("Temperature updated");
  } else {
    Serial.println("Failed to update temperature: " + fbdo.errorReason());
  }

  if (Firebase.RTDB.setFloat(&fbdo, humPath.c_str(), humidity)) {
    Serial.println("Humidity updated");
  } else {
    Serial.println("Failed to update humidity: " + fbdo.errorReason());
  }
}

// Monitor Firebase for relay state changes
void monitorRelayStates() {
// Monitor relay states from Firebase in real-time
  for (int i = 1; i <= NUM_RELAYS; i++) {
    String relayPath = "/users/" + userUID + "/relay" + String(i);

    // Get the integer value from Firebase to control the relay
    if (Firebase.RTDB.getInt(&fbdo, relayPath.c_str())) {
      // Check if the data is valid
      int relayState = fbdo.intData();
      if (relayState == 1) {
        digitalWrite(relayGPIOs[i-1], HIGH);  // Turn ON relay
        Serial.print("Relay ");
        Serial.print(i);
        Serial.println(" is ON");
      } else {
        digitalWrite(relayGPIOs[i-1], LOW);   // Turn OFF relay
        Serial.print("Relay ");
        Serial.print(i);
        Serial.println(" is OFF");
      }
    } else {
      Serial.println("Failed to read relay state from Firebase.");
    }
  }
}



void setup() {
  Serial.begin(115200);

  // Initialize relay pins
  for (int i = 1; i <= NUM_RELAYS; i++) {
    pinMode(relayGPIOs[i-1], OUTPUT);
    if (RELAY_NO) {
      digitalWrite(relayGPIOs[i-1], HIGH);
    } else {
      digitalWrite(relayGPIOs[i-1], LOW);
    }
  }

  // Initialize DHT sensor
  dht.begin();

  // Connect to Wi-Fi
  initWiFi();

  // Initialize Firebase
  initFirebase();

}

void loop() {
  // Periodically send DHT data to Firebase
  sendDHTData();
  
  // Monitor Firebase for relay state changes
  monitorRelayStates();
  
  delay(1000);  // Adjust the delay as needed
}
