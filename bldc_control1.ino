// define _PC when *not* compiling on Arduino
#ifdef _PC
#include <mock_arduino.h>
#include <fake_serial.h>
#include <memory.h>
#endif

#include "defines.h"
#include "ds1809.h"

/*
  ring buffer or last three hall sensor reads
*/
hall_state_t hall_buffer[HALL_NUMBERS];
ds1809 pot1(2,3); // uc_pin, dc_pin
hall_state_t  last_hall_state=0;
unsigned int  hall_buffer_index=0;

// total rotations since last cleared 
int  rotations=0;

// use for rpm calculations
unsigned long rotations_timestamp=0;
int rotation_count=0;

int           last_direction=0;   // last known motor direction
unsigned int  count=0;
unsigned int  last_rpm=0;         // last calculated rpm

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

  out: pattern is the matched pattern
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
  same as is_running arity 2, but does not use output pattern
 */
inline bool is_running(const direction_table_t &table) {
  unsigned int unused;
  return is_running(table,unused);
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
    ( digitalRead(HALL_A) ? 0 :HALL_A_BIT ) +
    ( digitalRead(HALL_B) ? 0 :HALL_B_BIT ) +
    ( digitalRead(HALL_C) ? 0 :HALL_C_BIT );
}

// return true if the speed control is in idle
// position
inline bool is_idle(unsigned int setting) {
  return (setting > ROCKER_IDLE_MIN && setting < ROCKER_IDLE_MAX);
}

inline void idle_motor() {
  //Serial.println("idle");
  pot1.set_target(0);
}
inline int get_rpm() {
  return last_rpm;
}
inline void set_motor_direction(const int direction)
{
  Serial.print(" SD<");
  Serial.print(direction);
  Serial.print(">");
  last_direction=direction;
}
inline void change_direction(int direction,unsigned int target) {
  if(get_rpm() > 0 || pot1.get_wiper()!=0){
    idle_motor();
  } else {
    set_motor_direction(direction);
    pot1.set_target(target);
  }    
}
void read_speed_control() {
  unsigned int setting = analogRead(SPEED_INPUT);
  //Serial.print("Target: ");
  //Serial.print(setting);
  //Serial.println();
  if(is_idle(setting)) 
    idle_motor();
  else if(setting > ROCKER_IDLE_MAX) {
    const unsigned target = (float)(setting - ROCKER_IDLE_MAX) / ROCKER_FORWARD_SCALE; 
    //Serial.print("F");
    //Serial.println(target);
    if(last_direction == FORWARD)    
      pot1.set_target(target);
    else
      change_direction(FORWARD,target);
  } else if(setting < ROCKER_IDLE_MIN) {
    const int target = ( POT_WIPER_STEPS - 1) -
      ( (float)(setting - MIN_ROCKER ) / ROCKER_REVERSE_SCALE );
    //Serial.print("R");
    //Serial.println(target);
    if(last_direction == REVERSE)
      pot1.set_target(target);
    else 
      change_direction(REVERSE,target);
  }

}
int read_brake_control() {
  return 0;
}
int read_reset_button() {
  return 0;
}
// update LCD
void set_counter() {
}

extern void lcd_setup();
void setup() {
  Serial.begin(9600);
  pot1.initialize();
  //  lcd_setup();
  Serial.println("Resetting ds1809 ...");
  pot1.hard_reset();
  Serial.println("Constants:");
  Serial.println(ROCKER_IDLE_MIN);
  Serial.println(ROCKER_IDLE_MAX);
  Serial.println(ROCKER_FORWARD_RANGE);
  Serial.println(ROCKER_REVERSE_RANGE);
  Serial.println(ROCKER_FORWARD_SCALE);
  Serial.println(ROCKER_REVERSE_SCALE);
  Serial.println("Ready.");

  pinMode(HALL_A,INPUT_PULLUP);
  pinMode(HALL_B,INPUT_PULLUP);
  pinMode(HALL_C,INPUT_PULLUP);
}

void calculate_rpm() {
  unsigned long current_time = millis();
  if(rotations_timestamp!=0) {
    unsigned long deltaT = current_time - rotations_timestamp;
    last_rpm = float(rotation_count/2*60) / ( (float)deltaT/1000.0);
  }
  rotations_timestamp = current_time;
  rotation_count = rotations;
}

void log_motor(const int direction,const direction_table_t &table) {
  unsigned int pattern;
  const bool running = is_running(table,pattern);
  if(running) {    
    rotations+=direction;
    memset(hall_buffer,0,sizeof(hall_buffer));
    hall_buffer_index=0;  
    if(count%RPM_SAMPLE_TIME==0) {
      calculate_rpm();
      Serial.print("revolutions-");
      Serial.print(rotations/2);
      //Serial.println();
      Serial.print(";rpm=");
    }
    //Serial.println();  
  }
}

void read_motor()
{
  hall_state_t hall_state = read_hall();
  if(hall_state!=last_hall_state) {
    hall_buffer[hall_buffer_index++]=hall_state;
    if(hall_buffer_index == HALL_NUMBERS)
      hall_buffer_index=0;
    //print_buffer();
    //Serial.println();
    log_motor(FORWARD,forward_patterns);
    log_motor(REVERSE,reverse_patterns);
    //    Serial.print(";Hall=");
    //    Serial.println(hall_state);
    last_hall_state = hall_state;
  }  
}
#if 0
extern void lcd_test_loop();
void loop() {
  lcd_test_loop();
}
#else
void loop() {
  /*
  digitalWrite(3,HIGH);
  digitalWrite(2,LOW);
  return;
  */
  
  pot1.service();

  //read_motor();

  if(count%SPEED_SAMPLE_TIME==0) 
    read_speed_control();

  //int brake = read_brake_control(); // this'll probably be wired straight to motor
  //int reset = read_reset_button();

  
  set_counter();
  
  count++;
  
  //delay(300);
}
#endif
void speed_up() {
}
