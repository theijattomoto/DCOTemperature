#ifndef TomotoOLED_h
#define TomotoOLED_h

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
//Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

class tomotooled{
    private:

        

    public:
        void screenbegin();
        void drawMainPage(int X, int Y,String title,String _ssid, String _ip, double temperature);
        void oledDrawRect(int X,int Y,int W, int H);
        void mainPage();
        void oledDrawMessage(int X,int Y, String Message);
        void printSSID_IP(String _ssid, String _ip);
};
#endif

