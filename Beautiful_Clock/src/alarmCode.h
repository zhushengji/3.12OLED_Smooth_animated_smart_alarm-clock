/**
 * @brief 闹钟的小时设定
 * @return 设定成功返回真
 */
bool AlarmSetHour()
{

  int hourNum = 12;
  if (Alarm.a_hour != 255)
    hourNum = Alarm.a_hour;
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
      Alarm.a_hour = (int)hourNum;
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
      EEPROM.write(2, Alarm.a_hour);
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
  Alarm.a_hour = 1;
  return false;
}
/**
 * @brief 闹钟的分钟设定
 * @return 设定成功返回真
 */
bool AlarmSetMin()
{
  // Alarm.a_hour = EEPROM.read(2);
  // Alarm.a_min = EEPROM.read(3);
  // Alarm.a_frequency = (short)EEPROM.read(4);
  int minNum = 30;
  if (Alarm.a_min != 255)
    minNum = Alarm.a_min;
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
      Alarm.a_min = (int)minNum;
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
      EEPROM.write(3, Alarm.a_min);
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
  return false;
  Alarm.a_min = 1;
  return false;
}
/**
 * @brief 闹钟频率设定动画
 * @param op 当前选项
 * @param x1 当前第1个小点下标
 * @param x2 当前第2个小点下标
 * @param x3 当前第3个小点下标
 * @param frx 当前方框光标下标
 */
void SetPre_box_annimation(short op, short *x1, short *x2, short *x3, short *frx)
{
  // Serial.printf("进入频次动画\n");
  float velocity = 0.0f;
  float step = 0.08f;
  // 三个长条的初识位置，目标变化长度，当前长度:bx1 = 102, bx2 = 144, bx3 = 151,
  short bl = 40, bd = 5;
  short fbx1, fbx2, fbx3, ffrx;
  short obl = 40, fobl = 5;
  switch (op)
  {
  case 1:
    fbx1 = 102;
    fbx2 = 144;
    fbx3 = 151;
    ffrx = 23;
    while (!ui_siglerun(frx, &ffrx, 7, 3) || !ui_siglerun(x2, &fbx2, 7, 3) || !ui_siglerun(&obl, &fobl, 7, 3) || !ui_siglerun(&bd, &bl, 7, 3) || !ui_siglerun(x3, &fbx3, 7, 3))
    {
      // Serial.printf("刷新动画选项：%d\n",op);
      u8g2.clearBuffer();
      u8g2.drawXBMP(*frx, 0, 58, 58, kuang);
      u8g2.drawXBMP(25, 2, 54, 54, danci);
      u8g2.drawXBMP(102, 2, 54, 54, gongzuori);
      u8g2.drawXBMP(177, 2, 54, 54, meitian);
      u8g2.drawBox(*x1, 60, bd, 3);
      u8g2.drawBox(*x2, 60, obl, 3);
      u8g2.drawBox(*x3, 60, 5, 3);
      u8g2.sendBuffer();
    }
    // Serial.printf("frx：%d--x1：%d--x2：%d--x3：%d\n",*frx,*x1,*x2,*x3);
    break;
  case 2:
    fbx1 = 102;
    fbx2 = 109;
    fbx3 = 151;
    ffrx = 100;
    while (!ui_siglerun(frx, &ffrx, 7, 3) || !ui_siglerun(&obl, &fobl, 7, 3) || !ui_siglerun(x3, &fbx3, 7, 3) || !ui_siglerun(x2, &fbx2, 7, 3) || !ui_siglerun(&bd, &bl, 7, 3))
    {
      // Serial.printf("刷新动画选项：%d\n",op);
      u8g2.clearBuffer();
      u8g2.drawXBMP(*frx, 0, 58, 58, kuang);
      u8g2.drawXBMP(25, 2, 54, 54, danci);
      u8g2.drawXBMP(102, 2, 54, 54, gongzuori);
      u8g2.drawXBMP(177, 2, 54, 54, meitian);
      u8g2.drawBox(*x1, 60, obl, 3);
      u8g2.drawBox(*x2, 60, bd, 3);
      u8g2.drawBox(*x3, 60, obl, 3);
      u8g2.sendBuffer();
    }
    // Serial.printf("frx：%d--x1：%d--x2：%d--x3：%d\n",*frx,*x1,*x2,*x3);
    break;
  case 3:
    fbx1 = 102;
    fbx2 = 109;
    fbx3 = 116;
    ffrx = 175;
    while (!ui_siglerun(frx, &ffrx, 7, 3) || !ui_siglerun(x2, &fbx2, 7, 3) || !ui_siglerun(&obl, &fobl, 7, 3) || !ui_siglerun(x3, &fbx3, 7, 3) || !ui_siglerun(&bd, &bl, 7, 3))
    {
      u8g2.clearBuffer();
      u8g2.drawXBMP(*frx, 0, 58, 58, kuang);
      u8g2.drawXBMP(25, 2, 54, 54, danci);
      u8g2.drawXBMP(102, 2, 54, 54, gongzuori);
      u8g2.drawXBMP(177, 2, 54, 54, meitian);
      u8g2.drawBox(*x1, 60, 5, 3);
      u8g2.drawBox(*x2, 60, obl, 3);
      u8g2.drawBox(*x3, 60, bd, 3);
      u8g2.sendBuffer();
    }
    break;
  default:
    break;
  }
}
/**
 * @brief 设定闹钟频次
 * @return 成功设定返回真并开启闹钟，否则不开启闹钟
 */
