
/**
 * @brief 获取网络时间以校准本地时钟
 * @return true 校准成功
 * @return false 校准失败
 */
bool getNetTime()
{
  // Connect to Wi-Fi
  if (WiFi.status() != WL_CONNECTED)
  {
    wifi_init();
  }
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("获取时间失败");
    return false;
  }
  Serial.print("  年: ");
  Serial.print(timeinfo.tm_year + 1900);
  Serial.print("  月: ");
  Serial.print(timeinfo.tm_mon + 1);
  Serial.print("  日: ");
  Serial.print(timeinfo.tm_mday);
  Serial.print("  周: ");
  Serial.print(timeinfo.tm_wday);
  Serial.print("  时: ");
  Serial.print(timeinfo.tm_hour);
  Serial.print("  分: ");
  Serial.println(timeinfo.tm_min);
  setTime(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec, timeinfo.tm_mday, timeinfo.tm_mon+1, timeinfo.tm_year - 100);
  // Set the RTC from the system time
  RTC_RX8025T.set(now());
  return true;
}

/*时钟校准功能*/

struct offlineTime
{
  int year;
  int month;
  int day;
  int hour;
  int min;
} oft;
/**
 * @brief Get the Maxday object
 * 根据年份和月份返回当前月最多多少天
 * @param year
 * @param moth
 * @return int
 */
int getMaxday(int y, int m)
{
  bool isleap_year = false;
  if (y % 400 == 0 || (y % 4 == 0 && y % 100 != 0))
  {
    isleap_year = true;
  }
  if (m == 2)
  {
    if (isleap_year)
      return 29;
    return 28;
  }
  else if (m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 11)
  {
    return 31;
  }
  else
  {
    return 30;
  }
}
/**
 * @brief 校准时间中的分钟的动画
 * @param oy 老图原坐标
 * @param ny 老图目标坐标
 * @param oy1 新图原坐标
 * @param ny1 新图目标坐标
 * @param minNum_old 分钟未变化前数据
 * @param minNum 分钟数据
 */
void Manual_min_animation(short *oy, short *ny, short *oy1, short *ny1, short minNum_old, short minNum)
{
  if (minNum_old < 0)
    minNum_old = 59;
  if (minNum_old > 59)
    minNum_old = 0;
  short t1 = *oy, t2 = *ny, t3 = *oy1, t4 = *ny1;
  short y1 = minNum / 10, y2 = minNum % 10;
  short y1_old = minNum_old / 10, y2_old = minNum_old % 10;
  if (y2 != y2_old)
  {
    while (!ui_siglerun(oy, ny, 3, 2) || !ui_siglerun(oy1, ny1, 3, 2))
    {
      u8g2.clearBuffer();
      u8g2.setDrawColor(1);
      u8g2.drawXBMP(190, 0, 64, 64, jiao_fen);
      u8g2.drawXBMP(55, 3, 38, 54, nli[y1_old]);
      u8g2.drawXBMP(100, *oy, 38, 54, nli[y2_old]);
      u8g2.drawXBMP(100, *oy1, 38, 54, nli[y2]);
      u8g2.drawRFrame(5, 2, 180, 58, 5);
      u8g2.setDrawColor(0);
      u8g2.drawBox(0, 0, 255, 2);
      u8g2.drawBox(0, 60, 255, 4);
      u8g2.sendBuffer();
    }
  }
  *oy = t1;
  *ny = t2;
  *oy1 = t3;
  *ny1 = t4;
  if (y1 != y1_old)
  {
    while (!ui_siglerun(oy, ny, 5, 2) || !ui_siglerun(oy1, ny1, 5, 2))
    {
      u8g2.clearBuffer();
      u8g2.setDrawColor(1);
      u8g2.drawXBMP(190, 0, 64, 64, jiao_fen);
      u8g2.drawXBMP(55, *oy, 38, 54, nli[y1_old]);
      u8g2.drawXBMP(55, *oy1, 38, 54, nli[y1]);
      u8g2.drawXBMP(100, 3, 38, 54, nli[y2]);
      u8g2.drawRFrame(5, 2, 180, 58, 5);
      u8g2.setDrawColor(0);
      u8g2.drawBox(0, 0, 255, 2);
      u8g2.drawBox(0, 60, 255, 4);
      u8g2.sendBuffer();
    }
  }
}
/**
 * @brief 校准时间中的小时的动画
 * @param oy 老图原坐标
 * @param ny 老图目标坐标
 * @param oy1 新图原坐标
 * @param ny1 新图目标坐标
 * @param hourNum_old 日未变化前数据
 * @param hourNum 日数据
 */
