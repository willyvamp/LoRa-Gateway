#include <SPI.h>
#include <LoRa.h>
#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>

#ifdef ARDUINO_SAMD_MKRWAN1300
#error "This example is not compatible with the Arduino MKR WAN 1300 board!"
#endif

/*-----------------------------------------------------
Node MCU Pins                             SX1278 Pins
D8                                        NSS
D7                                        MOSI
D6                                        MISO
D5                                        SCK
D0                                        RST
D2                                        DIO0
------------------------------------------------------*/

//Define wifi
/*
#define WIFI_SSID "VNPT1"
#define WIFI_PASSWORD "LeDucTho"
#define FIREBASE_HOST "testproject-2ed9b-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "GVqdZA3NZG0Tb7Lwl972oetFfzPrg6p1EaNl6NBX"
*/

#define WIFI_SSID "Chi Hieu"
#define WIFI_PASSWORD "hieunghia"
#define API_KEY "wX8SQvGptM7mBv2e1bftXsOtmYKJulC2HzoJVV9A"
#define DATABASE_URL "em-iot-default-rtdb.firebaseio.com"

//Define LoRa Pins
#define ss 15 //D8
#define reset 16 //D0
#define dio0 4 //D2


FirebaseData fbdt;
int dataLoraOK;
String dataSendFB, string_1, string_2, string_3, string_4, string_5, string_6, string_7, string_8;
byte comma;


void setup() {
  Serial.begin(115200);
  Serial.println();
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
      Serial.print(".");
      delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(DATABASE_URL, API_KEY);
  Firebase.reconnectWiFi(true);

  LoRa.setPins(ss, reset, dio0);

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  // Uncomment the next line to disable the default AGC and set LNA gain, values between 1 - 6 are supported
  // LoRa.setGain(6);
  
  // register the receive callback
  LoRa.onReceive(onReceive);

  // put the radio into receive mode
  LoRa.receive();
}

void loop()
{

    if(dataLoraOK == 1)
    {
      string_1 = splitString(dataSendFB, ",", 0);
      Firebase.setString(fbdt, "/Temperature", string_1);
      string_2 = splitString(dataSendFB, ",", 1);
      Firebase.setString(fbdt, "/Humidity", string_2);
      string_3 = splitString(dataSendFB, ",", 2);
      Firebase.setString(fbdt, "/Voltage", string_3);
      string_4 = splitString(dataSendFB, ",", 3);
      Firebase.setString(fbdt, "/Current", string_4);
      string_5 = splitString(dataSendFB, ",", 4);
      Firebase.setString(fbdt, "/Power", string_5);
      string_6 = splitString(dataSendFB, ",", 5);
      Firebase.setString(fbdt, "/Energy", string_6);
      string_7 = splitString(dataSendFB, ",", 6);
      Firebase.setString(fbdt, "/Frequency", string_7);
      string_8 = splitString(dataSendFB, ",", 7);
      Firebase.setString(fbdt, "/Power Factor", string_8);
        // Serial.println(splitString(dataSendFB, ",", 5));
      dataLoraOK = 0;
    }


}

void onReceive(int packetSize) 
{
  String message = "";

  while (LoRa.available()) {
    message += (char)LoRa.read();
  }
  
  dataSendFB = message;
  // string_1 = splitString(dataSendFB, ",", 0);
  // Firebase.setString(fbdt, "/Voltages", string_1);
  Serial.print("Gateway Receive: ");
  Serial.println(message);
  dataLoraOK = 1;

}

// void LoRa_rxMode(){
//   LoRa.disableInvertIQ();               // normal mode
//   LoRa.receive();                       // set receive mode
// }

// void LoRa_txMode(){
//   LoRa.idle();                          // set standby mode
//   LoRa.enableInvertIQ();                // active invert I and Q signals
// }

// void LoRa_sendMessage(String message) {
//   LoRa_txMode();                        // set tx mode
//   LoRa.beginPacket();                   // start packet
//   LoRa.print(message);                  // add payload
//   LoRa.endPacket(true);                 // finish packet and send it
// }


boolean runEvery(unsigned long interval)
{
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    return true;
  }
  return false;
}


String splitString(String str, String strRe, int posStr)
{
  String temp = str;
  for(int i = 0; i < posStr; i++)
  {
    temp = temp.substring(temp.indexOf(strRe) + 1);
    // if(temp.indexOf(strRe)== -1 && i != posStr -1 )
    // {
    //   return "";
    // }

  }
  return temp.substring(0, temp.indexOf(strRe));
}
