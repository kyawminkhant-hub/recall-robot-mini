#ifndef DRIVE_H
#define DRIVE_H

#include <Arduino.h>
#include <CytronMotorDriver.h>

#define MOTOR_RF_PINA       23
#define MOTOR_RF_PINB       22
#define MOTOR_LF_PINA       33
#define MOTOR_LF_PINB       32
#define MOTOR_RR_PINA       17
#define MOTOR_RR_PINB       16
#define MOTOR_LR_PINA       13
#define MOTOR_LR_PINB       12

enum MOTOR_MODE {
  DIRECT_MODE,
  REVERSE_MODE  
};

enum TRIGGER {
  L2,
  R2  
};

struct MotorPwm {
  int rf_pwm;
  int lf_pwm;
  int rr_pwm; 
  int lr_pwm;
  unsigned long period_ms;
};

enum State {
  MANUAL_DRIVE,
  RECORDING,
  REPLAY,
  REVERSE,
  LOOP_L1,
  LOOP_R1
};

void stop(MotorPwm& mecanum);
void drive(MotorPwm& mecanum, int x, int y);
void rotate(MotorPwm& mecanum, int x, int y, int trigger);
void motors_move(MotorPwm& mecanum, int mode);

#endif