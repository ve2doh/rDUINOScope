#include <MCUFRIEND_kbv.h>
#include <DS3231.h>
#include <TinyGPS++.h>
#include <SD.h>
#include <DHT.h>
#include <TouchScreen.h>


#ifndef __globalVarDef_h__
#define __globalVarDef_h__

#define DHTPIN 3
#define DHTTYPE DHT22

#define MINPRESSURE 200
#define MAXPRESSURE 1000

const String FirmwareDate = "03 08 17";
const String FirmwareNumber = "v2.3.1 Boiana EQ";
const String FirmwareName = "rDUINOScope";
const String FirmwareTime = "12:00:00";

const String Summer = "SUMMER";

extern int dateEntryPos;
extern MCUFRIEND_kbv tft;

extern int16_t texts, l_text, d_text, btn_l_border, btn_d_border, btn_l_text, btn_d_text, btn_l_selection, title_bg, title_texts, messie_btn, file_btn, ngc_btn, MsgBox_bg, MsgBox_t;    // defines string constants for the clor - Depending on the DAY/NIGHT modes

extern int XP, YP, XM, YM; 
extern TouchScreen  myTouch;

//
//..... Below are the VARs for the code. Most of them are self explenatory
//
extern DHT dht;
//ADS7843 touch(CS, DCLK, DIN, DOUT, IRQ);
//Point p;


extern String Messier_Array[120];
extern String Treasure_Array[130];
extern String ObservedObjects[50];
extern String Iter_Stars[50];
extern int int_star_count;       // Counter for how many stars are loaded into Iter_Stars[] array.... to be used with Pages, so that it does not show more pages than needed
extern String Stars[];

extern int Observed_Obj_Count;
extern int ALLIGN_STEP;  // Using this variable to count the allignment steps - 1: Synchronize, 2: Allign and Center, 3:....
extern int ALLIGN_TYPE;  // Variable to store the alignment type (0-Skip Alignment, 1-1 Star alignment, 2-2 Star alignment
extern float delta_a_RA;
extern float delta_a_DEC;
extern int Iterative_Star_Index;
extern String Prev_Obj_Start;
extern int lastScreenUpdateTimer;
extern unsigned long Slew_timer, Slew_RA_timer;
extern int OBJECT_Index;
extern String OBJECT_NAME;
extern String OBJECT_DESCR;
extern String OBJECT_DETAILS;
extern String BT_COMMAND_STR;
extern String START_TIME;
extern int STP_FWD;
extern int STP_BACK;
extern float OBJECT_RA_H;
extern float OBJECT_RA_M;
extern float OBJECT_DEC_D;
extern float OBJECT_DEC_M;
extern float OBJECT_MAG;
extern float curr_RA_H, curr_RA_M, curr_RA_S, curr_DEC_D, curr_DEC_M, curr_DEC_S;    // Global variables to store Mount's current RA and DEC.
extern char curr_RA_lz[9], curr_DEC_lz[10], curr_HA_lz[9];                                                // Global variable to store current RA and DEC with Leading Zeroes and sign (RA: 00:00:00; DEC: +/-00*00:00)
extern int SELECTED_STAR;
extern double DELTA_RA_ADJUST; // cos RA
extern double DELTA_DEC_ADJUST; // cos DEC
// Default values to load when CANCEL button is hit on the GPS screen
extern float OBSERVATION_LONGITUDE; // (23.3333* - Home)
extern float OBSERVATION_LATTITUDE; // (42.6378* - Home)
extern float OBSERVATION_ALTITUDE; // Sofia, Bulgaria
extern int TIME_ZONE;
// .............................................................
extern int GPS_iterrations;
extern double LST, HAHour, HAMin, ALT, AZ;
extern double JD;
extern String BTs;

