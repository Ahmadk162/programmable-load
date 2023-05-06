#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <Wire.h>
#include <RBDdimmer.h>//https://github.com/RobotDynOfficial/RBDDimmer

// PWM config
int pwm_cmd1, pwm_cmd2 = 0;
int pwm_percentage_1, pwm_percentage_2 = 0;
char pwm1[8]={'0','0','0','0','0','0','0','0'};
char pwm2[8]={'0','0','0','0','0','0','0','0'};
// Update these with values suitable for your network.
const char* ssid = "your wifi ssid";
const char* password = "your wifi password";
const char* mqtt_server = "your mqtt server link";
#define mqtt_port your_MQTT_port
#define MQTT_USER "your_mqtt_user_name"
#define MQTT_PASSWORD "your_mqqtt_password"
//list of topic
#define MQTT_SERIAL_PUBLISH_CH "/icircuit/ESP32/serialdata/tx"
#define MQTT_SERIAL_PUBLISH_CH_pwm1 "/icircuit/ESP32/serialdata/tx/pwm1"
#define MQTT_SERIAL_PUBLISH_CH_pwm2 "/icircuit/ESP32/serialdata/tx/pwm2"
#define MQTT_SERIAL_PUBLISH_CH_vac "/icircuit/ESP32/serialdata/tx/vac"
#define MQTT_SERIAL_PUBLISH_CH_vdc "/icircuit/ESP32/serialdata/tx/vdc"
#define MQTT_SERIAL_PUBLISH_CH_adc "/icircuit/ESP32/serialdata/tx/adc"
#define MQTT_SERIAL_PUBLISH_CH_aac "/icircuit/ESP32/serialdata/tx/aac"
#define MQTT_SERIAL_RECEIVER_CH "/icircuit/ESP32/serialdata/rx"
#define MQTT_SERIAL_RECEIVER_CH_pwm1 "/icircuit/ESP32/serialdata/rx/pwm1"
#define MQTT_SERIAL_RECEIVER_CH_pwm2 "/icircuit/ESP32/serialdata/rx/pwm2"
#define Vac 32
#define Vdc 33
#define sensorIn 35
//#define PWM_ch1 5
#define PWM_ch2 18
int zeroCrossPin   = 19;
int PWM_ch1  = 5;
//Variables
int power   = 0;
dimmerLamp acd(PWM_ch1,zeroCrossPin);
//curent_sensor
int readValue;                // value read from the sensor
int maxValue = 0;             // store max value here
int minValue = 1024;          // store min value here
int mVperAmp = 185;           // this the 5A version of the ACS712 -use 100 for 20A Module and 66 for 30A Module
double Voltage = 0;
double VRMS = 0;
double AmpsRMS = 0;
uint32_t start_time=0;
int i_get_max=0;
uint16_t r=0;
uint16_t max_v = 0;
//YOUR BROKER CERTIFICATE
const char* test_root_ca= \
          "-----BEGIN CERTIFICATE-----\n"\
      "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\n" \
      "-----END CERTIFICATE-----\n";

WiFiClientSecure wifiClient;
//WiFiClient wifiClient;
PubSubClient client(wifiClient);

//setup
void setup() {
  Serial.begin(115200);
  Serial.setTimeout(500);// Set time out for 
  setup_wifi();
  wifiClient.setCACert(test_root_ca);  //test_root_ca 
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  reconnect();
  client.publish(MQTT_SERIAL_PUBLISH_CH_pwm1, pwm1);
  client.publish(MQTT_SERIAL_PUBLISH_CH_pwm2, pwm2);
  
  acd.begin(NORMAL_MODE, ON);
}

//loop
void loop() {
   client.loop();
   DC_voltage();
   get_max();
   dc_curent();
   ac_curent();
   if (Serial.available() > 0) {
     char mun[501];
     memset(mun,0, 501);
     Serial.readBytesUntil( '\n',mun,500);
     publishSerialData(mun);
   }


 }
