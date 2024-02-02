//Questo sketch permette la lettura dei dati dai sensori e dalle celle di carico con l'invio sul portale "thingspeak.com".
//Ricordarsi di eseguire lo sketch "calibrating" presente in HX711 per fissare il valore dello 0kg.
//Info su http://github.com/Slaykristian/ArniUino

#include <SoftwareSerial.h>
#include <DHT.h>
#include "HX711.h"

SoftwareSerial gprsSerial(8, 9);

#define DHTPIN1 2  //pin del sensore1
#define DHTPIN2 5  //pin del sensore2

const int pinDout = 4;
const int pinSck = 3;

HX711 scale;

DHT dht1(DHTPIN1, DHT11);
DHT dht2(DHTPIN2, DHT11);

unsigned long startMillis; // Variabile per memorizzare il tempo di avvio
const unsigned long intervalRestart = 180000; // Intervallo di 30 minuti

void inviaLetturaDHT();
String leggiSMS();
void cancellaSMS();
String ShowSerialData();
void RipetizioneSMS();
String estraiNumeroMittente();

void setup() {
  gprsSerial.begin(9600);
  Serial.begin(9600);
  dht1.begin();
  dht2.begin();
  scale.begin(pinDout, pinSck);
  Serial.println();
  Serial.println(__FILE__);
  delay(1000);
  startMillis = millis(); // Memorizza il tempo di avvio all'inizio
}
 
