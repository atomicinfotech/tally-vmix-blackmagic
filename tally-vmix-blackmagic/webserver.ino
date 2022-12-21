


void webclient() {
  EthernetClient client = server.available();
  if (client) 
    {
    Serial.println(F("Client connected"));
    // an http request ends with a blank line
    boolean done = false;
    while (client.connected() && !done) 
      {
      while (client.available () > 0 && !done)
        done = processIncomingByte (client.read ());
      }  // end of while client connected

    // send a standard http response header
    client.println(F("HTTP/1.1 200 OK"));
    client.println(F("Content-Type: application/json"));
    client.println(F("Connection: close"));  // close after completion of the response
    client.println();   // end of HTTP header
    client.println(F("OK"));

    // give the web browser time to receive the data
    delay(10);
    // close the connection:
    client.stop();
    Serial.println(F("Client disconnected"));
  }  // end of got a new client
}

/**
 * queryString: the string with is to be parsed.
 * WARNING! This function overwrites the content of this string. Pass this function a copy
 * if you need the value preserved.
 * results: place to put the pairs of param name/value.
 * resultsMaxCt: maximum number of results, = sizeof(results)/sizeof(*results)
 */

int parseUrlParams(char *queryString, char *results[][2], int resultsMaxCt) {
  int ct = 0;

  while (queryString && *queryString && ct < resultsMaxCt) {
    results[ct][0] = strsep(&queryString, "&");
    results[ct][1] = strchrnul(results[ct][0], '=');
    if (*results[ct][1]) *results[ct][1]++ = '\0';
    ct++;
  }

  return ct;
}


const unsigned int MAX_INPUT = 150; // maximum serial data
const int MAX_PARAM = 150; // max length of url

// Example GET line: GET /?foo=bar HTTP/1.1
void processGet (const char * data) {

  // find where the parameters start
  const char * paramsPos = strchr (data, '?'); 
  if (paramsPos == NULL)
    return;  // no parameters
    
  // find the trailing space
  const char * spacePos = strchr (paramsPos, ' ');
  if (spacePos == NULL)
    return;  // no space found
    
  // work out how long the parameters are
  int paramLength = spacePos - paramsPos - 1;
  // see if too long
  if (paramLength >= MAX_PARAM)
    return;  // too long for us
    
  // copy parameters into a buffer
  char param [MAX_PARAM];
  memcpy (param, paramsPos + 1, paramLength);  // skip the "?"
  param [paramLength] = 0;  // null terminator
  
  Serial.println (F("param is"));
  Serial.println(param);

  char *params[5][2];
  // parse the buffer into key value pairs
  int resultsCt = parseUrlParams(param, params, 5);

  Serial.print (F("WEB COMMAND IS "));
  Serial.println (params[0][0]);
  
  if (strcmp(params[0][0],"tally") == 0) {

    Serial.println(F("WEB TALLY ----------------"));
    for (int i = 1; i < resultsCt; i++) {
    
      Serial.print(F(" Set Camera \""));
      Serial.print( params[i][0]);
      Serial.print(F("\" to  \""));
      Serial.println( params[i][1]);

      //TODO handle state of "program/PGM, preview/PVW" as well
      
      tally(atoi(params[i][0]),atoi(params[i][1]));
      
    }
    
    
  } else if (strcmp(params[0][0],"config") == 0) {
    Serial.println(F("WEB CONFIG ----------------"));
    for (int i = 1; i < resultsCt; i++) {
    
      Serial.print(i);
      Serial.print(F(" key \""));
      Serial.print( params[i][0]);
      Serial.print(F("\", value \""));
      Serial.print( params[i][1]);
      Serial.print("\".");
      Serial.println();

    }
    
  }
 
  Serial.println();
    
}  // end of processGet

// here to process incoming serial data after a terminator received
void processData (const char * data) {
  //Serial.println (data);
  if (strlen (data) < 4)
    return;
  
  if (memcmp (data, "GET ", 4) == 0)
    processGet (&data [4]);
}  // end of processData

bool processIncomingByte (const byte inByte) {
  static char input_line [MAX_INPUT];
  static unsigned int input_pos = 0;
  switch (inByte) {
    case '\n':   // end of text
      input_line [input_pos] = 0;  // terminating null byte
      if (input_pos == 0)
        return true;   // got blank line
      // terminator reached! process input_line here ...
      processData (input_line);
      // reset buffer for next time
      input_pos = 0;  
      break;

    case '\r':   // discard carriage return
      break;

    default:
      // keep adding if not full ... allow for terminating null byte
      if (input_pos < (MAX_INPUT - 1))
        input_line [input_pos++] = inByte;
      break;
  }  // end of switch
  return false;    // don't have a blank line yet
} // end of processIncomingByte  