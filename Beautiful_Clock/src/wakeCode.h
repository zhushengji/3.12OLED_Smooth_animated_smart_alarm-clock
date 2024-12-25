#include <driver/adc.h>
#define SIGNAL_PATH_RESET 0x68
#define I2C_SLV0_ADDR 0x37
#define ACCEL_CONFIG 0x1C
#define MOT_THR 0x1F // Motion detection threshold bits [7:0]
#define MOT_DUR 0x20 // This seems wrong // Duration counter threshold for motion interrupt generation, 1 kHz rate, LSB = 1 ms
#define MOT_DETECT_CTRL 0x69
#define INT_ENABLE 0x38
#define PWR_MGMT 0x6B // SLEEPY TIME
#define INT_STATUS 0x3A
#define MPU6050_ADDRESS 0x68 // AD0 is 0

void writeByte(uint8_t address, uint8_t subAddress, uint8_t data)
{
  //   Wire.begin(25,26);
  Wire.beginTransmission(address); // Initialize the Tx buffer
  Wire.write(subAddress);          // Put slave register address in Tx buffer
  Wire.write(data);                // Put data in Tx buffer
  Wire.endTransmission();          // Send the Tx buffer
}

// example showing using readbytev   ----    readByte(MPU6050_ADDRESS, GYRO_CONFIG);
uint8_t readByte(uint8_t address, uint8_t subAddress)
{
  uint8_t data;                          // `data` will store the register data
  Wire.beginTransmission(address);       // Initialize the Tx buffer
  Wire.write(subAddress);                // Put slave register address in Tx buffer
  Wire.endTransmission(false);           // Send the Tx buffer, but send a restart to keep connection alive
  Wire.requestFrom(address, (uint8_t)1); // Read one byte from slave register address
  data = Wire.read();                    // Fill Rx buffer with result
  return data;                           // Return data read from slave register
}

// sens argument configures wake up sensitivity
void configureMPU(int sens)
{
  Serial.println("设定mpu唤醒");
  writeByte(MPU6050_ADDRESS, 0x6B, 0x00);
  writeByte(MPU6050_ADDRESS, SIGNAL_PATH_RESET, 0x07); // Reset all internal signal paths in the MPU-6050 by writing 0x07 to register 0x68;
  // writeByte( MPU6050_ADDRESS, I2C_SLV0_ADDR, 0x20);//write register 0x37 to select how to use the interrupt pin. For an active high, push-pull signal that stays until register (decimal) 58 is read, write 0x20.
  writeByte(MPU6050_ADDRESS, ACCEL_CONFIG, 0x01);    // Write register 28 (==0x1C) to set the Digital High Pass Filter, bits 3:0. For example set it to 0x01 for 5Hz. (These 3 bits are grey in the data sheet, but they are used! Leaving them 0 means the filter always outputs 0.)
  writeByte(MPU6050_ADDRESS, MOT_THR, sens);         // Write the desired Motion threshold to register 0x1F (For example, write decimal 20).
  writeByte(MPU6050_ADDRESS, MOT_DUR, 40);           // Set motion detect duration to 1  ms; LSB is 1 ms @ 1 kHz rate
  writeByte(MPU6050_ADDRESS, MOT_DETECT_CTRL, 0x15); // to register 0x69, write the motion detection decrement and a few other settings (for example write 0x15 to set both free-fall and motion decrements to 1 and accelerometer start-up delay to 5ms total by adding 1ms. )
  writeByte(MPU6050_ADDRESS, 0x37, 140);             // now INT pin is active low
  writeByte(MPU6050_ADDRESS, INT_ENABLE, 0x40);      // write register 0x38, bit 6 (0x40), to enable motion detection interrupt.
  writeByte(MPU6050_ADDRESS, PWR_MGMT, 8);           // 101000 - Cycle & disable TEMP SENSOR
  writeByte(MPU6050_ADDRESS, 0x6C, 7);               // Disable Gyros
}
void writeRegister(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(LIS3DH_ADDR);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}
void configureLIS3DHInterrupt() {
   // 设置加速度计量程为 ±2g（提高灵敏度）
  writeRegister(0x23, 0x00);  // CTRL_REG4: 设置 ±2g 范围

  // 激活加速度计，设置数据输出速率为 10Hz
  writeRegister(0x20, 0x27);  // CTRL_REG1: ODR = 10Hz, 启用 X/Y/Z 轴

  // 使能 INT1 引脚为中断输出
  writeRegister(0x22, 0x40);  // CTRL_REG3: 使能 INT1 引脚

  // 配置 INT1 为震动检测
  writeRegister(0x30, 0x2A);  // INT1_CFG: 启用 X/Y/Z 方向的运动中断

  // 设置更低的阈值，提高灵敏度（减小阈值）
  writeRegister(0x32, 0x08);  // INT1_THS: 设置阈值为 8 LSB (~128mg)

  // 设置更短的持续时间为 0x00，即无持续时间要求
  writeRegister(0x33, 0x00);  // INT1_DURATION: 设置持续时间为 0（几乎立即触发）
}
// void configureLIS3DHForVibrationWakeup() {
//   // 设置加速度计量程为 ±2g（提高灵敏度）
//   writeRegister(0x23, 0x00);  // CTRL_REG4: 设置 ±2g 范围

