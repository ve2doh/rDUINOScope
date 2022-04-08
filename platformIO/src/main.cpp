//
//    rDUINOScope - Arduino based telescope control system (GOTO).
//    Copyright (C) 2016 Dessislav Gouzgounov (Desso)
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    any later version.
//
//    PROJECT Website: http://rduinoscope.byethost24.com
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//    ALLIGNMENT Method: 1 Star Alignment - The method I have implemented is part of Mr. Ralph Pass alignment procedure described on http://rppass.com/  
//                       Mr. Pass helped rDUINOScope by providing the calculations needed to implement the method. http://rppass.com/align.pdf - the actual PDF
//                       
//                       Iterative Alignment - The method is based on article from "New" Hamilton Astronomy.com website: http://astro.hcadvantage.com
//                       Actual PDF document: http://astro.hcadvantage.com/ASTRO_ARTICLES/Polar_Alignment_Part_II.pdf
//          
//
//

////////////////////////////////////////////////
#include <Arduino.h>
#include "DHT.h"
#include <Time.h>
//#include <ads7843.h>
//#include <UTouch.h>           // Because my TFT has on other Touch Chip, we use te UTouch Librarry for XPT2046 instied of ADS7843
#include <TouchScreen.h>         //Adafruit Library
#include <SPI.h>
#include <SD.h>
#include <Adafruit_GFX.h>       // Core graphics library
//#include <tftlib.h>           // Hardware-specific library
#include <MCUFRIEND_kbv.h>
#include <DueTimer.h>           // interruptions library0
//#include <DS3231.h>
#include <math.h>
#include "main.h"
#include "globalVariable.h"
#include "graphic_screens.h"
#include "colorDefine.h"
#include "soundDefine.h"
#include "pinout.h"
#include "regular_updates.h"
#include "sideralMath.h"
#include "bluetooth.h"
#include "touch_inputs.h"
#include "motionCtrl.h"

// HERE GOES THE Mount, Gears and Drive information.
// ... used to calculate the HourAngle to microSteps ratio
// UPDATE THIS PART according to your SET-UP
// ---------------------------------------------
// NB: RA and DEC uses the same gear ratio (144 tooth in my case)!
//----------------------------------------------
//#define PSTR(a)  a
// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
//#define LCD_CS A3 // Chip Select goes to Analog 3
//#define LCD_CD A2 // Command/Data goes to Analog 2
//#define LCD_WR A1 // LCD Write goes to Analog 1
//#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

/** ADS7843 pin map */
#ifdef ELECFREAKS_TFT_SHIELD_V2
#define DCLK     6
#define CS       5  
#define DIN      4 
#define DOUT     3
#define IRQ      2 
#elif defined ELECHOUSE_DUE_TFT_SHIELD_V1
/** elechouse TFT shield pin map */
#define DCLK     25
#define CS       26 
#define DIN      27 
#define DOUT     29
#define IRQ      30
#endif

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);  // Initialize GPS communication on PINs: 17 (RX) and 16 (TX) 
  Serial3.begin(9600); // Bluetooth communication on PINs:  15 (RX) and 14 (TX)  

  pinMode(speakerOut, OUTPUT);

  // below variables are used to calculate the paramters where the drive works
  float ww = WORM*REDUCTOR;
  float www = DRIVE_STP*MICROSteps;
 
  MicroSteps_360 = ww*www;
  RA_90 = MicroSteps_360 / 4;  // How much in microSteps the RA motor have to turn in order to make 6h = 90 degrees;
  DEC_90 = RA_90;   // How mich in microSteps the DEC motor have to turn in order to make 6h = 90 degrees;
  HA_H_CONST = MicroSteps_360/360;
  DEC_D_CONST = HA_H_CONST;
  
  Clock_Sidereal = 1000000/(MicroSteps_360/SIDEREAL_DAY);  // This way I make the interruption occuer 2wice faster than needed - REASON: allow max time for Pin-UP, Pin-DOWN action
  Clock_Solar = 1000000/(MicroSteps_360/(SIDEREAL_DAY-235.9095));
  Clock_Lunar = 1000000/(MicroSteps_360/(SIDEREAL_DAY-2089.2292));

  //analogWriteResolution(12);
  analogWrite(TFTBright, 255);  // LCD Backlight brightness 255 = ~100%)
  
