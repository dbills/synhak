// hall effect pin assignemnts
const int HALL_A = 4;
const int HALL_B = 5;
const int HALL_C = 6;
// hall effect bit assignment
const int HALL_A_BIT = 4;
const int HALL_B_BIT = 1;
const int HALL_C_BIT = 2;
// misc control panel digital IO functions
const int MOTOR_BRAKE = 4;
const int SPEED_PWN = 5;
// variable resistor input for motor speed control
const int SPEED_INPUT = 0;
// min and max values for analog read of motor control rocker switch pot
const int MIN_ROCKER = 0;
const int MAX_ROCKER = 1023;
// dead space on speed control rocker that we consider "idle" position  
const int ROCKER_DEADZONE = 12;
const int ROCKER_CENTER = (MAX_ROCKER - MIN_ROCKER) / 2; 
const int ROCKER_IDLE_MIN = ROCKER_CENTER - ROCKER_DEADZONE;
const int ROCKER_IDLE_MAX = ROCKER_CENTER + ROCKER_DEADZONE;
const int ROCKER_FORWARD_RANGE = MAX_ROCKER - ROCKER_IDLE_MAX;
const int ROCKER_REVERSE_RANGE = ROCKER_IDLE_MIN - MIN_ROCKER ;
const int POT_WIPER_STEPS = 64; 
const float ROCKER_FORWARD_SCALE = (float)ROCKER_FORWARD_RANGE / (float)POT_WIPER_STEPS;
const float ROCKER_REVERSE_SCALE = (float)ROCKER_REVERSE_RANGE / (float)POT_WIPER_STEPS;

const int FORWARD =  1;
const int STOPPED =  0;
const int REVERSE = -1;

typedef unsigned int hall_state_t;
// external pot is gnd = max speed
struct speed_input_t {
  unsigned long last_read_time; // millis of last time
  unsigned int set_speed;       // POT reading
};
// converging on the set speed is accomplished
// add 1/X the delta between inputs on each sample
// until Delta is less than some minimum
// then directly assign  the value of the speed input
const unsigned int SPEED_SAMPLE_TIME = 1000; // time between sampling speed input 
#define DELAYTIME 5000

speed_input_t last_speed_input;

/*
  your sensor wiring may vary.  
  you must have the sensor bit numbers set up properly so that
  the following is true:

  sensor bit numbers 
       1
     4   2

  possible patterns for one comlete
  revolution of motor
 */
const int HALL_PATTERNS = 6; 
const int HALL_NUMBERS = 6;
typedef hall_state_t direction_table_t[ HALL_PATTERNS ][ HALL_NUMBERS];
  
