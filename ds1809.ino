#include "ds1809.h"

ds1809::ds1809(int uc_pin,int dc_pin):
  wiper_position(64),
  uc_pin(uc_pin),
  dc_pin(dc_pin),
  pulse_start(0) 
{
  pinMode(uc_pin,OUTPUT);
  pinMode(dc_pin,OUTPUT);
  reset();
}

void ds1809::set_target(unsigned int target) {
  if(target<=64)
    wiper_target=target;
}
void ds1809::service() {
  if(pulse_start>0) {
    if( (abs(millis()-pulse_start) > pulse_width ) ) {
      reset();
      pulse_start=0;
    }
  } else {    
    if(wiper_target < wiper_position) {
      pulse_dc();
    } else if(wiper_target > wiper_position) {
      pulse_uc();
    }
    }
}

void ds1809::reset() {
  digitalWrite(uc_pin,HIGH);
  digitalWrite(dc_pin,HIGH);
}

void ds1809::pulse_pin(unsigned int pin) {
  if(pulse_start==0) {
    digitalWrite(pin,LOW);
    pulse_start=::millis();
  }
}

