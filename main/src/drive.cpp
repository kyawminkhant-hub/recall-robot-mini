#include "drive.h"

// Configure the motor driver.
CytronMD motor1(PWM_PWM, MOTOR_RF_PINA, MOTOR_RF_PINB);     // PWM 1A = Pin 12, PWM 1B = Pin 13.
CytronMD motor2(PWM_PWM, MOTOR_LF_PINA, MOTOR_LF_PINB);     // PWM 2A = Pin 16, PWM 2B = Pin 17.
CytronMD motor3(PWM_PWM, MOTOR_RR_PINA, MOTOR_RR_PINB);     // PWM 3A = Pin 32, PWM 1B = Pin 33.
CytronMD motor4(PWM_PWM, MOTOR_LR_PINA, MOTOR_LR_PINB);     // PWM 2A = Pin 22, PWM 2B = Pin 23.

void motors_move(MotorPwm& mecanum, int mode)
{
  if (mode == DIRECT_MODE) {
    motor1.setSpeed(mecanum.rf_pwm);   
    motor2.setSpeed(mecanum.lf_pwm);  
    motor3.setSpeed(mecanum.rr_pwm);   
    motor4.setSpeed(mecanum.lr_pwm);  
  }
  else if (mode == REVERSE_MODE) {
    motor1.setSpeed(-mecanum.rf_pwm);   
    motor2.setSpeed(-mecanum.lf_pwm);  
    motor3.setSpeed(-mecanum.rr_pwm);   
    motor4.setSpeed(-mecanum.lr_pwm);  
  }
}

void drive(MotorPwm& mecanum, int x, int y)
{
  int xaxis = map(x, -128, 127, -255, 254);
  int yaxis = map(y, -128, 127, -255, 254);

  if (((x > -64) && (x < 64)) && (y < 0)) {
    mecanum.rf_pwm = -yaxis;
    mecanum.lf_pwm = -yaxis;
    mecanum.rr_pwm = -yaxis;
    mecanum.lr_pwm = -yaxis;
    Serial.println("Move Forward");
  }
  
  else if (((x > -64) && (x < 64)) && (y > 0)) {
    mecanum.rf_pwm = -yaxis;
    mecanum.lf_pwm = -yaxis;
    mecanum.rr_pwm = -yaxis;
    mecanum.lr_pwm = -yaxis;
    Serial.println("Move Backward");
  }

  else if ((x < 0) && ((y > -64) && (y < 64))) {
    mecanum.rf_pwm = -xaxis;
    mecanum.lf_pwm = xaxis;
    mecanum.rr_pwm = xaxis;
    mecanum.lr_pwm = -xaxis;
    Serial.println("Move Sideway Left");
  }

  else if ((x > 0) && ((y > -64) && (y < 64))) {
    mecanum.rf_pwm = -xaxis;
    mecanum.lf_pwm = xaxis;
    mecanum.rr_pwm = xaxis;
    mecanum.lr_pwm = -xaxis;
    Serial.println("Move Sideway Right");
  }

  else if (((x > 64) && (y < 0)) || ((x > 0) && (y < -64))) {
    mecanum.rf_pwm = 0;
    mecanum.lf_pwm = -yaxis;
    mecanum.rr_pwm = -yaxis;
    mecanum.lr_pwm = 0;
    Serial.println("Move Diagonal 45");
  }

  else if (((x < -64) && (y < 0)) || ((x < 0) && (y < -64))) {
    mecanum.rf_pwm = -yaxis;
    mecanum.lf_pwm = 0;
    mecanum.rr_pwm = 0;
    mecanum.lr_pwm = -yaxis;
    Serial.println("Move Diagonal 135");
  }

  else if (((x < 0) && (y > 64)) || ((x < -64) && (y > 0))) {
    mecanum.rf_pwm = 0;
    mecanum.lf_pwm = -yaxis;
    mecanum.rr_pwm = -yaxis;
    mecanum.lr_pwm = 0;
    Serial.println("Move Diagonal 225");
  }

  else if (((x > 64) && (y > 0)) || ((x > 0) && (y > 64))) {
    mecanum.rf_pwm = -yaxis;
    mecanum.lf_pwm = 0;
    mecanum.rr_pwm = 0;
    mecanum.lr_pwm = -yaxis;
    Serial.println("Move Diagonal 315");
  }
  
  // Record timestamp
  mecanum.period_ms = millis();

  motors_move(mecanum, DIRECT_MODE);
}

