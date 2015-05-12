#include <cstdint>


class DC_Motor 
{
  uint8_t pin_r;             // pin 2a
  uint8_t pin_l;             // pin 1a
  uint8_t pin_e;             // pin enable
  uint8_t direction;         // 0 for right 1 for left

 public:
  DC_Motor(uint8_t _pin_e, uint8_t _pin_l, uint8_t _pin_r);
  void Motor_Start();
  void Motor_Stop();
  void Motor_setDirection(uint8_t _direct);
  void Motor_getDirection(uint8_t _direct);

};

