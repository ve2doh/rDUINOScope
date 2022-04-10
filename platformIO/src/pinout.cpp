#include "pinout.h"

// PIN selection
int speakerOut = 47;
//int dht_pin = 3;
// Day/Night mode - A6;
int RA_STP = 37;
int RA_DIR = 38;
int DEC_STP = 39;
int DEC_DIR = 40;

// New version of the HW 1.4_c was with changed pins for RA_MODE2 and RA_MODE1
// I needed to switch them in the code!
// int RA_MODE1 = 13;
// int RA_MODE2 = 12;

int RA_MODE0 = 41;     
int RA_MODE1 = 42;
int RA_MODE2 = 43;
int DEC_MODE0 = 44;     
int DEC_MODE1 = 45;
int DEC_MODE2 = 46;
// RTC (A4, A5); // (SDA, SCL) from the RTC board
// 17 (RX) - goes to TX on GPS;
// 16 (TX) - goes to RX on GPS;
// 15 (RX) - goes to Bluetooth RX;
// 14 (TX) - goes to Bluetooth TX;
int yPin = A7;
int xPin = A9;
int DEV1 = A10;
int DEV2 = 52;
int nightModePin =A6;
int TFTBright = DAC0;
int Joy_SW = A11;
int POWER_DRV8825 = A8;
int sd_cs = 53;               // TFTShield sd card chip select pin is Arduino digital pin 42

uint8_t rtcSDA=48;
uint8_t rtcSCL=49;