void Manual_hour_animation(short *oy, short *ny, short *oy1, short *ny1, short hourNum_old, short hourNum)
{
  if (hourNum_old < 1)
    hourNum_old = 0;
  if (hourNum_old > 23)
    hourNum_old = 1;
  short t1 = *oy, t2 = *ny, t3 = *oy1, t4 = *ny1;
  short y1 = hourNum / 10, y2 = hourNum % 10;
  short y1_old = hourNum_old / 10, y2_old = hourNum_old % 10;
  if (y2 != y2_old)
  {
    while (!ui_siglerun(oy, ny, 3, 2) || !ui_siglerun(oy1, ny1, 3, 2))
    {
      u8g2.clearBuffer();
      u8g2.setDrawColor(1);
      u8g2.drawXBMP(190, 0, 64, 64, jiao_shi);
      u8g2.drawXBMP(55, 3, 38, 54, nli[y1_old]);
      u8g2.drawXBMP(100, *oy, 38, 54, nli[y2_old]);
      u8g2.drawXBMP(100, *oy1, 38, 54, nli[y2]);
      u8g2.drawRFrame(5, 2, 180, 58, 5);
      u8g2.setDrawColor(0);
      u8g2.drawBox(0, 0, 255, 2);
      u8g2.drawBox(0, 60, 255, 4);
      u8g2.sendBuffer();
    }
  }
  *oy = t1;
  *ny = t2;
  *oy1 = t3;
  *ny1 = t4;
  if (y1 != y1_old)
  {
    while (!ui_siglerun(oy, ny, 5, 2) || !ui_siglerun(oy1, ny1, 5, 2))
    {
      u8g2.clearBuffer();
      u8g2.setDrawColor(1);
      u8g2.drawXBMP(190, 0, 64, 64, jiao_shi);
      u8g2.drawXBMP(55, *oy, 38, 54, nli[y1_old]);
      u8g2.drawXBMP(55, *oy1, 38, 54, nli[y1]);
      u8g2.drawXBMP(100, 3, 38, 54, nli[y2]);
      u8g2.drawRFrame(5, 2, 180, 58, 5);
      u8g2.setDrawColor(0);
      u8g2.drawBox(0, 0, 255, 2);
      u8g2.drawBox(0, 60, 255, 4);
      u8g2.sendBuffer();
    }
  }
}
/**
 * @brief 校准时间中的日的动画
 * @param oy 老图原坐标
 * @param ny 老图目标坐标
 * @param oy1 新图原坐标
 * @param ny1 新图目标坐标
 * @param maxday 当月最大日
 * @param dayNum_old 日未变化前数据
 * @param dayNum 日数据
 */
