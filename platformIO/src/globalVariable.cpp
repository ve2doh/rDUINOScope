#include "globalVariable.h"
#include "pinout.h"

int dateEntryPos = 0;
MCUFRIEND_kbv tft;
int16_t texts, l_text, d_text, btn_l_border, btn_d_border, btn_l_text, btn_d_text, btn_l_selection, title_bg, title_texts, messie_btn, file_btn, ngc_btn, MsgBox_bg, MsgBox_t;    // defines string constants for the clor - Depending on the DAY/NIGHT modes

const int TS_LEFT=160,TS_RT=834; //for X axis
const int TS_TOP=132,TS_BOT=921;//for y axis

int YP = A1, YM = 7;
int XP = 6, XM = A2; 
TouchScreen  myTouch(XP, YP, XM, YM, 300);

DHT dht(DHTPIN, DHTTYPE);

String Messier_Array[120];
String Treasure_Array[130];
String ObservedObjects[50];
String Iter_Stars[50];
int int_star_count = 0;       // Counter for how many stars are loaded into Iter_Stars[] array.... to be used with Pages, so that it does not show more pages than needed
String Stars[] = {            // Definition of the ALIGNMENT Stars. Add as many stars as you like in here
"And;Almaak;2h4m;42.3°",
"And;Alpheratz;0h8m;29.1°",
"And;Mirach;1h10m;35.6°",
"Aql;Altair;19h51m;8.9°",
"Aql;Althalimain;19h6m;-4.9°",
"Aql;Deneb el Okab;19h5m;13.9°",
"Aql;Tarazed;19h46m;10.6°",
"Aqr;Sadalmelik;22h6m;-0.3°",
"Aqr;Sadalsuud;21h32m;-5.6°",
"Aqr;Skat;22h55m;-15.8°",
"Ara;Choo;17h32m;-49.9°",
"Ari;Hamal;2h7m;23.5°",
"Ari;Sheratan;1h55m;20.8°",
"Aur;Almaaz;5h2m;43.8°",
"Aur;Capella;5h17m;46°",
"Aur;Hassaleh;4h57m;33.2°",
"Aur;Hoedus II;5h7m;41.2°",
"Aur;Menkalinan;6h0m;44.9°",
"Boo;Arcturus;14h16m;19.2°",
"Boo;Izar;14h45m;27.1°",
"Boo;Muphrid;13h55m;18.4°",
"Boo;Nekkar;15h2m;40.4°",
"Boo;Seginus;14h32m;38.3°",
"Cap;Dabih;20h21m;-14.8°",
"Cap;Deneb Algedi;21h47m;-16.1°",
"Car;Aspidiske;9h17m;-59.3°",
"Car;Avior;8h23m;-59.5°",
"Car;Canopus;6h24m;-52.7°",
"Car;Miaplacidus;9h13m;-69.7°",
"Cas;Achird;0h49m;57.8°",
"Cas;Caph;0h9m;59.2°",
"Cas;Cih;0h57m;60.7°",
"Cas;Ruchbah;1h26m;60.2°",
"Cas;Schedar;0h41m;56.5°",
"Cas;Segin;1h54m;63.7°",
"Cen;Al Nair al Kent;13h56m;-47.3°",
"Cen;Hadar;14h4m;-60.4°",
"Cen;Ke Kwan;14h59m;-42.1°",
"Cen;Menkent;14h7m;-36.4°",
"Cen;Muhlifain;12h42m;-49°",
"Cen;Rigil Kentaurus;14h40m;-60.8°",
"Cep;Alderamin;21h19m;62.6°",
"Cep;Alphirk;21h29m;70.6°",
"Cep;Errai;23h39m;77.6°",
"Cet;Dheneb;1h9m;-10.2°",
"Cet;Diphda;0h44m;-18°",
"Cet;Kaffaljidhma;2h43m;3.2°",
"Cet;Menkar;3h2m;4.1°",
"CMa;Adhara;6h59m;-29°",
"CMa;Aludra;7h24m;-29.3°",
"CMa;Mirzam;6h23m;-18°",
"CMa;Phurad;6h20m;-30.1°",
"CMa;Sirius;6h45m;-16.7°",
"CMa;Wezen;7h8m;-26.4°",
"CMi;Gomeisa;7h27m;8.3°",
"CMi;Procyon;7h39m;5.2°",
"Cnc;Tarf;8h17m;9.2°",
"Col;Phact;5h40m;-34.1°",
"Col;Wazn;5h51m;-35.8°",
"CrB;Alphecca;15h35m;26.7°",
"Cru;Acrux;12h27m;-63.1°",
"Cru;Gacrux;12h31m;-57.1°",
"Cru;Mimosa;2h48m;-59.7°",
"Crv;Algorel;12h30m;-16.5°",
"Crv;Gienah Ghurab;12h16m;-17.5°",
"Crv;Kraz;12h34m;-23.4°",
"Crv;Minkar;12h10m;-22.6°",
"CVn;Cor Caroli;12h56m;38.3°",
"Cyg;Albireo;19h31m;28°",
"Cyg;Deneb;20h41m;45.3°",
"Cyg;Gienah;20h46m;34°",
"Cyg;Sadr;20h22m;40.3°",
"Dra;Aldhibah;17h9m;65.7°",
"Dra;Aldhibain;16h24m;61.5°",
"Dra;Edasich;15h25m;59°",
"Dra;Eltanin;17h57m;51.5°",
"Dra;Rastaban;17h30m;52.3°",
"Dra;Tais;19h13m;67.7°",
"Eri;Acamar;2h58m;-40.3°",
"Eri;Achernar;1h38m;-57.2°",
"Eri;Kursa;5h8m;-5.1°",
"Eri;Rana;3h43m;-9.8°",
"Eri;Zaurak;3h58m;-13.5°",
"Gem;Alhena;6h38m;16.4°",
"Gem;Alzirr;6h45m;12.9°",
"Gem;Castor;7h35m;31.9°",
"Gem;Mebsuta;6h44m;25.1°",
"Gem;Pollux;7h45m;28°",
"Gem;Propus;6h15m;22.5°",
"Gem;Tejat;6h23m;22.5°",
"Gem;Wasat;7h20m;22°",
"Gru;Al Dhanab;22h43m;-46.9°",
"Gru;Alnair;22h8m;-47°",
"Her;Kornephoros;16h30m;21.5°",
"Her;Rasalgethi;17h15m;14.4°",
"Her;Rutilicus;16h41m;31.6°",
"Her;Sarin;17h15m;24.8°",
"Hya;Alphard;9h28m;-8.7°",
"Hyi;Head of Hydrus;1h59m;-61.6°",
"Ind;Persian;20h38m;-47.3°",
"Leo;Algieba;10h20m;19.8°",
"Leo;Chort;11h14m;15.4°",
"Leo;Denebola;11h49m;14.6°",
"Leo;dhafera;10h17m;23.4°",
"Leo;Ras Elased Austr;9h46m;23.8°",
"Leo;Regulus;10h8m;12°",
"Leo;Subra;9h41m;9.9°",
"Leo;Zosma;11h14m;20.5°",
"Lep;Arneb;5h33m;-17.8°",
"Lep;Nihal;5h28m;-20.8°",
"Lib;Brachium;15h04m;-25.3°",
"Lib;Zubenelgenubi;14h51m;-16°",
"Lib;Zubeneschamali;15h17m;-9.4°",
"Lup;Ke Kouan;14h59m;-43.1°",
"Lup;Men;14h42m;-47.4°",
"Lyr;Sheliak;18h50m;33.4°",
"Lyr;Sulaphat;18h59m;32.7°",
"Lyr;Vega;18h37m;38.8°",
"Oph;Cebalrai;17h43m;4.6°",
"Oph;Han;16h37m;-10.6°",
"Oph;Rasalhague;17h35m;12.6°",
"Oph;Sabik;17h10m;-15.7°",
"Oph;Yed Posterior;16h18m;-4.7°",
"Oph;Yed Prior;16h14m;-3.7°",
"Ori;Algjebbah;5h24m;-2.4°",
"Ori;Alnilam;5h36m;-1.2°",
"Ori;Alnitak;5h41m;-1.9°",
"Ori;Bellatrix;5h25m;6.3°",
"Ori;Betelgeuse;5h55m;7.41°",
"Ori;Hatysa;5h35m;-5.9°",
"Ori;Meissa;5h35m;9.9°",
"Ori;Mintaka;5h32m;-0.3°",
"Ori;Rigel;5h15m;-8.2°",
"Ori;Saiph;5h48m;-9.7°",
"Ori;Tabit;4h50m;7°",
"Pav;Peacock;20h26m;-56.7°",
"Peg;Algenib;0h13m;15.2°",
"Peg;Baham;22h10m;6.2°",
"Peg;Enif;21h44m;9.9°",
"Peg;Homam;22h41m;10.8°",
"Peg;Markab;23h5m;15.2°",
"Peg;Matar;22h43m;30.2°",
"Peg;Sadalbari;22h50m;24.6°",
"Peg;Scheat;23h4m;28.1°",
"Per;Algol;3h8m;41°",
"Per;Gorgonea Tertia;3h5m;38.8°",
"Per;Mirfak;3h24m;49.9°",
"Phe;Ankaa;0h26m;-42.3°",
"PsA;Fomalhaut;22h58m;-29.6°",
"Pup;Asmidiske;7h49m;-24.9°",
"Pup;Naos;8h4m;-40°",
"Pup;Turais;8h8m;-24.3°",
"Sco;Alniyat;16h21m;-25.6°",
"Sco;Antares;16h29m;-26.4°",
"Sco;Dschubba;16h0m;-22.6°",
"Sco;Girtab;17h42m;-39°",
"Sco;Graffias;16h5m;-19.8°",
"Sco;Lesath;17h31m;-37.3°",
"Sco;Sargas;17h37m;-43°",
"Sco;Shaula;17h34m;-37.1°",
"Sco;We;16h50m;-34.3°",
"Ser;Alava;18h21m;-2.9°",
"Ser;Unukalhai;15h44m;6.4°",
"Sgr;Albaldah;9h10m;-21°",
"Sgr;Alnasl;18h6m;-30.4°",
"Sgr;Ascella;19h3m;-29.9°",
"Sgr;Kaus Australis;18h24m;-34.4°",
"Sgr;Kaus Borealis;18h28m;-25.4°",
"Sgr;Kaus Meridionalis;18h21m;-29.8°",
"Sgr;Nunki;18h55m;-26.3°",
"Tau;Ain;4h29m;19.2°",
"Tau;Alcyone;3h47m;24.1°",
"Tau;Aldebaran;4h36m;16.5°",
"Tau;Alheka;5h38m;21.1°",
"Tau;Elnath;5h26m;28.6°",
"TrA;Atria;16h49m;-69°",
"Tri;Mothallah;1h53m;29.6°",
"UMa;Al Haud;9h33m;51.7°",
"UMa;Alioth;12h54m;56°",
"UMa;Alkaid;13h48m;49.3°",
"UMa;Alula Borealis;11h18m;33.1°",
"UMa;Dubhe;11h4m;61.8°",
"UMa;Megrez;12h15m;57°",
"UMa;Merak;11h2m;56.4°",
"UMa;Mizar;13h24m;54.9°",
"UMa;Muscida;8h30m;60.7°",
"UMa;Phecda;11h54m;53.7°",
"UMa;Talita;8h59m;48°",
"UMa;Tania Australis;10h22m;41.5°",
"UMa;Tania Borealis;10h17m;42.9°",
"UMi;Kochab;14h51m;74.2°",
"UMi;Pherkad Major;15h21m;71.8°",
"UMi;Polaris;2h32m;89.3°",
"Vel;Koo She;8h45m;-54.7°",
"Vel;Markeb;9h22m;-55°",
"Vel;Regor;8h10m;-47.3°",
"Vel;Suhail;9h8m;-43.4°",
"Vel;Tseen Ke;9h57m;-54.6°",
"Vir;Auva;12h56m;3.4°",
"Vir;Heze;13h35m;-0.6°",
"Vir;Porrima;12h42m;-1.5°",
"Vir;Spica;13h25m;-11.2°",
"Vir;Vindemiatrix;13h2m;11°"
};

