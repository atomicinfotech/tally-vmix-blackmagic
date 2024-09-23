
/*
 * state: 1 = program, 2 = preview, 0 = off
 */
void tally (int camera, char state) {
  bool program = false;
  bool preview = false;
  String statetext = "Off";
  
  if (state == 49) { //1
    program = true;
    statetext = "Program";
  }

  if (state == 50) { //2
    preview = true;
    statetext = "Preview";
  }
    
  sdiTallyControl.setCameraTally(camera,program,preview);

  Serial.print(camera);
  Serial.print(F(" -> Atem Camera "));
  Serial.print(camera);
  Serial.print(F(" Text state: "));
  Serial.println(statetext);
  

}

byte tallymap(int input) {
//  Serial.print(F("Look up input #"));
//  Serial.println(input);

  if (!input) return 0;
  
  for(int i=0;i<MAXTALLIES;i++) {

//      Serial.print(i);
//      Serial.print(F(" :"));
//      Serial.print(inputs[i]);
//      Serial.print(F(" ------> "));
//      Serial.println(cams[i]);

      if (inputs[i] == input) {

//        Serial.print(F("Set to cam #"));
//        Serial.println(cams[i]);
        return cams[i];
      }
  }

  return 0;
  
}