//  touch.begin();
//  myTouch.InitTouch(LANDSCAPE);
//  myTouch.setPrecision(PREC_MEDIUM); 

  rtc.begin();
  dht.begin();

  tft.begin(readID());
  tft.fillScreen(BLACK);

// DRB8825 - drive mode pins (determine Steppping Modes 1/8, 1/16 and etc.
  pinMode(RA_MODE0, OUTPUT); 
  pinMode(RA_MODE1, OUTPUT); 
  pinMode(RA_MODE2, OUTPUT);

  pinMode(DEC_MODE0, OUTPUT); 
  pinMode(DEC_MODE1, OUTPUT); 
  pinMode(DEC_MODE2, OUTPUT);

  pinMode(RA_STP,OUTPUT); // RA Step
  pinMode(RA_DIR,OUTPUT); // RA Dir
  pinMode(DEC_STP,OUTPUT); // DEC Step
  pinMode(DEC_DIR,OUTPUT); // DEC Dir

  //digitalWrite(RA_DIR,HIGH); // Set Dir high
  //digitalWrite(RA_STP,LOW);
  //digitalWrite(DEC_DIR,HIGH); // Set Dir high
  //digitalWrite(DEC_STP,LOW);

  // Joystick
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);

  // Device 1 & 2 command Pins
  pinMode(DEV1, OUTPUT);
  pinMode(DEV2, OUTPUT);

  // Pin to define Power to the DRV8825 using MOSFET!
  pinMode(POWER_DRV8825, OUTPUT);

  // Set RA and DEC microstep position 
  RA_microSteps = RA_90; //  --> point to North Sudereal Pole = -180 deg (-12h)
  DEC_microSteps = 0; //  --> Point to North Sudereal Pole = 90 deg
   
  Timer3.attachInterrupt(Sidereal_rate);
