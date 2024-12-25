void draw_weather_icon(String wtmes, int px, int py, bool *isdrop)
{
    if (wtmes.equalsIgnoreCase("Cloudy") || wtmes.equalsIgnoreCase("Partly Cloudy") || wtmes.equalsIgnoreCase("Partly Cloudy") || wtmes.equalsIgnoreCase("Mostly Cloudy") || wtmes.equalsIgnoreCase("Mostly Cloudy") || wtmes.equalsIgnoreCase("Overcast"))
    {
        u8g2.drawXBMP(px, py, 32, 32, duoyun);
        *isdrop = false;
    }
    else if (wtmes.equalsIgnoreCase("Sunny") || wtmes.equalsIgnoreCase("Clear"))
    {
        u8g2.drawXBMP(px, py, 32, 32, qing);
        *isdrop = false;
    }
    else if (wtmes.equalsIgnoreCase("Shower") || wtmes.equalsIgnoreCase("Moderate Rain"))
    {
        u8g2.drawXBMP(px, py, 32, 32, zhongyu);
        *isdrop = true;
    }
    else if (wtmes.equalsIgnoreCase("Thundershower"))
    {
        u8g2.drawXBMP(px, py, 32, 32, leiyu);
        *isdrop = true;
    }
    else if (wtmes.equalsIgnoreCase("Thundershower with Hail") || wtmes.equalsIgnoreCase("Ice Rain"))
    {
        u8g2.drawXBMP(px, py, 32, 32, bingbaojiayu);
        *isdrop = true;
    }
    else if (wtmes.equalsIgnoreCase("Light rain"))
    {
        u8g2.drawXBMP(px, py, 32, 32, xiaoyu);
        *isdrop = true;
    }
    else if (wtmes.equalsIgnoreCase("Heavy Rain") || wtmes.equalsIgnoreCase("Storm") || wtmes.equalsIgnoreCase("Heavy Storm") || wtmes.equalsIgnoreCase("Severe Storm"))
    {
        u8g2.drawXBMP(px, py, 32, 32, dayu);
        *isdrop = true;
    }
    else if (wtmes.equalsIgnoreCase("Sleet"))
    {
        u8g2.drawXBMP(px, py, 32, 32, yujiaxue);
        *isdrop = true;
    }
    else if (wtmes.equalsIgnoreCase("Snow Flurry") || wtmes.equalsIgnoreCase("Moderate Snow"))
    {
        u8g2.drawXBMP(px, py, 32, 32, zhongxue);
        *isdrop = true;
    }
    else if (wtmes.equalsIgnoreCase("Light Snow"))
    {
        u8g2.drawXBMP(px, py, 32, 32, xiaoxue);
        *isdrop = true;
    }
    else if (wtmes.equalsIgnoreCase("Heavy Snow") || wtmes.equalsIgnoreCase("Snowstorm"))
    {
        u8g2.drawXBMP(px, py, 32, 32, daxue);
        *isdrop = true;
    }
    else if (wtmes.equalsIgnoreCase("Foggy") || wtmes.equalsIgnoreCase("Dust") || wtmes.equalsIgnoreCase("Sand") || wtmes.equalsIgnoreCase("Duststorm") || wtmes.equalsIgnoreCase("Sandstorm") || wtmes.equalsIgnoreCase("Haze"))
    {
        u8g2.drawXBMP(px, py, 32, 32, dawu);
        *isdrop = false;
    }
    else if (wtmes.equalsIgnoreCase("Unknown"))
    {
        u8g2.drawXBMP(px, py, 32, 32, weizhitianqi);
        *isdrop = false;
    }
}
/**
 * @brief 获取要变化的天气界面数据，包括天气名、温度、日期
 */
void getdaydata(short dayn, String *daytxt, String *tempn, String *dateS)
{
    switch (dayn)
    {
    case 0:
        *daytxt = future.date0_text_day;
        *tempn = future.date0_high;
        *dateS = "今天";
        return;
    case 1:
        *daytxt = future.date1_text_day;
        *tempn = future.date1_high;
        *dateS = "明天";
        return;
    case 2:
        *daytxt = future.date2_text_day;
        *tempn = future.date2_high;
        *dateS = "后天";
        return;
    default:
        break;
    }
}

