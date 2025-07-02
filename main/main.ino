#include <Ps3Controller.h>
#include "src/recall.h"
#include "src/drive.h"

MotorPwm mecanum;
State current_state = MANUAL_DRIVE;

void Manual_Drive_Handler();

void notify()
{
  /* If Circle is pressed, start recording */
  if( Ps3.event.button_down.circle ) {
    if (current_state == MANUAL_DRIVE) {
      Serial.println("Recording start...");
      
      /* Removing old recoding file if it exists */
      LittleFS.remove(FILENAME);
    
      current_state = RECORDING;
    }
  }

  /* If Square is pressed, start replay */
  if( Ps3.event.button_down.square ) {
    if (current_state == MANUAL_DRIVE) {
      Serial.println("Replay start...");
      current_state = REPLAY;
    }
  }

  /* If Triangle is pressed, start reverse */
  if( Ps3.event.button_down.triangle ) {
    if (current_state == MANUAL_DRIVE) {
      Serial.println("Reverse start...");
      current_state = REVERSE;
    }
  }

  /* If L1 is pressed, start Loop L1 */
  if( abs(Ps3.event.analog_changed.button.l1) ) {
    if (current_state == MANUAL_DRIVE) {
      Serial.println("Loop_L1 start...");
      current_state = LOOP_L1;
    }
  }

  /* If R1 is pressed, start Loop R1 */
  if( abs(Ps3.event.analog_changed.button.r1) ) {
    if (current_state == MANUAL_DRIVE) {
      Serial.println("Loop_R1 start...");
      current_state = LOOP_R1;
    }
  }

  /* If Cross is pressed, stop recording OR playback */
  if( Ps3.event.button_down.cross ) {
    if (current_state != MANUAL_DRIVE) {
      Serial.println("Stopped!");
      current_state = MANUAL_DRIVE;
    } 
  }

}

void onConnect() {
    Serial.println("Connected.");
    current_state = MANUAL_DRIVE;
}

void setup()
{
  Serial.begin(115200);

  Ps3.attach(notify);
    
  /* Connect to PS3 Controller */
  Ps3.begin("00:00:00:00:00:00");
  Serial.println("Ready.");
  Ps3.attachOnConnect(onConnect);

  /* Initialize LittleFS */
  if (!LittleFS.begin(true)) {
    Serial.println("LittleFS Mount Failed");
    return;
  }
}

void loop()
{
  if (!Ps3.isConnected())
    return;

  switch (current_state) {

    case MANUAL_DRIVE:
      Manual_Drive_Handler();
      break;
      
    case RECORDING:
      Manual_Drive_Handler();
      record(mecanum);
      break;     

    case REPLAY:
      replay(current_state);
      current_state = MANUAL_DRIVE;
      break;
    
    case REVERSE:
      reverse(current_state);
      current_state = MANUAL_DRIVE;
      break;
    
    case LOOP_L1:
      replay(current_state);
      reverse(current_state);
      current_state = MANUAL_DRIVE;
      break;
    
    case LOOP_R1:
      reverse(current_state);
      replay(current_state);
      current_state = MANUAL_DRIVE;
      break;
  }
}

void Manual_Drive_Handler() 
{ 
  int trigger = 0;

  if(Ps3.data.button.l2) 
    trigger = L2;
  if(Ps3.data.button.r2)  
    trigger = R2;
  
  if (Ps3.data.button.up) 
    drive(mecanum, 0, -(Ps3.data.analog.button.up));
  
  else if (Ps3.data.button.down)
    drive(mecanum, 0, Ps3.data.analog.button.down);
  
  else if (Ps3.data.button.left)
    drive(mecanum, -(Ps3.data.analog.button.left), 0);
  
  else if (Ps3.data.button.right)
    drive(mecanum, Ps3.data.analog.button.right, 0);
  
  else if (Ps3.data.analog.stick.lx != 0 || Ps3.data.analog.stick.ly != 0)
    drive(mecanum, Ps3.data.analog.stick.lx, Ps3.data.analog.stick.ly);
  
  else if (Ps3.data.analog.stick.rx != 0 || Ps3.data.analog.stick.ry != 0)
    rotate(mecanum, Ps3.data.analog.stick.rx, Ps3.data.analog.stick.ry, trigger);
  
  else
    stop(mecanum);
}
