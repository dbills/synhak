// define _PC when *not* compiling on Arduino
#ifdef _PC
#include <mock_arduino.h>
#include <fake_serial.h>
#include <memory.h>
#endif

#include "defines.h"

direction_table_t forward_patterns={
  { 1,2,4},
  { 2,4,1},
  { 4,1,2}
};
direction_table_t reverse_patterns={
  { 1,4,2},
  { 4,2,1},
  { 2,1,4}
};
/*
   return true if motor is running in the direction
  specified by TABLE
*/
bool is_running(const direction_table_t &table) {
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
hall_state_t read_hall() {
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
  
  hall_state_t Hall_state = read_hall();
  int speed = read_speed_control();
  int brake = read_brake_control(); // this'll probably be wired straight to motor
  int reset = read_reset_button();

  set_motor_speed();
  set_counter();
  //  Serial.print("Hall=");
  //  Serial.println(Hall_state);
}


