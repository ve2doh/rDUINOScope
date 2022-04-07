#include <Arduino.h>
#include <DueTimer.h>
#include "motionCtrl.h"
#include "globalVariable.h"
#include "pinout.h"
#include "graphic_screens.h"
#include "soundDefine.h"

void setmStepsMode(char* P, int mod){    
// P means the axis: RA or DEC; mod means MicroSteppping mode: x32, x16, x8....
// setmStepsMode(R,2) - means RA with 1/2 steps; setmStepsMode(R,4) - means RA with 1/4 steps


// PINS Mapping for fast switching
// DEC_M2 - Pin 8 UP - PC22 - PIOC->PIO_SODR=(1u<<22);
// DEC_M1 - Pin 9 UP - PC21 -  PIOC->PIO_SODR=(1u<<21);
// DEC_M0 - Pin 10 UP - PC29 -  PIOC->PIO_SODR=(1u<<29);
// RA_M0 - Pin 11 UP - PD7 -  PIOD->PIO_SODR=(1u<<7);
// RA_M1 - Pin 12 UP - PD8 -  PIOD->PIO_SODR=(1u<<8);
// RA_M2 - Pin 13 UP - PB27 -  PIOB->PIO_SODR=(1u<<27);
// DEC_M2 - Pin 8 DOWN - PC22 - PIOC->PIO_CODR=(1u<<22);
// DEC_M1 - Pin 9 DOWN - PC21 -  PIOC->PIO_CODR=(1u<<21);
// DEC_M0 - Pin 10 DOWN - PC29 -  PIOC->PIO_CODR=(1u<<29);
// RA_M0 - Pin 11 DOWN - PD7 -  PIOD->PIO_CODR=(1u<<7);
// RA_M1 - Pin 12 DOWN - PD8 -  PIOD->PIO_CODR=(1u<<8);
// RA_M2 - Pin 13 DOWN - PB27 -  PIOB->PIO_CODR=(1u<<27);
//
// PIOC->PIO_SODR=(1u<<25); // Set Pin High
// PIOC->PIO_CODR=(1u<<25); // Set Pin Low

  if (P=="R"){  // Set RA modes
    if (mod == 1){                      // Full Step
        //digitalWrite(RA_MODE0, LOW);
        //digitalWrite(RA_MODE1, LOW);
        //digitalWrite(RA_MODE2, LOW);
        PIOD->PIO_CODR=(1u<<7);
        PIOD->PIO_CODR=(1u<<8);
        PIOB->PIO_CODR=(1u<<27);
    }
    if (mod == 2){                      // 1/2 Step
        //digitalWrite(RA_MODE0, HIGH);
        //digitalWrite(RA_MODE1, LOW);
        //digitalWrite(RA_MODE2, LOW);
        PIOD->PIO_SODR=(1u<<7);
        PIOD->PIO_CODR=(1u<<8);
        PIOB->PIO_CODR=(1u<<27);
    }
    if (mod == 4){                      // 1/4 Step
        //digitalWrite(RA_MODE0, LOW);
        //digitalWrite(RA_MODE1, HIGH);
        //digitalWrite(RA_MODE2, LOW);
        PIOD->PIO_CODR=(1u<<7);
        PIOD->PIO_SODR=(1u<<8);
        PIOB->PIO_CODR=(1u<<27);
    }
    if (mod == 8){                      // 1/8 Step
        //digitalWrite(RA_MODE0, HIGH);
        //digitalWrite(RA_MODE1, HIGH);
        //digitalWrite(RA_MODE2, LOW);
        PIOD->PIO_SODR=(1u<<7);
        PIOD->PIO_SODR=(1u<<8);
        PIOB->PIO_CODR=(1u<<27);
    }
    if (mod == 16){                      // 1/16 Step
        //digitalWrite(RA_MODE0, LOW);
        //digitalWrite(RA_MODE1, LOW);
        //digitalWrite(RA_MODE2, HIGH);
        PIOD->PIO_CODR=(1u<<7);
        PIOD->PIO_CODR=(1u<<8);
        PIOB->PIO_SODR=(1u<<27);
    }
    if (mod == 32){                      // 1/32 Step
        //digitalWrite(RA_MODE0, HIGH);
        //digitalWrite(RA_MODE1, LOW);
        //digitalWrite(RA_MODE2, HIGH);
        PIOD->PIO_SODR=(1u<<7);
        PIOD->PIO_CODR=(1u<<8);
        PIOB->PIO_SODR=(1u<<27);
    }
    RA_mode_steps = MICROSteps/mod;
  }
  if (P=="D"){  // Set RA modes
    if (mod == 1){                      // Full Step
        //digitalWrite(DEC_MODE0, LOW);
        //digitalWrite(DEC_MODE1, LOW);
        //digitalWrite(DEC_MODE2, LOW);
        PIOC->PIO_CODR=(1u<<29);
        PIOC->PIO_CODR=(1u<<21);
        PIOC->PIO_CODR=(1u<<22);
    }
    if (mod == 2){                      // 1/2 Step
        //digitalWrite(DEC_MODE0, HIGH);
        //digitalWrite(DEC_MODE1, LOW);
        //digitalWrite(DEC_MODE2, LOW);
        PIOC->PIO_SODR=(1u<<29);
        PIOC->PIO_CODR=(1u<<21);
        PIOC->PIO_CODR=(1u<<22);
    }
    if (mod == 4){                      // 1/4 Step
        //digitalWrite(DEC_MODE0, LOW);
        //digitalWrite(DEC_MODE1, HIGH);
        //digitalWrite(DEC_MODE2, LOW);
        PIOC->PIO_CODR=(1u<<29);
        PIOC->PIO_SODR=(1u<<21);
        PIOC->PIO_CODR=(1u<<22);
    }
    if (mod == 8){                      // 1/8 Step
        //digitalWrite(DEC_MODE0, HIGH);
        //digitalWrite(DEC_MODE1, HIGH);
        //digitalWrite(DEC_MODE2, LOW);
        PIOC->PIO_SODR=(1u<<29);
        PIOC->PIO_SODR=(1u<<21);
        PIOC->PIO_CODR=(1u<<22);
    }
    if (mod == 16){                      // 1/16 Step
        //digitalWrite(DEC_MODE0, LOW);
        //digitalWrite(DEC_MODE1, LOW);
        //digitalWrite(DEC_MODE2, HIGH);
        PIOC->PIO_CODR=(1u<<29);
        PIOC->PIO_CODR=(1u<<21);
        PIOC->PIO_SODR=(1u<<22);
    }
    if (mod == 32){                      // 1/32 Step
        //digitalWrite(DEC_MODE0, HIGH);
        //digitalWrite(DEC_MODE1, LOW);
        //digitalWrite(DEC_MODE2, HIGH);
        PIOC->PIO_SODR=(1u<<29);
        PIOC->PIO_CODR=(1u<<21);
        PIOC->PIO_SODR=(1u<<22);
    }
    DEC_mode_steps = MICROSteps/mod;
  }
  delayMicroseconds(5);   // Makes sure the DRV8825 can follow
}

