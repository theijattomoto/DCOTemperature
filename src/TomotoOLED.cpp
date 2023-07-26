#include "TomotoOLED.h"

/*#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
*/
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



void tomotooled::screenbegin()
{
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // Don't proceed, loop forever
    }

    display.clearDisplay(); 
    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(WHITE);        // Draw white text
    display.setCursor(5,25);
    display.println("System Initialize!");
    display.display();
}

void tomotooled::drawMainPage(int X, int Y,String title,String _ssid, String _ip, double temperature)
{
    display.clearDisplay(); 
    display.stopscroll();
    display.drawRect(19,10,109, 23,WHITE);

    display.setTextSize(1);             
    display.setTextColor(WHITE);        
    display.setCursor(X,Y);
    display.println(title);

    display.setCursor(0,10);
    display.println("PV:");

    display.setCursor(0,45);
    display.print(_ssid);
    display.setCursor(0,55);
    display.println(_ip);

    display.setTextSize(2);             
    display.setTextColor(WHITE);        
    display.setCursor(25,14);
    display.println(temperature);
    display.setCursor(100,14);
    display.println("`C");
    display.display();
}

void tomotooled::oledDrawRect(int X,int Y,int W, int H)
{
    //display.clearDisplay();
    display.drawRect(X,Y,W,H,WHITE);
    //display.display();
}

void tomotooled::oledDrawMessage(int X,int Y, String Message)
{
    //display.clearDisplay();
    display.setTextSize(1);             
    display.setTextColor(WHITE);  
    display.setCursor(X,Y);
    display.println(Message);
    display.display();
}

void tomotooled::printSSID_IP(String _ssid, String _ip)
{
    display.clearDisplay();
    display.setCursor(0,45);
    display.print(_ssid);
    display.setCursor(0,55);
    display.println(_ip);
    //display.setCursor(100,45);
    //display.println(_status);
    display.display();

}
 