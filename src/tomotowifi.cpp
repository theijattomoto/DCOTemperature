
#include "tomotowifi.h"
//#include "TomotoOLED.cpp"
#include <WiFiUdp.h>
#include <ArduinoJson.h>

WiFiUDP udp;
WiFiUDP udp1;
//========================================================================
/*WiFi Variables*/
//Station Configuration
/*
const char *SSID = "theijatM";
const char *PASSWORD = "ij201902*";
*/
IPAddress stationLocal_IP (192,168,4,76);
IPAddress stationBroadcast_IP (255,255,255,255);
IPAddress stationGateway (192,168,4,1);
IPAddress stationSubnet (255,255,255,0);
/*
unsigned long wifiTimeOut;
unsigned long packetReceiveTimeOut;

char IPbuffer[64];
String DcoTemperature;
float Temperature1 = 0.00;
bool newData;
int packetsize;*/
//========================================================================


//#define localPort 5007
//#define _remotePort 5006
//#define udpBufferSize 255

//char udpPacketOut[udpBufferSize];
//char udpPacketIn[udpBufferSize];

//Temperature1 = 0.00;
//========================================================================
/*Wifi Mode == Station*/
void tomotowifi::stationMode()
{
  WiFi.disconnect();
  delay(10);

  Serial.println("Connecting to WIFI plaese wait!");
/*
  display.clearDisplay(); 
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(10,25);
  display.print("Connecting to WIFI");
  display.setCursor(25,33);
  display.print("Plaese wait!");
  display.println(WiFi.SSID());
  display.display();
  display.startscrollright(0x00, 0x0F);*/

  WiFi.begin(SSID,PASSWORD);
  WiFi.config(stationLocal_IP,stationGateway,stationSubnet);
  wifiTimeOut = millis() + 10000;
  while((WiFi.status() != WL_CONNECTED) && (millis() < wifiTimeOut))
  {
    delay(500);
    Serial.print(".");    
  }
  
  if(millis() < wifiTimeOut)
  {
    Serial.println();
    Serial.print("Connected to >> ");
    Serial.println(WiFi.SSID());
    Serial.print("IP Address:\t");
    Serial.println(WiFi.localIP());
/*
    display.stopscroll();
    display.clearDisplay(); 
    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(WHITE);        // Draw white text
    display.setCursor(0,25);
    display.print("Connected to:" + WiFi.SSID());
    display.setCursor(25,35);
    sprintf(IPbuffer,"IP:%d.%d.%d.%d",WiFi.localIP()[0],WiFi.localIP()[1],WiFi.localIP()[2],WiFi.localIP()[3]);
    display.println(IPbuffer);
    display.display();*/
    udp.begin(localPort);
    //udp1.begin(steamLocalPort);
    
  }
}
//========================================================================
void tomotowifi::udpBroadcastPacket(double temperature)
{
  StaticJsonDocument<200> doc;

  doc["Station"] = "DCO";
  doc["sensor"] = "Temp1";

  JsonArray TemperatureValues = doc.createNestedArray("data");
  
  sprintf(udpPacketOut, "%.2f",temperature);
  //sprintf(udpPacketOut, "DCO,TEMP1:%.2f`C",temperature);
  TemperatureValues.add(udpPacketOut);
  udp.beginPacket(stationBroadcast_IP,_remotePort);
  serializeJsonPretty(doc, udp);
  //udp.printf(udpPacketOut);
  udp.endPacket();
  Serial.println(udpPacketOut);
  //Serial.println(doc);
  newData = true;
  /*
  //display.clearDisplay(); 
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE); 
  display.setCursor(0,55);
  display.print("Brodcast to:");
  display.println(_remotePort);
  display.display();*/
  delay(500);
}
/*void tomotowifi::udpBroadcastPacket1()
{
  sprintf(udpPacketOut,"abcdefgh");
  udp.beginPacket(stationBroadcast_IP,remortPort02);
  udp.printf(udpPacketOut);
  udp.endPacket();

  //display.clearDisplay(); 
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE); 
  display.setCursor(0,55);
  display.print("Brodcast to:");
  display.println(_remotePort);
  display.display();
  delay(500);
}*/
//========================================================================
void tomotowifi::udpReceivePacket()
{
  packetsize;
  char temp[256];
  packetsize = udp.parsePacket();
  
  //packetsize = udp.available();  //or packetsize = UDP01.parsePacket();
  packetReceiveTimeOut = millis() + 10000;
  while((packetsize < 0) && millis() > packetReceiveTimeOut)
  {
    Serial.println("NO DATA RECEIVE!");
  }
  if(/*(packetsize > 0) && */(millis() < packetReceiveTimeOut))
  {
    sprintf(temp, " ");
    udp.read(temp, packetsize);
    IPAddress remoteIp = udp.remoteIP();
    snprintf(udpPacketIn, packetsize+1, temp);
    newData = true;
    /*Serial.print("UDP01.available-<");
    Serial.println(packetsize);    
    Serial.print("UDP.remoteIP-<");
    Serial.println(remoteIp);
    Serial.print("UDP01.receive-<");
    Serial.println(temp);
    Serial.print("UDP01.read-<");
    Serial.println(udpPacketIn);*/
  }
  
}
//========================================================================
void tomotowifi::udpSteamReceivePacket()
{
  steamPacketSize;
  char temp[256];
  steamPacketSize = udp1.parsePacket();
  //packetsize = udp.available();  //or packetsize = UDP01.parsePacket();
  steampacketReceiveTimeOut = millis() + 10000;
  Serial.print("PacketSize >> ");
  Serial.println(steamPacketSize);
  Serial.println("CHECK UDP DATA IN");
  if(steamPacketSize > 0 && millis() < steampacketReceiveTimeOut)
  {
    sprintf(temp, " ");
    udp1.read(temp, steamPacketSize);
    IPAddress remoteIp = udp1.remoteIP();
    snprintf(udpSteamPacketIn, steamPacketSize+1, temp);
    //newData = true;
    steamData = true;
    Serial.print("UDPSteam.available-<");
    Serial.println(steamPacketSize);    
    Serial.print("UDPSteam.remoteIP-<");
    Serial.println(remoteIp);
    Serial.print("UDPSteam.receive-<");
    Serial.println(temp);
    Serial.print("UDPSteam.read-<");
    Serial.println(udpSteamPacketIn);
  }
  if( millis() < steampacketReceiveTimeOut && steamPacketSize == 0)
  {
    Serial.println("<<<<<<<<<<<<<<<<<<<<<<<< NO DATA RECEIVE >>>>>>>>>>>>>>>");
  }
}
//========================================================================
String tomotowifi::udpPacketExtraction()
{
  packetIn = udpPacketIn;
  return(packetIn);
}
//========================================================================
double tomotowifi::udpSteamPacketExtraction()
{
  while(steamPacketSize > 0 && steamData == 1)
  {
    Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>STEAM PACKET EXTRACTION");
    steampacketIn = udpSteamPacketIn;
    steamTemperature = steampacketIn.substring(0,5);
    receiveSteamTemperature = steamTemperature.toFloat();
    steamData = false;
  }
  //
  //extFlag = true;

  return(receiveSteamTemperature);
}
//========================================================================
int tomotowifi::wifiStatusCheck()
{
  //return(WiFi.status());
  wifiConnectionStatus = WiFi.status();
  Serial.print("WIFI STATUS = ");
  Serial.println(wifiConnectionStatus);
  //display.clearDisplay(); 
  return(wifiConnectionStatus);
}
//========================================================================
String tomotowifi::wifiStatusSSID()
{
  sprintf(ssidBuffer,"SSID :%s [%d]",WiFi.SSID(),wifiConnectionStatus);
  return(ssidBuffer);
}
//========================================================================
String tomotowifi::wifitomotoip()
{
  sprintf(IpBuffer,"IPAdr:%d.%d.%d.%d",WiFi.localIP()[0],WiFi.localIP()[1],WiFi.localIP()[2],WiFi.localIP()[3]);
  return(IpBuffer);
}
//========================================================================
void tomotowifi::initOTA()
{
    ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());


}

bool tomotowifi::checkFlag()
{
  //newData = true;
  return(newData);
  //newData = false;
}

bool tomotowifi::steamReceiveFlag()
{
  return(steamData);
}
bool tomotowifi::extractionFlag()
{
  return(extFlag);
}

void tomotowifi::falseFlagforextFlag()
{
  extFlag = false;
}