// define _PC when *not* compiling on Arduino
#ifdef _PC
#include <mock_arduino.h>
#include <fake_serial.h>
#include <memory.h>
#endif

#include "defines.h"
#define DELAYTIME 100
/*
  ring buffer or last three hall sensor reads
*/
hall_state_t hall_buffer[HALL_NUMBERS];

direction_table_t reverse_patterns={
  { 1,3,2,6,4,5},
  { 3,2,6,4,5,1},
  { 2,6,4,5,1,3},
  { 6,4,5,1,3,2},
  { 4,5,1,3,2,6},
  { 5,1,3,2,6,4}
};
direction_table_t forward_patterns={
  { 5,4,6,2,3,1},
  { 4,6,2,3,1,5},
  { 6,2,3,1,5,4},
  { 2,3,1,5,4,6},
  { 3,1,5,4,6,2},
  { 1,5,4,6,2,3}
};
void print_buffer() {
  for(int i=0;i<HALL_NUMBERS;i++) {
    Serial.print(hall_buffer[i]);
  }
}
/*
   return true if motor is running in the direction
  specified by TABLE
*/
bool is_running(const direction_table_t &table,unsigned int &pattern) {
  for(int i=0;i<HALL_PATTERNS;i++) {
    if(memcmp(table[i],hall_buffer,sizeof(table[i]))==0) {
      pattern=i;
      return true;
    }
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
    ( digitalRead(HALL_A) ? 0 : HALL_A_BIT ) +
    ( digitalRead(HALL_B) ? 0 :HALL_B_BIT ) +
    ( digitalRead(HALL_C) ? 0 :HALL_C_BIT );
}

void read_hall2() {
  Serial.print(digitalRead(HALL_A));
  Serial.print(digitalRead(HALL_B));
  Serial.print(digitalRead(HALL_C));
  Serial.println();
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

  pinMode(HALL_A,INPUT_PULLUP);
  pinMode(HALL_B,INPUT_PULLUP);
  pinMode(HALL_C,INPUT_PULLUP);
}
hall_state_t last_hall_state=0;
unsigned int hall_buffer_index=0;
unsigned int rotations=0;
unsigned long rotations_timestamp=0;
int last_direction=0;
unsigned int count=0;

void log_motor(int direction,const direction_table_t &table) {
  unsigned int pattern;
    const bool running = is_running(table,pattern);
    if(running) {    
      rotations+=direction;
      memset(hall_buffer,0,sizeof(hall_buffer));
      hall_buffer_index=0;  
      if(count++%DELAYTIME==0) {
        Serial.print("revolutions-");
        Serial.print(rotations/2);
        //Serial.println();
        Serial.print(";rpm=");
      }
      unsigned long current_time = millis();
      if(rotations_timestamp!=0) {
        unsigned long deltaT = current_time - rotations_timestamp;
        const float rpm = 1.0/((float)deltaT/1000.0/60.0);
        if(count%DELAYTIME==0) {
          Serial.println(rpm/2);
        }
      }
      rotations_timestamp = current_time;
      //Serial.println();  
    }
}
void loop() {


  //  read_speed_control();
  
  hall_state_t hall_state = read_hall();
  if(hall_state!=last_hall_state) {
    hall_buffer[hall_buffer_index++]=hall_state;
    if(hall_buffer_index == HALL_NUMBERS)
      hall_buffer_index=0;
    //print_buffer();
    //Serial.println();
    log_motor(1,forward_patterns);
    log_motor(-1,reverse_patterns);
//    Serial.print(";Hall=");
//    Serial.println(hall_state);
    last_hall_state = hall_state;
  }  
  int speed = read_speed_control();
  int brake = read_brake_control(); // this'll probably be wired straight to motor
  int reset = read_reset_button();

  
  set_motor_speed();
  set_counter();
  
  
  //read_hall2();
  //delay(500);
}


