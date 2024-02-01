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
- un convertitore USB-Seriale come questo https://www.ebay.it/itm/361612198155
- cavo dati (8 poli - minimo 2m);
- scatole di derivazione IP65 (2 tipo 84x58x34mm IP65; 4 tipo 100x68x50mm IP65);
- connettori passacavo IP68 tipo regolabile con controdado;
- 2 elementi in legno dimensione 50x70x2cm;
- una lamiera di tettuccio arnia da 10 favi;
- l'elemento "più complicato" da procurarsi sono le squadrette per alloggiare le celle di carico. Io ho fatto stampare queste: https://www.thingiverse.com/thing:2624188 Si trovano anche su ebay, altrimenti dovreste cercare il modo di stamparle in 3D;
- una SIM dati (Ho. Mobile o ThingsMobile o simili)

# Procediamo

# Installiamo l'IDE di Arduino

Comiciamo a giocare subito con arduino e, se non si è già abili, a prendere confidenza con gli sketch e coi sensori.
La cosa più semplice è seguire le guide apposite che si trovano in rete, ad es. in canali YouTube.
Imparate a connettere correttamente, cominciando uno per volta, gli elementi che avete acquistato e testatene il funzionamento.


# Lo shield GSM

A causa delle dimensioni geometriche, lo shield SIM900A è il giusto compromesso tra funzionalità e spazio occupato. Ma attenzione, deve esse dello stesso tipo della lista dei materiali.
Tralascio tutta la spiegazione su come collegarlo e sui comandi AT. Per chi volesse approfondire, in rete è pieno di documentazioni.

![immagine](https://github.com/Slaykristian/ArniUino/assets/9326918/4828eb5a-0bf7-4d25-ab82-a6288524ee09)

Se lo installate così com'è, troverete un errore dovuto al blocco di utilizzo sulle reti 4G europee.
Ve ne accorgete perchè, se collegato ad Arduino, e inserite una SIM, il led blinka ad ogni secondo perchè continua a cercare la rete senza trovarla, altrimenti blinkerebbe ogni 3 secondi.
La prima cosa da fare è connettere la parte di alimentazione ai pin presenti su Arduino e collegare i 2 pin (uno per Tx e uno per Rx) alla scheda con gli ingressi digitali.
Non è necessario includere librerie diverse da SoftwareSerial.h, quindi, dovreste preparare uno sketch molto semplice lanciando il comando "AT+GMR", Connetti TX a pin3 e RX a pin4 e usa questo sketch: https://github.com/Slaykristian/ArniUino/blob/main/Sketch_versione_SIM900A

E' quindi necessario provvedere a scaricare un firmware adatto (anche se per SIM900 e non per 900A) e installarlo secondo la procedura presente in questo sito:
https://acoptex.com/wp/basics-project-105b-sim900a-gsm-gprs-module-how-to-flash-another-firmware/

Firmware per scheda versione 64:
https://drive.google.com/file/d/0B-rBpaSGK1gpNXZFdWgtR3QtNk0/edit?usp=sharing
Firmware per scheda versione 32:
https://drive.google.com/file/d/0B-rBpaSGK1gpbmF6MnNkbEJwNW8/edit

Connettete lo shield all'adattatore seriale così:
![immagine](https://github.com/Slaykristian/ArniUino/assets/9326918/bf9804f7-cada-42fb-b549-8752365ca27d)

L'aggiornamento è un processo poco simpatico, in quanto è necessario:
- connettere lo shield tramite l'adattatore USB-Seriale come indicato nell'allegato precedente;
- aprire il software e scegliere il file, impostando l'interfaccia come previsto nel link;
- creare un ponte sulla connessione della funzione "reset"
  come indicato in questa figura:
![immagine](https://github.com/Slaykristian/ArniUino/assets/9326918/6b855a0d-1f4d-4945-9d2b-8c9d72ae0d10)
Il flasher comincerà ad aggiornare il firmware e a operazione completata, vi accorgerete subito del successo in quanto il led della scheda blinkerà ogni 3 secondi e non ogni secondo, connettendovi alla rete gprs e 4G della vostra SIM.
Adesso è utilizzabile.
 
