#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

//Configuracion Wi-Fi
const char wifissid[] = "Wifil";
const char wifipsk[] = "123456789";

//LED Definitions
const int LED_PIN = 16; //The one onboard
const int LED_PIN2 = 5;//El GPIO1
const int LED_PIN3 = 4;// EL GPIO2
const int LED_PIN4 = 2;// LA ALARMA EN EL d4


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
          
            //Cada vol representa a un led ;) ;)
            int vol = -1;
            int vol2 = -1;
            int vol3 = -1;

            //Colocamos la parte de la alarma
            int vol4 = -1;

            //Declaracion de los diferentes estados de la puerta
            //Se declara una entrada digital
            int volX = -1;
            int valor = digitalRead(DIGITAL_PIN);
          
          
          //led 1
            if (req.indexOf("/led/0") != -1)
               vol = 0;
            else if (req.indexOf("/led/1") != -1)
               vol = 1;
          
          //led 2
             else if (req.indexOf("/led/3")!=-1)
             vol2=0; 
            else if(req.indexOf("/led/4")!=-1)
             vol2=1;
          
          
          //led 3
              else if (req.indexOf("/led/5")!=-1)
             vol3=0; 
            else if(req.indexOf("/led/6")!=-1)
             vol3=1;

          //Parte de la puertta
              else if (req.indexOf("/door") !=-1) {
                volX = 1;
              }

          // Parte de la Alarma
              else if (req.indexOf("/a")) {
                vol4 = 1;
              }else if (req.indexOf("/b")) {
                vol4 = 0; 
              }

                
              
          
          //De lo contrario ninguno jejejejXDXDXDXD
            else if (req.indexOf("/read") != -1){
              vol = -2;
              vol2 = -2;
              vol3 = -2;
              vol4 = -2;
              
            }
               
          
          
          
            //De acuerdo a la respuesta que me dee el servidor, encenderé el led
            if (vol >= 0 ){
               digitalWrite(LED_PIN, vol);
              }
              if (vol2 >=0) {
                digitalWrite(LED_PIN2,vol2);
              }
              if(vol3 >=0){
                digitalWrite(LED_PIN3,vol3);
              }
              if (vol4 >=0) {
                digitalWrite(LED_PIN4,vol4);
              }
          
               
          
             client.flush();
          
             //Prepare html response
          
             String s = "HTTP/1.1 200 OK\r\n";
             s += "Content Type: text/html\r\n\r\n";
             s += "<!DOCTYPE HTML>\r\n<html>\r\n";
             s+="<body>";
              s+="<style> body{background-color:#FFF6F2;} p{color:#26BD2B;} section{color:#FF1414;} aside{color:#226882; font-size:0.4em;}</style>";
             //Print a message
             if (vol >= 0) {
              s += "<h5>Se encuentra: </h5>";
              s += (vol)?"<p>Encendido</p>":"<section>Apagado</section>";
             }

             else   if (vol2 >= 0) {
          
              s += "<h5>Se encuentra:</h5>";
              s += (vol2)?"<p>Encendido</p>":"<section>Apagado</section>";
              
             } 

             else  if (vol3 >=0) {
              s += "<h5>Se encuentra: </h5>";
              s += (vol3)?"<p>Encendido</p>":"<section>Apagado</section>";
             } 

             else if (vol4 >=0) {
              s += "<h5>La alarma se encuentra: </h5>";
              s += (vol3)?"<p>Encendida</p>":"<section>Apagada</section>";
             }
            //Entra la parte de la puerta
             else if (volX >=0 ) {

                 if (valor == HIGH) {
                    s += "Habierta";
                    client.flush();
                 }
                 if (valor == LOW) {
                    s += "Cerrada";
                    client.flush();
                 }
            }
            //Fin de la parte de la puerta
          
             else if (vol == -2) {
              s += "<h5>Todo esta bien</h5>";
              s +="<hr>";
              s +="<aside>Enciende/Apaga</aside>";
              s += "<br>";
             } else if (vol2 == -2) {
              s += "<h5>Todo esta bien</h5>";
              s +="<hr>";
              s +="<aside>Enciende/Apaga</aside>";
              s += "<br>";
              }
              else if (vol3 == -2) {
              s += "<h5>Todo esta bien</h5>";
              s +="<hr>";
              s +="<aside>Enciende/Apaga</aside>";
              s += "<br>";
             }
             else {
              s += "Invalid Request. <br> Try /led/1, /led/0 or /read";
             }
            s+="</body>";
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
                  WiFi.mode(WIFI_AP);
                
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
                  //Este sera el de la puerta
                  pinMode(DIGITAL_PIN, INPUT_PULLUP);
                
                  pinMode(LED_PIN, OUTPUT);
                  pinMode(LED_PIN2,OUTPUT);
                  pinMode(LED_PIN3,OUTPUT);
                  pinMode(LED_PIN4,OUTPUT);
                  
                 
                  digitalWrite(LED_PIN, LOW);
                  digitalWrite(LED_PIN2,LOW);
                  digitalWrite(LED_PIN3, LOW);
                  digitalWrite(LED_PIN4,LOW);
                }
