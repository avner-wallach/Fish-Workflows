String inString = "";    // string to hold input
int inValue;
int Twidth;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(57600);
  analogReadResolution(12);
  analogWriteResolution(12);
  Twidth=200;
  analogWrite(DAC1,0);  
  
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for native USB port only
//  }
//
//  // send an intro:
//  Serial.println("\n\nString toInt():");
//  Serial.println();
}

void loop() {
  // Read serial input:
  while (Serial.available() > 0) {
    int inChar = Serial.read();
    if (isDigit(inChar)) {
      // convert the incoming byte to a char and add it to the string:
      inString += (char)inChar;
//      Serial.print(inChar);
    }
    // if you get a newline, print the string, then the string's value:
    if (inChar == '\n') {
      inValue=inString.toInt();
      if(inValue<0) inValue=0;
      if(inValue>4095) inValue=4095;
      
      Serial.print("Value:");
      
      Serial.println(inValue);
      // clear the string for new input:
      inString = "";

     // send pulse to DAC
     analogWrite(DAC1,inValue);
     delayMicroseconds(Twidth);    
     analogWrite(DAC1,0);     
    }
  }
}
