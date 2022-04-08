

#ifndef graphic_screen
#define graphic_screen

#include <Arduino.h>
#include <SD.h>

void DrawButton(int X, int Y, int Width, int Height, String Caption, int16_t BodyColor, int16_t BorderColor, int16_t TextColor, int tSize);
void drawPic(File *StarMaps, uint16_t x, uint16_t y, uint16_t WW, uint16_t HH);
void removeTime_addXX();
void drawGPSScreen();
void drawClockScreen();
void drawSelectAlignment();
void drawMainScreen();
void drawCoordinatesScreen();
void drawLoadScreen();
void drawOptionsScreen();
void drawSTATScreen();
void drawStarMap();
void drawStarSyncScreen();
void drawConstelationScreen(int indx);
void OnScreenMsg(int Msg);
void considerDayNightMode();
void drawMainScreen_Menu(int fill);
void TimerUpdateDraw(int z);
uint16_t readID(void);

#endif