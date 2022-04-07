#include <Arduino.h>
#include <DueTimer.h>           // interruptions library0
#include "sideralMath.h"
#include "globalVariable.h"
#include "graphic_screens.h"
#include "soundDefine.h"
#include "motionCtrl.h"
#include "pinout.h"

void calculateLST_HA(){
//  HA = LST - RA 
    String Date_q = String(rtc.getDateStr());
    String Time_q = String(rtc.getTimeStr());
    int D = Date_q.substring(0,2).toInt();
    int M = Date_q.substring(3,5).toInt();
    int Y = Date_q.substring(6).toInt();
    int S = Time_q.substring(6).toInt();
    int H = Time_q.substring(0,2).toInt(); // hours
    if (Summer_Time == 1){
      H -= 1;
    }
    int MN = Time_q.substring(3,5).toInt();
    if (M < 3) {
       M = M + 12; 
       Y = Y - 1;
    }
    
    float HH = H + ((float)MN/60.00) + ((float)S/3600.00);
    float AA= (int)(365.25*(Y+4716));
    float BB= (int)(30.6001*(M+1)); 
    double CurrentJDN = AA + BB + D - 1537.5 + (HH-TIME_ZONE)/24;
    float current_day = CurrentJDN - 2451543.5;

    //calculate terms required for LST calcuation and calculate GMST using an approximation
    double MJD = CurrentJDN - 2400000.5;		
    int MJD0 = (int)MJD;
    float ut = (MJD - MJD0)*24.0;
    double t_eph  = (MJD0-51544.5)/36525.0;			
    double GMST = 6.697374558 + 1.0027379093*ut + (8640184.812866 + (0.093104 - 0.0000062*t_eph)*t_eph)*t_eph/3600.0;		

    LST = GMST + OBSERVATION_LONGITUDE/15.0;

    //reduce it to 24 format
    int LSTint = (int)LST;
    LSTint/=24;
    LST = LST - (double) LSTint * 24;

// Now I'll use the global Variables OBJECT_RA_H and OBJECT_RA_M  To calculate the Hour angle of the selected object.
    
    double dec_RA = OBJECT_RA_M/60 + OBJECT_RA_H;

    double HA_decimal = LST - dec_RA;
    
    HAHour = int(HA_decimal);
    HAMin = (HA_decimal - HAHour)*60; 
    
    if (HAMin < 0){
      HAHour -= 1;
      HAMin +=60;
    }
    if (HAHour < 0){
      HAHour +=24;
    }
    
    // Convert degrees into Decimal Radians
    double rDEC = 0;
    rDEC = OBJECT_DEC_D + (OBJECT_DEC_M/60);
 

    //rDEC += delta_a_DEC;
    rDEC *= 0.0174532925199;
    double rHA =  HA_decimal*0.26179938779915;     // 0.261799.. = 15 * 3.1415/180  (to convert to Deg. and * Pi) :)
    double rLAT = OBSERVATION_LATTITUDE * 0.0174532925199;

    IS_OBJ_VISIBLE = true;

    double sin_rDEC = sin(rDEC);
    double cos_rDEC = cos(rDEC);
    double sin_rLAT = sin(rLAT);
    double cos_rLAT = cos(rLAT);
    double cos_rHA = cos(rHA);
    double sin_rHA = sin(rHA);
    
    ALT = sin_rDEC * sin_rLAT;
    ALT += (cos_rDEC * cos_rLAT * cos_rHA);
    double sin_rALT = ALT;
    ALT =  asin(ALT);
    double cos_rALT = cos(ALT);
    ALT *= 57.2958;
    
    AZ = sin_rALT * sin_rLAT;
    AZ = sin_rDEC - AZ;
    AZ /= (cos_rALT * cos_rLAT);
    AZ = acos(AZ)*57.2957795;
    if (sin_rHA > 0){
      AZ = 360 - AZ;
    }
    
    if (ALT < 0){
      IS_OBJ_VISIBLE = false;
      if ((IS_BT_MODE_ON == true) && (IS_OBJ_FOUND == false)){
         Serial3.println("Object is out of sight! Telescope not moved.");
      }
      IS_OBJ_FOUND = true;
      IS_OBJECT_RA_FOUND = true;
      IS_OBJECT_DEC_FOUND = true;
      Slew_RA_timer = 0;
      RA_finish_last = 0;
    }else{
      IS_OBJ_VISIBLE = true;
    }

    
    // Take care of the Meridian Flip coordinates
    // This will make the telescope do Meridian Flip... depending on the current HA and predefined parameter: MIN_TO_MERIDIAN_FLIP
    if (IS_MERIDIAN_FLIP_AUTOMATIC){
        mer_flp = HAHour + ((HAMin + MIN_TO_MERIDIAN_FLIP)/60);
        float old_HAMin = HAMin;
        float old_HAHour = HAHour;
        if (IS_POSIBLE_MERIDIAN_FLIP == true){
            if (mer_flp >= 24){
                  HAMin = HAMin - 60;
                  HAHour = 0;
                  if (MERIDIAN_FLIP_DO == false){
                      IS_TRACKING = false;
                      Timer3.stop();
                      OnScreenMsg(1);
                      if (IS_SOUND_ON){
                        SoundOn(note_C,32);
                        delay(200);
                        SoundOn(note_C,32);
                        delay(200);
                        SoundOn(note_C,32);
                        delay(1000); 
                      }           
                      IS_OBJ_FOUND = false;
                      IS_OBJECT_RA_FOUND = false;
                      IS_OBJECT_DEC_FOUND = false;
                      Slew_timer = millis();
                      Slew_RA_timer = Slew_timer + 20000;   // Give 20 sec. advance to the DEC. We will revise later.
                      MERIDIAN_FLIP_DO = true;
                      drawMainScreen();
                  }else{
                      if ((old_HAHour == HAHour) && (old_HAMin == HAMin)){   // Meridian Flip is done so the code above will not execute
                          MERIDIAN_FLIP_DO = false;
                      }
                  }
                  //DEC is set as part of the SlewTo function
              }
        }else{
            if (mer_flp >= 24){
              IS_TRACKING = false;
              Timer3.stop();
            }
        }
    }
}

