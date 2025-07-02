#ifndef RECALL_H
#define RECALL_H

#include <Arduino.h>
#include <LittleFS.h>
#include "FS.h"
#include "drive.h"

#define FILENAME  "/recording.dat"

int record(MotorPwm& mecanum);
int replay(State& current_state);
int reverse(State& current_state);

#endif