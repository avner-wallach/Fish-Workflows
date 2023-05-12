int EODPin = 13; // EOD TTL input pin
int ModePin = 12; //mode output pin

int ttlstate=0;
unsigned int Ain0,Ain1,Aout0,Aout1;
unsigned int Tdelay,Twidth,Tref; //delay,width refraction of pulse
unsigned int n,Nprobe,Tprobe;
long t0,t,Tswitch;
bool mode;
    
void setup() {
  Serial.begin(57600);

  // put your setup code here, to run once:
  ADC->ADC_MR |= 0x80;  //set free running mode on ADC
  ADC->ADC_CHER = 0x80; //enable ADC on pin A0 (PA16=AD7) prev. 0x90?
  //REG_ADC_MR = (REG_ADC_MR & 0xFFF0FFFF) | 0x00020000;
//  REG_ADC_MR = (REG_ADC_MR & 0xFFFFFF0F) | 0x00000080; //enable FREERUN mode  
  analogReadResolution(12);
  analogWriteResolution(12);

  pinMode(EODPin, INPUT);
  pinMode(ModePin, OUTPUT);

  Tdelay=4*1000; //us delay
  Tprobe=46*1000; //additional us delay at probes (total 50ms)
  Twidth=200;   //us width
  Tref=5; //ms refraction

  Tswitch=20*60*1000; //switch every 20 minutes
  Nprobe=40; //skip stim every 40 EODs to probe negative image
  t0=millis();
  n=0;
  mode=false;
  digitalWrite(ModePin,LOW);
}

void loop() {

  //wait for EOD detection
  while(ttlstate==0)
  {
    ttlstate = digitalRead(EODPin);
  }
  ttlstate=0;
  n=n+1;
  t=millis()-t0; //time from last switch
  if(t>=Tswitch)
  {
    mode=not(mode); //switch mode
    digitalWrite(ModePin,mode); //output mode
    t0=t0+Tswitch; //update switch time
  }
  delayMicroseconds(Tdelay);    
  while((ADC->ADC_ISR & 0x80)==0); // wait for conversion on AD7 (pin A0)  
  Ain0=ADC->ADC_CDR[7]; //get value of Ain0
  if(n==Nprobe) //skip stim, reset counter
  {
/*    Aout0=0;
    Aout1=0;*/
    delayMicroseconds(Tprobe);
    n=0;
  }
  else
  {
    if(mode)
    {
      Aout0=Ain0; //in phase stim
      Aout1=4096-Ain0; //out of phase stim    
    }
    else
    {
      Aout1=Ain0; //in phase stim
      Aout0=4096-Ain0; //out of phase stim          
    }    
  }
  analogWrite(DAC0,Aout0);  
  analogWrite(DAC1,Aout1);  
  delayMicroseconds(Twidth);
  analogWrite(DAC0,0);  
  analogWrite(DAC1,0);  
  
  delayMicroseconds(Tref*1000);    
  
  
}
