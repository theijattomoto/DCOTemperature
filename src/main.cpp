#include <Arduino.h>
#include "TomotoOLED.h"
#include "tomotowifi.h"
#include "TomotoPt100.h"
#include <SoftwareSerial.h>
#include <ModbusMaster.h>

//=============================================
//MAX485////////////////////////////////////////
int MAX485_DE = 33;  //VCC=5V, GND=0V, DE=2(D4), RE=0V, R0=RX, DI=TX, A=RS485+, B=RS485-
int MAX485_RE = 32;
int MAX485_RX = 26;
int MAX485_TX = 27;


//========================================================================
SoftwareSerial modSerial(MAX485_RX,MAX485_TX);
ModbusMaster node;
tomotowifi io;
tomotooled ol;
//========================================================================
TomotoPT100 ADC1 =35;
TomotoPT100 ADC2 =34;
//========================================================================
/*FreeRTOS*/
TaskHandle_t Task1;
TaskHandle_t Task2;
TaskHandle_t Task3;
#define WIFI_RECOVER_TIME_MS 10000 // Wait 30 seconds after a failed connection attempt

//========================================================================
//mH2O value = bar value x 10.1972
float mH2O = 0.00;
float bar = 0.00;
float DCOTEMPERATURE = 0.00;
char Str_Temp1[10];

//========================================================================
void Get_Reading(void *pvParameters)
{
  (void)pvParameters;

  for(;;)
  {
    

    DCOTEMPERATURE = ADC1.getTemperatureReadingAverage(100);
    dtostrf(DCOTEMPERATURE, 4, 2, Str_Temp1);
    ol.drawMainPage(20,0,"DCO TEMPERATURE",io.wifiStatusSSID(), io.wifitomotoip(), DCOTEMPERATURE);
    
    /*io.udpBroadcastPacket(DCOTEMPERATURE);
    if(io.checkFlag()==1)
    {
      ol.oledDrawMessage(0,35, "Bradcast to > 5007");
    }
    io.wifiStatusCheck();*/
    delay(1000);
  }
}

void Broadcast_Data(void *pvParameters)
{
  (void)pvParameters;

  for(;;)
  {
    io.udpBroadcastPacket(DCOTEMPERATURE);
    if(io.checkFlag()==1)
    {
      ol.oledDrawMessage(0,35, "Bradcast to > 5007");
    }
    delay(10000);
  }
}

void checkWifiStatus(void *pvParameters)
{
  (void)pvParameters;
  for(;;)
  {

    if(WiFi.status() != WL_DISCONNECTED)
     {
      vTaskDelay(WIFI_RECOVER_TIME_MS / portTICK_PERIOD_MS);
      Serial.println(WiFi.status());
      continue;
        
      }
      io.stationMode();
    
    
/*
    if(WiFi.status() != WL_CONNECTED){
            Serial.println("[WIFI] FAILED");
            vTaskDelay(WIFI_RECOVER_TIME_MS / portTICK_PERIOD_MS);
			  continue;
        }*/
      
      //delay(1000);
  }
  
}
//========================================================================
void preTransmission()  //set up call back function
{
  digitalWrite(MAX485_RE, 1);
  digitalWrite(MAX485_DE, 1);
}
void postTransmission()   //set up call back function
{
  digitalWrite(MAX485_RE, 0);
  digitalWrite(MAX485_DE, 0);
}
//========================================================================
void setup() 
{
  pinMode(MAX485_RE, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);
  // Init in receive mode
  digitalWrite(MAX485_RE, 0);
  digitalWrite(MAX485_DE, 0);

  Serial.begin(115200);
  modSerial.begin(9600);
  ol.screenbegin();
  delay(2000);
  Serial.println("Initialize, Plaese Wait!");
  delay(2000);
  WiFi.mode(WIFI_STA);
  io.stationMode();

  // Modbus slave ID 1, numbers are in decimal format
  node.begin(1, modSerial);  //data from max 485 are communicating with serial2
  // Callbacks allow us to configure the RS485 transceiver correctly
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
  
  ArduinoOTA.begin();
  
  xTaskCreatePinnedToCore(Get_Reading,"Task1",10000,NULL,3,&Task1,0);  
  xTaskCreatePinnedToCore(Broadcast_Data,"Task2",10000,NULL,2,&Task2,ARDUINO_RUNNING_CORE);  
  xTaskCreatePinnedToCore(checkWifiStatus,"Task3",5000,NULL,1,&Task3,ARDUINO_RUNNING_CORE);  
}
//========================================================================
void loop() 
{
  ArduinoOTA.handle();

    uint8_t result;  
  // Read 16 registers starting at 0x00, read 11 register. Meaning that read 0x00, then read 0x01, so on and so forth. Until the eleventh resister 0x0A
  result = node.readHoldingRegisters(0x0000, 7);
  if (result == node.ku8MBSuccess)
  {
    Serial.println("------------");
    Serial.print("Address: ");
    Serial.println(node.getResponseBuffer(0x00));
    Serial.print("Baut Rate: ");
    Serial.println(node.getResponseBuffer(0x01));
    Serial.print("Pressure Unit: ");
    Serial.println(node.getResponseBuffer(0x02));
    Serial.print("Decimal Places: ");
    Serial.println(node.getResponseBuffer(0x03));
    Serial.print("Output Value: ");
    Serial.println(node.getResponseBuffer(0x04));
    Serial.print("ZERO Range: ");
    Serial.println(node.getResponseBuffer(0x05));
    Serial.print("FULL Range: ");
    Serial.println(node.getResponseBuffer(0x06));
    Serial.println("------------");
    bar = node.getResponseBuffer(0x04)/100.f;
    //bar = barStr.toFloat();
    mH2O = bar * 10.1972;
    Serial.print("Tank Level: ");
    Serial.print(mH2O);
    Serial.println(" mH2O");
    Serial.println("------------");
  }
  delay(5000);
}
//=========================================================================