bool Alarm_SetPre()
{
  u8g2.setDrawColor(1);
  short bx1 = 102, bx2 = 144, bx3 = 151, ffx = 25;
  short op = 1;
  SetPre_box_annimation(op, &bx1, &bx2, &bx3, &ffx);
  while (1)
  {
    if (digitalRead(key2) == LOW)
    {
      op++;
      if (op > 3)
        op = 3;
      SetPre_box_annimation(op, &bx1, &bx2, &bx3, &ffx);
    }
    if (digitalRead(key1) == LOW)
    {
      op--;
      if (op < 1)
        op = 1;
      SetPre_box_annimation(op, &bx1, &bx2, &bx3, &ffx);
    }
    if (digitalRead(key3) == LOW)
    {
      Alarm.a_frequency = op;
      EEPROM.write(4, Alarm.a_frequency);
      if (EEPROM.commit())
      {
        Serial.printf("已提交！闹钟小时：%d,闹钟分钟：%d,闹钟频率：%d\n", EEPROM.read(2), EEPROM.read(3), EEPROM.read(4));
        // EEPROM.write(4, Alarm.a_frequency);
        // EEPROM.commit();
        return true;
      }else{
        return false;
      }
    }
    if (digitalRead(key4) == LOW)
    {
      Alarm.a_frequency = 0;
      return false;
    }
  }
  return false;
}
/**
 * @brief 闹钟设定
 */
void set_Alarm()
{
  delay(500);
  if (AlarmSetHour())
  {
    if (AlarmSetMin())
    {
      if (Alarm_SetPre())
      {
        Serial.printf("%d点%d分的闹钟设定成功\n", Alarm.a_hour, Alarm.a_min);

        // EEPROM.write(2,Alarm.a_hour);
        // EEPROM.write(3,Alarm.a_min);
        // EEPROM.commit();
        alarmState = true;
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
          u8g2.print("闹钟已成功设定！");
          u8g2.sendBuffer();
        }
        delay(3000);
        disMenu();
      }
      else
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
          u8g2.print("闹钟取消或失败！");
          u8g2.sendBuffer();
        }
        EEPROM.write(4, 0);
        EEPROM.commit();
        disMenu();
      }
    }
    else
    {
      EEPROM.write(4, 0);
      EEPROM.commit();
      disMenu();
    }
  }
  else
  {
    EEPROM.write(4, 0);
    EEPROM.commit();
    disMenu();
  }
}
void drawTimeAlarm()
{
  RTC_RX8025T.read(RXtm);
  hsc = RXtm.Hour / 10;
  hgc = RXtm.Hour % 10;
  msc = RXtm.Minute / 10;
  mgc = RXtm.Minute % 10;
  u8g2.setDrawColor(1);
  u8g2.drawXBMP(-2, 3, 54, 54, xianglling);
  u8g2.drawXBMP(62, 3, 38, 54, nli[hsc]);
  u8g2.drawXBMP(98, 3, 38, 54, nli[hgc]);
  u8g2.drawXBMP(158, 3, 38, 54, nli[msc]);
  u8g2.drawXBMP(198, 3, 38, 54, nli[mgc]);
  u8g2.drawRFrame(54, 2, 95, 58, 5);
  u8g2.drawRFrame(151, 2, 95, 58, 5);
}
/**
 * @brief 闹铃响起
 */
void Alarm_ring()
{
  u8g2.clearBuffer();
  drawTimeAlarm();
  u8g2.sendBuffer();
  int k = (60 - RXtm.Second) * 500;
  while (digitalRead(key1) != LOW && digitalRead(key2) != LOW && digitalRead(key3) != LOW && digitalRead(key4) != LOW)
  {
    for (int i = 0; i < k; i++)
    {
      digitalWrite(BUZZ, HIGH);
      delay(1);
      digitalWrite(BUZZ, LOW);
      delay(1);
      if (digitalRead(key1) == LOW || digitalRead(key2) == LOW || digitalRead(key3) == LOW || digitalRead(key4) == LOW)
      {
        if (EEPROM.read(4) == 1)
        {

          EEPROM.write(4, 0);
          if (EEPROM.commit())
          {
            alarmState = false;
            Serial.println("单次闹钟已取消");
          }
        }
        return;
      }
    }
    u8g2.clearBuffer();
    drawTimeAlarm();
    u8g2.sendBuffer();
    k = (60 - RXtm.Second) * 500;
  }
  if (EEPROM.read(4) == 1)
  {
    EEPROM.write(4, 0);
    if (EEPROM.commit())
    {
      alarmState = false;
      Serial.println("单次闹钟已取消");
    }
  }
}