void selectOBJECT_M(int index_, int objects) {
   OBJECT_Index = index_;

   if (objects == 0){                                           // I've selected a Messier Object
      int i1 = Messier_Array[index_].indexOf(';');
      int i2 = Messier_Array[index_].indexOf(';',i1+1);
      int i3 = Messier_Array[index_].indexOf(';',i2+1);
      int i4 = Messier_Array[index_].indexOf(';',i3+1);
      int i5 = Messier_Array[index_].indexOf(';',i4+1);
      int i6 = Messier_Array[index_].indexOf(';',i5+1);
      int i7 = Messier_Array[index_].indexOf(';',i6+1);
      OBJECT_NAME = Messier_Array[index_].substring(0,i1);
      OBJECT_DESCR = Messier_Array[index_].substring(i7+1,Messier_Array[index_].length()-1);
      String OBJ_RA = Messier_Array[index_].substring(i1,i2);
      OBJECT_RA_H = OBJ_RA.substring(1,OBJ_RA.indexOf('h')).toFloat();
      OBJECT_RA_M = OBJ_RA.substring(OBJ_RA.indexOf('h')+1,OBJ_RA.length()-1).toFloat();
      String OBJ_DEC = Messier_Array[index_].substring(i2,i3);
      String sign = OBJ_DEC.substring(1,2);
      OBJECT_DEC_D = OBJ_DEC.substring(2,OBJ_DEC.indexOf('°')).toFloat();
      OBJECT_DEC_M = OBJ_DEC.substring(OBJ_DEC.indexOf('°')+1,OBJ_DEC.length()-1).toFloat();
      if (sign == "-"){ 
          OBJECT_DEC_D *= -1;
          OBJECT_DEC_M *= -1;
      }
      OBJECT_DETAILS = OBJECT_NAME + " is a ";
      OBJECT_DETAILS += Messier_Array[index_].substring(i4+1,i5) + " in constelation ";
      OBJECT_DETAILS += Messier_Array[index_].substring(i3+1,i4) + ", with visible magnitude of ";
      OBJECT_DETAILS += Messier_Array[index_].substring(i5+1,i6) + " and size of ";
      OBJECT_DETAILS += Messier_Array[index_].substring(i6+1,i7);

   }else if (objects == 1){                                     // I've selected a Treasure Object
      int i1 = Treasure_Array[index_].indexOf(';');
      int i2 = Treasure_Array[index_].indexOf(';',i1+1);
      int i3 = Treasure_Array[index_].indexOf(';',i2+1);
      int i4 = Treasure_Array[index_].indexOf(';',i3+1);
      int i5 = Treasure_Array[index_].indexOf(';',i4+1);
      int i6 = Treasure_Array[index_].indexOf(';',i5+1);
      int i7 = Treasure_Array[index_].indexOf(';',i6+1);
      OBJECT_NAME = Treasure_Array[index_].substring(0,i1);
      OBJECT_DESCR = Treasure_Array[index_].substring(i7+1,Treasure_Array[index_].length()-1);
      String OBJ_RA = Treasure_Array[index_].substring(i1,i2);
      OBJECT_RA_H = OBJ_RA.substring(1,OBJ_RA.indexOf('h')).toFloat();
      OBJECT_RA_M = OBJ_RA.substring(OBJ_RA.indexOf('h')+1,OBJ_RA.length()-1).toFloat();
      String OBJ_DEC = Treasure_Array[index_].substring(i2,i3);
      String sign = OBJ_DEC.substring(1,2);
      OBJECT_DEC_D = OBJ_DEC.substring(2,OBJ_DEC.indexOf('°')).toFloat();
      OBJECT_DEC_M = OBJ_DEC.substring(OBJ_DEC.indexOf('°')+1,OBJ_DEC.length()-1).toFloat();
      if (sign == "-"){ 
          OBJECT_DEC_D *= -1;
          OBJECT_DEC_M *= -1;
      }
      OBJECT_DETAILS = OBJECT_NAME + " is a ";
      OBJECT_DETAILS += Treasure_Array[index_].substring(i4+1,i5) + " in constelation ";
      OBJECT_DETAILS += Treasure_Array[index_].substring(i3+1,i4) + ", with visible magnitude of ";
      OBJECT_DETAILS += Treasure_Array[index_].substring(i5+1,i6) + " and size of ";
      OBJECT_DETAILS += Treasure_Array[index_].substring(i6+1,i7);

   }else if (objects == 2){                                     // I'm selecting a STAR for Synchronization - 1 Star ALLIGNMENT
      int i1 = Stars[index_].indexOf(';');
      int i2 = Stars[index_].indexOf(';',i1+1);
      int i3 = Stars[index_].indexOf(';',i2+1);
      OBJECT_NAME = Stars[index_].substring(i1+1,i2)+" from "+Stars[index_].substring(0,i1);
      String OBJ_RA = Stars[index_].substring(i2+1,i3);
      OBJECT_RA_H = OBJ_RA.substring(0,OBJ_RA.indexOf('h')).toFloat();
      OBJECT_RA_M = OBJ_RA.substring(OBJ_RA.indexOf('h')+1,OBJ_RA.length()-1).toFloat();
      String OBJ_DEC = Stars[index_].substring(i3,Stars[index_].length());
      String sign = OBJ_DEC.substring(0,1);
      OBJECT_DEC_D = OBJ_DEC.substring(1,OBJ_DEC.indexOf('°')).toFloat();
      if (sign == "-"){ 
          OBJECT_DEC_D *= (-1);
      }
      OBJECT_DEC_M = 0;
   }else if (objects == 3){                                     // I'm selecting a STAR for Synchronization - Iterative ALLIGNMENT
      int i1 = Iter_Stars[index_].indexOf(';');
      int i2 = Iter_Stars[index_].indexOf(';',i1+1);
      int i3 = Iter_Stars[index_].indexOf(';',i2+1);
      OBJECT_NAME = Iter_Stars[index_].substring(i1+1,i2)+" from "+Iter_Stars[index_].substring(0,i1);
      String OBJ_RA = Iter_Stars[index_].substring(i2+1,i3);
      OBJECT_RA_H = OBJ_RA.substring(0,OBJ_RA.indexOf('h')).toFloat();
      OBJECT_RA_M = OBJ_RA.substring(OBJ_RA.indexOf('h')+1,OBJ_RA.length()-1).toFloat();
      String OBJ_DEC = Iter_Stars[index_].substring(i3,Iter_Stars[index_].length());
      String sign = OBJ_DEC.substring(0, 1);
      OBJECT_DEC_D = OBJ_DEC.substring(1,OBJ_DEC.indexOf('°')).toFloat();
      if (sign == "-"){ 
          OBJECT_DEC_D *= (-1);
      }
      OBJECT_DEC_M = 0;
   }
}

