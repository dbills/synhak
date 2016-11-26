#ifdef _PC
#include <stdlib.h>
#include <mock_arduino.h>
#include <fake_serial.h>
#include <memory.h>
#endif

#include "ds1809.h"

ds1809::ds1809(const unsigned int uc_pin, const unsigned int dc_pin):
  wiper_target(0),
  is_pulsing(0),
  pulse_start(0),
  pulse_stop(0),
  wiper_direction(0),
  wiper_position(0),
  uc_pin(uc_pin),
  dc_pin(dc_pin),
  dc_count(0)
{
}

void ds1809::initialize() {
  pinMode(uc_pin,OUTPUT);
  pinMode(dc_pin,OUTPUT);
  Serial.print("ds1809: uc:");
  Serial.print(uc_pin);
  Serial.print(" dc:");
  Serial.print(dc_pin);
  Serial.println();
  reset();
}

void ds1809::set_target(unsigned int target) {
  if(target==wiper_target)
    return;
    
  Serial.print("set: ");
  Serial.println(target);
  
  if(target<=63)
    wiper_target=target;
}

void ds1809::update_wiper_position(int wiper_direction)
{
  const unsigned int new_position=wiper_position + wiper_direction;
  if(new_position<64 && new_position>=0) {
    wiper_position=new_position;
    //Serial.print(wiper_position);
  }
}

void ds1809::service() {
  if(pulse_start>0) {
    // note: fear not, unsigned long rollover makes this work ok
    if(millis()-pulse_start > pulse_width ) {
      reset();
      update_wiper_position(wiper_direction);
    }
  } else {    
    /*
    Serial.print("service: target: ");
    Serial.print(wiper_target);
    Serial.print(" position: ");
    Serial.println(wiper_position);
    */
    if(wiper_target==0) {
      pulse_dc();
      return;
    }
    if(wiper_target < wiper_position) {
      pulse_dc();
    } else if(wiper_target > wiper_position) {
      pulse_uc();
    }
  }
}
void ds1809::hard_reset() {
  // atempt to bring wiper to 0
  digitalWrite(uc_pin,HIGH);
  digitalWrite(dc_pin,LOW);
  delay(8000);
  reset();  
}
/*
  Reset uc and dc pins after a pulse has completed.

  Begin timer for amount of time pins have been high.

  Check if we've sent more than 64 consecutive "down" pulses
  and if so, we will assume the pot wiper is at 0
  and resynch our current notion of the pot wiper just in case
  something has gone wrong somehow.
 */
void ds1809::reset() {  
  digitalWrite(uc_pin,HIGH);
  digitalWrite(dc_pin,HIGH);
  pulse_stop = millis();
  pulse_start=0;
  if(dc_count>64) {
    dc_count=0;
    //Serial.println("R");
    wiper_position=0;
  }
}

static int column_count=0;

void ds1809::pulse_uc() { 
  dc_count=0;
  wiper_direction = 1; pulse_pin(uc_pin); 
}

void ds1809::pulse_dc() { 
  wiper_direction =-1; 
  if(pulse_pin(dc_pin))
    dc_count++;
}


bool ds1809::pulse_pin(unsigned int pin) {
  if(pin!=uc_pin&&pin!=dc_pin)
    return false;
    
  if(pulse_start==0) {
    // note: unsigned long rollover makes this work ok
    const unsigned int delta = millis()-pulse_stop;
    if(delta > off_time) {
      pulse_stop=0;
//      Serial.print(delta);
//      Serial.print(pin==uc_pin?"U":"D");
      if(++column_count%64==0)
        Serial.println();
      digitalWrite(pin,LOW);
      pulse_start=::millis();
      return true;
    }
  }
  return false;
}