void draw_Weather()
{
    Raindrop raindrops[NUM_RAINDROPS];
    initRaindrops(raindrops, NUM_RAINDROPS);
    if (WiFi.status() != WL_CONNECTED)
    {
        wifi_init();
    }
    Getfutrueweatherdata();
    const unsigned char *nli[6] = {bg1, bg2, bg3, bg4, bg5, bg6};
    short bgtem = random(0, 6), bgNum;
    bgNum = bgtem;
    short dayNum = 0;
    float velocity = 0.0f;
    float step = 0.08f;
    bool isdrop = false;
    draw_weather_icon(String(future.date0_text_day), 20, 0, &isdrop);
    String odates = "今天", otemps = String(future.date0_high);
    oweathers = String(future.date0_text_day);
    // effect = getWeatherEffect(oweathers);
    while (digitalRead(key4) != LOW)
    {
        // effect();
        u8g2.clearBuffer();
        u8g2.drawXBMP(0, 0, 256, 64, nli[bgNum]);
        u8g2.setFont(u8g2_font_courB18_tr);
        u8g2.setCursor(180, 20);
        u8g2.print(otemps);
        u8g2.print("'C");
        u8g2.setFont(u8g2_font_rosencrantz_nbp_t_all);
        u8g2.setCursor(180, 35);
        u8g2.print(future.location);
        draw_weather_icon(oweathers, 20, 0, &isdrop);
        u8g2.setFont(dinkie_9);
        u8g2.setCursor(20, 35);
        u8g2.print(odates);
        u8g2.setFont(u8g2_font_tinytim_tr);
        u8g2.setCursor(20, 45);
        u8g2.print(oweathers);
        if (isdrop)
        {
            updateRaindrops(raindrops, NUM_RAINDROPS);
            drawRaindrops(raindrops, NUM_RAINDROPS);
        }
        u8g2.sendBuffer();
        if (digitalRead(key1) == LOW)
        {
            dayNum--;
            if (dayNum < 0)
                dayNum = 2;
            short bgx = 0, bgfx = 255, nbgx = -256, nbgfx = 0; // 背景坐标
            short tx = 180, tfx = 435, ntx = -76, ntfx = 180;  // 温度信息坐标
            short px = 20, pfx = 275, npx = -235, npfx = 20;   // 天气图标坐标
            String dates, weathers, temps;
            bgNum = random(0, 6);
            getdaydata(dayNum, &weathers, &temps, &dates);
            while (ui_bounce(&bgx, &bgfx, &velocity, step) && ui_bounce(&nbgx, &nbgfx, &velocity, step) && ui_bounce(&tx, &tfx, &velocity, step) && ui_bounce(&ntx, &ntfx, &velocity, step) && ui_bounce(&px, &pfx, &velocity, step) && ui_bounce(&npx, &npfx, &velocity, step))
            {
                u8g2.clearBuffer();
                u8g2.drawXBMP(bgx, 0, 256, 64, nli[bgtem]);
                u8g2.drawXBMP(nbgx, 0, 256, 64, nli[bgNum]);
                u8g2.setFont(u8g2_font_courB18_tr);
                u8g2.setCursor(tx, 20);
                u8g2.print(otemps);
                u8g2.print("'C");
                u8g2.setCursor(ntx, 20);
                u8g2.print(temps);
                u8g2.print("'C");
                u8g2.setFont(u8g2_font_rosencrantz_nbp_t_all);
                u8g2.setCursor(180, 35);
                u8g2.print(future.location);
                draw_weather_icon(oweathers, px, 0, &isdrop);
                draw_weather_icon(weathers, npx, 0, &isdrop);
                u8g2.setFont(dinkie_9);
                u8g2.setCursor(px, 35);
                u8g2.print(odates);
                u8g2.setCursor(npx, 35);
                u8g2.print(dates);
                u8g2.setFont(u8g2_font_tinytim_tr);
                u8g2.setCursor(px, 45);
                u8g2.print(oweathers);
                u8g2.setCursor(npx, 45);
                u8g2.print(weathers);
                u8g2.sendBuffer();
            }
            oweathers = weathers;
            // effect = getWeatherEffect(oweathers);
            bgtem = bgNum;
            odates = dates;
            otemps = temps;
        }
        if (digitalRead(key2) == LOW)
        {
            dayNum++;
            if (dayNum > 2)
                dayNum = 0;
            short bgx = 0, bgfx = -256, nbgx = 255, nbgfx = 0; // 背景坐标
            short tx = 180, tfx = -76, ntx = 435, ntfx = 180;  // 温度信息坐标
            short px = 20, pfx = -235, npx = 275, npfx = 20;   // 天气图标坐标
            String dates, weathers, temps;
            bgNum = random(0, 6);
            getdaydata(dayNum, &weathers, &temps, &dates);
            while (ui_bounce(&bgx, &bgfx, &velocity, step) && ui_bounce(&nbgx, &nbgfx, &velocity, step) && ui_bounce(&tx, &tfx, &velocity, step) && ui_bounce(&ntx, &ntfx, &velocity, step) && ui_bounce(&px, &pfx, &velocity, step) && ui_bounce(&npx, &npfx, &velocity, step))
            {
                u8g2.clearBuffer();
                u8g2.drawXBMP(bgx, 0, 256, 64, nli[bgtem]);
                u8g2.drawXBMP(nbgx, 0, 256, 64, nli[bgNum]);
                u8g2.setFont(u8g2_font_courB18_tr);
                u8g2.setCursor(tx, 20);
                u8g2.print(otemps);
                u8g2.print("'C");
                u8g2.setCursor(ntx, 20);
                u8g2.print(temps);
                u8g2.print("'C");
                u8g2.setFont(u8g2_font_rosencrantz_nbp_t_all);
                u8g2.setCursor(180, 35);
                u8g2.print(future.location);
                draw_weather_icon(oweathers, px, 0, &isdrop);
                draw_weather_icon(weathers, npx, 0, &isdrop);
                u8g2.setFont(dinkie_9);
                u8g2.setCursor(px, 35);
                u8g2.print(odates);
                u8g2.setCursor(npx, 35);
                u8g2.print(dates);
                u8g2.setFont(u8g2_font_tinytim_tr);
                u8g2.setCursor(px, 45);
                u8g2.print(oweathers);
                u8g2.setCursor(npx, 45);
                u8g2.print(weathers);
                u8g2.sendBuffer();
            }
            oweathers = weathers;
            // effect = getWeatherEffect(oweathers);
            bgtem = bgNum;
            odates = dates;
            otemps = temps;
        }
    }
    ESP.restart();
}
void draw_UI2()
{
    u8g2.drawCircle(122, 31, 31);
    // u8g2.drawCircle(122, 31, 27);
    u8g2.setFont(u8g2_font_luRS12_tr);
    u8g2.setCursor(100, 62);
    u8g2.print("01:31");
    u8g2.setFont(dinkie_9);
    u8g2.setCursor(10, 10);
    u8g2.print("Shanghai");
    u8g2.setCursor(5, 28);
    u8g2.print("最近闹铃");
    u8g2.setCursor(200, 58);
    u8g2.print("周一");
    u8g2.drawXBMP(0, 38, 16, 16, naozhong);
    u8g2.drawXBMP(55, 15, 32, 32, yin);
    u8g2.drawXBMP(55, 48, 8, 8, san);
    // u8g2.drawXBMP(151, 48, 8, 8, penzai);
    u8g2.setCursor(183, 20);
    u8g2.print("农历四月初五");
    u8g2.setFont(u8g2_font_smart_patrol_nbp_tf);
    u8g2.setCursor(170, 40);
    u8g2.print("2024/5/12");
    u8g2.setFont(u8g2_font_6x13_tf);
    u8g2.setCursor(16, 50);
    u8g2.print("08:30");
    // u8g2.print("Tuesday");
    u8g2.sendBuffer();
}
/**
 * @brief 给设置界面的图标排序
 * @param option 选项标记
 * @param op_arr 要排序的数组
 */