void Sidereal_rate(){
  // when a manual movement of the drive happens. - This will avoid moving the stepepers with a wrong Step Mode.
  if ((IS_MANUAL_MOVE == false) && (IS_TRACKING) && (IS_STEPPERS_ON)){
    if (RA_mode_steps != MICROSteps){
        setmStepsMode("R",MICROSteps);
    }
    digitalWrite(RA_DIR,STP_BACK);
    PIOC->PIO_SODR=(1u<<26);
    delayMicroseconds(2);
    PIOC->PIO_CODR=(1u<<26);
//    digitalWrite(RA_STP,HIGH);
    RA_microSteps += 1;
//    digitalWrite(RA_STP,LOW);
  }
}

void Current_RA_DEC(){
      //curr_RA_H, curr_RA_M, curr_RA_S, curr_DEC_D, curr_DEC_M, curr_DEC_S;
      // curr_RA_lz, curr_DEC_lz, curr_HA_lz;
      // DEC

      // To ALSO correct for the Star Alignment offset
      float tmp_dec = (float(DEC_90) - float(abs(DEC_microSteps))) / float(DEC_D_CONST);
      tmp_dec -= delta_a_DEC;
      int sDEC_tel = 0;
      if (tmp_dec < 0){
         sDEC_tel = 45;
      }else{
         sDEC_tel = 43;
      }
      if (tmp_dec > 0){
        curr_DEC_D = floor(tmp_dec);
      }else{
        curr_DEC_D = ceil(tmp_dec);        
      }
      curr_DEC_M = (tmp_dec - floor(curr_DEC_D))*60;
      curr_DEC_S = (curr_DEC_M - floor(curr_DEC_M))*60;
        
      sprintf(curr_DEC_lz, "%c%02d%c%02d:%02d", sDEC_tel, int(abs(curr_DEC_D)), 223, int(abs(curr_DEC_M)), int(curr_DEC_S));

      // HOUR ANGLE
      // To correct for the Star Alignment
      double tmp_ha = double(RA_microSteps)/double(HA_H_CONST);
      tmp_ha -= delta_a_RA; 
      if (DEC_microSteps > 0){
          tmp_ha +=180;
      }
      tmp_ha /= 15;

      float tmp_ha_h = 0;
      float tmp_ha_m = 0;
      float tmp_ha_s = 0;
      tmp_ha_h = floor(tmp_ha);
      tmp_ha_m = (tmp_ha - floor(tmp_ha))*60;
      tmp_ha_s = (tmp_ha_m - floor(tmp_ha_m))*60;
      sprintf(curr_HA_lz, "%02d:%02d:%02d", int(tmp_ha_h), int(tmp_ha_m), int(tmp_ha_s));

      // RIGHT ASC.
      double tmp_ra = LST - tmp_ha; 
      if (LST < tmp_ha){
        tmp_ra += 24;
      }
      
      float tmp_ra_h = 0;
      float tmp_ra_m = 0;
      float tmp_ra_s = 0;
      curr_RA_H = floor(tmp_ra);
      curr_RA_M = (tmp_ra - curr_RA_H)*60;
      curr_RA_S = (curr_RA_M - floor(curr_RA_M))*60;
      sprintf(curr_RA_lz, "%02d:%02d:%02d", int(curr_RA_H), int(curr_RA_M), int(curr_RA_S));
}