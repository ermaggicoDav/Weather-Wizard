#include <BMP180.h>
#include "DHT.h"
#include <SD.h>
#include <SPI.h>
#include <SFE_BMP180.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>

const int sensoreUmiditaTerreno = ; //indica il pin del sensore
DHT dht;  //sensore temperatura e umidita
const int sensoreDHT = ;  //indica il pin del sensore

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(sensoreUmiditaTerreno, INPUT);

  dht.setup(sensoreDHT);


}

const int interval = 1000;  //tempo tra le rilevazioni
void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    //attacco alimentazione ai sensori

    //misurazioni
    int pressioneAtmosferica = misuraPressioneAtmosferica(/*BMP280 o BMP180*/);
    float temperatora = misuraTemperatura(/*BMP280 o BMP180*/);
    int altitudine = misuraAltitudine(/*BMP280 o BMP180*/);
    float umidita = misuraUmidita(/*DHT11*/);
    float umiditaTerreno = misuraUmiditaTerreno(sensoreUmiditaTerreno);


    //stacco alimentazione ai sensori

    //scrivo sulla scheda ??qui oppure i sensori scrivono nel file??

    previousMillis = currentMillis;
  }

}



float misuratemperatura(DHT dht)  {
  return dht.temperature();
}

float misuraUmidita(DHT dht)  {
  return dht.humidity();
}

float misuraUmiditaTerreno(int sensoreUmiditaTerreno)  {
  return analogRead(sensoreUmiditaTerreno);
}

void writeToFile(char fileName[], char text[])
{
  myFile = SD.open(fileName, FILE_WRITE);
  if (myFile) // it opened OK
    {
    Serial.println("Writing to file");
    myFile.println(text);
    myFile.close(); 
    Serial.println("Done");
    }
  else 
    Serial.println("Error opening file");
}

void deleteFile(char fileName[])
{
 //delete a file:
  if (SD.exists(fileName)) 
    {
    Serial.println("Removing text from file");
    SD.remove(fileName);
    Serial.println("Done");
   } 
}


