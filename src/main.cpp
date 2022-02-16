#include <Arduino.h>
#include <WiFi.h>
#include <esp_wpa2.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>



//for personal wifi networks, use the following block
#define WIFI_NETWORK "your wifi network name" //define name of Wifi Network
#define WIFI_PASSWORD "your wifi network password" //Wifi Password
#define WIFI_TIMEOUT_MS "20000" //20 seconds

//For University of Toronto Wifi (eduroam), use the following block
#define EAP_ANONYMOUS_IDENTITY "anonymous@utoronto.ca"
#define EAP_IDENTITY "barrytyl@utoronto.ca"
#define EAP_PASSWORD "BlueBarry1!"
//SSID name
const char* ssid = "eduroam";
uint8_t counter = 0;


//For BLE setup
#define SERVICE_UUID "c947c9a0-4c67-4122-942e-4f58db73695e"
#define CHARACTERISTIC_UUID "0cf1a111-9e55-4fe5-9f08-0c326f089553"



//function to connect to  Persona Wifi network NOT UOFT WIFI
void initPersonalWifi() {

  Serial.print("Connecting to Wifi ..");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_NETWORK, WIFI_PASSWORD);

  unsigned long startAttemptTime = millis(); //start timer to limit time spent trying to connect to Wifi

// check if WiFi is connected, stop if more that 20 seconds passes
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 20000) {
    Serial.println(".. still trying ..");
    delay(1000);
  }
  if(WiFi.status() != WL_CONNECTED){
    Serial.println("Wifi connection failed");
  }else{
    Serial.println("Wifi Connected");
  };

  delay(1000);
  Serial.println(WiFi.localIP());
}

//function to connect to UofT Wifi
void initUofTWifi() {
  
  WiFi.disconnect(true); //disconnect from current wifi
  WiFi.mode(WIFI_STA);
  Serial.print("MAC address:  ");
  Serial.println(WiFi.macAddress());
  Serial.printf("Connecting to WiFi: %s ", ssid);
  esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY));
  esp_wifi_sta_wpa2_ent_set_username((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY));
  esp_wifi_sta_wpa2_ent_set_password((uint8_t *)EAP_PASSWORD, strlen(EAP_PASSWORD));
  esp_wpa2_config_t config = WPA2_CONFIG_INIT_DEFAULT();
  esp_wifi_sta_wpa2_ent_enable(&config);
  WiFi.begin(ssid);

  //check if wifi is connected. If it is not connected, restart ESP32
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(F("."));
    counter++;
    if (counter >= 60){
      ESP.restart();
    }
  }

  Serial.println(F(" connected"));
  Serial.print(F("IP address is: "));
  Serial.println(WiFi.localIP());
}

void initBLE() {
  //this function starts the BLE service on the ESP32

  BLEDevice::init("Powermeter");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ |
    BLECharacteristic::PROPERTY_WRITE
  );
  pCharacteristic->setValue("Zero");
  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic is defined, it is now ready to be received");
}

void setup() {
  // put your setup code here, to run once:
   Serial.begin(115200);

   //Initialize Bluetooth
   initBLE();

 

  //connect to Personal Wifi Network NOT UOFT
  //initPersonalWifi();

  //connect to UofT Wifi
  //initUofTWifi();


}

void loop() {
  // put your main code here, to run repeatedly:
  
  

  delay(2000);

}