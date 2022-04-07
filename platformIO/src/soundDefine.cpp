#include <Arduino.h>
#include "soundDefine.h"
#include "pinout.h"

void SoundOn(int note, int duration){
  duration *= 10000;
  long elapsed_time = 0;
  while (elapsed_time < duration) {
      digitalWrite(speakerOut,HIGH);
      delayMicroseconds(note / 2);
      // DOWN
      digitalWrite(speakerOut, LOW);
      delayMicroseconds(note / 2);
      // Keep track of how long we pulsed
      elapsed_time += (note);
  } 
}