void Manual_day_animation(short maxday, short *oy, short *ny, short *oy1, short *ny1, short dayhNum_old, short dayhNum)
{
  if (dayhNum_old < 1)
    dayhNum_old = maxday;
  if (dayhNum_old > maxday)
    dayhNum_old = 1;
  short t1 = *oy, t2 = *ny, t3 = *oy1, t4 = *ny1;
  short y1 = dayhNum / 10, y2 = dayhNum % 10;
  short y1_old = dayhNum_old / 10, y2_old = dayhNum_old % 10;
  if (y2 != y2_old)
  {
    while (!ui_siglerun(oy, ny, 3, 2) || !ui_siglerun(oy1, ny1, 3, 2))
    {
      u8g2.clearBuffer();
      u8g2.setDrawColor(1);
      u8g2.drawXBMP(190, 0, 64, 64, jiao_ri);
      u8g2.drawXBMP(55, 3, 38, 54, nli[y1_old]);
      u8g2.drawXBMP(100, *oy, 38, 54, nli[y2_old]);
      u8g2.drawXBMP(100, *oy1, 38, 54, nli[y2]);
      u8g2.drawRFrame(5, 2, 180, 58, 5);
      u8g2.setDrawColor(0);
      u8g2.drawBox(0, 0, 255, 2);
      u8g2.drawBox(0, 60, 255, 4);
      u8g2.sendBuffer();
    }
  }
  *oy = t1;
  *ny = t2;
  *oy1 = t3;
  *ny1 = t4;
  if (y1 != y1_old)
  {
    // Serial.println("日十位开始变化");
    while (!ui_siglerun(oy, ny, 5, 2) || !ui_siglerun(oy1, ny1, 5, 2))
    {
      u8g2.clearBuffer();
      u8g2.setDrawColor(1);
      u8g2.drawXBMP(190, 0, 64, 64, jiao_ri);
      u8g2.drawXBMP(55, *oy, 38, 54, nli[y1_old]);
      u8g2.drawXBMP(55, *oy1, 38, 54, nli[y1]);
      u8g2.drawXBMP(100, 3, 38, 54, nli[y2]);
      u8g2.drawRFrame(5, 2, 180, 58, 5);
      u8g2.setDrawColor(0);
      u8g2.drawBox(0, 0, 255, 2);
      u8g2.drawBox(0, 60, 255, 4);
      u8g2.sendBuffer();
    }
    // Serial.println("日十位变化结束");
  }
}
/**
 * @brief 校准时间中的月的动画
 * @param oy 老图原坐标
 * @param ny 老图目标坐标
 * @param oy1 新图原坐标
 * @param ny1 新图目标坐标
 * @param monthNum_old 月未变化前数据
 * @param monthNum 月数据
 */
void Manual_month_animation(short *oy, short *ny, short *oy1, short *ny1, short monthNum_old, short monthNum)
{
  if (monthNum_old < 1)
    monthNum_old = 12;
  if (monthNum_old > 12)
    monthNum_old = 1;
  short t1 = *oy, t2 = *ny, t3 = *oy1, t4 = *ny1;
  short y1 = monthNum / 10, y2 = monthNum % 10;
  short y1_old = monthNum_old / 10, y2_old = monthNum_old % 10;
  if (y2 != y2_old)
  {
    while (!ui_siglerun(oy, ny, 3, 2) || !ui_siglerun(oy1, ny1, 3, 2))
    {
      u8g2.clearBuffer();
      u8g2.setDrawColor(1);
      u8g2.drawXBMP(190, 0, 64, 64, jiao_yue);
      u8g2.drawXBMP(55, 3, 38, 54, nli[y1_old]);
      u8g2.drawXBMP(100, *oy, 38, 54, nli[y2_old]);
      u8g2.drawXBMP(100, *oy1, 38, 54, nli[y2]);
      u8g2.drawRFrame(5, 2, 180, 58, 5);
      u8g2.setDrawColor(0);
      u8g2.drawBox(0, 0, 255, 2);
      u8g2.drawBox(0, 60, 255, 4);
      u8g2.sendBuffer();
    }
  }
  *oy = t1;
  *ny = t2;
  *oy1 = t3;
  *ny1 = t4;
  if (y1 != y1_old)
  {
    Serial.println("月份十位开始变化");
    while (!ui_siglerun(oy, ny, 5, 2) || !ui_siglerun(oy1, ny1, 5, 2))
    {
      u8g2.clearBuffer();
      u8g2.setDrawColor(1);
      u8g2.drawXBMP(190, 0, 64, 64, jiao_yue);
      u8g2.drawXBMP(55, *oy, 38, 54, nli[y1_old]);
      u8g2.drawXBMP(55, *oy1, 38, 54, nli[y1]);
      u8g2.drawXBMP(100, 3, 38, 54, nli[y2]);
      u8g2.drawRFrame(5, 2, 180, 58, 5);
      u8g2.setDrawColor(0);
      u8g2.drawBox(0, 0, 255, 2);
      u8g2.drawBox(0, 60, 255, 4);
      u8g2.sendBuffer();
    }
    Serial.println("月份十位变化结束");
  }
}
/**
 * @brief 校准时间中的年的动画
 * @param oy 老图原坐标
 * @param ny 老图目标坐标
 * @param oy1 新图原坐标
 * @param ny1 新图目标坐标
 * @param yearNum_old 年未变化前数据
 * @param yearNum 年数据
 */
