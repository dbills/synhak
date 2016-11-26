#include "ds1809.h"

ds1809::ds1809(int uc_pin,int dc_pin):
  wiper_position(0),
  uc_pin(uc_pin),
  dc_pin(dc_pin),
  pulse_start(0),
  pulse_stop(0),
  wiper_direction(0)
{
  pinMode(uc_pin,OUTPUT);
  pinMode(dc_pin,OUTPUT);
  reset();
}

void ds1809::set_target(unsigned int target) {
  if(target==wiper_target)
    return;
    
  //Serial.print("set: ");
  //Serial.println(target);
  
  if(target<=64)
    wiper_target=target;
}

void ds1809::update_wiper_position(int wiper_direction)
{
  if(wiper_position<64 && wiper_position>=0)
    wiper_position+=wiper_direction;
}

void ds1809::service() {
  if(pulse_start>0) {
    if((abs(millis()-pulse_start) > pulse_width ) ) {
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
  pulse_stop = millis();
  pulse_start=0;
//  Serial.println("reset");
}
static int column_count=0;
void ds1809::pulse_pin(unsigned int pin) {
  if(pulse_start==0) {
    const int delta = abs(millis()-pulse_stop);
    if(delta > off_time) {
      pulse_stop=0;
//      Serial.print(delta);
//      Serial.print(" pressed ");
      Serial.print(pin==uc_pin?"U":"D");
      if(++column_count%64==0)
        Serial.println();
      digitalWrite(pin,LOW);
      pulse_start=::millis();
    }
  }
}

