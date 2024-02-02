# ArniUino

ArniUino è un progetto nato per creare in casa una bilancia pesa-alveari, dotata di sensori in grado di dare informazioni circa la temperatura/umidità interna all'alveare rispetto all'esterno.
Il sistema è basato su Arduino Uno R3; è dotato di sensori DHT11, shield GSM SIM900A, Celle di carico con amplificatore HX711.
Ogni 30 minuti circa, invia le letture a Thingspeak il quale, se configurato correttamente, fornisce i grafici relativi all'evoluzione dei dati inseriti.
Nei circa 28 minuti di attesa, il sistema resta in ascolto di SMS. Se il testo inviato è "Dati", viene inviata una risposta al medesimo numero, riportante la lettura istantanea dei dati;
se il testo inviato è "Reset", viene riavviato il sistema, forzando la riconnessione (in caso di prolungato distacco dalla rete GPRS) e riprendendo il ciclo di letture e pubblicazioni.

L'alimentazione utlizza una batteria 12V 7.2Ah, un pannello fotovoltaico da 20W e 12V, un regolatore di carica da 20A con uscite USB 5V e 2A; il tutto collegato all'ingresso USB di Arduino.


![immagine](https://github.com/Slaykristian/ArniUino/assets/9326918/3c814f74-ea29-4562-90fd-5aea14c77e6c)


Lista dei materiali usati:
- Arduino UNO R3;
- Shield GSM SIM900A (https://www.ebay.it/itm/134579153986)
- Celle di carico 50kg con HX711;
- Jumper vari (femmina-maschio);
- 2 sensori DHT11;
- 2 case per i sensori DHT11 (così: https://www.ebay.it/itm/403573240690?var=673533907768&mkevt=1&mkcid=1&mkrid=724-53478-19255-0&campid=5338748322&toolid=20006&customid=F5H2CzYcAAAA7OEnkl9yK-LQ__cVAAAAAA)
- un convertitore USB-Seriale come questo https://www.ebay.it/itm/361612198155
- cavo dati (8 poli - minimo 2m);
- scatole di derivazione IP65 (2 tipo 84x58x34mm IP65; 4 tipo 100x68x50mm IP65);
- connettori passacavo IP68 tipo regolabile con controdado;
- 2 elementi in legno dimensione 50x70x2cm;
- una lamiera di tettuccio arnia da 10 favi;
- l'elemento "più complicato" da procurarsi sono le squadrette per alloggiare le celle di carico. Io ho fatto stampare queste: https://www.thingiverse.com/thing:2624188 Si trovano anche su ebay, altrimenti dovreste cercare il modo di stamparle in 3D;
- una SIM dati (Ho. Mobile o ThingsMobile o simili)
- un cavo USB per arduino-stampante almeno di 1,5m, meglio 2m;
- registrare un account gratuito su ThingSpeak: https://thingspeak.com/

# Procediamo

# Installiamo l'IDE di Arduino

Comiciamo a giocare subito con arduino e, se non si è già abili, a prendere confidenza con gli sketch e coi sensori.
La cosa più semplice è seguire le guide apposite che si trovano in rete, ad es. in canali YouTube.
Imparate a connettere correttamente, cominciando a provare uno per volta, gli elementi che avete acquistato e testatene il funzionamento.


# Lo shield GSM

A causa delle dimensioni geometriche, lo shield SIM900A è il giusto compromesso tra funzionalità e spazio occupato. Ma attenzione, deve essere dello stesso tipo della lista dei materiali.
Tralascio tutta la spiegazione su come collegarlo e sui comandi AT. Per chi volesse approfondire, in rete è pieno di documentazioni.

![immagine](https://github.com/Slaykristian/ArniUino/assets/9326918/4828eb5a-0bf7-4d25-ab82-a6288524ee09)

Se lo installate così com'è, troverete un errore dovuto al blocco di utilizzo sulle reti 4G europee.
Ve ne accorgete perchè, se collegato ad Arduino, e inserite una SIM, il led blinka ad ogni secondo perchè continua a cercare la rete senza trovarla, altrimenti blinkerebbe ogni 3 secondi.
La prima cosa da fare è connettere la parte di alimentazione ai pin presenti su Arduino e collegare i 2 pin (uno per Tx e uno per Rx) alla scheda con gli ingressi digitali.
Come connetterlo ad Arduino?
Estraendo una delle immagini è ben visibile dove connettere i pin "Rx" e "Tx", ricordandosi che il Pin "Rx" (nella foto è 5VR; il 5VT è quello "Tx") che esce dallo shield, sull'Arduino corrisponde al suo slot "Tx" e viceversa:



![immagine](https://github.com/Slaykristian/ArniUino/assets/9326918/cfb2daf1-ca84-4d85-ae0d-1245bd2b95c6)




Negli sketch che potreste provare su Arduino con lo shield, non è necessario includere librerie diverse da SoftwareSerial.h (che è l'unica da includere), quindi, dovreste preparare uno sketch molto semplice lanciando il comando "AT+GMR", Connetti TX a pin3 e RX a pin4 e usa questo sketch: https://github.com/Slaykristian/ArniUino/blob/main/Sketch_versione_SIM900A

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


A contatto avvenuto, il flasher comincerà ad aggiornare il firmware e a operazione completata, vi accorgerete subito del successo in quanto il led della scheda blinkerà ogni 3 secondi e non ogni secondo, connettendovi alla rete gprs e 4G della vostra SIM.
Adesso è utilizzabile.
 
# Prepariamo le plastiche e foriamole

Il titolo è esaustivo.
Qui viene fuori il vero senso del fai da te.
Vi posto le foto dell'ossatura della bilancia, con l'idea di massima di come ho collocato la sua elettronica:














![immagine](https://github.com/Slaykristian/ArniUino/assets/9326918/543266d5-9778-4033-8d66-ba67b44d8859)

Questa è la versione invece "chiusa", ma non ancora fissata:















![immagine](https://github.com/Slaykristian/ArniUino/assets/9326918/a7b0129b-dbf0-4d4c-81c2-bc4201cf9bc1)

Le plastiche da forare sono 3: le 2 nere, nelle quali fare in una 3 fori, in una 4 e una sola bianca la quale ospiterà Arduino, lo shield, la scheda HX711, le connessioni e l'antenna GSM.
Non ho disegni con quote e sono andato ad occhio, ma vi serve una punta del trapano per forare con lo stesso diametro dei passacavo IP68 più piccolo possibile, PG7.
Come visibile nell'immagine, quello bianco necessita di 4 fori. 3 fori per i PG7 e uno ce faccia passare l'USB di Arduino.
Non mi dilungherò in merito, ma lascio alle foto quanto è necessario fare. Una volta eseguiti i fori, è necessario curarsi di inserire i passacavi. Le scatole bianche sono l'appoggio per le squadrette con le celle di carico.

# Connettere i sensori di temperatura DHT11

Sono economici e nelle prove che ho eseguito, più affidabili anche son si tratta di strumenti "di precisione" (non è questo che stiamo cercando).
Sono praticamente plug&play.

Sono dotati di 3 pin: il Vcc+, l'uscita con la lettura e il GND:

![immagine](https://github.com/Slaykristian/ArniUino/assets/9326918/f5c2d425-f73d-43e1-be99-e3949cb9e8c3)


Potete provare con gli sketch di esempio contenuti nella libreria DHT.h (che sarà quella che ci servirà anche nel codice, quindi scaricatevela), per testarne il funzionamento.

Nel nostro progetto ne abbiamo inseriti 2 di DHT11: uno che sarà dotato di un cavo più lungo che andremo a realizzargli e che andrà posizionato, passando da dietro, all'interno dell'arnia e uno che andrà messo sulla struttura stessa, in posizione protetta, ma che sarà adibito alla rilevazione della temperatura esterna.
Nel nostro progetto, come vedrete nel paragrafo sotto, riguardante le connessioni, il sensore che va all'interno dell'arnia è il sensore collegato al pin5, chiamato "dht2" (che restituisce il valore "t2"), mentre al pin2 c'è il sensore chiamato "dht1" che restituisce il valore "t1". 

# Connettere le celle di carico

Lo schema di connessione delle celle di carico è questo:







![immagine](https://github.com/Slaykristian/ArniUino/assets/9326918/82a98358-fc56-42a8-be53-660defa3d13b)


Questo è per darvi un'idea di massima, della logica di connessione delle celle e dell'HX711.

Per quanto riguarda la parte di connessione tra celle e HX711, basatevi sullo schema in immagine precedente; per la parte da HX711 notate che sono 4 i pin da connettere: il polo positivo Vcc+, il GND e i 2 pin digitali denominati Data e Sck o qualcosa di simile. Questi 2 vengono citati nello sketch, quindi nel punto sull'assemblaggio vedremo a quali pin connetterli per poter essere utilizzati col codice fornito.
In Arduino è possibile divertirsi con le celle di carico, usando gli sketch forniti dalla libreria HX711.h che vi consiglio di provare.
Sarà poi una di quelle librerie che installeremo e richiameremo.

E' importante procurarsi le squadrette indicate nella lista dei materiali all'inizio, altrimenti dovrete costruirvi un alloggio simile per utilizzarle sui ciascuna scatola plastica bianca.
La squadretta contenente la cella poi, io l'ho incollata sulle plastiche bianche.

Nel prossimo punto assembleremo celle, shield e sensori, con la sequenza di pin previsti dal codice che andremo a realizzare.

# Connettiamo tutto

Un consiglio è di provare tutte le connessioni senza riporle nelle scatole. 
Comprese le logiche di connessione dei sensori, dell'HX711 e dello shield GSM.
Nello schema qui sotto (perdonatemi, ma non sono un "elettronico"), trovate le connessioni corrispondenti ai pin gestiti dal codice dell'Arduino che andremo ad inserire.
Questo è il legame tra le connessioni. Provatelo prima con i jumper e basta, poi provate, come nella mia immagine dello scheletro a realizzarlo, utilizzando un cavo dati, munendosi di carta per annotarsi a qualce colore del cavo dati corrisponde la connessione che state predisponendo e così via.
Punto fondamentale è che lo schema condivide l'alimentazione e che, a differenza di quello che viene scritto sui forum di Arduino (anche in italiano), lo shield SIM900A supporta tranquillamente i 2A per le connessioni.
Inoltre, non l'ho detto in precedenza, munitevi di pazienza e saldate a stagno tutte le componenti, specialmente le celle di carico con l'HX711 e con Arduino, poiché è un comparto sensibile alle variazioni di corrente e se la connessione tra i suoi pin non è regolare, è facile avere letture sbagliate.


![immagine](https://github.com/Slaykristian/ArniUino/assets/9326918/6e2e9de8-da3f-4f6a-96b6-f064d8bb40f9)



Volutamente non mi sbilancio su come fare la parte di bricolage. Il bello è questo... Ognuno lo realizza come meglio crede.
Notate nell'immagine dove si mostra lo "scheletro" che il tutto poggia su due pezzi di legno che sono lunghi quanto il lato più lungo di un'arnia e larghi pocho di più del lato della scatoletta grigia, con uno spessore di almeno 2 cm.
Sono stati ricavati da una vecchia cornice utilizzata per l'ingabbio di regina su favo orizzontale (ormai non più utilizzati), le cui dimensioni le ho riportate sopra.
Nulla vieta di utilizzare altri materiali più resistenti. 
Questi pezzi, come si può intravedere dall'immagine all'inizio della guida, sono stati passati con la catramina per renderli impermeabili. 
Le plastiche sono state tutte attaccate con colla "millechiodi" essendo quelle plastiche tali da legarsi con quel materiale ed il legno trattato. Infine, cosa importantissima, sono state create alla fine, due "bane", chiamiamole così.
Ho preso la lamiera di un tettuccio e l'ho tagliata con una cesoia in modo da creare 2 coperture che facciano da elemento di appoggio delle arnie.
In questa maniera la distribuzione del peso è omogenea. E le connessioni, compreso il sensore di temperatura per l'esterno, protette.

Non l'ho testata ancora alle condizioni climatiche avverse; al gelo, va bene.

# Il codice

Il codice sicuramente può essere modificato e migliorato.
Lo trovate in allegato a questo progetto e si chiama "ArniUino".
E' necessario installare nell'IDE le librerie DHT.h e HX711.h . La SoftwareSerial.h dovrebbe già essere inclusa. 
Cosa fa in sostanza questo codice...
All'avvio inizializza le porte e assegna i ruoli a tutto ciò che è connesso.
Parte ed effettua la taratura delle celle, procedura la cui comprensione non è stata semplice.
Fissa il valore a zero del peso e se carica legge e restituisce il peso letto e corretto della tara inutile.
Legge e restituisce i valori di Temperatura ed Umidità dei 2 sensori poi setta la connessione GPRS e invia a ThingSpeak la lettura sottoforma di una stringa.
Se la connessione esiste arriva a tentare l'invio, altrimenti riprova daccapo.
A inserimento riuscito, restituisce il numero di dato mandato e chiude la connessione GPRS, mettendosi in attesa di ricezione SMS per 28 minuti circa.
Durante questi 28 minuti, se, al numero della SIM si invia un SMS contenente la stringa "Dati" (senza spazi), il sistema effettua una lettura istantanea col medesimo sistema di quando la invia a ThingSpeak, solo che vi costruirà un SMS e vi risponderà. Non sempre la ricezione gli riesce al primo colpo. Quindi può essere necessario inviargli più di un SMS (in genere massimo 3-4), ma la maggior parte delle volte riesce a leggerlo al primo colpo.
Se, invece, gli si invia un SMS contenente la stringa "Reset", allora sempre se lo riesce a leggere subito (altrimenti ritenteremo), riavvia Arduino per riconnettersi.
Questa funzione è utile quando si nota che il sistema sta rtiscontrando difficoltà di invio delle stringhe a ThingSpeak.
Passati i 28 minuti circa, si riavvia lo sketch e proverà a pubblicare un altro pacchetto di dati su ThingSpeak.



