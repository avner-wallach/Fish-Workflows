int MotorPin = 11; // motor on/off pin (latch)
int ModePin = 12; //mode on/off pin (toggle)
int PowerPin = 13; //pin for high

unsigned int Aout0,Aout1;
unsigned int dt,n,m,t;
float f,v,u; //sine freq
unsigned int min0,max0,min1,max1;
bool mode,motor,mpin,flag;
    
void setup() {
  Serial.begin(57600);

  analogWriteResolution(12);

  pinMode(MotorPin, INPUT);
  pinMode(ModePin, INPUT);
  pinMode(PowerPin, OUTPUT);
  digitalWrite(PowerPin,1);
  
  min0=(.5/3)*4096;
  max0=(2.5/3)*4096;
  min1=(.4/3)*4096;
  max1=(.6/3)*4096;  

  n=0;
  m=0;
  dt=500; //us between samples
  mode=false;
  motor=true;
  flag=false;
  Serial.print("motor: ");
  Serial.println(digitalRead(MotorPin));       
  Serial.print("mode: ");
  Serial.println(digitalRead(ModePin));       
  Serial.print("Starting loop!\n");
  f=0.25;
}

void loop() {

  t=micros();

  //read toggle switch
  if(digitalRead(ModePin)==1 and flag==false)
  {
    mode=!mode; //toggle mode
//    Serial.print("mode: ");
//    Serial.println(mode);     
    flag=true;
  }
  else if(digitalRead(ModePin)==0 and flag==true)
  {
    flag=false;
  }

  v=cos(2*PI*f*dt/1000000*n)/2+0.5;
  u=cos(2*PI*f*dt/1000000*m)/2+0.5;  
  Aout0=int(v*(max0-min0)+min0);
  Aout1=int(u*(max1-min1)+min1);
  mpin=digitalRead(MotorPin);
  if(!motor and mpin) //turn on motor
  {
    motor=true;
    n=m; //to avoid motor jerks  
//    Serial.print("motor: ");
//    Serial.println(motor);     
  }
  if(motor and !mpin) //motor off
  {
    motor=false;
//    Serial.print("motor: ");
//    Serial.println(motor);         
  }
  
  //wait timestep
  while((micros()-t)<dt);

  if(mode)
  {
    analogWrite(DAC0,Aout0);
    Serial.println(Aout1);  
    if(motor)
    {
      analogWrite(DAC1,Aout1);  
      m++;
    }
    n++;
  }
      
}
