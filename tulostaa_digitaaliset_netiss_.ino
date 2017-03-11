

#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };  //määritetään mac-address
  EthernetClient client;                 //Ottaa IP:n dhcp:llä
// IPAddress ip(77,223,40,141);          //tai määritä staattinen
EthernetServer server(80);               //laitetaan palvelu pyörimään porttiin 80

void setup() {
  Serial.begin(9600);                    //serial yhteyden aloitus ja portin avaamisen odotus

   Ethernet.begin(mac);                  // aloittaa yhteyden ja serverin
  server.begin();
  
  Serial.println(Ethernet.localIP());    //tulostaa IP:n serial portin kautta
}


void loop() {
  EthernetClient client = server.available();   // kuunnellaan tulevia asiakkaita
  if (client) {
    Serial.println("new client");
	//http requesti loppuu tyhjään riviin"
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
		//Rivin lopussa newline, jotta http requesti päättyy, jolloin client voi lähettää vastauksen.
        if (c == '\n' && currentLineIsBlank) {
          
          // kirjottelee perus html-sivun rivi riviltä 
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<title>Digitaaliset pinnit...</title>");    // tulostaa pinnien arvot
          for (int digitalChannel = 0; digitalChannel < 14; digitalChannel++) {  //tulosteta joka pinni lisäämällä arvoa joka kierros.
            int sensorReading = digitalRead(digitalChannel);
            client.print("digital input ");
            client.print(digitalChannel);
            client.print(" is ");
            client.print(sensorReading);
            client.println("<br />");      
            }
          client.println("</html>");
          break;
        }
          if (c == '\n') {
          currentLineIsBlank = true; // alottaa uuden rivin
        }
        
        else if (c != '\r') {
          currentLineIsBlank = false; // kirjain tällä rivillä
        }}}
    
    delay(1); // annetaan selaimelle aikaa vastaanottaa data
    
    client.stop(); //suljetaan yhteys
    Serial.println("yhteys suljettu");
  }}
