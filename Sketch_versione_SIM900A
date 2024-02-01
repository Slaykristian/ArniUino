#include <SoftwareSerial.h>

// Definisci i pin RX e TX per la comunicazione con lo shield GSM
#define RX_PIN 3
#define TX_PIN 4

// Crea un oggetto SoftwareSerial per la comunicazione con lo shield GSM
SoftwareSerial gsmSerial(RX_PIN, TX_PIN);

void setup() {
  // Inizializza la comunicazione seriale con il monitor seriale
  Serial.begin(9600);
  
  // Inizializza la comunicazione seriale con lo shield GSM
  gsmSerial.begin(9600);

  // Attendi che lo shield GSM sia pronto
  delay(1000);

  // Invia il comando "AT+GMR" allo shield GSM
  gsmSerial.println("AT+GMR");

  // Attendi qualche secondo per ricevere la risposta
  delay(2000);
}

void loop() {
  // Leggi e stampa la risposta dallo shield GSM
  while (gsmSerial.available()) {
    char c = gsmSerial.read();
    Serial.print(c);
  }
}