void Manual_year_animation(short *oy, short *ny, short *oy1, short *ny1, short yearNum_old, short yearNum)
{
  short t1 = *oy, t2 = *ny, t3 = *oy1, t4 = *ny1;
  short y1 = yearNum / 1000, y2 = yearNum / 100 % 10, y3 = yearNum / 10 % 10, y4 = yearNum % 10;
  short y1_old = yearNum_old / 1000, y2_old = yearNum_old / 100 % 10, y3_old = yearNum_old / 10 % 10, y4_old = yearNum_old % 10;
  if (y4 != y4_old)
  {
    while (!ui_siglerun(oy, ny, 3, 2) || !ui_siglerun(oy1, ny1, 3, 2))
    {
      u8g2.clearBuffer();
      u8g2.setDrawColor(1);
      u8g2.drawXBMP(190, 0, 64, 64, jiao_nian);
      u8g2.drawXBMP(10, 3, 38, 54, nli[y1_old]);
      u8g2.drawXBMP(55, 3, 38, 54, nli[y2_old]);
      u8g2.drawXBMP(100, 3, 38, 54, nli[y3_old]);
      u8g2.drawXBMP(145, *oy, 38, 54, nli[y4_old]);
      u8g2.drawXBMP(145, *oy1, 38, 54, nli[y4]);
      u8g2.drawRFrame(5, 2, 180, 58, 5);
      u8g2.setDrawColor(0);
      u8g2.drawBox(0, 0, 255, 2);
      u8g2.drawBox(0, 60, 255, 4);
      u8g2.sendBuffer();
    }
  }
  // 坐标数据初始化
  *oy = t1;
  *ny = t2;
  *oy1 = t3;
  *ny1 = t4;
  if (y3 != y3_old)
  {
    while (!ui_siglerun(oy, ny, 5, 2) || !ui_siglerun(oy1, ny1, 5, 2))
    {
      u8g2.clearBuffer();
      u8g2.setDrawColor(1);
      u8g2.drawXBMP(190, 0, 64, 64, jiao_nian);
      u8g2.drawXBMP(10, 3, 38, 54, nli[y1_old]);
      u8g2.drawXBMP(55, 3, 38, 54, nli[y2_old]);
      u8g2.drawXBMP(100, *oy, 38, 54, nli[y3_old]);
      u8g2.drawXBMP(100, *oy1, 38, 54, nli[y3]);
      u8g2.drawXBMP(145, 3, 38, 54, nli[y4]);
      u8g2.drawRFrame(5, 2, 180, 58, 5);
      u8g2.setDrawColor(0);
      u8g2.drawBox(0, 0, 255, 2);
      u8g2.drawBox(0, 60, 255, 4);
      u8g2.sendBuffer();
    }
  }
  // 坐标数据初始化
  *oy = t1;
  *ny = t2;
  *oy1 = t3;
  *ny1 = t4;
  if (y2 != y2_old)
  {
    while (!ui_siglerun(oy, ny, 7, 2) || !ui_siglerun(oy1, ny1, 7, 2))
    {
      u8g2.clearBuffer();
      u8g2.setDrawColor(1);
      u8g2.drawXBMP(190, 0, 64, 64, jiao_nian);
      u8g2.drawXBMP(10, 3, 38, 54, nli[y1_old]);
      u8g2.drawXBMP(55, *oy, 38, 54, nli[y2_old]);
      u8g2.drawXBMP(55, *oy1, 38, 54, nli[y2]);
      u8g2.drawXBMP(100, 3, 38, 54, nli[y3]);
      u8g2.drawXBMP(145, 3, 38, 54, nli[y4]);
      u8g2.drawRFrame(5, 2, 180, 58, 5);
      u8g2.setDrawColor(0);
      u8g2.drawBox(0, 0, 255, 2);
      u8g2.drawBox(0, 60, 255, 4);
      u8g2.sendBuffer();
    }
  }
  // 坐标数据初始化
  *oy = t1;
  *ny = t2;
  *oy1 = t3;
  *ny1 = t4;
  if (y1 != y1_old)
  {
    while (!ui_siglerun(oy, ny, 8, 2) || !ui_siglerun(oy1, ny1, 8, 2))
    {
      u8g2.clearBuffer();
      u8g2.setDrawColor(1);
      u8g2.drawXBMP(190, 0, 64, 64, jiao_nian);
      u8g2.drawXBMP(10, *oy, 38, 54, nli[y1_old]);
      u8g2.drawXBMP(10, *oy1, 38, 54, nli[y1]);
      u8g2.drawXBMP(55, 3, 38, 54, nli[y2]);
      u8g2.drawXBMP(100, 3, 38, 54, nli[y3]);
      u8g2.drawXBMP(145, 3, 38, 54, nli[y4]);
      u8g2.drawRFrame(5, 2, 180, 58, 5);
      u8g2.setDrawColor(0);
      u8g2.drawBox(0, 0, 255, 2);
      u8g2.drawBox(0, 60, 255, 4);
      u8g2.sendBuffer();
    }
  }
}