void rotate(MotorPwm& mecanum, int x, int y, int trigger)
{
  int xaxis = map(x, -128, 127, -255, 254);
  int yaxis = map(y, -128, 127, -255, 254);

  if (((x > -64) && (x < 64)) && (y < 0)) {
    if (trigger == L2) {
      mecanum.rf_pwm = 0;
      mecanum.lf_pwm = 0;
      mecanum.rr_pwm = yaxis;
      mecanum.lr_pwm = -yaxis;
      Serial.println("Strafe Forward Left");
    }
    else if (trigger == R2) {
      mecanum.rf_pwm = 0;
      mecanum.lf_pwm = 0;
      mecanum.rr_pwm = -yaxis;
      mecanum.lr_pwm = yaxis;
      Serial.println("Strafe Forward Right");
    }
  }
  
  else if (((x > -64) && (x < 64)) && (y > 0)) {
    if (trigger == L2) {
      mecanum.rf_pwm = yaxis;
      mecanum.lf_pwm = -yaxis;
      mecanum.rr_pwm = 0;
      mecanum.lr_pwm = 0;
      Serial.println("Strafe Backward Left");
    }
    else if (trigger == R2) {
      mecanum.rf_pwm = -yaxis;
      mecanum.lf_pwm = yaxis;
      mecanum.rr_pwm = 0;
      mecanum.lr_pwm = 0;
      Serial.println("Strafe Backward Right");
    }
  }

  else if ((x < 0) && ((y > -64) && (y < 64))) {
    mecanum.rf_pwm = -xaxis;
    mecanum.lf_pwm = xaxis;
    mecanum.rr_pwm = -xaxis;
    mecanum.lr_pwm = xaxis;
    Serial.println("Rotate counterclockwise");
  }

  else if ((x > 0) && ((y > -64) && (y < 64))) {
    mecanum.rf_pwm = -xaxis;
    mecanum.lf_pwm = xaxis;
    mecanum.rr_pwm = -xaxis;
    mecanum.lr_pwm = xaxis;
    Serial.println("Rotate clockwise");
  }

  else if (((x > 64) && (y < 0)) || ((x > 0) && (y < -64))) {
    mecanum.rf_pwm = 0;
    mecanum.lf_pwm = -yaxis;
    mecanum.rr_pwm = 0;
    mecanum.lr_pwm = -yaxis;
    Serial.println("Pivot Right 45");
  }

  else if (((x > 64) && (y > 0)) || ((x > 0) && (y > 64))) {
    mecanum.rf_pwm = 0;
    mecanum.lf_pwm = -yaxis;
    mecanum.rr_pwm = 0;
    mecanum.lr_pwm = -yaxis;
    Serial.println("Pivot Right 315");
  }

  else if (((x < -64) && (y < 0)) || ((x < 0) && (y < -64))) {
    mecanum.rf_pwm = -yaxis;
    mecanum.lf_pwm = 0;
    mecanum.rr_pwm = -yaxis;
    mecanum.lr_pwm = 0;
    Serial.println("Pivot Left 135");
  }

  else if (((x < 0) && (y > 64)) || ((x < -64) && (y > 0))) {
    mecanum.rf_pwm = -yaxis;
    mecanum.lf_pwm = 0;
    mecanum.rr_pwm = -yaxis;
    mecanum.lr_pwm = 0;
    Serial.println("Pivot Left 225");
  }
  
  // Record timestamp
  mecanum.period_ms = millis();

  motors_move(mecanum, DIRECT_MODE);
}

void stop (MotorPwm& mecanum) {
    mecanum.rf_pwm = 0;
    mecanum.lf_pwm = 0;
    mecanum.rr_pwm = 0;
    mecanum.lr_pwm = 0;
    
    mecanum.period_ms = millis();

    motors_move(mecanum, DIRECT_MODE);
}