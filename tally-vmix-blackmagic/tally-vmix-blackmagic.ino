/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Addition to original blink sketch also turns on and off camera 1's tally indicator.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://www.arduino.cc

  This example code is in the public domain.
*/

#include <BMDSDIControl.h>                                // need to include the library
#include <Arduino_JSON.h>
#include <SPI.h>
#include <Ethernet.h>


// EEPROM Addresses
const byte CHECK = 0; // Has a configuration
const byte MAC1 = 1; // Mac address
const byte MAC2 = 2; 
const byte MAC3 = 3;
const byte MAC4 = 4;
const byte MAC5 = 5;
const byte MAC6 = 6;
const byte IP1 = 7; // Static IP-address
const byte IP2 = 8;
const byte IP3 = 9;
const byte IP4 = 10;
const byte GW1 = 11; // Gateway
const byte GW2 = 12;
const byte GW3 = 13; 
const byte GW4 = 14;
const byte MASK1 = 15; // Subnet Mask
const byte MASK2 = 16;
const byte MASK3 = 17;
const byte MASK4 = 18;
const byte DHCP = 19; // DHCP boolean
const byte VMIX1 = 20; // Static IP-address
const byte VMIX2 = 21;
const byte VMIX3 = 22;
const byte VMIX4 = 23;
const byte CAM1 = 24; // Static IP-address
const byte CAM2 = 25;
const byte CAM3 = 26;
const byte CAM4 = 27;
const byte CAM5 = 24; // Static IP-address
const byte CAM6 = 25;
const byte CAM7 = 26;
const byte CAM8 = 27;


int VMIX_PORT = 8099;
String M_TALLY = "";
int TALLY_NR = 1;
char currentState = -1;
char screen = 1;

#define TALLY_COUNT 100
char tallymap[TALLY_COUNT];   


//MAC is A8:61:0A:AE:74:D2
//OUI is A8:61:0A
EthernetClient client;


BMD_SDITallyControl_I2C sdiTallyControl(0x6E);            // define the Tally object using I2C using the default shield address

// the setup function runs once when you press reset or power the board
void setup()
{

  //TODO
  //Load settings from EEPROM
  //look flag bite to see if EEPROM is initialized, and if not zero out positions we'll be using
  //check and see if we have a MAC address set
  //randomly generate MAC address if we don't have one wtarting with A8:61:0A:00:00
  //check if using DHCP
  //if not using DHCP snag ip, subnet and gateway and use that
  //
  

  
  tallymap[2] = 1;      // map vmix input 1 to ATEM input 1 
  tallymap[3] = 2;      // map vmix input 1 to ATEM input 1 
  tallymap[4] = 3;      // map vmix input 1 to ATEM input 1 
  tallymap[5] = 4;      // map vmix input 1 to ATEM input 1 



  sdiTallyControl.begin();                                 // initialize tally control
  sdiTallyControl.setOverride(true);                       // enable tally override

  pinMode(13, OUTPUT);                                     // initialize digital pin 13 as an output
  Serial.begin(115200);

  // the media access control (ethernet hardware) address for the shield:
  byte mac[] = { 0xA8, 0x61, 0x0A, 0xAE, 0x74, 0xD2 };  
  //the IP address for the shield:
  //byte ip[] = { 10, 0, 0, 177 };

  Serial.println("Connecting to ethernet");
  Ethernet.begin(mac);

  byte vmixip[] = { 192, 168, 10, 225 };
  
  delay(2000);

  Serial.println("Connecting to vmix");
  if(client.connect(vmixip, VMIX_PORT)) {
    Serial.println("Connected to vmix");
    client.println("SUBSCRIBE TALLY");
  } else {
    Serial.println("Unable to connect to vmix");
  }
  
}

// the loop function runs over and over again forever
void loop()
{


  while (client.available()) {
    String data = client.readStringUntil('\r\n');
    //Serial.println(data);
    handleData(data);
  }
  
}


void handleData(String data)
{

  //digitalWrite(13, LOW);  //blink the onboard LED

  //loop through the data

  if (data.indexOf("TALLY") == 0) {

    String tallydata = data.substring(9);
    Serial.println(tallydata);

    int str_len = tallydata.length();

    char tallies[str_len];
         
    // Copy it over 
    tallydata.toCharArray(tallies, str_len);
    
//    for(int i=0;i<str_len-1;i++) {
    for(int i=0;i<5;i++) {

    // Serial.print(tallies[i]);
    // Serial.print("+");

      //todo convert vmix input (i) to blackmagic camera number
      
      if (tallydata.charAt(i) == '1') {  //program
        Serial.print(i + 1);
        Serial.print(" -> ");
        Serial.print(tallymap[i + 1]);
        Serial.print(" PGM\n");
        
        sdiTallyControl.setCameraTally(                         
          i,                                                     // Camera Number
          true,                                                  // Program Tally
          false                                                  // Preview Tally
        );
      } else if (tallydata.charAt(i) == '2') { //preview
        Serial.print(i + 1);
        Serial.print(" -> ");
        Serial.print(tallymap[i + 1]);
        Serial.print(" PVW\n");
        
        sdiTallyControl.setCameraTally(                          
          i,                                                     // Camera Number
          false,                                                  // Program Tally
          true                                                  // Preview Tally
        );

      } else { //tally off
        Serial.print(i + 1);
        Serial.print(" -> ");
        Serial.print(tallymap[i + 1]);
        Serial.print(" OFF\n");

        sdiTallyControl.setCameraTally(                        
          i,                                                     // Camera Number
          false,                                                  // Program Tally
          false                                                  // Preview Tally
        );
      }
      
    }

    

    //tallystate = tallies;
    
  } else {
    //we didn't get tally data, got something else
  }

  //digitalWrite(13, HIGH); //blink the LED
}
