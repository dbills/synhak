// UNO prototype 1 board connections when facing board hat
/*   ________________________
    /                        |
    |      C4/3              |
    |      C2/3              |
    | C3/4                   |
    |      C1/4              |
    
    
    C1 yellow/white/black/black
    C2 motor controller
    C2-1 Pot Pwr   ( ds1809 p1 )
    C2-2 Pot Wiper ( ds1809 p6 )
    C2-3 Pot Gnd   ( ds1809 p4 )
    
    C3 rocker Switch green/yellow/white/blue
    C3-1 : Gnd
    C3-2 : +5
    C4-Wiper ( Arduino A0)   
    
    C4-1 Hall-A
    C4-2 Hall-B
    C4-3 Hall-C
       
    2560
     __________________________________________
    /   
    |
    |     C1/4  C2/3
    |
    |     C3/4
    |
    |           C4/4
    |
    |     C5/5
    
    
    C1 = Rocker
    C1-1 : Gnd
    C1-2 : +5
    C1-3 : Rocker Wiper ( Arduino A0 )
    C1-4 : NC
    
    C2 = Reset Input
    C2-1 : Gnd
    C2-2 : +5
    C2-3 : ( Arduino D12 )    
    
    C3 = LCD
    C3-1 : Gnd
    C3-2 : Red
    C3-3 : SDA ( i2c data )
    C3-4 : SCL ( i2c clock )

    C4 = motor controller output
    C4-1 : Nc
    C4-2 : Pot Gnd
    C4-3 : Pot Wpr
    C4-4 : Pot Pwr
    
    
    C5 = Hall Effect Input
    C5-1 : Gnd
    C5-2 : +5
    C5-3 : Hall Input A ( Arduino D2 )
    C5-4 : Hall Input B (    "    D3 )
    C5-5 : Hall Input C (    "    D4 )
    
    DS1809 UC = D18
           DC = D17
*/

// hall effect pin assignemnts
const int HALL_A = 10;
const int HALL_B = 11;
const int HALL_C = 12;
// hall effect bit assignment
const int HALL_A_BIT = 4;
const int HALL_B_BIT = 1;
const int HALL_C_BIT = 2;
// misc control panel digital IO functions
const int MOTOR_BRAKE = 9;
const int RESET_BUTTON = 4;
const int DIRECTION_OUTPUT = 5;
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

const unsigned int SPEED_SAMPLE_TIME = 5000; // time between sampling speed input 
const unsigned int RPM_SAMPLE_TIME = 5000;   // loops between rpm calcs
//speed_input_t last_speed_input;

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
  
