#include <AirQualityClass.h>
#include <Arduino_MKRIoTCarrier.h>
#include <Arduino_MKRIoTCarrier_Buzzer.h>
#include <Arduino_MKRIoTCarrier_Qtouch.h>
#include <Arduino_MKRIoTCarrier_Relay.h>
#include <EnvClass.h>
#include <IMUClass.h>
#include <MKRIoTCarrierDefines.h>
#include <PressureClass.h>

MKRIoTCarrier carrier;

const int sensoreUmiditaTerreno = A5; //indica il pin del sensore

float temperature = 0;
float humidity = 0;
float pressure = 0;
float soilMoisture = 0;

const int interval = 30000;  //tempo tra le rilevazioni
const int tSpento = 20000;  //tempo che rimane spento
const int tShow = 700;  //tempo in cui si vede a schermo le misure
unsigned long previousMillis = 0;

#include <WiFiNINA.h>

char ssid[] = "f-factory";             //  your network SSID (name) between the " "
char pass[] = "Z89sn30Pfuq+s9";      // your network password between the " "
int keyIndex = 0;                 // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;      //connection status
WiFiServer server(80);            //server socket

WiFiClient client = server.available();


void setup() {
  Serial.begin(9600);
  carrier.begin();
  pinMode(sensoreUmiditaTerreno, INPUT);

  while (!Serial);
  
  enable_WiFi();
  connect_WiFi();

  server.begin();
  printWifiStatus();

}

unsigned long currentMillis = 0;
void loop() {
  client = server.available();

  spento();
  
  misura();

  if(client)  {
    printWEB();

  } else  {
    currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      scrive();

        

      previousMillis = currentMillis;
      
    }
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

            client.print(toString());
           
            

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
void misura() {
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
void spento() {
  carrier.leds.clear();
  carrier.leds.show();

  carrier.display.fillScreen(ST77XX_BLACK);
  carrier.display.setTextColor(ST77XX_WHITE);
  carrier.display.setTextSize(2);

  carrier.display.setCursor(80, 110);
  carrier.display.println("spento");

  delay(tSpento);
}

//mette i led a rosso quando scrive
void scrive() {
  carrier.leds.clear();

  carrier.leds.setPixelColor(0,255,0,0);
  carrier.leds.setPixelColor(1,255,0,0);
  carrier.leds.setPixelColor(2,255,0,0);
  carrier.leds.setPixelColor(3,255,0,0);
  carrier.leds.setPixelColor(4,255,0,0);
  carrier.leds.show();

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

    printTemperature();
    delay(tShow);
    printHumidity();
    delay(tShow);
    printPressure();
    delay(tShow);
    printSoilMoisture();
    delay(tShow);
}

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