void loop() {


      float h1 = dht1.readHumidity();
      float t1 = dht1.readTemperature(); 
      delay(100);   
         
      Serial.print("Temperatura = ");
      Serial.print(t1);
      Serial.println(" °C");
      Serial.print("Umidità = ");
      Serial.print(h1);
      Serial.println(" %");    

      delay(100);   

      float h2 = dht2.readHumidity();
      float t2 = dht2.readTemperature(); 
      delay(100);   
         
      Serial.print("Temperatura = ");
      Serial.print(t2);
      Serial.println(" °C");
      Serial.print("Umidità = ");
      Serial.print(h2);
      Serial.println(" %");    
    
      // Leggi il peso
    Serial.println("\nCalibrazione a bilancia scarica");
  
  scale.tare();
  scale.set_offset(221872); // impone l'offset calcolato dalla procedura di calibrazione
  scale.set_scale(20.287900);       // gli dice che ogni unità va moltiplicata per 20.287900 calcolato sempre dalla proc. di calibrazione
  Serial.print("UNITS: ");
  Serial.println(scale.get_units(10));

  Serial.println("\nBilancia calibrata, buon lavoro!");

  float weight = scale.get_units(10);  // Legge il peso e calcola la media in 10 letture

  float temperature_riferimento = 25.0;  // Temperatura di riferimento (potrebbe essere la temperatura ambiente di calibrazione)
  float percentuale_FS_per_10C = 0.05;  // Variazione percentuale di output per ogni 10℃

  float weight_corr = weight * (1 + ((t1 - temperature_riferimento) / 10.0) * (percentuale_FS_per_10C / 100.0));
  
  // Moltiplica il peso per -0,001 per averlo in kg e lo corregge aumentandolo del coefficente di calibrazione. A barre montate e scariche, correggere fino ad ottenere 0
  weight_corr *= 0.001;
  weight_corr += 1.9;


  Serial.print("Peso: ");
  Serial.print(weight_corr, 1);
  Serial.println(" kg");

  delay(1000);  // Attendere un secondo prima di effettuare la prossima lettura

   
  if (gprsSerial.available())
    Serial.write(gprsSerial.read());
 
  gprsSerial.println("AT");
  delay(1000);
 
  gprsSerial.println("AT+CPIN?");
  delay(1000);
 
  gprsSerial.println("AT+CREG?");
  delay(1000);
 
  gprsSerial.println("AT+CGATT?");
  delay(1000);
 
  gprsSerial.println("AT+CIPSHUT");
  delay(1000);
 
  gprsSerial.println("AT+CIPSTATUS");
  delay(2000);
 
  gprsSerial.println("AT+CIPMUX=0");
  delay(2000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CSTT=\"**********\""); //al posto degli ********** inserire il vostro indirizzo APN per la connessione, fornito dal gestore della SIM

  delay(1000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIICR");//bring up wireless connection
  delay(3000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIFSR");//get local IP adress
  delay(2000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIPSPRT=0");
  delay(3000);
 
  ShowSerialData();
  
  gprsSerial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");//start up the connection
  delay(6000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIPSEND");//begin send data to remote server
  delay(4000);
  ShowSerialData();

  // Mostra i dati seriali ricevuti
  String serialData = ShowSerialData();

  // Controlla se la risposta contiene "ERROR"
  if (serialData.indexOf("ERROR") != -1) {
    Serial.println("Errore rilevato. Riavvio Arduino...");
    delay(4000);
    // Puoi aggiungere qui il codice per riavviare Arduino
    // ad esempio, utilizzando il reset del microcontrollore
    asm volatile ("  jmp 0");
    } else {
  
  String str="GET https://api.thingspeak.com/update?api_key=*************&field1="+String(h1)+"&field2="+String(t1)+"&field3="+String(h2)+"&field4="+String(t2)+"&field5="+String(weight_corr,1);  //invia la lettura del sensore 1 (ricordarsi di creare "field1" e "field2")
// Nella stringa, al posto degli ************* inserite la chiave di pubblicazione sul vostro spazio thingspeak che avete creato in precedenza  
Serial.println(str);
  gprsSerial.println(str);//begin send data to remote server

  delay(4000);
  ShowSerialData();

  gprsSerial.println((char)26);//sending
  delay(5000);//waitting for reply, important! the time is base on the condition of internet 
  gprsSerial.println();
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIPSHUT");//close the connection
  delay(100);
  ShowSerialData();
  delay(100);
  cancellaSMS();
  delay(3000);
      RipetizioneSMS();
      delay(2000); // Aggiungi un ritardo per evitare l'esecuzione troppo rapida
//    }
  }
}

String ShowSerialData() {
  String response = "";

  while (gprsSerial.available() > 0) {
    char c = gprsSerial.read();
    response += c;
  }

  // Mostra la risposta nella console seriale
  Serial.print("Risposta GPRS: ");
  Serial.println(response);

  return response;
}
// void ShowSerialData()
// {
//  while(gprsSerial.available()!=0)
//  Serial.write(gprsSerial.read());
//  delay(5000); 
// }
String leggiSMS() {
  String sms = "";

  // Attendi che il modulo GSM risponda
  delay(1000);

  // Leggi il messaggio SMS
  while (gprsSerial.available()) {
    char c = gprsSerial.read();
    sms += c;
  }

  // Stampa la risposta sulla porta seriale per debug
  Serial.println("Risposta del modulo GSM: " + sms);

  return sms;
}

String estraiNumeroMittente(String sms) {
  // Estrai il numero del mittente dal messaggio SMS
  int pos1 = sms.indexOf("+CMT: \"") + 7;  // Posizione iniziale del numero del mittente
  int pos2 = sms.indexOf("\"", pos1);  // Posizione finale del numero del mittente
  String mittente = sms.substring(pos1, pos2);
  return mittente;
}
void RipetizioneSMS() {
  // Memorizza il tempo di inizio della ripetizione SMS
  unsigned long repeatStartMillis = millis();
  
  // Imposta il tempo massimo per la ripetizione a 28 minuti
  const unsigned long maxRepeatTime = 1700000; // 28 minuti in millisecondi (1680000ms) e qualcosa in più per fare un ciclo di 30 min (1800000ms). Caricamento a Thingspeak prende 100000ms.

  // Ciclo della ripetizione SMS fino a quando non sono trascorsi 28 minuti
  while (millis() - repeatStartMillis < maxRepeatTime) {  
    gprsSerial.println("AT+CNMI=2,2,0,0,0");
    delay(1000);

    if (gprsSerial.available() > 0) {
      String sms = leggiSMS();
      String mittente = estraiNumeroMittente(sms);

      
      // Verifica se il messaggio contiene la parola "DATI"
      // Se contiene "Reset", esegue il reset
      if (sms.indexOf("Reset") != -1) {
        delay(5000);
        inviaResetSMS(mittente);
        delay(6000);
      //  cancellaSMS();
        delay(2000);
        asm volatile ("  jmp 0");
      } else if (sms.indexOf("Dati") != -1) {
        // Altrimenti, esegue l'invio dei dati DHT
        String mittente = estraiNumeroMittente(sms);
        inviaLetturaDHT(mittente);
      //  cancellaSMS();
      }

      // Aggiungi ritardo tra le iterazioni, se necessario
      delay(2000);
    }
  }

  // Dopo 500 ripetizioni, riavvia lo sketch
  asm volatile ("  jmp 0"); // COmando di reset per Arduino Uno;
}
void inviaResetSMS(String mittente) {
  // Avviso che sto per resettare Arduino

  String messaggio = "Riavvio il sistema";

  // Invia il messaggio SMS al numero destinatario
  gprsSerial.print("AT+CMGS=\"");
  gprsSerial.print(mittente);
  gprsSerial.println("\"");
  delay(2000);
  gprsSerial.print(messaggio);
  delay(100);
  gprsSerial.println((char)26);  // Invia il carattere di controllo per inviare il messaggio
  delay(1000);
}
void inviaLetturaDHT(String mittente) {
  // Leggi la temperatura e l'umidità dal sensore DHT22
float h1 = dht1.readHumidity();
float t1 = dht1.readTemperature(); 
delay(100);
float h2 = dht2.readHumidity();
float t2 = dht2.readTemperature(); 
delay(100);

float weight = scale.get_units(10);  // Legge il peso e calcola la media in 10 letture
  
float temperature_riferimento = 25.0;  // Temperatura di riferimento (potrebbe essere la temperatura ambiente di calibrazione)
float percentuale_FS_per_10C = 0.05;  // Variazione percentuale di output per ogni 10℃

float weight_corr = weight * (1 + ((t1 - temperature_riferimento) / 10.0) * (percentuale_FS_per_10C / 100.0));
  
// Moltiplica il peso per -0,001 per averlo in kg e lo corregge aumentandolo del coefficente di calibrazione. A barre montate e scariche, correggere fino ad ottenere 0
weight_corr *= 0.001;
weight_corr += 2.4;


Serial.print("Peso: ");
Serial.print(weight_corr, 1);
Serial.println(" kg");
  // Crea il messaggio SMS con la lettura del sensore
  
  String messaggio = "Temperatura ext: " + String(t1) + "*C, Umidita' ext: " + String(h1) + "% Temperatura int: " + String(t2) + "*C, Umidita' int: " + String(h2) + "% Peso : " + String(weight_corr,1) + "Kg";

  // Invia il messaggio SMS al numero destinatario
  gprsSerial.print("AT+CMGS=\"");
  gprsSerial.print(mittente);
  gprsSerial.println("\"");
  delay(2000);
  gprsSerial.print(messaggio);
  delay(100);
  gprsSerial.println((char)26);  // Invia il carattere di controllo per inviare il messaggio
  delay(1000);
}
void cancellaSMS() {
  // Cancella tutti i messaggi sulla SIM
  gprsSerial.println("AT+CMGDA=\"");
  gprsSerial.println("DEL ALL\"");
  delay(3000);
  Serial.println("Ho cancellato tutti gli SMS");
  delay(3000);

  // Controlla la risposta del modulo GSM
  while (gprsSerial.available()) {
    Serial.write(gprsSerial.read());
   }
}