int Observed_Obj_Count = 0;
int ALLIGN_STEP = 0;  // Using this variable to count the allignment steps - 1: Synchronize, 2: Allign and Center, 3:....
int ALLIGN_TYPE = 0;  // Variable to store the alignment type (0-Skip Alignment, 1-1 Star alignment, 2-2 Star alignment
float delta_a_RA = 0;
float delta_a_DEC = 0;
int Iterative_Star_Index = 0;
String Prev_Obj_Start;
int lastScreenUpdateTimer;
unsigned long Slew_timer, Slew_RA_timer = 0;
int OBJECT_Index;
String OBJECT_NAME;
String OBJECT_DESCR;
String OBJECT_DETAILS;
String BT_COMMAND_STR;
String START_TIME;
int STP_FWD = LOW;
int STP_BACK = HIGH;
float OBJECT_RA_H;
float OBJECT_RA_M;
float OBJECT_DEC_D;
float OBJECT_DEC_M;
float OBJECT_MAG;
float curr_RA_H, curr_RA_M, curr_RA_S, curr_DEC_D, curr_DEC_M, curr_DEC_S;    // Global variables to store Mount's current RA and DEC.
char curr_RA_lz[9], curr_DEC_lz[10], curr_HA_lz[9];                                                // Global variable to store current RA and DEC with Leading Zeroes and sign (RA: 00:00:00; DEC: +/-00*00:00)
int SELECTED_STAR = 0;
double DELTA_RA_ADJUST = 1; // cos RA
double DELTA_DEC_ADJUST = 1; // cos DEC
// Default values to load when CANCEL button is hit on the GPS screen
float OBSERVATION_LONGITUDE = 23.3333; // (23.3333* - Home)
float OBSERVATION_LATTITUDE = 42.6378; // (42.6378* - Home)
float OBSERVATION_ALTITUDE = 760.60; // Sofia, Bulgaria
int TIME_ZONE = 2;
// .............................................................
int GPS_iterrations = 0;
double LST, HAHour, HAMin, ALT, AZ;
double JD;
String BTs;