extern int last_button, MESS_PAGER, TREAS_PAGER, STARS_PAGER;
extern boolean IS_TFT_ON;
extern boolean IS_STEPPERS_ON;
extern boolean IS_OBJ_VISIBLE;
extern boolean IS_IN_OPERATION;  // This variable becomes True when Main screen appears
extern boolean IS_TRACKING;
extern boolean IS_NIGHTMODE;
extern boolean IS_OBJ_FOUND;
extern boolean IS_OBJECT_RA_FOUND;
extern boolean IS_OBJECT_DEC_FOUND;
extern boolean IS_MERIDIAN_PASSED;
extern boolean IS_POSIBLE_MERIDIAN_FLIP;
extern boolean IS_MERIDIAN_FLIP_AUTOMATIC;
extern boolean IS_BT_MODE_ON;
extern boolean IS_MANUAL_MOVE;
extern boolean IS_DEV1_ON;
extern boolean IS_DEV2_ON;
extern boolean IS_CUSTOM_MAP_SELECTED;
extern boolean IS_SOUND_ON;
extern int TFT_Brightness;
extern int MAIN_SCREEN_MENU;
extern int CURRENT_SCREEN; 
extern int LOAD_SELECTOR;   // selector to show which LOADING mechanism is used: 1 - Messier, 2 - File, 3 - NGCs

extern int RA_microSteps, DEC_microSteps, rev_RA_microSteps, rev_DEC_microSteps;              // Current position of the motors in MicroSteps! - when movement occures, values are changed accordingly (manual, tracking or slew to);
extern int RA_mode_steps, DEC_mode_steps;
extern int SLEW_RA_microsteps, SLEW_DEC_microsteps;    // Where the mottors needs to go in order to point to the object
extern int RA_finish_last;
extern int map_r;    // Used to determine the StarMap Row ... image name (1-1.bmp; 1-2.bmp ....)
extern int map_c;    // Ued to determine the StarMap Column .... image name

extern TinyGPSPlus gps;
extern DS3231 rtc;           // (SDA, SCL) from the RTC board
extern int sd_cs;               // TFTShield sd card chip select pin is Arduino digital pin 42

extern String old_t, old_d;
extern String Start_date;
extern unsigned long update_time, Tupdate_time, TFT_timeout;
extern unsigned long UPD_T, UPD_coord, DELAY_Slew, UPD_LST, TFT_Timer;
extern int RA_move_ending;
extern int w_DateTime[12];  // array to store date - as updated from updater screen - Wishing_Date
extern int Summer_Time;
extern int xPosition;  // Joystick
extern int yPosition;  // Joystick
extern float _temp,_Stemp;
extern float _humid,_Shumid;
extern File roots;
extern File StarMaps;                    // bmp files

// Some variables used for Alignment procedure:
extern double Star_1_HA;
extern double Star_1_DEC;
extern double Star_2_HA;
extern double Star_2_DEC;
extern double err_RA;
extern double err_DEC;
extern double Eerr_RA;
extern double Eerr_DEC;
extern double err_ALT;
extern double err_AZ;
extern double det;


extern int WORM;
extern float REDUCTOR;      // 1:4 gear reduction
extern int DRIVE_STP;   // Stepper drive have 200 steps per revolution
extern int MICROSteps;   // I'll use 1/16 microsteps mode to drive sidereal - also determines the LOWEST speed.

// below variables are used to calculate the paramters where the drive works
extern int ARCSEC_F_ROTAT;    // ArcSeconds in a Full earth rotation;
extern float SIDEREAL_DAY;   // Sidereal day in seconds
extern float ArcSECstep;
extern int MicroSteps_360;
extern int RA_90;  // How much in microSteps the RA motor have to turn in order to make 6h = 90 degrees;
extern int DEC_90;   // How mich in microSteps the DEC motor have to turn in order to make 6h = 90 degrees;
extern float HA_H_CONST;
extern int HA_M_CONST;
extern float DEC_D_CONST;
extern int DEC_M_CONST;
extern int MIN_TO_MERIDIAN_FLIP;   // This constant tells the system when to do the Meridian Flip. "= 2" means 2 minutes before 24:00h (e.g. 23:58h)
extern int MIN_SOUND_BEFORE_FLIP;   // This constant tells the system to start Sound before it makes Meridian Flip
extern float mer_flp;                   // The calculateLST_HA() function depending on this timer will convert the HA and DEC to the propper ones to do the flip.
extern boolean MERIDIAN_FLIP_DO;
extern int Tracking_type;  // 1: Sidereal, 2: Solar, 0: Lunar;
extern int Clock_Sidereal;  // Variable for the Interruptions. nterruption is initialized depending on the DATA above -in miliseconds
extern int Clock_Solar;  // Variable for the Interruptions. nterruption is initialized depending on the DATA above -in miliseconds
extern int Clock_Lunar;  // Variable for the Interruptions. nterruption is initialized depending on the DATA above -in miliseconds


#endif