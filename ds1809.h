#ifndef _ds1809
#define _ds1809

class ds1809 {
public:
  static const int pulse_width = 2; // ms
  ds1809(int uc_pin,int dc_pin);
  void set_target(unsigned int target);
  void service();
private:  
  void reset();
  void pulse_uc() { pulse_pin(uc_pin); }
  void pulse_dc() { pulse_pin(dc_pin); }

  void pulse_pin(unsigned int pin);

  unsigned int           wiper_target;
  unsigned long          pulse_start;
  unsigned int           wiper_position;
  const unsigned int     uc_pin,dc_pin;
};
#endif

