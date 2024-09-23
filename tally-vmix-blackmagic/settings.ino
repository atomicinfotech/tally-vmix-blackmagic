
//EEPROM Memory Addresses
const byte CHECK = 0; // Has a configuration
const int ADDRVMIX0 = 20; // vmix IP address
const int ADDRVMIX1 = 22;
const int ADDRVMIX2 = 24;
const int ADDRVMIX3 = 26;

// RESERVE Memory Addresses 100-120 for input numbers (2 bytes per int)
// RESERVE Memory Addresses 200-220 for camera numbers (2 bytes per int)


//TODO look flag bite to see if EEPROM is initialized, and if not zero out positions we'll be using
//check and see if we have a MAC address set
//randomly generate MAC address if we don't have one starting with A8:61:0A:00:00
//check if using DHCP
//if not using DHCP snag ip, subnet and gateway and use that
  
void savesettings() {
  Serial.println(F("Saving settings"));
  writeIntIntoEEPROM(ADDRVMIX0, vmixip[0]);
  writeIntIntoEEPROM(ADDRVMIX1, vmixip[1]);
  writeIntIntoEEPROM(ADDRVMIX2, vmixip[2]);
  writeIntIntoEEPROM(ADDRVMIX3, vmixip[3]);

  Serial.println(F("Saving tally map"));
  //write tally map to 100, 200 address space
  for(int i=0;i<MAXTALLIES;i++) {

    Serial.print(i);
    Serial.print(F(" : saving to address " ));
    Serial.print(100+(i*2));
    Serial.print(F(" input values " ));
    Serial.print(inputs[i]);
    Serial.print(F(" cams address "));
    Serial.print(200+(i*2));
    Serial.print(F(" ------> "));
    Serial.println(cams[i]);
    
    writeIntIntoEEPROM(100+(i*2), inputs[i]); //two bytes per int
    writeIntIntoEEPROM(200+(i*2), cams[i]); //two bytes per int
  }

  //re-initialize
  //TODO we really only need to reconnect to vmix if the vmix IP has changed
  vmixconnect();
}

void readsettings() {
  Serial.println(F("Reading back from EEPROM the vmix IP"));
  Serial.println(readIntFromEEPROM(ADDRVMIX0));
  Serial.println(readIntFromEEPROM(ADDRVMIX1));
  Serial.println(readIntFromEEPROM(ADDRVMIX2));
  Serial.println(readIntFromEEPROM(ADDRVMIX3));
  
  vmixip[0] = readIntFromEEPROM(ADDRVMIX0);
  vmixip[1] = readIntFromEEPROM(ADDRVMIX1);
  vmixip[2] = readIntFromEEPROM(ADDRVMIX2);
  vmixip[3] = readIntFromEEPROM(ADDRVMIX3);
  
  Serial.println(F("EEPROM Tally Map ---------"));
  //read tally map from 100, 200 address space
  for(int i=0;i<MAXTALLIES;i++) {
    inputs[i] = readIntFromEEPROM(100+(i*2));
    cams[i] = readIntFromEEPROM(200+(i*2));
    
    Serial.print(i);
    Serial.print(F(" : reading from address " ));
    Serial.print(100+(i*2));
    Serial.print(F(" input values " ));
    Serial.print(inputs[i]);
    Serial.print(F(" cams address "));
    Serial.print(200+(i*2));
    Serial.print(F(" ------> "));
    Serial.println(cams[i]);
  }
  
}


void writeIntIntoEEPROM(int address, int number)
{ 
  EEPROM.write(address, number >> 8);
  EEPROM.write(address + 1, number & 0xFF);
}

int readIntFromEEPROM(int address)
{
  return (EEPROM.read(address) << 8) + EEPROM.read(address + 1);
}

void(* reboot) (void) = 0; //declare reboot function @ address 0