/**
 * @brief 手动设置年份
 * @return true 设置成功返回true
 * @return false 设置失败返回false
 */
bool ManualSetYear()
{
  short yearNum = 2025;
  short y1 = yearNum / 1000, y2 = yearNum / 100 % 10, y3 = yearNum / 10 % 10, y4 = yearNum % 10;
  u8g2.clearBuffer();
  u8g2.setDrawColor(1);
  u8g2.drawXBMP(190, 0, 64, 64, jiao_nian);
  u8g2.drawXBMP(10, 3, 38, 54, nli[y1]);
  u8g2.drawXBMP(55, 3, 38, 54, nli[y2]);
  u8g2.drawXBMP(100, 3, 38, 54, nli[y3]);
  u8g2.drawXBMP(145, 3, 38, 54, nli[y4]);
  u8g2.drawRFrame(5, 2, 180, 58, 5);
  u8g2.setDrawColor(0);
  u8g2.drawBox(0, 0, 255, 2);
  u8g2.drawBox(0, 60, 255, 4);
  u8g2.sendBuffer();
  while (1)
  {
    if (digitalRead(key2) == LOW)
    {
      yearNum++;
      // 原图当前坐标，要去的坐标
      // 新图当前坐标，要去的坐标
      short oy, ny, oy1, ny1;
      // 变大了，下翻
      oy = 3;
      ny = 60;
      oy1 = -54;
      ny1 = 3;
      Manual_year_animation(&oy, &ny, &oy1, &ny1, yearNum - 1, yearNum);
    }
    if (digitalRead(key1) == LOW)
    {
      yearNum--;
      // 原图当前坐标，要去的坐标
      // 新图当前坐标，要去的坐标
      short oy, ny, oy1, ny1;
      oy = 3;
      ny = -54;
      oy1 = 60;
      ny1 = 3;
      Manual_year_animation(&oy, &ny, &oy1, &ny1, yearNum + 1, yearNum);
    }
    if (digitalRead(key3) == LOW)
    {
      oft.year = yearNum;
      Serial.printf("确认设定，oft.year为%d\n", oft.year);
      y1 = yearNum / 1000;
      y2 = yearNum / 100 % 10;
      y3 = yearNum / 10 % 10;
      y4 = yearNum % 10;
      while (1)
      {
        u8g2.clearBuffer();
        u8g2.setDrawColor(1);
        u8g2.drawXBMP(190, 0, 64, 64, jiao_nian);
        u8g2.drawXBMP(10, 3, 38, 54, nli[y1]);
        u8g2.drawXBMP(55, 3, 38, 54, nli[y2]);
        u8g2.drawXBMP(100, 3, 38, 54, nli[y3]);
        u8g2.drawXBMP(145, 3, 38, 54, nli[y4]);
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
      return true;
    }
    if (digitalRead(key4) == LOW)
    {
      break;
    }
  }
  oft.year = 1997;
  while (1)
  {
    u8g2.clearBuffer();
    u8g2.setDrawColor(1);
    u8g2.drawXBMP(190, 0, 64, 64, jiao_nian);
    u8g2.drawXBMP(10, 3, 38, 54, nli[1]);
    u8g2.drawXBMP(55, 3, 38, 54, nli[9]);
    u8g2.drawXBMP(100, 3, 38, 54, nli[9]);
    u8g2.drawXBMP(145, 3, 38, 54, nli[7]);
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
  return false;
}
/**
 * @brief
 * 手动设置月份
 * @return true
 * @return false
 */
bool ManualSetMonth()
{
  int mothNum = 1;
  short y1 = mothNum / 10, y2 = mothNum % 10;
  u8g2.clearBuffer();
  u8g2.setDrawColor(1);
  u8g2.drawXBMP(190, 0, 64, 64, jiao_yue);
  u8g2.drawXBMP(55, 3, 38, 54, nli[y1]);
  u8g2.drawXBMP(100, 3, 38, 54, nli[y2]);
  u8g2.drawRFrame(5, 2, 180, 58, 5);
  u8g2.setDrawColor(0);
  u8g2.drawBox(0, 0, 255, 2);
  u8g2.drawBox(0, 60, 255, 4);
  u8g2.sendBuffer();
  while (1)
  {
    if (digitalRead(key2) == LOW)
    {
      mothNum++;
      if (mothNum == 13)
        mothNum = 1;
      // 原图当前坐标，要去的坐标
      // 新图当前坐标，要去的坐标
      short oy, ny, oy1, ny1;
      // 变大了，下翻
      oy = 3;
      ny = 60;
      oy1 = -54;
      ny1 = 3;
      Manual_month_animation(&oy, &ny, &oy1, &ny1, mothNum - 1, mothNum);
    }
    if (digitalRead(key1) == LOW)
    {
      mothNum--;
      if (mothNum < 1)
        mothNum = 12;
      // 原图当前坐标，要去的坐标
      // 新图当前坐标，要去的坐标
      short oy, ny, oy1, ny1;
      oy = 3;
      ny = -54;
      oy1 = 60;
      ny1 = 3;
      Manual_month_animation(&oy, &ny, &oy1, &ny1, mothNum + 1, mothNum);
    }
    if (digitalRead(key3) == LOW)
    {
      oft.month = mothNum;
      // Serial.printf("确认设定，oft.month%d\n", oft.month);
      y1 = mothNum / 10;
      y2 = mothNum % 10;
      while (1)
      {
        u8g2.clearBuffer();
        u8g2.setDrawColor(1);
        u8g2.drawXBMP(190, 0, 64, 64, jiao_yue);
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
      return true;
    }
    if (digitalRead(key4) == LOW)
    {
      break;
    }
  }
  oft.month = 1;
  while (1)
  {
    u8g2.clearBuffer();
    u8g2.setDrawColor(1);
    u8g2.drawXBMP(190, 0, 64, 64, jiao_yue);
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
  return false;
}

/**
 * @brief
 * 手动设置天
 * @return true
 * @return false
 */
bool ManualSetDay()
{
  int dayNum = 1;
  int maxday = getMaxday(oft.year, oft.month);
  Serial.printf("当前月最大值：%d\n",maxday);
  short y1 = dayNum / 10, y2 = dayNum % 10;
  u8g2.clearBuffer();
  u8g2.setDrawColor(1);
  u8g2.drawXBMP(190, 0, 64, 64, jiao_ri);
  u8g2.drawXBMP(55, 3, 38, 54, nli[y1]);
  u8g2.drawXBMP(100, 3, 38, 54, nli[y2]);
  u8g2.drawRFrame(5, 2, 180, 58, 5);
  u8g2.setDrawColor(0);
  u8g2.drawBox(0, 0, 255, 2);
  u8g2.drawBox(0, 60, 255, 4);
  u8g2.sendBuffer();
  while (1)
  {
    if (digitalRead(key2) == LOW)
    {
      dayNum++;
      if (dayNum > maxday)
        dayNum = 1;
      // 原图当前坐标，要去的坐标
      // 新图当前坐标，要去的坐标
      short oy, ny, oy1, ny1;
      // 变大了，下翻
      oy = 3;
      ny = 60;
      oy1 = -54;
      ny1 = 3;
      Manual_day_animation(maxday, &oy, &ny, &oy1, &ny1, dayNum - 1, dayNum);
    }
    if (digitalRead(key1) == LOW)
    {
      dayNum--;
      if (dayNum < 1)
        dayNum = maxday;
      // 原图当前坐标，要去的坐标
      // 新图当前坐标，要去的坐标
      short oy, ny, oy1, ny1;
      oy = 3;
      ny = -54;
      oy1 = 60;
      ny1 = 3;
      Manual_day_animation(maxday, &oy, &ny, &oy1, &ny1, dayNum + 1, dayNum);
    }
    if (digitalRead(key3) == LOW)
    {
      oft.day = dayNum;
      // Serial.printf("确认设定，oft.month%d\n", oft.month);
      y1 = dayNum / 10;
      y2 = dayNum % 10;
      while (1)
      {
        u8g2.clearBuffer();
        u8g2.setDrawColor(1);
        u8g2.drawXBMP(190, 0, 64, 64, jiao_ri);
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
      return true;
    }
    if (digitalRead(key4) == LOW)
    {
      break;
    }
  }
  oft.day = 1;
  while (1)
  {
    u8g2.clearBuffer();
    u8g2.setDrawColor(1);
    u8g2.drawXBMP(190, 0, 64, 64, jiao_ri);
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
  return false;
}
/**
 * @brief
 * 手动设置小时
 * @return true
 * @return false
 */
bool ManualSetHour()
{
  int hourNum = 0;
  short y1 = hourNum / 10, y2 = hourNum % 10;
  u8g2.clearBuffer();
  u8g2.setDrawColor(1);
  u8g2.drawXBMP(190, 0, 64, 64, jiao_shi);
  u8g2.drawXBMP(55, 3, 38, 54, nli[y1]);
  u8g2.drawXBMP(100, 3, 38, 54, nli[y2]);
  u8g2.drawRFrame(5, 2, 180, 58, 5);
  u8g2.setDrawColor(0);
  u8g2.drawBox(0, 0, 255, 2);
  u8g2.drawBox(0, 60, 255, 4);
  u8g2.sendBuffer();
  while (1)
  {
    if (digitalRead(key2) == LOW)
    {
      hourNum++;
      if (hourNum > 23)
        hourNum = 0;
      // 原图当前坐标，要去的坐标
      // 新图当前坐标，要去的坐标
      short oy, ny, oy1, ny1;
      // 变大了，下翻
      oy = 3;
      ny = 60;
      oy1 = -54;
      ny1 = 3;
      Manual_hour_animation(&oy, &ny, &oy1, &ny1, hourNum - 1, hourNum);
    }
    if (digitalRead(key1) == LOW)
    {
      hourNum--;
      if (hourNum < 1)
        hourNum = 23;
      // 原图当前坐标，要去的坐标
      // 新图当前坐标，要去的坐标
      short oy, ny, oy1, ny1;
      oy = 3;
      ny = -54;
      oy1 = 60;
      ny1 = 3;
      Manual_hour_animation(&oy, &ny, &oy1, &ny1, hourNum + 1, hourNum);
    }
    if (digitalRead(key3) == LOW)
    {
      oft.hour = (int)hourNum;
      // Serial.printf("确认设定，oft.month%d\n", oft.month);
      y1 = hourNum / 10;
      y2 = hourNum % 10;
      while (1)
      {
        u8g2.clearBuffer();
        u8g2.setDrawColor(1);
        u8g2.drawXBMP(190, 0, 64, 64, jiao_shi);
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
      Serial.printf("确认设定，oft.hour%d\n", oft.hour);
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
    u8g2.drawXBMP(190, 0, 64, 64, jiao_shi);
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
  return false;
  // Manual_day_animation
  oft.hour = 1;
  return false;
}

/**
 * @brief
 * 手动设置分钟
 * @return true
 * @return false
 */
bool ManualSetMin()
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
      oft.min = (int)minNum;
      // Serial.printf("确认设定，oft.month%d\n", oft.month);
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
      Serial.printf("确认设定，oft.min%d\n", oft.min);
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
  return false;
  // Manual_day_animation
  oft.min = 1;
  return false;
}
/**
 * @brief 手动设定时间功能
 */
void ManualsetRTC()
{
  delay(500);
  RTC_RX8025T.init();
  ManualSetYear();
  ManualSetMonth();
  ManualSetDay();
  ManualSetHour();
  ManualSetMin();
  int y = oft.year,m=oft.month,d=oft.day,h=oft.hour,mm=oft.min,s=0;
  Serial.printf("待设定的时间信息:%d年%d月%d日%d时%d分\n", y, m, d, h, mm);
  setTime(h, mm, 0, d, m, y);
  RTC_RX8025T.set(now());
  RTC_RX8025T.read(RXtm);
  Serial.printf("已设定的时间:%d年%d月%d日%d时%d分\n", RXtm.Year, RXtm.Month, RXtm.Day, RXtm.Hour, RXtm.Minute);
}
