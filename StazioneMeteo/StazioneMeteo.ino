#include <AirQualityClass.h>
#include <Arduino_MKRIoTCarrier.h>
#include <Arduino_MKRIoTCarrier_Buzzer.h>
#include <Arduino_MKRIoTCarrier_Qtouch.h>
#include <Arduino_MKRIoTCarrier_Relay.h>
#include <EnvClass.h>
#include <IMUClass.h>
#include <MKRIoTCarrierDefines.h>
#include <PressureClass.h>

File myFile;  //inizializzo il file in cui voglio scrivere e salvare i dati

MKRIoTCarrier carrier;

const int sensoreUmiditaTerreno = A5; //indica il pin del sensore

float temperature;
float humidity;
float pressure;
float soilMoisture;

const int interval = 30000;  //tempo tra le rilevazioni
const int tOff = 20000;  //tempo in cui l'arduino rimane spento
const int tShow = 700;  //tempo in cui si stampa a schermo le misure
unsigned long previousMillis = 0; //ultima misurazione di tempo
unsigned long currentMillis;  //misurazione attuale del tempo, viene usata principalmente nel main
WiFiClient client = 0;  //inizializza la libreria del client

#include <WiFiNINA.h>

char ssid[] = "f-factory";             //  your network SSID (name) between the " "
char pass[] = "Z89sn30Pfuq+s9";      // your network password between the " "
int keyIndex = 0;                 // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;      //connection status
WiFiServer server(80);            //server socket




void setup() {
  Serial.begin(9600);   //inizializza la seriale
  carrier.begin();      //inizializza il cacchetto dell'oggetto
  pinMode(sensoreUmiditaTerreno, INPUT);  //inizializza il pin A5 per il sensore di umidità del terreno

  while (!Serial);    //finche non trova la seriale non inizia
  
  enable_WiFi();
  connect_WiFi();

  server.begin();
  printWifiStatus();

}


void loop() {
  
  client = server.available();
  realmain();
  //freetry();

}

void realmain() { 

   currentMillis = millis();    //aggiorno il tempo corrente
    if (currentMillis - previousMillis >= interval) {   //se la differenza di tempo supera il tempo di intervallo impostato, entra nelle graffe
      measuring();    //la funzione misura e aggiorna le variabili
      write();        //la funzione scrive sul terminale e sullo schermo le variabili
      previousMillis = currentMillis;   //aggiorna il tempo della misurazione
    }
  if(client)  {   //se il cliente è presente connesso alla pagina/server
    printWEB();   //stampo a schermo
  }
    
}

//modalità di prova con i bottoni per testare le singole funzioni
void freetry() {
  
  measuring();

  client = server.available();
  if(client)  {
    printWEB();

      carrier.Buttons.update();

    if(carrier.Button1.onTouchDown()) {
        closed();
    }
    if(carrier.Button2.onTouchDown()) {
        write();
    }
  } else  {

  }
}



void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");

  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}

void enable_WiFi() {
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }
}

void connect_WiFi() {
  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
}

String dati = "";
void printWEB() {
  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {

            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
           
            //create the buttons
            client.print("Click <a href=\"/H\">here</a> refresh<br>");
            //client.print("Click <a href=\"/L\">here</a> turn the LED off<br><br>");
            
            int randomReading = analogRead(A1);
            //client.print(randomReading);
            dati += CSVtoString() + "<br>";
            client.print(dati);
           
            

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          }
          else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}



//scrive la temperatura sullo schermino
void printTemperature() {
  carrier.display.fillScreen(ST77XX_RED);
  carrier.display.setTextColor(ST77XX_BLACK);
  carrier.display.setTextSize(2);

  carrier.display.setCursor(50, 100);
  carrier.display.print("Temperatura: ");
  carrier.display.setCursor(75, 120);
  carrier.display.print(temperature);
  carrier.display.print(" C");
}

//scrive l'umidità sullo schermino
void printHumidity()  {
  carrier.display.fillScreen(ST77XX_BLUE);
  carrier.display.setTextColor(ST77XX_WHITE);
  carrier.display.setTextSize(2);

  carrier.display.setCursor(65, 100);
  carrier.display.print("Umidita': ");
  carrier.display.setCursor(75, 120);
  carrier.display.print(humidity);
  carrier.display.print(" %");
}

