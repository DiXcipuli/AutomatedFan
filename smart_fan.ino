// smart_fan.ino
// Controlling 3 fan (propeller speed, and post rotation) through a rf remote

#include <Stepper.h>

// Specifications for the 28byj-48 stepper motor
const int STEPS_PER_REV = 32;  
const int GEAR_RED = 64;
const int STEPS_PER_OUT_REV = STEPS_PER_REV * GEAR_RED;
const int post_rotation_speed = 500;
const int delay_between_cycles = 2000;
// From which value we consider it as a HIGH state.
const int analog_voltage_threshold = 900;
  
// Number of Steps Required
int StepsRequired;

// input pins def
#define input_pin_1 A4
#define input_pin_2 A5
#define input_pin_3 A6
#define input_pin_4 A7
#define pwm_pin 10

// Those pins will define the dc motors rotation direction. On pin will be set to 1 and the other to 0
#define dc_motor_dir_pin_1 11
#define dc_motor_dir_pin_2 12

// Stepper motor 1 pins definition
#define stepper_motor_1_pin_1 2
#define stepper_motor_1_pin_2 4
#define stepper_motor_1_pin_3 3
#define stepper_motor_1_pin_4 5

// Stepper motor 2 pins definition
#define stepper_motor_2_pin_1 6
#define stepper_motor_2_pin_2 8
#define stepper_motor_2_pin_3 7
#define stepper_motor_2_pin_4 9

// Stepper motor 3 pins definition
#define stepper_motor_3_pin_1 A0
#define stepper_motor_3_pin_2 A2
#define stepper_motor_3_pin_3 A1
#define stepper_motor_3_pin_4 A3

// variables to store input values
int input_value_1 = 0;
int input_value_2 = 0;
int input_value_3 = 0;
int input_value_4 = 0;

int previous_input_value_1 = 0;
int previous_input_value_2 = 0;
int previous_input_value_3 = 0;
int previous_input_value_4 = 0;

// Variables to store the states of the fan
bool are_fan_running = False;
bool are_post_rotating = false;
bool is_post_rotation_cw = true;

// Defines different speeds for the fans to run at.
int fan_speed[] = {50, 100, 150, 200, 255};
int current_fan_speed = 4;

// Defines an array of stepper motor, change pins if needed
Stepper stepperArray[] = {Stepper(STEPS_PER_REV, stepper_motor_1_pin_1, stepper_motor_1_pin_2, stepper_motor_1_pin_3, stepper_motor_1_pin_4),
                           Stepper(STEPS_PER_REV, stepper_motor_2_pin_1, stepper_motor_2_pin_2, stepper_motor_2_pin_3, stepper_motor_2_pin_4),
                            Stepper(STEPS_PER_REV, stepper_motor_3_pin_1, stepper_motor_3_pin_2, stepper_motor_3_pin_3, stepper_motor_3_pin_4)};


void setup() {
  pinMode(input_pin_1, INPUT);
  pinMode(input_pin_2, INPUT);
  pinMode(input_pin_3, INPUT);
  pinMode(input_pin_4, INPUT);
  pinMode(pwm_pin, OUTPUT);
  pinMode(dc_motor_dir_pin_1, OUTPUT);
  pinMode(dc_motor_dir_pin_2, OUTPUT);

  digitalWrite(dc_motor_dir_pin_1, LOW);
  digitalWrite(dc_motor_dir_pin_2, HIGH);
  analogWrite(pwm_pin, 0);
  
  Serial.begin(9600);
}

void loop() {
  // update input values
  input_value_1 = digitalRead(input_pin_1);
  input_value_2 = digitalRead(input_pin_2);
  input_value_3 = analogRead(input_pin_3);
  input_value_4 = analogRead(input_pin_4);

  if(input_value_1 == HIGH && previous_input_value_1 == LOW)
    are_fan_running = !are_fan_running;

  if(input_value_2 == HIGH && previous_input_value_2 == LOW)
    are_post_rotating = !are_post_rotating;


  if(are_fan_running)
    analogWrite(pwm, fan_speed[current_fan_speed]);

  else{
    analogWrite(pwm, 0);
  }    

  if(are_post_rotating){
    if(is_post_rotation_cw)
      StepsRequired = STEPS_PER_OUT_REV / 10; 
    else{
      StepsRequired = - STEPS_PER_OUT_REV / 10; 
    }

    for ( auto motor : stepperArray){
      motor.setSpeed(post_rotation_speed);
      // step() is a blocking function  
      motor.step(StepsRequired);
      // Then, we release the motor, as the position can be kept without keeping the motors active.
      digitalWrite(stepper_motor_1_pin_1, LOW);
      digitalWrite(stepper_motor_1_pin_2, LOW);
      digitalWrite(stepper_motor_1_pin_3, LOW);
      digitalWrite(stepper_motor_1_pin_4, LOW);
      digitalWrite(stepper_motor_2_pin_1, LOW);
      digitalWrite(stepper_motor_2_pin_2, LOW);
      digitalWrite(stepper_motor_2_pin_3, LOW);
      digitalWrite(stepper_motor_2_pin_4, LOW);
      digitalWrite(stepper_motor_3_pin_1, LOW);
      digitalWrite(stepper_motor_3_pin_2, LOW);
      digitalWrite(stepper_motor_3_pin_3, LOW);
      digitalWrite(stepper_motor_3_pin_4, LOW);
    }

    is_post_rotation_cw = !is_post_rotation_cw;
    delay(delay_between_cycles);
  }

  // Increase fan speed
  if(input_value_3 > analog_voltage_threshold && previous_input_value_3 < analog_voltage_threshold){
    current_fan_speed ++;
    if(current_fan_speed > 4){
      current_fan_speed = 0;
    }
  }

  // Decrease fan speed
  if(input_value_4 > analog_voltage_threshold && previous_input_value_4 < analog_voltage_threshold){
    current_fan_speed --;
    if(current_fan_speed < 0){
      current_fan_speed = 4;
    }
  }

  // Update the previous inputs
  previous_input_value_1 = input_value_1;
  previous_input_value_2 = input_value_2;
  previous_input_value_3 = input_value_3;
  previous_input_value_4 = input_value_4;
}
