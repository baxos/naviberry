/**
  * @file motordriver.hpp
  * @author Jan Emil Bacher
  * @date 2015-2016
  * @brief This class is used for controlling h-bridge connected with one dc-motor.
  *
  *
  * This class is used for controlling a dc motor. It is designed for control the dc motor via a h-bridge.
  * It can use PWM or just simply on/off.
  */

#include <cstdint>


/**
  * @class DC_Motor
  * @brief A class designed for making an easy to use API for controlling a h-bridge.
  * 
  *
  * The class is designed for be as simple to use as possible. 
  * It is designed for use with a h-bridge and a dc-motor connected to that.
  */
class DC_Motor 
{
  uint8_t pin_r;             ///< pin 2a.
  uint8_t pin_l;             ///< pin 1a.
  uint8_t pin_e;             ///< pin enable.
  uint8_t direction;         ///< 0 for right 1 for left.
  uint8_t running;           ///< A 0 or 1 value wether the motor is running or not.
  uint8_t pwm_high_duration; ///< for controlling speed of motor, meausred in ms
  bool pwm_on;               ///< Boolean value wether pwm is running or not.
  void PWM();
public:
  DC_Motor(uint8_t _pin_e, uint8_t _pin_l, uint8_t _pin_r); ///< Class constructor
  ~DC_Motor();  ///< Class deconstructor
  void Start();  ///< Starts the motor
  void StartPWM(int _val); ///< Start PMW on the motor
  void StopPWM(); ///< Stops the PWM on motor
  void Stop(); ///< Stops the motor
  void IncreaseSpeed(); ///< Increases the speed of the motor.
  void DecreaseSpeed(); ///< Decrease the speed of the motor.
  void setDirection(uint8_t _direct); ///< Sets the direction of the motor
  uint8_t getDirection();       ///< Gets the direction of the motor

};

