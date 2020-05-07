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
  REG_ADC_MR = (REG_ADC_MR & 0xFFF0FFFF) | 0x00020000;
  REG_ADC_MR = (REG_ADC_MR & 0xFFFFFF0F) | 0x00000080; //enable FREERUN mode  
  analogReadResolution(12);
  analogWriteResolution(12);

  pinMode(PosPin, INPUT);
  pinMode(NegPin, INPUT);
  pinMode(BothPin, INPUT);
  pinMode(DoutPin, OUTPUT);

  Tdelay=0;
  Twidth=1000;
  Tcheck=500; //check and output ref value every 500 ms
  Tref=10; //ms refraction
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
  t0 = micros();

  Ain = analogRead(AinPin);
  Pos = Ain>Aref;
  
  if ( Pos )
  {
//    Tdelay = analogRead(DelayPin);
//    Twidth = analogRead(WidthPin);
    delayMicroseconds(Tdelay);    
    digitalWrite(DoutPin, HIGH);   // turn the LED on (HIGH is the voltage level)
    delayMicroseconds(Twidth);    
    digitalWrite(DoutPin, LOW);    // turn the LED off by making the voltage LOW
    delayMicroseconds(Tref*1000);    
  } 
  else
  { 
//    T1=millis();
    
//    if ((T1-T)>=Tcheck)
//    {
//      T=T1;
//      Aref= analogRead(ThPin);
//      Tdelay = analogRead(DelayPin);
//      Twidth = analogRead(WidthPin);
//      Serial.print("Delay (us): ");
//      Serial.println(float(Tdelay));
//      Serial.print("Width (us): ");
//      Serial.println(float(Twidth));
//      
//    }
  }
//  t=micros()-t0;
//  Serial.print("Time per sample: ");
//  Serial.println((float)t);
  
}
