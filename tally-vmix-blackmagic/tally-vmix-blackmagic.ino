#include <BMDSDIControl.h>
#include <SPI.h>
#include <Ethernet.h>
#include <EEPROM.h>

int VMIX_PORT = 8099;

#define MAXTALLIES 10

byte inputs[MAXTALLIES];
byte cams[MAXTALLIES];


//MAC is A8:61:0A:AE:74:D2 D4
//OUI is A8:61:0A
EthernetClient client;
EthernetServer server(80);

BMD_SDITallyControl_I2C sdiTallyControl(0x6E);            // define the Tally object using I2C using the default shield address

byte vmixip[] = { 10, 40, 50, 236 };

// the setup function runs once when you press reset or power the board
void setup()
{
  Serial.begin(115200);

  readsettings();

  pinMode(13, OUTPUT);                                     // initialize digital pin 13 as an output
  

  // the media access control (ethernet hardware) address for the shield:
  byte mac[] = { 0xA8, 0x61, 0x0A, 0xAE, 0x74, 0xD4 };  
  //the IP address for the shield:
  //byte ip[] = { 10, 0, 0, 177 };

  Serial.println(F("Network..."));
  Ethernet.begin(mac);

  delay(2000);

  IPAddress myIPAddress = Ethernet.localIP(); 
  Serial.println(myIPAddress);
    
  vmixconnect();
  
  server.begin();

  sdiTallyControl.begin();                                 // initialize tally control
  sdiTallyControl.setOverride(true);                       // enable tally override
}
    
// the loop function runs over and over again forever
void loop()
{

  while (client.available()) {
    String rawdata = client.readStringUntil('\r\n');
    Serial.println(rawdata);

  
    
   vmixdata(String(rawdata));
  }

  //WEB SERVER
  webclient();
}


void vmixconnect() {
  // vMix connections 
  Serial.println(F("vMix..."));
  Serial.print(vmixip[0]); Serial.print(F(".")); Serial.print(vmixip[1]); Serial.print(F(".")); Serial.print(vmixip[2]); Serial.print(F(".")); Serial.print(vmixip[3]);
   
  Serial.println(VMIX_PORT);
  if(client.connect(vmixip, VMIX_PORT)) {
    Serial.println(F("vMix OK"));
    client.println("SUBSCRIBE TALLY");
  } else {
    Serial.println(F("FAIL"));
  }
}

void vmixdata(String rawdata) {

    Serial.println(F("data is"));
    Serial.println(rawdata);
    Serial.println(rawdata.indexOf("TALLY"));
    
    if (rawdata.indexOf("TALLY") == 0) {
      Serial.println(F("Tally Data"));
  
      String tallydata = rawdata.substring(9);
      Serial.println(tallydata);
  
      int str_len = tallydata.length();
  
      char tallies[str_len];


      tallydata.toCharArray(tallies, str_len);

      for(int i=0;i<15;i++) { //loop through the first fifteen tallies
        tally(tallymap(i + 1),tallies[i]);      
      }
      
    }
  
}


void types(String a) { Serial.println("it's a String"); }
void types(int a) { Serial.println("it's an int"); }
void types(char *a) { Serial.println("it's a char*"); }
void types(float a) { Serial.println("it's a float"); }
void types(bool a) { Serial.println("it's a bool"); }

  