//   // 激活加速度计，设置数据输出速率为 10Hz
//   writeRegister(0x20, 0x27);  // CTRL_REG1: ODR = 10Hz, 启用 X/Y/Z 轴

//   // 使能 INT1 引脚为中断输出
//   writeRegister(0x22, 0x40);  // CTRL_REG3: 使能 INT1 引脚

//   // 配置 INT1 为震动检测
//   writeRegister(0x30, 0x2A);  // INT1_CFG: 启用 X/Y/Z 方向的运动中断

//   // 设置更低的阈值，提高灵敏度（减小阈值）
//   writeRegister(0x32, 0x08);  // INT1_THS: 设置阈值为 8 LSB (~128mg)

//   // 设置更短的持续时间，提高灵敏度
//   writeRegister(0x33, 0x01);  // INT1_DURATION: 设置持续时间为 1 LSB (~1/10 秒)
// }

/**
 * @brief 判断启动原因
 */
void print_wakeup_reason()
{
  // Serial.print("定时器启动次数统计：");
  // Serial.println(bootcount);
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
  if (wakeup_reason == ESP_SLEEP_WAKEUP_TIMER)
  {
    // // 更新时间数据
    // RTC_RX8025T.read(RXtm);
    // u64_t us;
    // int ac = Alarm.a_hour * 60 + Alarm.a_min;
    // int nc = RXtm.Hour * 60 + RXtm.Minute;
    // //计算闹钟还剩多少分钟
    // us = ac - nc;
    // if (offlineweatherState)
    // {
    //   if (bmp.takeForcedMeasurement())
    //   {
    //     float f = getVariance(bmp.readPressure() / 100);
    //     if (f > 10 && arrindex > 5 || f >= 6 && arrindex <= 5)
    //     {
    //       digitalWrite(GPIO_NUM_35, LOW);
    //     }
    //     else if(us!=0)
    //     {
    //       u64_t u = 60 * 1000000;
    //       esp_sleep_enable_timer_wakeup(u);
    //       esp_deep_sleep_start();
    //     }
    //   }
    // }
    //根据频次设定判断是否需要响铃
    if (Alarm.a_frequency == 1 || Alarm.a_frequency == 3)
    {
      Alarm_ring();
    }
    else if (Alarm.a_frequency == 2)
    {
      if (RXtm.Wday != 1 && RXtm.Wday != 7)
      {
        Alarm_ring();
      }
    }

    RTC_RX8025T.read(RXtm);
    cliptimeRun();
  }
  // if (wakeup_reason == ESP_SLEEP_WAKEUP_TIMER)
  // {
  //   // 更新时间数据
  //   RTC_RX8025T.read(RXtm);
  //   // 计算闹钟时间和当前时间（以分钟为单位）
  //   int64_t alarm_time_minutes = Alarm.a_hour * 60 + Alarm.a_min;
  //   int64_t current_time_minutes = RXtm.Hour * 60 + RXtm.Minute;
  //   int64_t time_remaining = alarm_time_minutes - current_time_minutes;

  //   // 处理跨日情况
  //   if (time_remaining < 0)
  //   {
  //     time_remaining += 24 * 60; // 加上24小时
  //   }

  //   // 检查离线天气状态并检测异常
  //   if (offlineweatherState)
  //   {
  //     Serial.println("唤醒计算环境变化");
  //     if (bmp.takeForcedMeasurement())
  //     {
  //       float variance = getVariance(bmp.readPressure() / 100);
  //       if ((variance > 10 && arrindex > 5) || (variance >= 6 && arrindex <= 5))
  //       {
  //         Serial.println("提醒界面唤醒");
  //         digitalWrite(GPIO_NUM_35, LOW); // 唤醒设备处理异常
  //       }
  //       else
  //       {
  //         Serial.println("数据正常，休眠一分钟后重新检测");
  //         // 数据正常，休眠一分钟后重新检测
  //         uint64_t sleep_time = 60 * 1000000; // 1分钟（单位：微秒）
  //         esp_sleep_enable_timer_wakeup(sleep_time);
  //         esp_deep_sleep_start();
  //       }
  //     }
  //   }
  //   // 判断是否到达闹钟时间
  //   if (time_remaining == 0)
  //   {
  //     // 根据频次设定判断是否需要响铃
  //     if (Alarm.a_frequency == 1 || Alarm.a_frequency == 3)
  //     {
  //       // 每天或自定义频率
  //       Alarm_ring();
  //     }
  //     else if (Alarm.a_frequency == 2)
  //     {
  //       // 工作日（周一到周五）响铃
  //       if (RXtm.Wday != 1 && RXtm.Wday != 7)
  //       {
  //         Alarm_ring();
  //       }
  //     }
  //   }
  // }
  switch (wakeup_reason)
  {
  case ESP_SLEEP_WAKEUP_EXT0:
    Serial.println("使用 RTC_IO 的外部信号引起的唤醒");
    cliptimeRun();
    break;
  case ESP_SLEEP_WAKEUP_EXT1:
    Serial.println("使用 RTC_CNTL 的外部信号引起的唤醒");
    break;
  case ESP_SLEEP_WAKEUP_TOUCHPAD:
    Serial.println("触摸板引起的唤醒");
    break;
  case ESP_SLEEP_WAKEUP_ULP:
    Serial.println("ULP 程序引起的唤醒");
    break;
  default:
    Serial.printf("唤醒不是由深度睡眠引起的: %d\n", wakeup_reason);
    break;
  }
}
bool SleepSetMin()
{
  int minNum = 0;
  short y1 = minNum / 10, y2 = minNum % 10;
  u8g2.clearBuffer();
  u8g2.setDrawColor(1);
  u8g2.drawXBMP(190, 0, 64, 64, jiao_fen);
  u8g2.drawXBMP(55, 3, 38, 54, nli[y1]);
  u8g2.drawXBMP(100, 3, 38, 54, nli[y2]);
  u8g2.drawRFrame(5, 2, 180, 58, 5);
  u8g2.setDrawColor(0);
  u8g2.drawBox(0, 0, 255, 2);
  u8g2.drawBox(0, 60, 255, 4);
  u8g2.sendBuffer();
  delay(1000);
  while (1)
  {
    if (digitalRead(key2) == LOW)
    {
      minNum++;
      if (minNum > 59)
        minNum = 0;
      // 原图当前坐标，要去的坐标
      // 新图当前坐标，要去的坐标
      short oy, ny, oy1, ny1;
      // 变大了，下翻
      oy = 3;
      ny = 60;
      oy1 = -54;
      ny1 = 3;
      Manual_min_animation(&oy, &ny, &oy1, &ny1, minNum - 1, minNum);
    }
    if (digitalRead(key1) == LOW)
    {
      minNum--;
      if (minNum < 0)
        minNum = 59;
      // 原图当前坐标，要去的坐标
      // 新图当前坐标，要去的坐标
      short oy, ny, oy1, ny1;
      oy = 3;
      ny = -54;
      oy1 = 60;
      ny1 = 3;
      Manual_min_animation(&oy, &ny, &oy1, &ny1, minNum + 1, minNum);
    }
    if (digitalRead(key3) == LOW)
    {
      sleepTime = (int)minNum * 60;
      y1 = minNum / 10;
      y2 = minNum % 10;
      while (1)
      {
        u8g2.clearBuffer();
        u8g2.setDrawColor(1);
        u8g2.drawXBMP(190, 0, 64, 64, jiao_fen);
        u8g2.drawXBMP(55, 3, 38, 54, nli[y1]);
        u8g2.drawXBMP(100, 3, 38, 54, nli[y2]);
        u8g2.drawRFrame(5, 2, 180, 58, 5);
        u8g2.setDrawColor(0);
        u8g2.drawBox(0, 0, 255, 2);
        u8g2.drawBox(0, 60, 255, 4);
        if (ui_disapper() == 0)
        {
          u8g2.sendBuffer();
          break;
        }
        u8g2.sendBuffer();
      }
      EEPROM.write(5, minNum);
      EEPROM.commit();
      return true;
    }
    if (digitalRead(key4) == LOW)
    {
      break;
    }
  }
  while (1)
  {
    u8g2.clearBuffer();
    u8g2.setDrawColor(1);
    u8g2.drawXBMP(190, 0, 64, 64, jiao_fen);
    u8g2.drawXBMP(55, 3, 38, 54, nli[0]);
    u8g2.drawXBMP(100, 3, 38, 54, nli[1]);
    u8g2.drawRFrame(5, 2, 180, 58, 5);
    u8g2.setDrawColor(0);
    u8g2.drawBox(0, 0, 255, 2);
    u8g2.drawBox(0, 60, 255, 4);
    if (ui_disapper() == 0)
    {
      u8g2.sendBuffer();
      break;
    }
    u8g2.sendBuffer();
  }
  sleepTime = 120;
  return false;
}
void setsleeptime()
{
  if (SleepSetMin())
  {
    u8g2.setFont(dinkie_9);
    for (int i = 0; i < 50; i++)
    {
      u8g2.clearBuffer();
      u8g2.setDrawColor(1);
      u8g2.drawXBMP(103, 2, 50, 50, gou);
      u8g2.setDrawColor(0);
      u8g2.drawBox(103 + i, 2, 51 - i, 50);
      u8g2.setCursor(5, 58);
      u8g2.setDrawColor(1);
      u8g2.print("熄屏休眠时间已设定！休眠时间：");
      u8g2.print(sleepTime / 60);
      u8g2.print("分钟！");
      u8g2.sendBuffer();
    }
    delay(3000);
    disMenu();
  }
}