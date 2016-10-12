// hall effect pin assignemnts
const int HALL_A = 7;
const int HALL_B = 8;
const int HALL_C = 9;
// hall effect bit assignment
const int HALL_A_BIT = 1;
const int HALL_B_BIT = 2;
const int HALL_C_BIT = 4;
// misc control panel digital IO functions
const int MOTOR_BRAKE = 4;
const int SPEED_PWN = 5;
// variable resistor input for motor speed control
const int SPEED_INPUT = 1;

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
speed_input_t last_speed_input;

/*
  ring buffer or last three hall sensor reads
*/
hall_state_t hall_buffer[3];

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
typedef hall_state_t direction_table_t[3][3];