//scrive la pressione sullo schermino
void printPressure()  {
  carrier.display.fillScreen(ST77XX_GREEN);
  carrier.display.setTextColor(ST77XX_MAGENTA);
  carrier.display.setTextSize(2);

  carrier.display.setCursor(40, 100);
  carrier.display.print("pressione atm: ");
  carrier.display.setCursor(65, 120);
  carrier.display.print(pressure);
  carrier.display.print(" Pa");
}

//scrive l'umidità del suolo sullo schermino
void printSoilMoisture()  {
  carrier.display.fillScreen(ST77XX_ORANGE);
  carrier.display.setTextColor(ST77XX_BLUE);
  carrier.display.setTextSize(2);

  carrier.display.setCursor(20, 100);
  carrier.display.print("umidita' terreno: ");
  carrier.display.setCursor(70, 120);
  carrier.display.print(soilMoisture);
  carrier.display.print(" %");
}


//mette i led gialli quando misura
void measuring() {
  carrier.leds.clear();
  carrier.leds.setBrightness(20);
  //carrier.leds.setPixelColor(index, red, green, blue);
  carrier.leds.setPixelColor(0,255,255,0);
  carrier.leds.setPixelColor(1,255,255,0);
  carrier.leds.setPixelColor(2,255,255,0);
  carrier.leds.setPixelColor(3,255,255,0);
  carrier.leds.setPixelColor(4,255,255,0);

  carrier.leds.show();

  carrier.display.fillScreen(ST77XX_YELLOW);
  carrier.display.setTextColor(ST77XX_BLACK);
  carrier.display.setTextSize(2);

  carrier.display.setCursor(25, 110);
  carrier.display.println("sta misurando...");

  temperature = carrier.Env.readTemperature();
  humidity = carrier.Env.readHumidity();
  pressure = carrier.Pressure.readPressure();
  soilMoisture = map(analogRead(sensoreUmiditaTerreno),0,1023,0,100);
}

//spegne i led quando è fermo
void closed() {
  carrier.leds.clear();
  carrier.leds.show();

  carrier.display.fillScreen(ST77XX_BLACK);
  carrier.display.setTextColor(ST77XX_WHITE);
  carrier.display.setTextSize(2);

  carrier.display.setCursor(80, 110);
  carrier.display.println("closed");

  delay(tOff);
}

//mette i led a rosso quando scrive
void write() {

  serialWrite();
  displayWrite();
  fileWrite();
  
}

//scrive tutti i dati sullo schermino
void displayWrite() {
  carrier.leds.clear();

  carrier.leds.setPixelColor(0,255,0,0);
  carrier.leds.setPixelColor(1,255,0,0);
  carrier.leds.setPixelColor(2,255,0,0);
  carrier.leds.setPixelColor(3,255,0,0);
  carrier.leds.setPixelColor(4,255,0,0);
  carrier.leds.show();

  

    printTemperature();
    delay(tShow);
    printHumidity();
    delay(tShow);
    printPressure();
    delay(tShow);
    printSoilMoisture();
    delay(tShow);
}

//scrive tutti i dati sulla seriale
void serialWrite()  {
    Serial.print(temperature);
    Serial.print(" °C, ");
    Serial.print(humidity);
    Serial.print(" %, ");
    Serial.print(pressure);
    Serial.print(" Pa, ");
    Serial.print(soilMoisture);
    Serial.print(" %, ");
    Serial.print(currentMillis);
    Serial.println(" ms");
}

//scrive tutti i dati su un file su schedina SD inserita
void fileWrite()  {
  myFile = SD.open("StazioneMeteo.csv", FILE_WRITE);
  if(myFile)  {
    myFile.print(CSVtoString());
  } else  {
    Serial.println("errore apertura file");
  }
}

//ritorna una string con tutti i dati delle misurazioni
String toString()  {
  String s = "";
  s += "Temperature = ";
  s += temperature;
  s += " C <br>";

  s += "Humidity = ";
  s += humidity;
  s += " % <br>";

  s += "Pressure = ";
  s += pressure;
  s += " Pa <br>";

  s += "Soil Moisture = ";
  s += soilMoisture;
  s += " % <br>";

  s += "Time = ";
  s += millis();
  s += " ms";

  return s;
}

//ritorna una string con tutti i dati delle misurazioni in formato compatibile con file CSV
String CSVtoString()  {
  String s = "";
  s += temperature;
  s += " C,";

  s += humidity;
  s += " %,";

  s += pressure;
  s += " Pa,";

  s += soilMoisture;
  s += " %,";

  s += currentMillis;
  s += " ms";

  return s;
}
