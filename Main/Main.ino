#include <Wire.h>

#define TMP117_ADDRESS 0x48 // Adresse I2C du TMP117
bool tmp117Found = false; // Variable globale pour indiquer si le TMP117 est trouvé

void setup() {
  Serial.begin(115200); // Démarrer la communication série à 115200 bauds
  Wire.begin();
  Serial.println("\nI2C Scanner");
}

void loop() {
  scanI2CAddress(); // Effectuer le balayage I2C
  if (tmp117Found) { // Si le TMP117 est trouvé, lire et afficher la température
    readTMP117();
  }
  delay(1000); // Attendre 5 secondes avant le prochain balayage
}

void scanI2CAddress() {
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");
  nDevices = 0;
  tmp117Found = false;

  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
      nDevices++;
      if (address == TMP117_ADDRESS) {
        tmp117Found = true; // Indiquer que le TMP117 est trouvé
        Serial.println("TMP117 Found!");
      }
    }
    else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
    }    
  }

  if (nDevices == 0) Serial.println("No I2C devices found\n");
  else Serial.println("Scan done\n");
}

void readTMP117() {
  Wire.beginTransmission(TMP117_ADDRESS);
  Wire.write(0x00); // Pointeur vers le registre de température
  Wire.endTransmission(false);
  Wire.requestFrom(TMP117_ADDRESS, (uint8_t)2); // Demande de 2 octets du registre de température

  if (Wire.available() == 2) {
    uint16_t raw = (Wire.read() << 8) | Wire.read(); // Combine les deux octets en un entier 16 bits
    double tempC = raw * 0.0078125; // Convertit les données brutes en température
    Serial.print("Température du TMP117: ");
    Serial.print(tempC);
    Serial.println(" °C");
  } else {
    Serial.println("Erreur de lecture du TMP117");
  }
}