void cosiderSlewTo(){
//  int RA_microSteps, DEC_microSteps;
//  int SLEW_RA_microsteps, SLEW_DEC_microsteps;
//  INT data type -> -2,147,483,648 to 2,147,483,647 
//  for more details see the XLS file with calculations
//...

    float HAH;
    float HAM;
    float DECD;
    float DECM;
    double HA_decimal, DEC_decimal;

    if (HAHour >= 12){
        HAH = HAHour - 12;
        HAM = HAMin;
        IS_MERIDIAN_PASSED = false;
    }else{
        HAH = HAHour;
        HAM = HAMin;
        IS_MERIDIAN_PASSED = true;
    }

    //  ADD Correction for RA && DEC according to the Star Alignment
    HA_decimal = ((HAH+(HAM/60))*15) + delta_a_RA;   // In degrees - decimal
    DEC_decimal = OBJECT_DEC_D + (OBJECT_DEC_M/60) + delta_a_DEC; //I n degrees - decimal   

    SLEW_RA_microsteps  = HA_decimal * HA_H_CONST;     // Hardware Specific Code  
    SLEW_DEC_microsteps = DEC_90 - (DEC_decimal * DEC_D_CONST);    // Hardware specific code

    if(IS_MERIDIAN_PASSED == true){
        SLEW_DEC_microsteps*= -1;
    }

// If Home Position selected .... Make sure it goes to 0.

// DO I REALLY NEED THIS.... ????
// CONSIDER THE CODE WHEN YOU HAVE TIME!!!
    int home_pos = 0;
    if ((OBJECT_RA_H == 12) && (OBJECT_RA_M == 0) && (OBJECT_DEC_D == 90) && (OBJECT_DEC_M == 0)){
        SLEW_RA_microsteps = RA_90;
        SLEW_DEC_microsteps = 0;
        home_pos = 1;
    }

// Make the motors START slow and then speed-up - using the microsteps!
// Speed goes UP in 2.2 sec....then ..... FULL Speed ..... then....Slpeed goes Down for 3/4 Revolution of the drive  
int delta_DEC_time = millis() - Slew_timer;
int delta_RA_timer = millis() - Slew_RA_timer;

    if (delta_DEC_time >= 0 && delta_DEC_time < 900){
        if (DEC_mode_steps != 8){
            setmStepsMode("D",8);
        }
    }
    if (delta_DEC_time >= 900 && delta_DEC_time < 1800){
        if (DEC_mode_steps != 4){
            setmStepsMode("D",4);
        }
    }
    if (delta_DEC_time >= 1800 && delta_DEC_time < 2200){
        if (DEC_mode_steps != 2){
            setmStepsMode("D",2);
        }
    }
    if (delta_DEC_time >= 2200){
        if (DEC_mode_steps != 1){
            setmStepsMode("D",1);
        }
    }

    if (delta_RA_timer >= 0 && delta_RA_timer < 900){
        if (RA_mode_steps != 8){
            setmStepsMode("R",8);
        }
    }
    if (delta_RA_timer >= 900 && delta_RA_timer < 1800){
        if (RA_mode_steps != 4){
            setmStepsMode("R",4);
        }
    }
    if (delta_RA_timer >= 1800 && delta_RA_timer < 2200){
        if (RA_mode_steps != 2){
            setmStepsMode("R",2);
        }
    }
    if (delta_RA_timer >= 2200){
        if (RA_mode_steps != 1){
            setmStepsMode("R",1);
        }
    }

    int delta_RA_steps = SLEW_RA_microsteps - RA_microSteps;
    int delta_DEC_steps = SLEW_DEC_microsteps - DEC_microSteps;

// Make the motors SLOW DOWN and then STOP - using the microsteps!
// Speed goes DOWN in 2.2 sec....then ..... FULL Speed ..... then....Slpeed goes Down for 3/4 Revolution of the drive  

    if ((abs(delta_DEC_steps) >= 1200) && (abs(delta_DEC_steps) <= 3000)){ 
        if (DEC_mode_steps != 4){
            setmStepsMode("D",4);
        }
    }
    if ((abs(delta_DEC_steps) < 1200)){ 
        if (DEC_mode_steps != 8){
            setmStepsMode("D",8);
        }
    }
    if ((abs(delta_RA_steps) >= 1200) && (abs(delta_RA_steps) <= 3000)){
        if (RA_mode_steps != 4){
            setmStepsMode("R",4);
        }
    }
    if (abs(delta_RA_steps) < 1200){
        if (RA_mode_steps != 8){
            setmStepsMode("R",8);
            RA_move_ending = 1;
        }
    }

    // Taking care of the RA Slew_To.... and make sure it ends Last
    // NB: This way we can jump to TRACK and be sure the RA is on target   
    if(abs(delta_RA_steps)>= abs(delta_DEC_steps)){
        if (RA_finish_last == 0){
          RA_finish_last = 1;
          Slew_RA_timer = millis();
        }
    }


// RA_STP, HIGH - PIOC->PIO_SODR=(1u<<26)
// RA_STP, LOW - PIOC->PIO_CODR=(1u<<26)
// DEC_STP, HIGH - PIOC->PIO_SODR=(1u<<24)
// DEC_STP, LOW - PIOC->PIO_CODR=(1u<<24)

    if ((IS_OBJECT_RA_FOUND == false) && (RA_finish_last == 1)){    
            if (SLEW_RA_microsteps >= (RA_microSteps-RA_mode_steps) && SLEW_RA_microsteps <= (RA_microSteps+RA_mode_steps)){
               IS_OBJECT_RA_FOUND = true;               
            } else {
                  if (SLEW_RA_microsteps > RA_microSteps){
                      digitalWrite(RA_DIR,STP_BACK);
                      //digitalWrite(RA_STP,HIGH);
                      //digitalWrite(RA_STP,LOW);
                      PIOC->PIO_SODR=(1u<<26);
                      delayMicroseconds(5);
                      PIOC->PIO_CODR=(1u<<26);
                      RA_microSteps += RA_mode_steps;
                  }else{
                      digitalWrite(RA_DIR,STP_FWD);
                      //digitalWrite(RA_STP,HIGH);
                      //digitalWrite(RA_STP,LOW);
                      PIOC->PIO_SODR=(1u<<26);
                      delayMicroseconds(5);
                      PIOC->PIO_CODR=(1u<<26);
                      RA_microSteps -= RA_mode_steps;
                  } 
            }
    }

    // Taking care of the DEC Slew_To....  
    if (IS_OBJECT_DEC_FOUND == false){    
        if (SLEW_DEC_microsteps >= (DEC_microSteps-DEC_mode_steps) && SLEW_DEC_microsteps <= (DEC_microSteps+DEC_mode_steps)){
              IS_OBJECT_DEC_FOUND = true;
        } else {
              if (SLEW_DEC_microsteps > DEC_microSteps){
                  digitalWrite(DEC_DIR,STP_BACK);
                  //digitalWrite(DEC_STP,HIGH);
                  //digitalWrite(DEC_STP,LOW);
                  PIOC->PIO_SODR=(1u<<24);
                  delayMicroseconds(5);
                  PIOC->PIO_CODR=(1u<<24);
                  DEC_microSteps += DEC_mode_steps;
              }else{
                  digitalWrite(DEC_DIR,STP_FWD);
                  //digitalWrite(DEC_STP,HIGH);
                  //digitalWrite(DEC_STP,LOW);
                  PIOC->PIO_SODR=(1u<<24);
                  delayMicroseconds(5);
                  PIOC->PIO_CODR=(1u<<24);
                  DEC_microSteps -= DEC_mode_steps;
              } 
        }
    }

    // Check if Object is found on both Axes...
    if (IS_OBJECT_RA_FOUND == true && IS_OBJECT_DEC_FOUND == true){
      IS_OBJ_FOUND = true;
      RA_move_ending = 0;

      if ((home_pos == 0 ) && (ALT > 0)){
          IS_TRACKING = true;
          setmStepsMode("R",MICROSteps);
          if (Tracking_type == 1){  // 1: Sidereal, 2: Solar, 0: Lunar;
              Timer3.start(Clock_Sidereal);
          }else if (Tracking_type == 2){
              Timer3.start(Clock_Solar);
          }else if (Tracking_type == 0){
              Timer3.start(Clock_Lunar);            
          }
      }
      if (IS_SOUND_ON){
       SoundOn(note_C,64);
      }
      Slew_RA_timer = 0;
      RA_finish_last = 0;
      if (IS_BT_MODE_ON == true){
         Serial3.println("Slew done! Object in scope!");
      }
      if (IS_IN_OPERATION == true){
        drawMainScreen();
      }else{
        drawConstelationScreen(SELECTED_STAR);
      }
    }

}