void arr_sort(short option, short op_arr[])
{
    short k = 0;
    for (int i = option; i < option + 5; i++)
    {
        if (i < 5)
        {
            op_arr[k] = i;
        }
        else
        {
            op_arr[k] = i - 5;
        }
        k++;
    }
}
/**
 * @brief 实现滚动动画
 * @param option 当前功能选项
 * @param mode 真：下移 假：上移
 */
void changeSettingPage()
{
    String set_fun[5] = {"Calibration", "Sleep time", "Brightness", "Offline Weather", "Wake method"};
    String describe[5] = {"-选择手动或联网校准时间", "-设定自动休眠时间", "-设定屏幕亮度", "-离线天气开关", "-设定休眠后唤醒方式"};
    short str_len = set_fun[1].length() * 6;
    short box_len = 0;
    short p1, p2, p3, p4, p5, p6;
    short np0 = (32 + 16) * -1 - 16, np1 = (32 + 16) * 0 + 16, np2 = (32 + 16) * 1 + 16, np3 = (32 + 16) * 2 + 16, np4 = (32 + 16) * 3 + 16, np5 = (32 + 16) * 4 + 16, np6 = (32 + 16) * 5 + 16;
    // 变换坐标的资源
    short x_Nposition[7] = {np0, np1, np2, np3, np4, np5, np6};
    short ix1 = x_Nposition[6], nx1 = x_Nposition[1], ix2 = (32 + 16) * 6 + 16, nx2 = x_Nposition[2], ix3 = (32 + 16) * 7 + 16, nx3 = x_Nposition[3], ix4 = (32 + 16) * 8 + 16, nx4 = x_Nposition[4], ix5 = (32 + 16) * 9 + 16, nx5 = x_Nposition[5];
    short initxli[5] = {ix1, ix2, ix3, ix4, ix5};
    short option = 0;
    short cur_op[5];

    p1 = x_Nposition[1];
    p2 = x_Nposition[2];
    p3 = x_Nposition[3];
    p4 = x_Nposition[4];
    p5 = x_Nposition[5];
    arr_sort(option, cur_op);
    float velocity = 0.0f;
    float step = 0.08f;
    while (!ui_siglerun(&box_len, &str_len, 7, 3) && ui_bounce(&initxli[0], &nx1, &velocity, step) && ui_bounce(&initxli[1], &nx2, &velocity, step) && ui_bounce(&initxli[2], &nx3, &velocity, step) && ui_bounce(&initxli[3], &nx4, &velocity, step) && ui_bounce(&initxli[4], &nx5, &velocity, step))
    {
        u8g2.clearBuffer();
        u8g2.drawXBMP(p1, 2, 32, 33, settingli[cur_op[1]]);
        u8g2.drawXBMP(p2, 2, 32, 33, settingli[cur_op[2]]);
        u8g2.drawXBMP(p3, 2, 32, 33, settingli[cur_op[3]]);
        u8g2.drawXBMP(p4, 2, 32, 33, settingli[cur_op[4]]);
        u8g2.drawXBMP(p5, 2, 32, 33, settingli[cur_op[0]]);
        u8g2.setFont(u8g2_font_6x13_tf);
        u8g2.setCursor(16, 44);
        u8g2.print(set_fun[1]);
        u8g2.drawBox(16, 46, box_len, 3);
        u8g2.setFont(dinkie_9);
        u8g2.setCursor(16, 62);
        u8g2.print(describe[1]);
        u8g2.sendBuffer();
    }
    while (1)
    {
        if (digitalRead(key2) == LOW)
        {
            option++;
            if (option > 4)
                option = 0;
            p1 = x_Nposition[1];
            p2 = x_Nposition[2];
            p3 = x_Nposition[3];
            p4 = x_Nposition[4];
            p5 = x_Nposition[5];
            p6 = x_Nposition[6];
            arr_sort(option, cur_op);
            box_len = 0;
            str_len = set_fun[cur_op[1]].length() * 6;
            while (!ui_siglerun(&box_len, &str_len, 7, 3) && ui_bounce(&p1, &x_Nposition[0], &velocity, step) && ui_bounce(&p2, &x_Nposition[1], &velocity, step) && ui_bounce(&p3, &x_Nposition[2], &velocity, step) && ui_bounce(&p4, &x_Nposition[3], &velocity, step) && ui_bounce(&p5, &x_Nposition[4], &velocity, step) && ui_bounce(&p6, &x_Nposition[5], &velocity, step))
            {
                u8g2.clearBuffer();
                menu_bg_animation(&p1, &p2, &p3, &p4, &p5, &p6, cur_op, set_fun[cur_op[1]], describe[cur_op[1]], &box_len);
                u8g2.sendBuffer();
            }
        }
        if (digitalRead(key1) == LOW)
        {
            option--;
            if (option < 0)
                option = 4;
            p1 = x_Nposition[1];
            p2 = x_Nposition[2];
            p3 = x_Nposition[3];
            p4 = x_Nposition[4];
            p5 = x_Nposition[5];
            p6 = x_Nposition[6];
            arr_sort(option, cur_op);
            box_len = 0;
            // Serial.println(set_fun[cur_op[1]]);
            str_len = set_fun[cur_op[1]].length() * 6;
            while (!ui_siglerun(&box_len, &str_len, 7, 3) && ui_bounce(&p1, &x_Nposition[0], &velocity, step) && ui_bounce(&p2, &x_Nposition[1], &velocity, step) && ui_bounce(&p3, &x_Nposition[2], &velocity, step) && ui_bounce(&p4, &x_Nposition[3], &velocity, step) && ui_bounce(&p5, &x_Nposition[4], &velocity, step) && ui_bounce(&p6, &x_Nposition[5], &velocity, step))
            {
                u8g2.clearBuffer();
                menu_bg_animation(&p1, &p2, &p3, &p4, &p5, &p6, cur_op, set_fun[cur_op[1]], describe[cur_op[1]], &box_len);
                u8g2.sendBuffer();
            }
        }
        if (digitalRead(key3) == LOW)
        {
            p1 = x_Nposition[1];
            p2 = x_Nposition[2];
            p3 = x_Nposition[3];
            p4 = x_Nposition[4];
            p5 = x_Nposition[5];
            arr_sort(option, cur_op);
            // 处理三级菜单并回弹
            get_3rd_dec(option, cur_op, set_fun[cur_op[1]], describe[cur_op[1]], &str_len);
        }
        if (digitalRead(key4) == LOW)
        {
            ESP.restart();
        }
    }
}

