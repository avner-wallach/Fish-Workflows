/*     Simple Stepper Motor Control Exaple Code
 *      
 *  by Dejan Nedelkovski, www.HowToMechatronics.com
 *  
 */

// defines pins numbers
const int LEDPin = 13; // mimic loop closing
const int AUXPin = 12; //auxilliary output- daylight control
const int MLeftPin = 10; // left and right commands from master
const int MRightPin = 11;
const int Trig1Pin = 9; //dynamic object control
const int Trig2Pin = 8; //
char serbuff = 0;
int flag = 0;
int randomnumber;
int inrange=0;

void setup() {
  // Sets pin direction
  pinMode(LEDPin,OUTPUT);
  pinMode(AUXPin,OUTPUT);
  pinMode(MLeftPin,OUTPUT);
  pinMode(MRightPin,OUTPUT);
  pinMode(Trig1Pin,OUTPUT);
  pinMode(Trig2Pin,OUTPUT);

 //LED off
  digitalWrite(LEDPin,LOW); // mimic disconnected
  digitalWrite(AUXPin,HIGH); // LOW = lights off; HIGH = lights on
  digitalWrite(MLeftPin,LOW);
  digitalWrite(MRightPin,LOW);

  //resting position of object is DOWN
  digitalWrite(Trig1Pin,LOW);
  digitalWrite(Trig2Pin,LOW); 

  // serial
  Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps

}
void loop() {
  if (Serial.available() > 0){ //command recieved 
      // read the incoming byte:
      Serial.readBytes(&serbuff,1);
      switch(serbuff)
      {
        case 'S': //start trial
          inrange=1;
          break;
        case 'E': //end trial
          if(flag==1)
          {
//              randomnumber=random(30,120);
//              delay(randomnumber*1000);  //delay between trials                  
              flag=0;
          }
          inrange=0;
          break;
        case 'D':
          //daylight on
          digitalWrite(AUXPin,HIGH);
          break;
        case 'N':
          //daylight off
          digitalWrite(AUXPin,LOW);
          break;          
      }
        
  }
  else
  {
    if(inrange==1)
    {
      if(flag==0)
      {
        digitalWrite(AUXPin,HIGH);   //opene lights
        delay(500); //.5s 
        digitalWrite(AUXPin,LOW);   //close lights
        delay(1000); //1s
        flag=1;
      }
      else
      {
        digitalWrite(AUXPin,HIGH);
        delay(50);
        digitalWrite(AUXPin,LOW);
        delay(50);
      }
    }
  }
      
  
}
