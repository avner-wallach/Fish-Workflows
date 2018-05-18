/*     Simple Stepper Motor Control Exaple Code
 *      
 *  by Dejan Nedelkovski, www.HowToMechatronics.com
 *  
 */

// defines pins numbers
const int LEDPin = 13;
const int AUXPin = 12; //auxilliary output- currently unused
const int MLeftPin = 10; // left and right commands from master
const int MRightPin = 11;
const int Trig1Pin = 9;
const int Trig2Pin = 8;
int serbuff = 0;

void setup() {
  // Sets pin direction
  pinMode(LEDPin,OUTPUT);
  pinMode(MLeftPin,OUTPUT);
  pinMode(MRightPin,OUTPUT);
  pinMode(Trig1Pin,OUTPUT);
  pinMode(Trig2Pin,OUTPUT);

 //LED off
  digitalWrite(LEDPin,LOW);
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
      serbuff = Serial.read();
      switch(serbuff)
      {
        case 'B':
          digitalWrite(LEDPin,HIGH);
          delay(200);
          digitalWrite(LEDPin,LOW);
          break;
        case 'L':
          digitalWrite(MLeftPin,HIGH);
          delay(5);
          digitalWrite(MLeftPin,LOW);
          break;
        case 'R':
          digitalWrite(MRightPin,HIGH);
          delay(5);
          digitalWrite(MRightPin,LOW);
          break;
        case 'T':
          //trigger relay1
          digitalWrite(Trig1Pin,HIGH);
          delay(250);  //wait 0.25 sec
          //back 
          digitalWrite(Trig1Pin,LOW);
          break;
        case 'G':
          //trigger relay2
          digitalWrite(Trig2Pin,HIGH);
          delay(250);  //wait 0.25 sec
          //back 
          digitalWrite(Trig2Pin,LOW);
          break;
      }
        
  }
  
}