// u8g2.setContrast();亮度设定

void disMenu()
{
    Serial.println("进入主菜单");
    // 翻页时钟，天气时钟，闹钟设定，系统设定，固件升级
    const unsigned char *Menuli[5] = {clipClock, weatherClock, alarmClock, sysStting, OTAsetting};
    const unsigned char(*aniArry[5])[512] PROGMEM = {ani_clock, ani_weather, ani_alarm, ani_sys, ani_grade};
    u8g2.setDrawColor(1);
    // Serial.println("刷新第一个菜单功能");
    u8g2.clearBuffer();
    u8g2.setDisplayRotation(U8G2_R0);
    u8g2.drawXBMP(192, 0, 64, 64, aniArry[0][0]);
    u8g2.setDisplayRotation(U8G2_R2);
    u8g2.drawXBMP(64, 0, 192, 64, Menuli[0]);
    u8g2.sendBuffer();
    // Serial.println("进入菜单选择功能");
    short mCount = 0;
    int sleepCount = 0;
    int ani_count = 0;
    while (1)
    {
        delay(10);
        sleepCount++;
        u8g2.clearBuffer();
        u8g2.setDisplayRotation(U8G2_R0);
        u8g2.drawXBMP(192, 0, 64, 64, aniArry[mCount][ani_count]);
        u8g2.setDisplayRotation(U8G2_R2);
        u8g2.drawXBMP(64, 0, 192, 64, Menuli[mCount]);
        u8g2.sendBuffer();
        ani_count++;
        if (ani_count >= 27)
            ani_count = 0;
        if (sleepCount > 6000)
        {
            cliptimeRun();
        }
        if (digitalRead(keyright) == LOW)
        {
            pomodoroAnimate();
            pomodoroSet();
            ESP.restart();
        }
        if (digitalRead(keyleft) == LOW)
        {
            stopwatchAnimate();
            stopwatchStart();
        }
        if (digitalRead(key2) == LOW)
        {
            mCount++;
            if (mCount >= 5)
                mCount = 0;
            changeMenuPage(mCount, 1);
        }
        if (digitalRead(key1) == LOW)
        {
            mCount--;
            if (mCount <= -1)
                mCount = 4;
            changeMenuPage(mCount, 0);
        }
        if (digitalRead(key3) == LOW)
        {
            switch (mCount)
            {
            case 0:
                while (1)
                {
                    u8g2.clearBuffer();
                    u8g2.setDisplayRotation(U8G2_R0);
                    u8g2.drawXBMP(192, 0, 64, 64, aniArry[mCount][ani_count]);
                    u8g2.setDisplayRotation(U8G2_R2);
                    u8g2.drawXBMP(64, 0, 192, 64, Menuli[mCount]);
                    if (ui_disapper() == 0)
                    {
                        u8g2.sendBuffer();
                        break;
                    }
                    u8g2.sendBuffer();
                }
                cliptimeRun();
                break;
            case 1:
                while (1)
                {
                    u8g2.clearBuffer();
                    u8g2.setDisplayRotation(U8G2_R0);
                    u8g2.drawXBMP(192, 0, 64, 64, aniArry[mCount][ani_count]);
                    u8g2.setDisplayRotation(U8G2_R2);
                    u8g2.drawXBMP(64, 0, 192, 64, Menuli[mCount]);
                    if (ui_disapper() == 0)
                    {
                        u8g2.sendBuffer();
                        break;
                    }
                    u8g2.sendBuffer();
                }
                draw_Weather();
                ESP.restart();
                break;
            case 2:
                set_Alarm();
                break;
            case 3:
                while (1)
                {
                    u8g2.clearBuffer();
                    u8g2.setDisplayRotation(U8G2_R0);
                    u8g2.drawXBMP(192, 0, 64, 64, aniArry[mCount][ani_count]);
                    u8g2.setDisplayRotation(U8G2_R2);
                    u8g2.drawXBMP(64, 0, 192, 64, Menuli[mCount]);
                    if (ui_disapper() == 0)
                    {
                        u8g2.sendBuffer();
                        break;
                    }
                    u8g2.sendBuffer();
                }
                changeSettingPage();
                break;
            case 4:
                otafun();
                return;
            default:
                break;
            }
        }
    }
}
void cliptimeRun()
{
    RTC_RX8025T.read(RXtm);
    if (Alarm.a_frequency != 0 || Alarm.a_frequency != 255)
    {
        alarmState = true;
        if (Alarm.a_frequency == 2 && (RXtm.Wday == 1 || RXtm.Wday == 7))
        {
            alarmState = false;
        }
    }
    u8g2.clearBuffer();
    drawTimenow();
    u8g2.sendBuffer();
    int16_t nx, ny, nz;
    short sleepCount = RXtm.Second;
    while (digitalRead(key4) != LOW)
    {
        if (bmp.takeForcedMeasurement())
        {
            u8g2.setDrawColor(1);
            float f = getVariance(bmp.readPressure() / 100);
            if (f > 10 && arrindex > 5 || f >= 6 && arrindex <= 5)
            {
                remindAnimate();
            }
        }

        // 更新时间数据
        RTC_RX8025T.read(RXtm);
        // temperature=bmp.readTemperature();
        temperature = aht20.getTemperature();
        humidity = aht20.getHumidity();
        clipTime(RXtm.Hour, RXtm.Minute);
        if (alarmState && RXtm.Hour == Alarm.a_hour && RXtm.Minute == Alarm.a_min)
        {
            Alarm_ring();
        }
        delay(1000);
        sleepCount++;
        if (sleepCount > sleepTime)
        {
            // Serial.print("休眠时间到，熄屏！唤醒引脚是");
            sleepCount = 0;
            while (1)
            {
                u8g2.clearBuffer();
                drawTimedisapper();
                if (ui_disapper() == 0)
                {
                    u8g2.sendBuffer();
                    break;
                }
                u8g2.sendBuffer();
            }
            // esp_sleep_enable_ext0_wakeup(GPIO_NUM_34, 0);
            if (wakeFlagState)
            {
                Serial.print("已休眠，唤醒方式是按钮");
                esp_sleep_enable_ext0_wakeup(GPIO_NUM_34, 0);
            }
            else
            {
                Serial.print("已休眠，唤醒方式是振动");
                esp_sleep_enable_ext0_wakeup(GPIO_NUM_13, 1);
            }
            delay(1500); // 1 = High, 0 = Low
            if (alarmState)
            {
                u64_t u;
                int ac = Alarm.a_hour * 60 + Alarm.a_min;
                int nc = RXtm.Hour * 60 + RXtm.Minute;
                if (ac < nc)
                {
                    u = 24 * 60 + ac - nc;
                    Serial.println("闹钟因为已过时,改为次日");
                }
                else
                {
                    u = ac - nc;
                }
                Serial.printf("休眠分钟数：%d\n", u);
                u = u * 60 * 1000000;
                esp_sleep_enable_timer_wakeup(u);
                esp_deep_sleep_start();
            }
            Serial.println("没有闹钟，直接熄屏！");
            esp_deep_sleep_start();
        }
        if (digitalRead(keyright) == LOW)
        {
            pomodoroAnimate();
            pomodoroSet();
            ESP.restart();
        }
        if (digitalRead(keyleft) == LOW)
        {
            stopwatchAnimate();
            stopwatchStart();
        }
    }
    // delay(300);
    disMenu();
}