int last_button, MESS_PAGER, TREAS_PAGER, STARS_PAGER;
boolean IS_TFT_ON = true;
boolean IS_STEPPERS_ON = true;
boolean IS_OBJ_VISIBLE = false;
boolean IS_IN_OPERATION = false;  // This variable becomes True when Main screen appears
boolean IS_TRACKING = false;
boolean IS_NIGHTMODE;
boolean IS_OBJ_FOUND = true;
boolean IS_OBJECT_RA_FOUND = true;
boolean IS_OBJECT_DEC_FOUND = true;
boolean IS_MERIDIAN_PASSED = false;
boolean IS_POSIBLE_MERIDIAN_FLIP = true;
boolean IS_MERIDIAN_FLIP_AUTOMATIC = true;
boolean IS_BT_MODE_ON = false;
boolean IS_MANUAL_MOVE = false;
boolean IS_DEV1_ON = false;
boolean IS_DEV2_ON = false;
boolean IS_CUSTOM_MAP_SELECTED = false;
boolean IS_SOUND_ON = true;
int TFT_Brightness = 255;
int MAIN_SCREEN_MENU = 0;
int CURRENT_SCREEN = 0; 
int LOAD_SELECTOR;   // selector to show which LOADING mechanism is used: 1 - Messier, 2 - File, 3 - NGCs

