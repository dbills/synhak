#ifdef _PC
#include <mock_arduino.h>
#include <fake_serial.h>
#include <memory.h>
#endif

// hall effect pin assignemnts
const int HALL_A = 7;
const int HALL_B = 8;
const int HALL_C = 9;
// hall effect bit assignment
const int HALL_A_BIT = 1;
const int HALL_B_BIT = 2;
const int HALL_C_BIT = 4;

const int MOTOR_BRAKE = 4;
const int SPEED_PWN = 5;
// variable resistor input for motor speed control
const int SPEED_INPUT = 1;

typedef unsigned int hall_state;

struct speed_input {
  unsigned long last_read_time; // millis of last time
  unsigned int;                 // POT reading
};
// speed control is accomplished as a moving average of the speed inputs
// add 1/2 the delta between inputs on each sample
// until .5Delta is less than some minimum
// then assume the value of the speed input
const unsigned int SPEED_SAMPLE_TIME = 1000; // time between sampling speed input 
speed_input last_speed_input;

/*
  ring buffer or last three hall sensor reads
*/
hall_state hall_buffer[3];

/*
  your sensor wiring may vary.  
  you must have the sensor bit numbers set up properly so that
  the following is true:

  sensor bit numbers 
       1
     4   2

  define possible patterns for one comlete
  revolution of motor
 */
typedef hall_state direction_table[3][3];

direction_table forward_patterns={
  { 1,2,4},
  { 2,4,1},
  { 4,1,2}
};
direction_table reverse_patterns={
  { 1,4,2},
  { 4,2,1},
  { 2,1,4}
};
/*
  return true if motor is running in the direction
  specified by TABLE
*/
bool is_running(direction_table &table) {
  for(int i=0;i<3;i++) {
    if(memcmp(table[i],hall_buffer,sizeof(table[i]))==0)
      return true;
  }
  return false;
}

/*
  read hall effect sensors and return 
  integer value 1,2 or 4
  any other value is a fault of some sort
  as no two hall sensors should be on at the same
  time
 */
hall_state read_hall() {
  return 
    ( digitalRead(HALL_A) ? HALL_A_BIT : 0) +
    ( digitalRead(HALL_B) ? HALL_B_BIT : 0) +
    ( digitalRead(HALL_C) ? HALL_C_BIT : 0);
}

int read_speed_control() {
  analogRead(SPEED_INPUT);
}
int read_brake_control() {
  return 0;
}
int read_reset_button() {
}
void set_motor_speed() {
}
void set_counter() {
}

void setup() {
  Serial.begin(9600);
  Serial.println("hello");

  digitalWrite(HALL_C,1);

}
void loop() {


  //  read_speed_control();
  
  hall_state Hall_state = read_hall();
  int speed = read_speed_control();
  int brake = read_brake_control(); // this'll probably be wired straight to motor
  int reset = read_reset_button();

  set_motor_speed();
  set_counter();
  //  Serial.print("Hall=");
  //  Serial.println(Hall_state);
}

