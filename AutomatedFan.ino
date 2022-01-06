#define remote_pin_1 A4
#define remote_pin_2 A5
#define remote_pin_3 A6
#define remote_pin_4 A7

int input_1 = 0;
int input_2 = 0;
int input_3 = 0;
int input_4 = 0;

int previous_input_1 = 0;
int previous_input_2 = 0;
int previous_input_3 = 0;
int previous_input_4 = 0;

int mode = 0;

int pwm = 10;

void setup() {
  // put your setup code here, to run once:
  pinMode(remote_pin_1, INPUT);
  pinMode(remote_pin_2, INPUT);
  pinMode(remote_pin_3, INPUT);
  pinMode(remote_pin_4, INPUT);
  pinMode(pwm, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);

  digitalWrite(11, LOW);
  digitalWrite(12, HIGH);
  analogWrite(pwm, 250);
  
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  input_1 = digitalRead(remote_pin_1);
  input_2 = digitalRead(remote_pin_2);
  input_3 = analogRead(remote_pin_3);
  input_4 = analogRead(remote_pin_4);

/*
  if(a0_input != previous_a0_input && a0_input == HIGH){
    mode = ! mode;
  }


  switch(mode){
  case 0:
    analogWrite(3, 0);
  break;

  case 1:
    analogWrite(3, 250);
    Serial.println("lll");
  break;
    
  }*/

  Serial.print("A4");
  Serial.print(input_1);
  Serial.print("\t");

  Serial.print("A5");
  Serial.print(input_2);
  Serial.print("\t");

  Serial.print("A6");
  Serial.print(input_3);
  Serial.print("\t");

  Serial.print("A7");
  Serial.println(input_4);
  //Serial.println(mode);

  //analogWrite(pwm, 225);


  //previous_a0_input = a0_input;
}
