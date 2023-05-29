int trig_pin=6;
int echo_pin=5;

double getDistance(){
  digitalWrite(6,LOW);
  delayMicroseconds(3);
  digitalWrite(6,HIGH);
  delayMicroseconds(5);
  digitalWrite(6,LOW);
  
  long duration = pulseIn(5,HIGH);
  long distanceCm = (duration*0.034)/2;
  return distanceCm;
}
  
void setup(){
  pinMode(7,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(5,INPUT);
  pinMode(4,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(2,OUTPUT);
  Serial.begin(9600);
}

void loop(){
  long distanceCm = getDistance();
  Serial.println(distanceCm);
  if(distanceCm>=150){
    digitalWrite(7,LOW);
    digitalWrite(4,HIGH);
    digitalWrite(3,LOW);
    digitalWrite(2,LOW);
    delay(50);
  }
  else if(distanceCm<150 && distanceCm>50){
    digitalWrite(7,HIGH);
    digitalWrite(4,LOW);
    digitalWrite(3,HIGH);
    digitalWrite(2,LOW);
    delay(100);
    digitalWrite(7,LOW);
    digitalWrite(4,LOW);
    digitalWrite(3,LOW);
    digitalWrite(2,LOW);
    delay(100);
  }
  else{   
    digitalWrite(4,HIGH);
    digitalWrite(3,HIGH);
    digitalWrite(2,HIGH);
    digitalWrite(7,HIGH);
    delay(distanceCm*10);
    digitalWrite(7,LOW);
    digitalWrite(4,LOW);
    digitalWrite(3,LOW);
    digitalWrite(2,LOW);
    delay(distanceCm*10);
  }
}