#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

//Configuracion Wi-Fi
const char wifissid[] = "Wifil";
const char wifipsk[] = "123456789";

//LED Definitions
const int LED_PIN = 16; //The one onboard
const int LED_PIN2 = 14;//El GPIO14
//

const int ANALOG_PIN = A0; //Analog pin
const int DIGITAL_PIN = 12; //Digitial pin to be read

WiFiServer server(80);


void setup() {
  // put your setup code here, to run once:
  initHardWare();
  connectWiFi();
  server.begin();
  setupMDNS();
}



//· ····························· Mètodo Principal ·······································
void loop() {
  // Check if connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  //Read the first line of the req.
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  //Match the request
  int vol = -1;
  int vol2 = -1;

  if (req.indexOf("/led/0") != -1)
     vol = 0;
  else if (req.indexOf("/led/1") != -1)
     vol = 1;
   else if (req.indexOf("/led/3")!=-1)
   vol2=0; 
  else if(req.indexOf("/led/4")!=-1)
   vol2=1;

  else if (req.indexOf("/read") != -1)
     vol = -2;


  //Set GPIOS according to the req.
  if (vol >= 0 ){
     digitalWrite(LED_PIN, vol);
    }
    if (vol2 >=0) {
      digitalWrite(LED_PIN2,vol2);
    }
     

   client.flush();

   //Prepare html response

   String s = "HTTP/1.1 200 OK\r\n";
   s += "Content Type: text/html\r\n\r\n";
   s += "<!DOCTYPE HTML>\r\n<html>\r\n";

   //Print a message
   if (vol >= 0) {
    s += "Se encuentra: ";
    s += (vol)?"Encendido":"Apagado";
    s+= "<\br>";
    
   }else   if (vol2 >= 0) {

    s += "Se encuentra: ";
    s += (vol2)?"Encendido":"Apagado";
    
   }

   else if (vol == -2) {
    s += "Analog PIN = ";
    s += String(analogRead(ANALOG_PIN));
    s += "<br>";
    s += "Digital PIN 12 = ";
    s += String(digitalRead(DIGITAL_PIN));
   }   else if (vol2 == -2) {
    s += "Analog PIN = ";
    s += String(analogRead(ANALOG_PIN));
    s += "<br>";
    s += "Digital PIN 12 = ";
    s += String(digitalRead(DIGITAL_PIN));
   }
   else {
    s += "Invalid Request. <br> Try /led/1, /led/0 or /read";
   }
   s += "</html>\n";

   //send response to the client
   client.print(s);
   delay(1);
   Serial.println("Client disconnected");

}
// ······················· Fin del método principal



void connectWiFi() {
  byte ledStatus = LOW;
  Serial.println();
  Serial.println("Connecting to: " + String(wifissid));
  WiFi.mode(WIFI_STA);

  WiFi.begin(wifissid,wifipsk);

  while (WiFi.status() != WL_CONNECTED) {
    //Blink the LED
    digitalWrite(LED_PIN, ledStatus);
    ledStatus = (ledStatus == LOW) ? LOW : HIGH;

    delay(100);
  }
  Serial.println("WiFi Connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(16, LOW);
  
}

void setupMDNS()
{
  //Call mdns to setup mdns to pint to domain.local
  if (!MDNS.begin("thing"))
  {
    Serial.println("Error setting up MDNS responder!");
    while(1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");
}


//--- Configuración del sistema
void initHardWare()
{
  Serial.begin(115200);
  pinMode(DIGITAL_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_PIN2,OUTPUT);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(LED_PIN2,LOW);
}