void consider_Manual_Move(int xP, int yP){
    if ((xP > 0) && (xP <= 150)){
      setmStepsMode("R",1);
      digitalWrite(RA_DIR,STP_BACK);
      digitalWrite(RA_STP,HIGH);
      digitalWrite(RA_STP,LOW);
      RA_microSteps += RA_mode_steps;
    }else if ((xP > 150) && (xP <= 320)){
      setmStepsMode("R",4);
      digitalWrite(RA_DIR,STP_BACK);
      digitalWrite(RA_STP,HIGH);
      digitalWrite(RA_STP,LOW);
      RA_microSteps += RA_mode_steps;
    }else if ((xP > 320) && (xP <= 470)){
      setmStepsMode("R",8);
      digitalWrite(RA_DIR,STP_BACK);
      digitalWrite(RA_STP,HIGH);
      digitalWrite(RA_STP,LOW);
      RA_microSteps += RA_mode_steps;
    }else if ((xP > 620) && (xP <= 770)){
      setmStepsMode("R",8);
      digitalWrite(RA_DIR,STP_FWD);
      digitalWrite(RA_STP,HIGH);
      digitalWrite(RA_STP,LOW);
      RA_microSteps -= RA_mode_steps;
    }else if ((xP > 770) && (xP <= 870)){
      setmStepsMode("R",4);
      digitalWrite(RA_DIR,STP_FWD);
      digitalWrite(RA_STP,HIGH);
      digitalWrite(RA_STP,LOW);
      RA_microSteps -= RA_mode_steps;
    }else if ((xP > 870) && (xP <= 1023)){
      setmStepsMode("R",1);
      digitalWrite(RA_DIR,STP_FWD);
      digitalWrite(RA_STP,HIGH);
      digitalWrite(RA_STP,LOW);
      RA_microSteps -= RA_mode_steps;
    }

    if ((yP > 0) && (yP <= 150)){
      setmStepsMode("D",1);
      digitalWrite(DEC_DIR,STP_BACK);
      digitalWrite(DEC_STP,HIGH);
      digitalWrite(DEC_STP,LOW);
      DEC_microSteps += DEC_mode_steps;
    }else if ((yP > 150) && (yP <= 320)){
      setmStepsMode("D",4);
      digitalWrite(DEC_DIR,STP_BACK);
      digitalWrite(DEC_STP,HIGH);
      digitalWrite(DEC_STP,LOW);
      DEC_microSteps += DEC_mode_steps;
    }else if ((yP > 320) && (yP <= 470)){
      setmStepsMode("D",8);
      digitalWrite(DEC_DIR,STP_BACK);
      digitalWrite(DEC_STP,HIGH);
      digitalWrite(DEC_STP,LOW);
      DEC_microSteps += DEC_mode_steps;
    }else if ((yP > 620) && (yP <= 770)){
      setmStepsMode("D",8);
      digitalWrite(DEC_DIR,STP_FWD);
      digitalWrite(DEC_STP,HIGH);
      digitalWrite(DEC_STP,LOW);
      DEC_microSteps -= DEC_mode_steps;
    }else if ((yP > 770) && (yP <= 870)){
      setmStepsMode("D",4);
      digitalWrite(DEC_DIR,STP_FWD);
      digitalWrite(DEC_STP,HIGH);
      digitalWrite(DEC_STP,LOW);
      DEC_microSteps -= DEC_mode_steps;
    }else if ((yP > 870) && (yP <= 1023)){
      setmStepsMode("D",1);
      digitalWrite(DEC_DIR,STP_FWD);
      digitalWrite(DEC_STP,HIGH);
      digitalWrite(DEC_STP,LOW);
      DEC_microSteps -= DEC_mode_steps;
    }
    delayMicroseconds(1500);
}

