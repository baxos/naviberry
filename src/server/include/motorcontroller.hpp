/**
  * @file motorcontroller.hpp
  * @Author Jan Emil Bacher
  * @date 2015-2016
  * @brief This class is used for controlling h-bridge connected with one dc-motor.
  *
  *
  * This class is used for controlling a dc motor. It is designed for control the dc motor via a h-bridge.
  * It can use PWM or just simply on/off.
  */

#include <cstdint>


/**
  * @class MotorController
  * @brief A class designed for making an easy to use API for controlling a h-bridge.
  * 
  *
  * The class is designed for be as simple to use as possible. 
  * It is designed for use with a h-bridge and a dc-motor connected to that.
  */
class MotorController
{
  bool isReady;
  bool isMoving;
  int16_t speed;
  DC_Motor* motor_left;
  DC_Motor* motor_right;
  void threadFuncTurnRight();
  void threadFuncTurnLeft();
public:
  MotorController(DC_Motor* _motor_left, DC_Motor* _motor_right);
  void Start();
  void Stop();
  void TurnRight();
  void TurnLeft();

  void DriveDistance(int16_t _dist);
};