int RA_microSteps, DEC_microSteps, rev_RA_microSteps, rev_DEC_microSteps;              // Current position of the motors in MicroSteps! - when movement occures, values are changed accordingly (manual, tracking or slew to);
int RA_mode_steps, DEC_mode_steps;
int SLEW_RA_microsteps, SLEW_DEC_microsteps;    // Where the mottors needs to go in order to point to the object
int RA_finish_last = 0;
int map_r = 0;    // Used to determine the StarMap Row ... image name (1-1.bmp; 1-2.bmp ....)
int map_c = 0;    // Ued to determine the StarMap Column .... image name

TinyGPSPlus gps;
DS3231 rtc(rtcSDA, rtcSCL);           // (SDA, SCL) from the RTC board

String old_t, old_d;
String Start_date;
unsigned long update_time, Tupdate_time, TFT_timeout;
unsigned long UPD_T, UPD_coord, DELAY_Slew, UPD_LST, TFT_Timer;
int RA_move_ending;
int w_DateTime[12]={0,0,0,0,0,0,0,0,0,0,0,0};  // array to store date - as updated from updater screen - Wishing_Date
int Summer_Time = 0;
int xPosition = 0;  // Joystick
int yPosition = 0;  // Joystick
float _temp,_Stemp;
float _humid,_Shumid;
File roots;
File StarMaps;                    // bmp files

