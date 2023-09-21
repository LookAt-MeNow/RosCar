/***************************************************************
   Motor driver definitions
   
   Add a "#elif defined" block to this file to include support
   for a particular motor driver.  Then add the appropriate
   #define near the top of the main ROSArduinoBridge.ino file.
   
   *************************************************************/

#ifdef USE_BASE
   
#ifdef POLOLU_VNH5019
  /* Include the Pololu library */
  #include "DualVNH5019MotorShield.h"

  /* Create the motor driver object */
  DualVNH5019MotorShield drive;
  
  /* Wrap the motor driver initialization */
  void initMotorController() {
    drive.init();
  }

  /* Wrap the drive motor set speed function */
  void setMotorSpeed(int i, int spd) {
    if (i == LEFT) drive.setM1Speed(spd);
    else drive.setM2Speed(spd);
  }

  // A convenience function for setting both motor speeds
  void setMotorSpeeds(int leftSpeed, int rightSpeed) {
    setMotorSpeed(LEFT, leftSpeed);
    setMotorSpeed(RIGHT, rightSpeed);
  }
#elif defined POLOLU_MC33926
  /* Include the Pololu library */
  #include "DualMC33926MotorShield.h"

  /* Create the motor driver object */
  DualMC33926MotorShield drive;
  
  /* Wrap the motor driver initialization */
  void initMotorController() {
    drive.init();
  }

  /* Wrap the drive motor set speed function */
  void setMotorSpeed(int i, int spd) {
    if (i == LEFT) drive.setM1Speed(spd);
    else drive.setM2Speed(spd);
  }

  // A convenience function for setting both motor speeds
  void setMotorSpeeds(int leftSpeed, int rightSpeed) {
    setMotorSpeed(LEFT, leftSpeed);
    setMotorSpeed(RIGHT, rightSpeed);
  }
#elif defined L298_MOTOR_DRIVER
  void initMotorController() {
    digitalWrite(RIGHT_MOTOR_ENABLE, HIGH);
    digitalWrite(LEFT_MOTOR_ENABLE, HIGH);
  }
  
  void setMotorSpeed(int i, int spd) {
    unsigned char reverse = 0;
  
    if (spd < 0)
    {
      spd = -spd;
      reverse = 1;
    }
    if (spd > 255)
      spd = 255;
    
    if (i == LEFT) { 
      if      (reverse == 0) { analogWrite(RIGHT_MOTOR_FORWARD, spd); analogWrite(RIGHT_MOTOR_BACKWARD, 0); }
      else if (reverse == 1) { analogWrite(RIGHT_MOTOR_BACKWARD, spd); analogWrite(RIGHT_MOTOR_FORWARD, 0); }
    }
    else /*if (i == RIGHT) //no need for condition*/ {
      if      (reverse == 0) { analogWrite(LEFT_MOTOR_FORWARD, spd); analogWrite(LEFT_MOTOR_BACKWARD, 0); }
      else if (reverse == 1) { analogWrite(LEFT_MOTOR_BACKWARD, spd); analogWrite(LEFT_MOTOR_FORWARD, 0); }
    }
  }
  
 // void setMotorSpeeds(int leftSpeed, int rightSpeed) {
  //  setMotorSpeed(LEFT, leftSpeed);
 //   setMotorSpeed(RIGHT, rightSpeed);
  //}

#elif defined L298N_MOTOR_DRIVER
  //初始化
  void initMotorController(){
    pinMode(LEFT_DIR1,OUTPUT);
    pinMode(LEFT_DIR2,OUTPUT);
    pinMode(LEFT_PWM,OUTPUT);
    pinMode(RIGHT_DIR1,OUTPUT);
    pinMode(RIGHT_DIR2,OUTPUT);
    pinMode(RIGHT_PWM,OUTPUT);
  }
  //设置单个电机的速度
  void setMotorSpeed(int i, int spd){
    unsigned char reverse = 0;
    if (spd < 0)
    {
      spd = -spd;
      reverse = 1;
    }
    if (spd > 255)
      spd = 255;
    
    if (i == LEFT) { 
      if(reverse == 1) { //正转
        digitalWrite(LEFT_DIR1,HIGH);
        digitalWrite(LEFT_DIR2,LOW);
      }
      else if (reverse == 0) { 
        digitalWrite(LEFT_DIR1,LOW);
        digitalWrite(LEFT_DIR2,HIGH);
      }
       analogWrite(LEFT_PWM,spd);
    }
    if (i == RIGHT) { 
      if(reverse == 1) { //正转
        digitalWrite(RIGHT_DIR1,HIGH);
        digitalWrite(RIGHT_DIR2,LOW);
      }
      else if (reverse == 0) { 
        digitalWrite(RIGHT_DIR1,LOW);
        digitalWrite(RIGHT_DIR2,HIGH);
      }
      analogWrite(RIGHT_PWM,spd);
    }
  }
  //设置两个电机的速度
  void setMotorSpeeds(int leftSpeed, int rightSpeed){
    setMotorSpeed(RIGHT, rightSpeed);
    setMotorSpeed(LEFT, leftSpeed);
  }
#else
  #error A motor driver must be selected!
#endif

#endif
