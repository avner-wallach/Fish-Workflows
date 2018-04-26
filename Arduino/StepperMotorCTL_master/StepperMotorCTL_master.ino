/*     Simple Stepper Motor Control Exaple Code
 *      
 *  by Dejan Nedelkovski, www.HowToMechatronics.com
 *  
 */

// defines pins numbers
const int LEDPin = 13;
const int MLeftPin = 10; // left and right commands from master
const int MRightPin = 11;
const int TrigUpPin = 9;
const int TrigDownPin = 8;
int serbuff = 0;

void setup() {
  // Sets pin direction
  pinMode(LEDPin,OUTPUT);
  pinMode(MLeftPin,OUTPUT);
  pinMode(MRightPin,OUTPUT);
  pinMode(TrigUpPin,OUTPUT);
  pinMode(TrigDownPin,OUTPUT);

 //LED off
  digitalWrite(LEDPin,LOW);
  digitalWrite(MLeftPin,LOW);
  digitalWrite(MRightPin,LOW);

  //resting position of object is DOWN
  digitalWrite(TrigUpPin,LOW);
  digitalWrite(TrigDownPin,HIGH);

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
          //switch to UP position
          digitalWrite(TrigUpPin,HIGH);
          digitalWrite(TrigDownPin,HIGH);
          delay(250);  //wait 0.1 sec
          //back to DOWN position
          digitalWrite(TrigUpPin,LOW);
          digitalWrite(TrigDownPin,LOW);
          break;
      }
        
  }
  
}
