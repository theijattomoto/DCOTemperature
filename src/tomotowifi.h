//tomotowifi.h.h
#ifndef tomotowifi_h
#define tomotowifi_h

#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoOTA.h>

#define localPort 9003
#define _remotePort 5007
//#define remortPort02 5009
//#define steamLocalPort 5007


class tomotowifi {
  private:
    const char *SSID = "oilroom_1";
    const char *PASSWORD = "oilroom_1";

    /*const char *SSID = "theijatM";
    const char *PASSWORD = "ij201902*";*/

    /*const char *SSID = "TMSB_2.4GHz_5GHz@unifi";
    const char *PASSWORD = "TMSB28981Htmsb";*/

    /*const char *SSID = "oilroom_1";
    const char *PASSWORD = "oilroom_1";*/

    unsigned long wifiTimeOut;
    unsigned long packetReceiveTimeOut;
    unsigned long steampacketReceiveTimeOut;

    #define udpBufferSize 255
    char udpPacketOut[udpBufferSize];
    char udpPacketIn[udpBufferSize];
    char udpSteamPacketIn[udpBufferSize];
    

    char IPbuffer[64];
    String DcoTemperature;
    float Temperature1;
    bool newData;
    bool steamData;
    bool extFlag = false;
    int packetsize;
    int steamPacketSize;

    String packetIn;
    String steampacketIn,steamTemperature;
    int wifiConnectionStatus;
    char IpBuffer[64];
    char ssidBuffer[64];

    double receiveSteamTemperature;
    

  public:
    void stationMode();
    void udpBroadcastPacket(double temperature);
    void udpReceivePacket();
    String udpPacketExtraction();
    int wifiStatusCheck();
    String wifiStatusSSID();
    String wifitomotoip();
    void udpSteamReceivePacket();
    double udpSteamPacketExtraction();
    void udpBroadcastPacket1();
    //void OLED_INIT();
    void initOTA();
    bool checkFlag();
    bool steamReceiveFlag();
    bool extractionFlag();
    void falseFlagforextFlag();
    
};

#endif