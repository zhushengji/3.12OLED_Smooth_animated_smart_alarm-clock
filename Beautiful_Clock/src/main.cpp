#include <Arduino.h>
#include <math.h>
#include <EEPROM.h>
#include "nvs.h"
#include "nvs_flash.h"
#include <U8g2lib.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <WebSocketsServer.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include "ESPmDNS.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <SPI.h>
#include <AHT20.h>
#include <RTC_RX8025T.h>
#include <TimeLib.h>
#include "gb2312.c"
#include "driver/rtc_io.h"
#include "esp_sleep.h"
// LIS3DH I2C 地址
#define LIS3DH_ADDR 0x18
// #include <FastLED.h>
RTC_DATA_ATTR float arr[16];
RTC_DATA_ATTR float voltage;
#include "BMP280Code.h"
#include "bmp.h"
// Adafruit_BMP280 bmp;
// #define LED_PIN 33
// #define NUM_LEDS 4
// #define BRIGHTNESS 180
// #define LED_TYPE WS2812
// #define COLOR_ORDER GRB
// CRGB leds[NUM_LEDS];
const char *ntpServer = "cn.pool.ntp.org";
const long gmtOffset_sec = 8 * 60 * 60;
const int daylightOffset_sec = 3600;
String daysOfTheWeek[] = {"-", "星期天", "星期一", "星期二", "星期三", "星期四", "星期五", "星期六"};
String monthsNames[] = {"-", "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12"};
struct Alarm_obj
{
  short a_hour;
  short a_min;
  short a_frequency;
} Alarm;
short sleepTime = 120; // 单位：秒
String oweathers = "";
void writeRegister(uint8_t reg, uint8_t value);
void configureLIS3DHInterrupt();
void remindAnimate();
void pomodoroSet();
void stopwatchStart();
void pomodoroAnimate();
void stopwatchAnimate();
bool getNetTime();
void ManualsetRTC();
void cliptimeRun();
void disMenu();
void setsleeptime();
void Alarm_ring();
extern const uint8_t dinkie_9[52247] U8G2_FONT_SECTION("dinkie_9");
#define key4 23
#define key3 22
#define key2 21
#define key1 0
#define keyleft 14
#define keyright 34
#define BUZZ 32
// #define WSLED 33
#define BAT 12
AHT20 aht20;
tmElements_t RXtm;
// 闹钟是否打开标记位
bool alarmState = false;
// 离线天气开关
// bool offlineweatherState = false;
//唤醒方式
bool wakeFlagState = false;
struct tm timeinfo;
AsyncWebServer server(80);
U8G2_SSD1362_256X64_F_4W_HW_SPI u8g2(U8G2_R2, /* cs=*/4, /* dc=*/16, /* reset=*/17);
short temperature, humidity;
// 公开或私有标记
bool isSale = true;
String XZcode = "SxzwEXUQLKZoS1-nv";
#include "pic.h"
#include "MenuCode.h"
#include "Webservice.h"
#include "NTP_RTC.h"
#include "getData.h"
#include "wakeCode.h"
#include "alarmCode.h"
#include "displayUI.h"
#include "PomodoroClock.h"
void setup()
{
  /**
   * eeprom位置说明
   * 1：存储亮度
   * 5: 存储休眠时长
   * 2：存储闹钟小时数据
   * 3：存储闹钟分钟数据
   * 4：存储闹钟频次数据
   * 5：存储离线时钟/天气时钟种类
   * 6:存储离线天气开关
   * 7.唤醒方式
   */
  Serial.begin(115200);

  EEPROM.begin(64);
  Wire.begin(25, 26);
  Alarm.a_hour = EEPROM.read(2);
  Alarm.a_min = EEPROM.read(3);
  Alarm.a_frequency = (short)EEPROM.read(4);
  Serial.printf("闹钟小时：%d,闹钟分钟：%d,闹钟频率：%d\n", Alarm.a_hour, Alarm.a_min, Alarm.a_frequency);

  if (EEPROM.read(5) != 255)
  {
    sleepTime = EEPROM.read(5) * 60;
  }
  if (EEPROM.read(7) == 1)
  {
    wakeFlagState = true;
  }
  // if (EEPROM.read(6) == 1)
  // {
  //   offlineweatherState = true;
  //   pinMode(GPIO_NUM_35, LOW);
  //   esp_sleep_enable_ext0_wakeup(GPIO_NUM_35, 1);
  //   attachInterrupt(digitalPinToInterrupt(GPIO_NUM_35), gpio_isr_handler, FALLING);
  // }
  Serial.printf("休眠时长：%d\n", sleepTime);
  bmp.begin();
  init280();
  aht20.begin();
  RTC_RX8025T.init();
  u8g2.begin();
  u8g2.setContrast(EEPROM.read(1));
  Serial.printf("屏幕亮度：%d\n", EEPROM.read(1));
  u8g2.enableUTF8Print(); // 开启UTF8字符功能

  pinMode(key4, INPUT_PULLUP);
  pinMode(key3, INPUT_PULLUP);
  pinMode(key2, INPUT_PULLUP);
  pinMode(key1, INPUT_PULLUP);
  pinMode(keyleft, INPUT_PULLUP);
  pinMode(keyright, INPUT_PULLUP);
  // remindAnimate();
  // return;

  pinMode(BUZZ, OUTPUT);
  pinMode(BAT, INPUT);

  // 判断什么原因启动的
  print_wakeup_reason();
  // 公开或私有标记
  if (isSale)
  {
    int num = random(0, 10);
    u8g2.clearBuffer();
    u8g2.drawXBMP(0, 0, 256, 64, shengming);
    u8g2.sendBuffer();
    if (num % 3 == 0)
    {
      while (digitalRead(key1) != LOW && digitalRead(key2) != LOW && digitalRead(key3) != LOW && digitalRead(key4) != LOW)
      {
        delay(100);
      }
    }
  }
  // configureMPU(1);
   configureLIS3DHInterrupt();
  disMenu();
}

void loop()
{
}
// 设置中断处理函数
// void IRAM_ATTR gpio_isr_handler()
// {
//   u8g2.setDrawColor(1);
//   remindAnimate();
//   cliptimeRun();
// }