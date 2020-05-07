unsigned int Twidth,Amin,Amax,Tper,randNumber,DoutPin; 
    
void setup() {
  DoutPin = 13;

  Serial.begin(57600);
  // put your setup code here, to run once:
//  REG_ADC_MR = (REG_ADC_MR & 0xFFF0FFFF) | 0x00020000;
//  REG_ADC_MR = (REG_ADC_MR & 0xFFFFFF0F) | 0x00000080; //enable FREERUN mode  
  analogReadResolution(12);
  analogWriteResolution(12);
  Twidth=200; //pulse width microsec
  Tper=25; //inter-pulse interval, ms
  analogWrite(DAC1,0);  
  pinMode(DoutPin, OUTPUT);
  Amax=4095;  
  Amin=0;
  digitalWrite(DoutPin, LOW);    // turn the LED off by making the voltage LOW

  randomSeed(analogRead(0));
  
}

void loop() {
  
  // wait for start:
  if (Serial.available() > 0) {
    int inChar = Serial.read();
    if (inChar=='S') {

     //%start
     while(Serial.available()==0){
      randNumber = random(Amin, Amax); //randomize amp
      // send pulse to DAC
      analogWrite(DAC1,randNumber);
      digitalWrite(DoutPin, HIGH);
      delayMicroseconds(Twidth);    
      analogWrite(DAC1,0);
      digitalWrite(DoutPin, LOW);      
      delay(Tper);                       
     }          
    }
    if (inChar=='C') {

     //%start
     while(Serial.available()==0){
//      randNumber = random(Amin, Amax); //randomize amp
      // send pulse to DAC
      analogWrite(DAC1,Amax);
      digitalWrite(DoutPin, HIGH);
      delayMicroseconds(Twidth);    
      analogWrite(DAC1,0);
      digitalWrite(DoutPin, LOW);      
      delay(Tper);                       
     }          
    }
    
  }    
}
