# ArniUino

ArniUino è un progetto nato per creare in casa una bilancia pesa-alveari, dotata di sensori in grado di dare informazioni circa la temperatura/umidità interna all'alveare rispetto all'esterno.
Il sistema è basato su Arduino Uno R3; è dotato di sensori DHT11, shield GSM SIM900A, Celle di carico con amplificatore HX711.
Ogni 30 minuti circa, invia le letture a Thingspeak il quale, se configurato correttamente, fornisce i grafici relativi all'evoluzione dei dati inseriti.
Nei circa 28 minuti di attesa, il sistema resta in ascolto di SMS. Se il testo inviato è "Dati", viene inviata una risposta al medesimo numero, riportante la lettura istantanea dei dati;
se il testo inviato è "Reset", viene riavviato il sistema, forzando la riconnessione (in caso di prolungato distacco dalla rete GPRS) e riprendendo il ciclo di letture e pubblicazioni.

L'alimentazione utlizza una batteria 12V 7.2Ah, un pannello fotovoltaico da 20W e 12V, un regolatore di carica da 20A con uscite USB 5V e 2A; il tutto collegato all'ingresso USB di Arduino.


Lista dei materiali usati:
- Arduino UNO R3;
- Shield GSM SIM900A (https://www.ebay.it/itm/134579153986)
- Celle di carico 50kg con HX711;
- Jumper vari (femmina-maschio);
- 2 sensori DHT11;
- cavo dati (8 poli - minimo 2m);
- scatole di derivazione IP65 (2 tipo 84*58*34mm IP65; 4 tipo 100*68*50mm IP65);
- connettori passacavo IP68 tipo regolabile con controdado
- 2 elementi in legno dimensione 50*70*2cm
