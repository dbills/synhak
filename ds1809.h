#ifndef _ds1809
#define _ds1809

class ds1809 {
public:
  static const int pulse_width = 2;   // ms
  static const int off_time  = 623; // ms
  
  ds1809(int uc_pin,int dc_pin);
  void set_target(unsigned int target);
  void service();

  void activate_dc(){}

private:  
  void reset();
  void pulse_uc() { wiper_direction = 1; pulse_pin(uc_pin); }
  void pulse_dc() { wiper_direction =-1; pulse_pin(dc_pin); }

  void pulse_pin(unsigned int pin);
  void update_wiper_position(int wiper_direction);

  unsigned int           wiper_target;    // desired wiper position
  boolean                is_pulsing;      // 
  unsigned long          pulse_start;     // time since button pressed
  unsigned long          pulse_stop;      // time since button released
  unsigned int           wiper_direction; // direction of last pulse ( up or down )
  unsigned int           wiper_position;  // virtual wiper pos
  const unsigned int     uc_pin, dc_pin;
};
#endif

