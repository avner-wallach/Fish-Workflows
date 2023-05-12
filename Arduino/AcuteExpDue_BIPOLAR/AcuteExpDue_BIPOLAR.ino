int EODPin = 13; // EOD TTL input pin
int ModePin = 12; //mode output pin
int ProbePin = 11; //probe NI output pin
int GProbePin = 10; //globa probe output pin

int ttlstate=0;
unsigned int Ain0,Aout0;
unsigned int Tdelay,Tgdelay,Twidth,Tref; //delay from command, delay between local and global,width refraction of pulse
unsigned int n,Nprobe;
unsigned int Amax,Amin;
float Afl,Arange,V0,Vm0,Vs0;
float Min0,Range0,Mid0,Max0;
long t0,t,Tswitch,Tswitch_coupling,Tswitch_probing,teod,Tprobe,tprobe,tswitch;
bool mode;
unsigned int local,global,llevel;
char srbuff;
    
void setup() {
  Serial.begin(57600);

  // put your setup code here, to run once:
  ADC->ADC_MR |= 0x80;  //set free running mode on ADC
  ADC->ADC_CHER = 0x80; //enable ADC on pin A0 (PA16=AD7) prev. 0x90?
  analogReadResolution(12);
  analogWriteResolution(12);

  pinMode(EODPin, INPUT);
  pinMode(ModePin, OUTPUT);
  pinMode(ProbePin, OUTPUT);
  pinMode(GProbePin, OUTPUT);

  //parameters
  Tdelay=.5*1000; //us delay between command and context
  Tgdelay=2*1000; //us between command and local
  Twidth=200;   //us width of mimic
  Tref=5; //ms refraction

  //input range as measured
  Amax=4090;
  Amin=200;
  Arange=float(Amax-Amin);

  //switching and probing
  Tswitch_coupling=10*60*1000; //switch period time, ms
  Tswitch_probing=6*60*1000; //switch period time, ms  
  Tprobe=3*60*1000; //time pre switch for probinbg, ms

  //output paramteres
  //local
  Vm0=1.5; //mean for local, V
  Vs0=0.35; //+-change in local, V  //Vm0+Vs0 must be smaller than 2.2V

  //output mins and ranges
  Min0=Vm0*(1-Vs0);
  Range0=Vm0*Vs0*2;
  Mid0=0.5*Range0+Min0;
  Max0=Range0+Min0;
  
  tprobe=0;
  Tswitch=Tswitch_coupling;
  tswitch=Tswitch;
  t0=millis();
  Nprobe=0; 
  n=1;
  mode=false;
  local=1;
  llevel=1; //local on middle
  global=1;
  digitalWrite(ModePin,LOW);
  digitalWrite(ProbePin,LOW);
  digitalWrite(GProbePin,LOW);
  analogWrite(DAC0,(Mid0)/2.2*4095);  
  
}