// Some variables used for Alignment procedure:
double Star_1_HA = 0;
double Star_1_DEC = 0;
double Star_2_HA = 0;
double Star_2_DEC = 0;
double err_RA = 0;
double err_DEC = 0;
double Eerr_RA = 0;
double Eerr_DEC = 0;
double err_ALT = 0;
double err_AZ = 0;
double det = 0;


int WORM = 144;
float REDUCTOR = 4;      // 1:4 gear reduction
int DRIVE_STP = 200;   // Stepper drive have 200 steps per revolution
int MICROSteps = 16;   // I'll use 1/16 microsteps mode to drive sidereal - also determines the LOWEST speed.

// below variables are used to calculate the paramters where the drive works
int ARCSEC_F_ROTAT = 1296000;    // ArcSeconds in a Full earth rotation;
float SIDEREAL_DAY = 86164.0905;   // Sidereal day in seconds
float ArcSECstep;
int MicroSteps_360;
int RA_90;  // How much in microSteps the RA motor have to turn in order to make 6h = 90 degrees;
int DEC_90;   // How mich in microSteps the DEC motor have to turn in order to make 6h = 90 degrees;
float HA_H_CONST;
int HA_M_CONST;
float DEC_D_CONST;
int DEC_M_CONST;
int MIN_TO_MERIDIAN_FLIP = 2;   // This constant tells the system when to do the Meridian Flip. "= 2" means 2 minutes before 24:00h (e.g. 23:58h)
int MIN_SOUND_BEFORE_FLIP = 3;   // This constant tells the system to start Sound before it makes Meridian Flip
float mer_flp;                   // The calculateLST_HA() function depending on this timer will convert the HA and DEC to the propper ones to do the flip.
boolean MERIDIAN_FLIP_DO = false;
int Tracking_type = 1;  // 1: Sidereal, 2: Solar, 0: Lunar;
int Clock_Sidereal;  // Variable for the Interruptions. nterruption is initialized depending on the DATA above -in miliseconds
int Clock_Solar;  // Variable for the Interruptions. nterruption is initialized depending on the DATA above -in miliseconds
int Clock_Lunar;  // Variable for the Interruptions. nterruption is initialized depending on the DATA above -in miliseconds
int pixel_x, pixel_y;


void printDebugValuesAndWait(int timeDelay){
    Serial.println("-------------------------------------------");

    Serial.print("millis(): ");
    Serial.println(millis());


    Serial.print("RA_move_ending: ");
    Serial.println(RA_move_ending);

    Serial.print("IS_MERIDIAN_FLIP_AUTOMATIC: ");
    Serial.println(IS_MERIDIAN_FLIP_AUTOMATIC);

    Serial.print("UPD_LST: ");
    Serial.println(UPD_LST);

    Serial.print("IS_OBJ_FOUND: ");
    Serial.println(IS_OBJ_FOUND);

    Serial.print("DELAY_Slew: ");
    Serial.println(DELAY_Slew);

    Serial.print("IS_STEPPERS_ON: ");
    Serial.println(IS_STEPPERS_ON);

    Serial.print("IS_BT_MODE_ON: ");
    Serial.println(IS_BT_MODE_ON);

    Serial.print("IS_MANUAL_MOVE: ");
    Serial.println(IS_MANUAL_MOVE);

    Serial.print("JOYSTICK Movements(xPosition) : ");
    Serial.println(xPosition);

    Serial.print("JOYSTICK Movements(yPosition): ");
    Serial.println(yPosition);

    Serial.print("TFT_timeout: ");
    Serial.println(TFT_timeout);

    Serial.print("TFT_Timer: ");
    Serial.println(TFT_Timer);

    Serial.print("IS_TFT_ON: ");
    Serial.println(IS_TFT_ON);

    Serial.print("IS_IN_OPERATION: ");
    Serial.println(IS_IN_OPERATION);

    Serial.print("IS_NIGHTMODE: ");
    Serial.println(IS_NIGHTMODE);

    Serial.print("UPD_T: ");
    Serial.println(UPD_T);

    Serial.print("CURRENT_SCREEN: ");
    Serial.println(CURRENT_SCREEN);

    Serial.print("update_time: ");
    Serial.println(update_time);

    delay(timeDelay);
/*
    Serial.print(": ");
    Serial.println();

*/
}