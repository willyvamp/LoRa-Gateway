#include <SPI.h>
#include <LoRa.h>
#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>


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

#define WIFI_SSID "name"
#define WIFI_PASSWORD "pass"

#define API_KEY "API_KEY"
#define DATABASE_URL "URL"

//Define LoRa Pins
#define ss 15 //D8
#define reset 16 //D0
#define dio0 4 //D2


FirebaseData fbdt;
int dataLoraOK;
String dataSendFB, string_0, string_1, string_2, string_3, string_4, string_5, string_6, string_7, string_8;
String string_9, string_10, string_11, string_12, string_13, string_14, string_15, string_16;
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
  
  // register the receive callback
  LoRa.onReceive(onReceive);

  // put the radio into receive mode
  LoRa.receive();
}

void loop()
{
    if(dataLoraOK == 1)
    {
      string_0 = splitString(dataSendFB, ",", 0);

      if(string_0 == "1"){
        string_1 = splitString(dataSendFB, ",",1);
        Firebase.setString(fbdt, "Node1/Temperature", string_1);
        string_2 = splitString(dataSendFB, ",", 2);
        Firebase.setString(fbdt, "Node1/Humidity", string_2);
        string_3 = splitString(dataSendFB, ",", 3);
        Firebase.setString(fbdt, "Node1/Voltage", string_3);
        string_4 = splitString(dataSendFB, ",", 4);
        Firebase.setString(fbdt, "Node1/Current", string_4);
        string_5 = splitString(dataSendFB, ",", 5);
        Firebase.setString(fbdt, "Node1/Power", string_5);
        string_6 = splitString(dataSendFB, ",", 6);
        Firebase.setString(fbdt, "Node1/Energy", string_6);
        string_7 = splitString(dataSendFB, ",", 7);
        Firebase.setString(fbdt, "Node1/Frequency", string_7);
        string_8 = splitString(dataSendFB, ",", 8);
        Firebase.setString(fbdt, "Node1/Power Factor", string_8);
          // Serial.println(splitString(dataSendFB, ",", 5));
        dataLoraOK = 0;
      }

      if(string_0 == "2"){
        string_9 = splitString(dataSendFB, ",",1);
        Firebase.setString(fbdt, "Node2/Temperature", string_9);
        string_10 = splitString(dataSendFB, ",", 2);
        Firebase.setString(fbdt, "Node2/Humidity", string_10);
        string_11 = splitString(dataSendFB, ",", 3);
        Firebase.setString(fbdt, "Node2/Voltage", string_11);
        string_12 = splitString(dataSendFB, ",", 4);
        Firebase.setString(fbdt, "Node2/Current", string_12);
        string_13 = splitString(dataSendFB, ",", 5);
        Firebase.setString(fbdt, "Node2/Power", string_13);
        string_14 = splitString(dataSendFB, ",", 6);
        Firebase.setString(fbdt, "Node2/Energy", string_14);
        string_15 = splitString(dataSendFB, ",", 7);
        Firebase.setString(fbdt, "Node2/Frequency", string_15);
        string_16 = splitString(dataSendFB, ",", 8);
        Firebase.setString(fbdt, "Node2/Power Factor", string_16);
        dataLoraOK = 0;
      }
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