void rainstart()
{

    Raindrop raindrops[NUM_RAINDROPS];
    initRaindrops(raindrops, NUM_RAINDROPS);

    while (true)
    {
        // 更新雨滴位置
        updateRaindrops(raindrops, NUM_RAINDROPS);

        // 清屏
        // OLED_Clear();
        u8g2.clearBuffer();

        // 绘制雨滴

        // drawTimenow();
        u8g2.setDrawColor(1);
        drawRaindrops(raindrops, NUM_RAINDROPS);
        // u8g2.sendBuffer();

        // 延时，控制雨滴速度
        // delay(30);
    }
}

/**
 * @brief 离线天气提醒界面
 */
void remindAnimate()
{
    Raindrop raindrops[NUM_RAINDROPS];
    initRaindrops(raindrops, NUM_RAINDROPS);
    RTC_RX8025T.read(RXtm);
    int sec_old, sec_now;
    sec_old = min(RXtm.Second - 1, 60);
    if (sec_old < 0)
        sec_old = 59;
    while (digitalRead(keyright) != LOW)
    {
        u8g2.clearBuffer();
        u8g2.drawXBMP(0, 0, 256, 64, tixing);
        updateRaindrops(raindrops, NUM_RAINDROPS);
        drawRaindrops(raindrops, NUM_RAINDROPS);
        u8g2.sendBuffer();
        RTC_RX8025T.read(RXtm);
        sec_now = RXtm.Second;
        if (sec_now == sec_old)
            return;
    }
}