int light_pin = A0;

void setup(){
  Serial.begin(9600);
  pinMode(7, OUTPUT);
  pinMode(light_pin, INPUT);
}

void loop(){
 int abe = analogRead(light_pin);
 Serial.println(abe);
  if(abe<=500){
    digitalWrite(7, HIGH);
    delay(100);
  }
  else{
    digitalWrite(7, LOW);
  }
 delay(100);
}