//  Timer3.start(Clock_Sidereal); // executes the code every 62.329 ms.

  if (analogRead(nightModePin) < 800){
    IS_NIGHTMODE = true;
    texts = Maroon;
    l_text = RED;
    d_text = Maroon;
    title_bg = RED;
    title_texts = BLACK;
    messie_btn = Maroon;
    btn_l_text = RED;
    btn_d_text = Maroon;
    btn_l_border = RED;
    btn_d_border = Maroon;
    btn_l_selection = RED;
    MsgBox_bg = RED;
    MsgBox_t = BLACK;
  } else {
    IS_NIGHTMODE = false;
    texts = LightGrey;
    l_text = WHITE;
    d_text = LightGrey;
    btn_l_text = GreenYellow;
    btn_d_text = DarkGreen;
    btn_l_border = GREEN;
    btn_d_border = DarkGreen;
    btn_l_selection = DarkGreen;
    title_bg = Orange;
    title_texts = BLACK;
    messie_btn = DarkGrey;
    MsgBox_bg = Purple;
    MsgBox_t = GreenYellow;
  }


  // Draw initial screen - INITIALIZE
  // The below part cannot be removed form the code
  // You can add messages, but not remove!
  tft.setCursor(20, 10);
  tft.setTextColor(title_bg);    
  tft.setTextSize(3);
  tft.println("rDUINOScope");
  tft.setTextColor(l_text);    
  tft.setTextSize(2);
  tft.setCursor(20, 35);
  tft.setTextColor(l_text);    
  tft.print("coded by <dEskoG>");
  tft.setCursor(0, 50);
  tft.print("Dessislav Gouzgounov");
  tft.setCursor(33, 70);
  tft.setTextSize(1);
  tft.print("rduinoscope.byethost24.com");
  tft.setCursor(33, 80);
  tft.setTextColor(d_text);    
  tft.print("GNU General Public License");
  tft.setCursor(42, 90);
  tft.setTextColor(d_text);    
  tft.println("Version: " + FirmwareNumber);


  if (!SD.begin(sd_cs)) {
    tft.println("ERROR: Card failed, or not present");
    // don't do anything more:
    while(1);
  }
  
  // Draw Supporters Logos
  String PIC_Supporters = "hackad16.bmp";
  StarMaps = SD.open(PIC_Supporters.c_str(), FILE_READ);
  drawPic(&StarMaps, 0, 375, 120, 24);
  StarMaps.close();
  delay(200);
  //tft.setCursor(5, 290);
  //tft.println("FINALIST");
  tft.setCursor(0, 365);
  tft.setTextColor(btn_l_text);
  tft.println("SUPPORTERS:");
  tft.setTextColor(d_text); 
  tft.setCursor(0, 120);


  // EMPIRIAL MARCH   :)
  if (IS_SOUND_ON){
    SoundOn(note_f, 48);
    delay(100);
    SoundOn(note_f, 48);
    delay(100);
    SoundOn(note_f, 48);
    delay(100);
    SoundOn(note_cb, 32);
    delay(140);
    SoundOn(note_gb, 8);
    delay(50);
    SoundOn(note_f, 48);
    delay(100);
    SoundOn(note_cb, 32);
    delay(140);
    SoundOn(note_gb, 8);
    delay(50);
    SoundOn(note_f, 48);
  }

  // Indiana Jones :)
  // notes: E-E-F-G-C-C-C-C   D-D-E-F-F-F-F   G-G-A-B-F-F-F-F-F   A-A-B-C-D-E-E-E-E 
  // Write the code if you are a fan :)
  
  // see if the card is present and can be initialized:
  char in_char;
  String items="";
  int j=0;
  int k=0;
  MESS_PAGER = 0;
  TREAS_PAGER = 0;
  STARS_PAGER = 0;

  tft.println("...card initialized");
  delay(200);
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("messier.csv");

  // if the file is available, write to it:
  if (dataFile) {
    tft.println("...loading data from MESSIER.CSV");
    delay(100);
    while (dataFile.available()) {
        in_char=dataFile.read();
        items += in_char;
        k=k+1;
        if(in_char == '\n'){
          Messier_Array[j] = items;
          j=j+1;
//          Serial.print(items);
          items="";
        }
    }
    tft.println("...loading Messier COMPLETED");
    delay(100);
  
  }else {
    tft.println("ERROR opening: messier.csv");
  }
  
  dataFile.close();  
  items="";
  j=0;
  k=0;
  dataFile = SD.open("treasure.csv");


  // if the file is available, write to it:
  if (dataFile) {
    tft.println("...loading data from TREASURE.CSV");
    delay(100);
    while (dataFile.available()) {
        in_char=dataFile.read();
        items += in_char;
        k=k+1;
        if(in_char == '\n'){
          Treasure_Array[j] = items;
          j=j+1;
//          Serial.print(items);
          items="";
        }
    }
    tft.println("...loading Treasures COMPLETED");
    delay(100);
  
  }else {
    tft.println("ERROR opening: treasure.csv");
  }
  dataFile.close();
  last_button=0;
  LOAD_SELECTOR = 0;

  tft.println(".................................");
  tft.println("...initializing BlueTooth");
  delay(100);
  tft.println("...initializing GPS");

  delay(1000);
  CURRENT_SCREEN = 0;
  drawGPSScreen();
  UPD_T = millis();
  UPD_LST = millis();
  DELAY_Slew = millis();
  TFT_Timer = millis();
  TFT_timeout = 0;
  RA_move_ending = 0;
  considerTempUpdates();

  digitalWrite(POWER_DRV8825, HIGH); // Switch on the Motor Diver Power!
}

