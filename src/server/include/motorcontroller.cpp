#include <thread>
#include <chrono>
#include "./motordriver.hpp"
#include "./motorcontroller.hpp"


/**
  * @name MotorController
  * @brief Constructor, initialize class
  * @param DC_Motor* Left motor
  * @param DC_Motor* Right motor
  * @retval None
  *
  *
  * Initializes the class variables, check motor condition and if everything is ok
  * set the ready flag. 
  *
  **/
MotorController::MotorController(DC_Motor* _motor_left, DC_Motor* _motor_right)
{
  // Speed is in range [0..100]
  speed = 30;

  motor_left = _motor_left;
  motor_right = _motor_right;
}

/**
  * @name Start
  * @brief Starts both motors
  * @retval None
  *
  *
  * Start both DC_Motors
  **/
void MotorController::Start()
{
  motor_left->StartPWM(speed);
  motor_right->StartPWM(speed);

  isMoving = true;
}

/**
  * @name Stop
  * @brief Stops both motors
  * @retval None
  *
  *
  * Stop both DC_Motors
  **/
void MotorController::Stop()
{
  motor_left->StopPWM();
  motor_right->StopPWM();

  isMoving = false;
}

/**
  * @name TurnRight
  * @brief Makes the motors turn to right.
  * @retval None
  *
  *
  * Spawns a thread with function threadFuncTurnRight() to 
  * make the robots turn to right. A thread is needed because of the use of sleep in the functions.
  * We can't hang in code here.
  *
  **/
void MotorController::TurnRight()
{
  // Make thread
  std::thread turnThread (&MotorController::threadFuncTurnRight, this);


  // detach from scope
  turnThread.detach();
}

void MotorController::TurnLeft()
{
  // Make thread
  std::thread turnThread (&MotorController::threadFuncTurnLeft, this);


  // detach from scope
  turnThread.detach();
}

void MotorController::threadFuncTurnRight()
{
  auto turn_time = 1500; // 1.5 second

  // always run at 50% when we turn,
  this->speed = 50;
  
  motor_left->setDirection(0);

  motor_left->Start();
  std::this_thread::sleep_for(std::chrono::milliseconds(turn_time));
  motor_left->Stop();
}

void MotorController::threadFuncTurnLeft()
{

}
