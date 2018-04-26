int AinPin = 3;
int ThPin = 0;
int DelayPin = 1;
int WidthPin = 2;

int PosPin = 10;
int NegPin = 11;
int BothPin = 12;
int DoutPin = 13;

long t0, t;
unsigned int Ain,Aref,Aref2,Amid;
bool Pos,Neg,Both;
unsigned int Tdelay,Twidth,Tref,Tcheck,T,T1; //delay,width refraction of pulse
    
void setup() {
  Serial.begin(57600);

  // put your setup code here, to run once:
//  REG_ADC_MR = (REG_ADC_MR & 0xFFF0FFFF) | 0x00020000;
//  REG_ADC_MR = (REG_ADC_MR & 0xFFFFFF0F) | 0x00000080; //enable FREERUN mode  
  analogReadResolution(12);
  analogWriteResolution(12);

  pinMode(PosPin, INPUT);
  pinMode(NegPin, INPUT);
  pinMode(BothPin, INPUT);
  pinMode(DoutPin, OUTPUT);

  Tdelay=5000;
  Twidth=5000;
  Tcheck=500; //check and output ref value every 500 ms
  Tref=50; //ms refraction
  Amid=4096/2;  
  T=millis();
  Serial.print("T: ");
  Serial.println((float)T);     
  
  Aref= analogRead(ThPin);
  Aref2=2*Amid-Aref;
  Serial.print("Aref: ");
  Serial.println((float)Aref);     
  analogWrite(DAC1,Aref);
  digitalWrite(DoutPin, LOW);    // turn the LED off by making the voltage LOW
  
}

void loop() {
  // put your main code here, to run repeatedly:
//  t0 = micros();

  Ain = analogRead(AinPin);
  Pos = digitalRead(PosPin) && Ain>Aref;
  Neg = digitalRead(NegPin) &&  Ain<Aref;  
  Both = digitalRead(BothPin) && ( (Aref>Amid && (Ain>Aref || Ain<Aref2)) || (Aref<Amid && (Ain>Aref2 || Ain<Aref)) );
  
  if ( Pos || Neg || Both )
  {
//    Tdelay = analogRead(DelayPin);
//    Twidth = analogRead(WidthPin);
    delayMicroseconds(Tdelay);    
    digitalWrite(DoutPin, HIGH);   // turn the LED on (HIGH is the voltage level)
    delayMicroseconds(Twidth);    
    digitalWrite(DoutPin, LOW);    // turn the LED off by making the voltage LOW
    delayMicroseconds(Tref);    
  } 
  else
  { 
    T1=millis();
    
    if ((T1-T)>=Tcheck)
    {
      T=T1;
      Aref= analogRead(ThPin);
      Tdelay = analogRead(DelayPin)*20;
      Twidth = analogRead(WidthPin);
      Both = digitalRead(BothPin);
      if(Both)          
      {
        Aref2=2*Amid-Aref;
        analogWrite(DAC1,Aref2);
        Serial.print("Aref2: ");
        Serial.println((float)Aref2);     
      }
    }
  }
/*  if ( Ain>Aref && Pos)// | Ain<(Amid-Aref)) % add for opposite polarity
  {
    Tdelay = analogRead(DelayPin);
    Twidth = analogRead(WidthPin);
    delayMicroseconds(Tdelay);    
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delayMicroseconds(Twidth);    
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    //while(analogRead(AinPin)>=Amid);//analogRead(ArefPin));
    delayMicroseconds(Tref);    
  }  
  else if (Ain<Aref && Neg)
  {
    Tdelay = analogRead(DelayPin);
    Twidth = analogRead(WidthPin);
    delayMicroseconds(Tdelay);    
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delayMicroseconds(Twidth);    
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    //while(analogRead(AinPin)>=Amid);//analogRead(ArefPin));
    delayMicroseconds(Tref);        
  }
  else if (Both && ( (Aref>Amid && (Ain>Aref || Ain<(2*Amid-Aref))) || (Aref<Amid && (Ain>(2*Amid-Aref) || Ain<Aref)) ))
  {
    Tdelay = analogRead(DelayPin);
    Twidth = analogRead(WidthPin);
    delayMicroseconds(Tdelay);    
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delayMicroseconds(Twidth);    
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    //while(analogRead(AinPin)>=Amid);//analogRead(ArefPin));
    delayMicroseconds(Tref);        
  }
  */
//  t=micros()-t0;
//  Serial.print("Time per sample: ");
//  Serial.println((float)t);
  
}
