#ifndef _ds1809
#define _ds1809
/*
  I model a ds1809 digital potentiometer

  This is a horrible chip to use for an arduino, as it
  only supports relative setting of the wiper
  so we must "hope" we never lose track of the wiper
 */
class ds1809 {
public:
  static const unsigned int pulse_width = 4;   // ms
  static const unsigned int off_time  = 4; // ms
  
  ds1809(const unsigned int uc_pin,const unsigned int dc_pin);
  void initialize();
  void set_target(unsigned int target);
  unsigned int get_wiper() { return wiper_position; }
  void service();

  //void activate_dc(){}
  void hard_reset();    // holds DC for 7 seconds and then resets

private:  
  void reset();         // clears all control pins
  void pulse_uc();
  void pulse_dc();
  /*
    in: pin to pulse ( must be uc_pin, or dc_pin )
    out: true if pulse started, false if
         insufficient pin high time has occurred since last pulse
  */
  bool pulse_pin(unsigned int pin);                   
  void update_wiper_position(int wiper_direction);

  unsigned int              wiper_target;    // desired wiper position
  bool                      is_pulsing;      // 
  unsigned long             pulse_start;     // time since button pressed
  unsigned long             pulse_stop;      // time since button released
  unsigned int              wiper_direction; // direction of last pulse ( up or down )
  unsigned int              wiper_position;  // virtual wiper pos
  const unsigned int        uc_pin, dc_pin;  // up and down "buttons" for wiper
  unsigned int              dc_count;        // consecutive "dc" pulses
};
#endif

