int AinPin = 3;
int ThPin = 0;
int DelayPin = 1;
int WidthPin = 2;

int PosPin = 10;
int NegPin = 11;
int BothPin = 12;
int DoutPin = 13;

long t0, t;
unsigned int Ain,Aref,Alast;
bool Mode;
bool Pos,Neg,Both;
unsigned int Tdelay,Twidth,Tref,Tcheck,T,T1; //delay,width refraction of pulse
int Adiffs;
int dA[3]; //array of diffs to find peak
    
void setup() {
  Serial.begin(57600);

  // put your setup code here, to run once:
  ADC->ADC_MR |= 0x80;  //set free running mode on ADC
  ADC->ADC_CHER = 0xF0; //enable ADC on pins A0, A1, A2, A3 (PA16=AD7, PA24=AD6, PA23=AD5, PA22=AD4) 0x80 for A0
  analogReadResolution(12);
  analogWriteResolution(12);

  pinMode(PosPin, INPUT);
  pinMode(NegPin, INPUT);
  pinMode(BothPin, INPUT);
  pinMode(DoutPin, OUTPUT);

  Tdelay=0;
  Twidth=1000;
  Tcheck=500; //check and output ref value every 500 ms
  Tref=50; //ms refraction
//  Amid=4096/2;  
  T=millis();
//  Serial.print("T: ");
//  Serial.println((float)T);     

  //get threshold
//  Aref= analogRead(ThPin); 
  while((ADC->ADC_ISR & 0x80)==0); // wait for conversion on AD7 (pin A0)
  Aref=ADC->ADC_CDR[7]; //get value of A0  
  Serial.print("Aref: ");
  Serial.println((float)Aref);     

  //get Ain
  while((ADC->ADC_ISR & 0x10)==0); // wait for conversion on AD4 (pin A3) // previously 0x80 = A0  
  Ain=ADC->ADC_CDR[4]; //get value of A3
  Serial.print("Ain: ");
  Serial.println((float)Ain);       
  Alast=Ain;

  //get Delay
  while((ADC->ADC_ISR & 0x60)==0); // wait for conversion on AD6 (pin A1) 
  Tdelay=ADC->ADC_CDR[6]; //get value of A1
  Tdelay=Tdelay*20;
  Serial.print("Delay: ");
  Serial.println((float)Tdelay);       

  //get Width
  while((ADC->ADC_ISR & 0x40)==0); // wait for conversion on AD5 (pin A2) 
  Twidth=ADC->ADC_CDR[5]; //get value of A2
  Serial.print("Width: ");
  Serial.println((float)Twidth);       
  
  digitalWrite(DoutPin, LOW);    // turn the LED off by making the voltage LOW
  Mode=digitalRead(PosPin);
  Serial.print("Mode: ");
  Serial.println((float)Mode);         
}

void loop() {
  // put your main code here, to run repeatedly:
  while((ADC->ADC_ISR & 0x10)==0); // wait for conversion on AD4 (pin A3) // previously 0x80 = A0  
  Ain=ADC->ADC_CDR[4]; //get value of Ain3

  //Ain = analogRead(AinPin);    
  if ( (!Mode && Ain<Aref) || (Mode && Ain>Aref) )
  {
    // find extremum

    //shift register of diffs
/*    dA[2]=dA[1];
    dA[1]=dA[0];
    dA[0]=Ain-Alast;*/
    Adiffs=Ain-Alast;
//    Serial.print("Adiffs: ");
//    Serial.println((float)Adiffs);         
    if( (~Mode && Adiffs>=0) || (Mode && Adiffs<=0)) //sign change    
    {
      delayMicroseconds(Tdelay);    
      digitalWrite(DoutPin, HIGH);   // turn the LED on (HIGH is the voltage level)
      delayMicroseconds(Twidth);    
      digitalWrite(DoutPin, LOW);    // turn the LED off by making the voltage LOW
    
  
      // update params
//      Aref= analogRead(ThPin);
      while((ADC->ADC_ISR & 0x80)==0); // wait for conversion on AD7 (pin A0)
      Aref=ADC->ADC_CDR[7]; //get value of Ain3

//      Tdelay = analogRead(DelayPin)*20;
//      Twidth = analogRead(WidthPin);
      //get Delay
      while((ADC->ADC_ISR & 0x60)==0); // wait for conversion on AD6 (pin A1) 
      Tdelay=ADC->ADC_CDR[6]; //get value of A1
      Tdelay=Tdelay*20;

      //get Width
      while((ADC->ADC_ISR & 0x40)==0); // wait for conversion on AD5 (pin A2) 
      Twidth=ADC->ADC_CDR[5]; //get value of A2

//      Mode=digitalRead(PosPin);
      delay(Tref);   
      T=millis(); 
    }
     
  } 
  Alast=Ain;  

  //ref watchdog
  if(millis()-T>Tcheck)
  {
      while((ADC->ADC_ISR & 0x80)==0); // wait for conversion on AD7 (pin A0)
      Aref=ADC->ADC_CDR[7]; //get value of Ain3    
  }
  
}