void loop() {

  //wait for EOD detection
  while(ttlstate==0)
  {
    ttlstate = digitalRead(EODPin);
  }
  teod=micros(); //time of eod detection
  if(Nprobe>0) n=n+1;    

  //read analog signal
  while((ADC->ADC_ISR & 0x80)==0); // wait for conversion on AD7 (pin A0)  
  Ain0=ADC->ADC_CDR[7]; //get value of Ain0
  Afl=float(Ain0-Amin)/Arange;

  //set output levels
  if(mode)
  {
    Aout0=(Afl*Range0+Min0)/2.2*4096;
  }
  else
  {
    Aout0=((1-Afl)*Range0+Min0)/2.2*4095;
  }    


  //delay from command to EOD
  if(n==Nprobe or local==0) //skip stim, reset counter
  {
    switch(llevel)
    {
      case 0:
        Aout0=(Min0)/2.2*4095;
        break;
      case 1:
        Aout0=(Mid0)/2.2*4095;
        break;
      case 2:
        Aout0=(Max0)/2.2*4095;
        break;            
    }        
    if(Nprobe>0)
    {
      n=0;
      digitalWrite(ProbePin,1); //output probe             
    }
  }

  while(micros()<(teod+Tdelay));

  //produce mimic commands
  if(Tgdelay==0)
  {
    analogWrite(DAC0,Aout0);
    digitalWrite(GProbePin,mode);     
    while(micros()<(teod+Tdelay+Twidth));
    analogWrite(DAC0,(Mid0)/2.2*4095);  
    digitalWrite(GProbePin,LOW);         
  }
  else
  {
    digitalWrite(GProbePin,mode);     
    while(micros()<(teod+Tdelay+Twidth));
    digitalWrite(GProbePin,LOW);             

    while(micros()<(teod+Tgdelay));
      
    analogWrite(DAC0,Aout0);  
    while(micros()<(teod+Tgdelay+Twidth));
    analogWrite(DAC0,(Mid0)/2.2*4095);  
  }

  if(Nprobe>0 and n==0 and local==1)
  {
    digitalWrite(ProbePin,0); //output probe             
  }

 // Serial.println((float)Ain0);  


  //context switching
  t=millis()-t0; //time from last switch
  
  if(tprobe>0)
  {
    if(t>=(Tswitch-tprobe/2) and t<(Tswitch) and local==1)
    {
      //probe on
      digitalWrite(ProbePin,1); //output probe       
      local=0;
    }
    if(t>=tprobe/2 and t<Tswitch-tprobe/2 and local==0)
    {
      //probe off
      digitalWrite(ProbePin,0); //output probe       
      local=1;    
    }                
  }

  if(t>=Tswitch)
  {
    mode=not(mode); //switch mode
    digitalWrite(ModePin,mode); //output mode
    t0=t0+Tswitch; //update switch time
  }      

  //refractory period
  delayMicroseconds(Tref*1000);    
  ttlstate=0; //  reset eod flag

  //get serial commands
  if(Serial.available()>0)
  {
    srbuff = Serial.read();
    switch(srbuff)
    {
      case 'L': //toggle local  (probe on/off manually)
        digitalWrite(ProbePin,local); //output probe       
        local=1-local;
        Serial.print("Local: ");
        Serial.println((float)local);          
        break;
      case 'K': //local probing high
        llevel=2;
        Serial.print("Local Level: ");
        Serial.println((float)llevel);          
        break;
      case 'J': //local probing middle
        llevel=1;
        Serial.print("Local Level: ");
        Serial.println((float)llevel);          
        break;
      case 'H': //local probing low
        llevel=0;
        Serial.print("Local Level: ");
        Serial.println((float)llevel);          
        break;
      case 'S': //switch mode
        mode=not(mode);
        digitalWrite(ModePin,mode); //output mode       
        Serial.print("Mode: ");
        Serial.println((float)mode);  
        t0=millis(); //reset clock           
        break;       
      case 'P': //auto probing 
        Tswitch=Tswitch_probing;
        tprobe=Tprobe; //time pre switch for probinbg        
        Nprobe=0;
        n=1;
        Serial.print("Auto Probing on \n");
        break;        
      case 'C': //coupling
        Tswitch=Tswitch_coupling;
        tprobe=0; //time pre switch for probinbg        
        Nprobe=0;
        n=1;
        Serial.print("Pairing on \n");
        break;  
      case 'R': //sporadic probing
        Tswitch=Tswitch_probing;
        tprobe=0; //time pre switch for probinbg        
        Nprobe=10; //skip stim every 10 EODs to probe negative image
        n=0;
        Serial.print("Sporadic Probing on \n");
        break;
      case 'I': //no cycling (manual)
        Tswitch=24*60*60*1000;
        tprobe=0; //time pre switch for probinbg        
        Nprobe=0;
        n=1;
        Serial.print("Cycling off \n");      
        break;
    }
  }
    
}
