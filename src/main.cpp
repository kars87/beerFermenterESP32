// Lage input for settpunkt mangler
// PWM på varme?
// Lage logikk for dødbånd varme mangler
// Lage logikk for timer/dager med forskjellige settpunkt mangler
// Lage webserver/sende til HA/sende til hjemmeside mangler


#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is connected to GPIO 4
#define ONE_WIRE_BUS 4

const int relayHeating = 18;
const int relayCooling = 19;


// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

DeviceAddress sensor1 = { 0x28, 0xFF, 0x64, 0x1E, 0x26, 0x74, 0x58, 0xBA };
DeviceAddress sensor2 = { 0x28, 0xFF, 0x64, 0x1E, 0x26, 0x7B, 0x11, 0x78 };

long count;


void setup(void){
  Serial.begin(115200);
  sensors.begin();
  pinMode(relayHeating, OUTPUT);
  pinMode(relayCooling, OUTPUT);
}


void loop(void){ 
  
  Serial.print("Requesting temperatures..."); // fjernes når kode ferdig
  sensors.requestTemperatures(); // Send the command to get temperatures

  float tempFridge = sensors.getTempC(sensor1);
  Serial.println("Sensor 1(*C): "); // fjernes når kode ferdig
  Serial.print(tempFridge); // fjernes når kode ferdig
  if(tempFridge == -127.00) { // kanskje brukes til noe feil på logg visning
    Serial.println("Failed to read from DS18B20 sensor"); // kanskje brukes til noe feil på logg visning
  }
  float tempBeer = sensors.getTempC(sensor2);
  Serial.println("Sensor 2(*C): "); // fjernes når kode ferdig
  Serial.print(tempBeer); // fjernes når kode ferdig
  if(tempBeer == -127.00) { // kanskje brukes til noe feil på logg visning
    Serial.println("Failed to read from DS18B20 sensor"); // kanskje brukes til noe feil på logg visning 
  }
  // Varme/kjølestyring start:
  if(tempBeer >= 22){
    digitalWrite(relayHeating, HIGH);
  }else{
    digitalWrite(relayHeating, LOW);
  }
  if(tempBeer >= 22 && count >= 50){
    digitalWrite(relayCooling, HIGH);
    count = 0; // setter teller til 0, dvs at kjøling ikke får starte før telleren har nåd et nivå (50 pr nå)
  }
  if(tempBeer <= 22){
    digitalWrite(relayCooling, LOW);
  }
  // Varme/kjølestyring slutt


  // Variabel for å lese relestatus
  int relayState1 = digitalRead(relayHeating);
  int relayState2 = digitalRead(relayCooling);
  
  // teller hvis kjølerele er av
  if(relayState2 == LOW){
    count += 1;
  }
  
  Serial.println(count); // fjernes når kode ferdig
  delay(2000);
}