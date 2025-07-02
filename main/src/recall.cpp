/*
 * TO-DO: 
 * Interrupt during playback
 * Implement more efficient buffer streaming
 * Clean code
 */ 

#include "recall.h"

int record(MotorPwm& mecanum)
{   
  File file = LittleFS.open(FILENAME, "a");
  if (!file) {
    Serial.println("Failed to open file");
    return 1;
  }

  file.write((uint8_t *)&mecanum, sizeof(MotorPwm));  
  file.close();

  return 0;
}

int replay(State& current_state)
{
  MotorPwm temp;
  State last_state = current_state; 
  unsigned long last_timestamp = 0;
  unsigned long duration = 0;

  File file = LittleFS.open(FILENAME, "r");
  if (!file) {
    Serial.println("Failed to open file");
    return 1;
  }

  if (file.read((uint8_t *)&temp, sizeof(MotorPwm)) == sizeof(MotorPwm)) {
    last_timestamp = temp.period_ms;
    motors_move(temp, DIRECT_MODE);
  }

  while (file.read((uint8_t *)&temp, sizeof(MotorPwm)) == sizeof(MotorPwm)) {
    if (last_state != current_state) {
      file.close();
      return 0; 
    }
    
    duration = temp.period_ms - last_timestamp;
  
    if (duration > 0) {
      delay(duration);
    }

    motors_move(temp, DIRECT_MODE);
    last_timestamp = temp.period_ms;
  }
  
  file.close();
  
  return 0;
}

int reverse(State& current_state)
{
  MotorPwm temp;
  State last_state = current_state; 
  unsigned long last_timestamp = 0;
  unsigned long duration = 0;

  File file = LittleFS.open(FILENAME, "r");
  
  if (!file) {
    Serial.println("Failed to open file");
    return 1;
  }

  int total_records = file.size() / sizeof(MotorPwm);
  Serial.print("total records = "); Serial.println(total_records);

  int i = total_records - 1;

  file.seek(i * sizeof(MotorPwm), SeekSet);
  
  if (file.read((uint8_t *)&temp, sizeof(MotorPwm)) == sizeof(MotorPwm)) {
    last_timestamp = temp.period_ms;
    motors_move(temp, REVERSE_MODE);
  }

  for (i = total_records - 2; i >= 0; i--) {
    
    if (last_state != current_state) {
      file.close();
      return 0; 

    }
    
    file.seek(i * sizeof(MotorPwm), SeekSet);

    if (file.read((uint8_t *)&temp, sizeof(MotorPwm)) == sizeof(MotorPwm)) {
      duration = last_timestamp - temp.period_ms;

      if (duration > 0) {
        delay(duration);
      }

      motors_move(temp, REVERSE_MODE);
      last_timestamp = temp.period_ms;
    }
  }

  file.close();
  
  return 0;
}