void loop() {

  // This is done in order to prevent multiple calculations of LST_HA per second (especially while SlewTo) and only 
  // do it once the DEC SlewTo slows down, but before stopping OR once every 10 seconds (in order to do the Meridian Flip)
  if (RA_move_ending == 1){
     calculateLST_HA();
  }

  if ((IS_MERIDIAN_FLIP_AUTOMATIC) && ((UPD_LST + 10000) <= millis()) && (IS_OBJ_FOUND == true)){
     calculateLST_HA();
     UPD_LST = millis();
  }

 // Adding this delay to SLOW DOWN the Arduino so that the motors can catch up!
 // The delay is only needed when in full speed.... otherways the CalculateLST_HA() takes over and
 // slows down the arduino enought. CalculateLST_HA() when slewing only fires when the motors slows down
 // after they are very close to the Object Position.
 if ((DELAY_Slew + 1 <= millis()) && (IS_OBJ_FOUND == false)){
  
    // If you wonder how I get to this delay - 800 uS
    // When I optimised the code for speed, the main delay was coming from calculateLST_HA() which back then was calculated on every Loop();
    // Once I optimized it to only calculate when the SlewTo stops (to fine tune after DEC stops) it turned out that
    // the code is too fast and the motors only "screemed" but not rotating - due to the low voltage/current.
    // This variable depends on How You Limit the Current to your motors and the Voltage you use!
    // I use 12V and 1.6A (70% in full step = 1.10A) to drive my NEMA 17 SY42STH47-1684B Motors.
    // Please note that Potentiometer does not really give consistent results for current on every restart (it drifted between 1.12A - 0.9A).

    // HINT: you can try to play with the Current/Voltage that powers the mottors to get faster speeds.
    if (IS_STEPPERS_ON){
      cosiderSlewTo();
    }else{
      IS_OBJECT_RA_FOUND = true;
      IS_OBJECT_DEC_FOUND = true;
      IS_OBJ_FOUND = true;
      RA_move_ending = 0;
    }
    DELAY_Slew = millis();
    // delayMicroseconds(800);
 }

  // The below part of the code makes sure that the system does NOT process any other inputs while SlweingTo!
  // Since both motors need every STEP to come from Arduino board, it needs it's entire power to run the motors in fastest possible way
  // The fastes possible from this board in the current state of the software is approx 3 turns/sec (600 steps/sec)
  // IS_OBJ_FOUND == true --> Means that SLEW command have completed
  //
  if (IS_OBJ_FOUND == true){    


      // BLUETOOTH Considerations ? ... if any
      if ((IS_BT_MODE_ON == true)&&(Serial3.available()>0)&&(IS_MANUAL_MOVE == false)){
           BT_COMMAND_STR = Serial3.readStringUntil('#');
           //Serial.println(BT_COMMAND_STR);
           considerBTCommands();
      }


      // JOYSTICK Movements ? ... if any
      xPosition = analogRead(xPin);
      yPosition = analogRead(yPin);
      
      if ((xPosition < 470) || (xPosition > 620) || (yPosition < 470) || (yPosition > 620)){
        IS_MANUAL_MOVE = true;
        if (IS_STEPPERS_ON){
          consider_Manual_Move(xPosition, yPosition);
        }
      }else{
        IS_MANUAL_MOVE = false;
      }

      // TOUCH SCREEN Inputs ? ... if any
      //uint8_t flag;
      //int tx, ty;
      //p=touch.getpos(&flag);
      //tx=(p.x- 310)/14;
      //ty=(p.y- 150)/9;

      // This will take care of turning OFF the TFT's background light if the device is not used
      // for XXX amont of seconds and IS_IN_OPERATION = TRUE
      if ((TFT_timeout > 0) && (millis() - TFT_Timer > TFT_timeout) && (IS_TFT_ON)&&(IS_IN_OPERATION)){
         analogWrite(TFTBright, 0);
         IS_TFT_ON = false;
      }

      int tx = 0;
      int ty = 0;
    
      if (touchDetection() == true){  
          tx = myTouch.getPoint().x;
          ty = myTouch.getPoint().y;
       }
      considerTouchInput(tx, ty);

      // OTHER UPDATES ?  ... if any
      // Happens every 2 seconds
      if (((millis()- UPD_T) > 2000)&&(IS_MANUAL_MOVE == false)){
        calculateLST_HA();  // Make sure it Updates the LST! used on Main Screen and When Calculating current Coords. 
        considerTimeUpdates();
        considerDayNightMode();
        considerTempUpdates();
        // I need to make sure the Drives are not moved to track the stars,
        // if Object is below horizon ALT < 0 - Stop tracking.
        if ((ALT <= 0) && (IS_TRACKING==true) && (IS_IN_OPERATION == true)){
            IS_TRACKING = false;
            Timer3.stop();
            drawMainScreen();
        }
        UPD_T = millis();
      }
  }
}