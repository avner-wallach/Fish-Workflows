int EODPin = 13; // EOD TTL input pin
int ModePin = 12; //mode output pin
int ProbePin = 11; //probe NI output pin
int GProbePin = 10; //globa probe output pin

int ttlstate=0;
unsigned int Ain0,Ain1,Aout0,Aout1;
unsigned int Tdelay,Tgdelay,Twidth,Tref; //delay from command, delay between local and global,width refraction of pulse
unsigned int n,Nprobe;
unsigned int Amax,Amin;
float Afl,Arange,V0,V1,Vm0,Vm1,Vs0,Vs1;
float Min0,Min1,Range0,Range1;
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
  Tdelay=4.5*1000; //us delay between command and mimic
  Tgdelay=500; //us between local and global
  Twidth=200;   //us width of mimic
  Tref=5; //ms refraction

  //input range as measured
  Amax=3090;
  Amin=570;
  Arange=float(Amax-Amin);

  //switching and probing
  Tswitch_coupling=20*60*1000; //switch period time, ms
  Tswitch_probing=5*60*1000; //switch period time, ms  
  Tprobe=3*60*1000; //time pre switch for probinbg, ms

  //output paramteres
  //local
  Vm0=1.5; //mean for local, V
  Vs0=0.35; //+-change in local, V  //Vm0+Vs0 must be smaller than 2.2V

  //global
  Vm1=.5;    //mean for global, V
  Vs1=0.35; //+-change in global, V //Vm1+Vs1 must be smaller than 2.2V

  //output mins and ranges
  Min0=Vm0*(1-Vs0);
  Min1=Vm1*(1-Vs1);
  Range0=Vm0*Vs0*2;
  Range1=Vm1*Vs1*2;
  
  tprobe=0;
  Tswitch=Tswitch_coupling;
  tswitch=Tswitch;
  t0=millis();
  Nprobe=0; //skip stim every 40 EODs to probe negative image
  n=0;
  mode=false;
  local=1;
  llevel=1; //local on middle
  global=1;
  digitalWrite(ModePin,LOW);
  digitalWrite(ProbePin,LOW);
  digitalWrite(GProbePin,LOW);  
}

void loop() {

  //wait for EOD detection
  while(ttlstate==0)
  {
    ttlstate = digitalRead(EODPin);
  }
  teod=micros(); //time of eod detection
//  n=n+1;

  //read analog signal
  while((ADC->ADC_ISR & 0x80)==0); // wait for conversion on AD7 (pin A0)  
  Ain0=ADC->ADC_CDR[7]; //get value of Ain0
  Afl=float(Ain0-Amin)/Arange;

  //set output levels
  if(mode)
  {
      if(local==1)
      {
        Aout0=(Afl*Range0+Min0)/2.2*4096;
      }
      else
      {
        switch(llevel)
        {
          case 0:
            Aout0=(Min0)/2.2*4095;
            break;
          case 1:
            Aout0=(0.5*Range0+Min0)/2.2*4095;
            break;
          case 2:
            Aout0=(Range0+Min0)/2.2*4095;
            break;            
        }        
      }
      if(global==1)
      {
        Aout1=((1-Afl)*Range1+Min1)/2.2*4095;
       //Aout1=(Min1)/2.2*4095;
      }
      else
      {
        Aout1=(0.5*Range1+Min1)/2.2*4095;
      }
      
  }
  else
  {
      if(local==1)
      {
        Aout0=((1-Afl)*Range0+Min0)/2.2*4095;
      }
      else
      {
        switch(llevel)
        {
          case 0:
            Aout0=(Min0)/2.2*4095;
            break;
          case 1:
            Aout0=(0.5*Range0+Min0)/2.2*4095;
            break;
          case 2:
            Aout0=(Range0+Min0)/2.2*4095;
            break;            
        }        
      }
      if(global==1)
      {
        Aout1=(Afl*Range1+Min1)/2.2*4095;
//        Aout1=(Range1+Min1)/2.2*4095;      
      }
      else
      {
        Aout1=(0.5*Range1+Min1)/2.2*4095;
      }
  }    

  //delay from command to EOD
/*  if(n==Nprobe) //skip stim, reset counter
  {
    while(micros()<(teod+Tdelay+Tprobe));
    n=0;
  }
  else
  {*/
    while(micros()<(teod+Tdelay));
  //}

  //produce mimic commands
  if(Tgdelay==0)
  {
    analogWrite(DAC0,Aout0);  
    analogWrite(DAC1,Aout1);  
    delayMicroseconds(Twidth);
    analogWrite(DAC0,0);  
    analogWrite(DAC1,0);    
  }
  else
  {
    analogWrite(DAC0,Aout0);  
    delayMicroseconds(Twidth);
    analogWrite(DAC0,0);  

    delayMicroseconds(Tgdelay); //delay between local and global
    analogWrite(DAC1,Aout1);  
    delayMicroseconds(Twidth);
    analogWrite(DAC1,0);        
  }

  //context switching
  t=millis()-t0; //time from last switch
  if(tprobe>0)
  {
    if(t>=(tswitch-tprobe/2) and t<(tswitch) and local==1)
    {
      //probe on
      digitalWrite(ProbePin,local); //output probe       
      local=0;
    }
    if(t>=tprobe/2 and t<tswitch-tprobe/2 and local==0)
    {
      //probe off
      digitalWrite(ProbePin,local); //output probe       
      local=1;
    }                
  }
  if(t>=tswitch)
  {
    mode=not(mode); //switch mode
    digitalWrite(ModePin,mode); //output mode
    t0=t0+tswitch; //update switch time
  }      

  //refractory period
  delayMicroseconds(Tref*1000);    
  ttlstate=0; //  reset eod flag
  Aout0=0;
  Aout1=0;

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
      case 'H': //local probing middle
        llevel=0;
        Serial.print("Local Level: ");
        Serial.println((float)llevel);          
        break;
      case 'G': //toggle global
        digitalWrite(GProbePin,global); //output probe       
        global=1-global;
        Serial.print("Global: ");
        Serial.println((float)global);            
        break;
      case 'F': //toggle global + local
        digitalWrite(GProbePin,global); //output probe       
        global=1-global;
        digitalWrite(ProbePin,local); //output probe       
        local=1-local;
        Serial.print("Global: ");
        Serial.println((float)global);            
        Serial.print("Local: ");
        Serial.println((float)local);          
        break;
      case 'S': //switch mode
        mode=not(mode);
        digitalWrite(ModePin,mode); //output mode       
        Serial.print("Mode: ");
        Serial.println((float)mode);  
        t0=millis(); //reset clock           
        break;       
      case 'P': //start auto probing 
        Tswitch=Tswitch_probing;
        tswitch=Tswitch;
        tprobe=Tprobe; //time pre switch for probinbg        
        Serial.print("Probing on \n");
        break;        
      case 'C': //stop auto probing (coupling)
        Tswitch=Tswitch_coupling;
        tswitch=Tswitch;
        tprobe=0; //time pre switch for probinbg        
        Serial.print("Probing off \n");
        break;  
      case 'I': //stop cycling modes
        tswitch=24*60*60*1000;
        Serial.print("Cycling off \n");      
        break;
      case 'T': //cycle modes
        tswitch=Tswitch;
        Serial.print("Cycling on \n");      
        break;
    }
  }
    
}
