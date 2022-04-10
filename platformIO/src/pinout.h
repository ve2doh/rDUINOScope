#ifndef pinout
#define pinout

#include <Arduino.h>

// PIN selection
extern int speakerOut;
//int dht_pin = 3;
// Day/Night mode - A6;
extern int RA_STP;
extern int RA_DIR;
extern int DEC_STP;
extern int DEC_DIR;

// New version of the HW 1.4_c was with changed pins for RA_MODE2 and RA_MODE1
// I needed to switch them in the code!
// int RA_MODE1 = 13;
// int RA_MODE2 = 12;

extern int RA_MODE0;     
extern int RA_MODE1;
extern int RA_MODE2;
extern int DEC_MODE0;     
extern int DEC_MODE1;
extern int DEC_MODE2;
// RTC (A4, A5); // (SDA, SCL) from the RTC board
// 17 (RX) - goes to TX on GPS;
// 16 (TX) - goes to RX on GPS;
// 15 (RX) - goes to Bluetooth RX;
// 14 (TX) - goes to Bluetooth TX;
extern int yPin;
extern int xPin;
extern int DEV1;
extern int DEV2;
extern int TFTBright;
extern int Joy_SW;
extern int POWER_DRV8825;
extern int nightModePin;
extern int sd_cs;               // TFTShield sd card chip select pin is Arduino digital pin 42
extern uint8_t rtcSDA, rtcSCL;

#endif