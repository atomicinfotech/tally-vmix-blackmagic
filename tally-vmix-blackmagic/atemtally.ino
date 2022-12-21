
/*
 * state: 1 = program, 2 = preview, 0 = off
 */
void tally (int camera, int state) {
  bool program = false;
  bool preview = false;
  String statetext = "Off";
  
  if (state == 1) {
    program = true;
    statetext = "Program";
  }

  if (state == 2) {
    preview = true;
    statetext = "Preview";
  }
    
  sdiTallyControl.setCameraTally(camera,program,preview);

  Serial.print(camera);
  Serial.print(F(" -> Atem Camera "));
  Serial.print(camera);
  Serial.println(statetext);

}
