const int rPin = 9;
const int gPin = 10;
const int bPin = 11;
const int potPinR = A0;
const int potPinG = A1;
const int potPinB = A2;
/*comment2*/
int potValueR;
int potValueG;
int potValueB;


void setup() {
  Serial.begin(9600);
  pinMode(rPin,OUTPUT);
  pinMode(gPin,OUTPUT);
  pinMode(bPin,OUTPUT);
  pinMode(potPinR,INPUT);
  pinMode(potPinG,INPUT);
  pinMode(potPinB,INPUT);
  
  

}
void loop() {
  potValueR = analogRead(potPinR);
  potValueG = analogRead(potPinG);
  potValueB = analogRead(potPinB );
  
  analogWrite(rPin,map(potValueR,0,1023,0,127));
  analogWrite(gPin,map(potValueG,0,1023,0,127));
  analogWrite(bPin,map(potValueB,0,1023,0,127));
  
}
