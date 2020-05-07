/*     Simple Stepper Motor Control Exaple Code
 *      
 *  by Dejan Nedelkovski, www.HowToMechatronics.com
 *  
 */

// defines pins numbers
const int en_Pin = 0;
const int ms1Pin = 1;
const int ms2Pin = 2;
const int ms3Pin = 3;
const int rst_Pin = 4;
const int slp_Pin = 5;
const int stepPin = 6; 
const int dirPin = 7; 
const int LeftPin = 8;  //left and right manual commands
const int RightPin = 9;
const int LLEDPin = 13; // left and right LED control
const int RLEDPin = 12;
const int MLeftPin = 10; // left and right commands from master
const int MRightPin = 11;
int right = 0;
int left = 0;
//int serbuff = 0;
int dir = 0;

void setup() {
  // Sets the two pins as Outputs
  pinMode(en_Pin,OUTPUT); 
  pinMode(ms1Pin,OUTPUT); 
  pinMode(ms2Pin,OUTPUT); 
  pinMode(ms3Pin,OUTPUT); 
  pinMode(rst_Pin,OUTPUT); 
  pinMode(slp_Pin,OUTPUT); 
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  
  pinMode(LeftPin,INPUT);
  pinMode(RightPin,INPUT);
  pinMode(LLEDPin,OUTPUT);
  pinMode(RLEDPin,OUTPUT);
  pinMode(MLeftPin,INPUT);
  pinMode(MRightPin,INPUT);

  // enable 
  digitalWrite(en_Pin,LOW);
  //out of reset
  digitalWrite(rst_Pin,HIGH);
  //full speed
  digitalWrite(ms1Pin,LOW);
  digitalWrite(ms2Pin,LOW);
  digitalWrite(ms3Pin,LOW);
  //sleep
  digitalWrite(slp_Pin,LOW);

 //LED off
  digitalWrite(LLEDPin,LOW);
  digitalWrite(RLEDPin,LOW);

  // serial
//  Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps


}
void loop() {
  right = digitalRead(RightPin) | digitalRead(MRightPin); //read right turn command
  left = digitalRead(LeftPin) | digitalRead(MLeftPin); //read right turn command
//  if (Serial.available() > 0){ //command recieved 
//      // read the incoming byte:
//      serbuff = Serial.read();
//      if(serbuff == 'L'){left = 1;}
//      if(serbuff == 'R'){right = 1;}      
//  }
   
  if(right | left){ //command given    
    if(right)
    {
      dir = 1;
      digitalWrite(RLEDPin,HIGH);
    }
    else
    {
      dir=0;        
      digitalWrite(LLEDPin,HIGH);
    }
    digitalWrite(dirPin,dir); //  set motor direction

    //exit sleep mode
    digitalWrite(slp_Pin,HIGH);
    delayMicroseconds(1000); 
    
    // Makes 200 pulses for making one full cycle rotation
    for(int x = 0; x < 200; x++) {
      digitalWrite(stepPin,HIGH); 
      delayMicroseconds(2000); 
      digitalWrite(stepPin,LOW); 
      delayMicroseconds(2000); 
    }
    while((digitalRead(RightPin) | digitalRead(MRightPin))&dir | (digitalRead(LeftPin) | digitalRead(MLeftPin))&~dir )
    {
      delay(500); 
    }
    //sleep
    digitalWrite(slp_Pin,LOW);
    digitalWrite(LLEDPin,LOW);
    digitalWrite(RLEDPin,LOW);
  }
  delayMicroseconds(1000); 
  
}
