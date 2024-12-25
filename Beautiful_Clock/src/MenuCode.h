
void menu_bg_static(short li_order[], String set_str, String des_str, short *box_len);
/**
 * @brief
 * 绘制时针函数
 * @param x0 起始x
 * @param y0 起始y
 * @param len 时针长度
 * @param t 具体时间（时）
 */
void drawHline(int16_t x0, int16_t y0, int16_t len, float t)
{
    float b = 0;   // 三角形的底
    float a = 0;   // 三角形的高
    int16_t angle; // 偏转角度
    int fx = 0, fy = 0;
    if (t > 12)
        t -= 12;
    if (t == 3)
    {
        b = len;
    }
    else if (t == 12 || t == 0)
    {
        a = -1 * len;
    }
    else if (t == 9)
    {
        b = 0 - len;
    }
    else if (t == 6)
    {
        a = len;
    }
    else if (t < 3)
    {
        angle = 90 - t * 30;
        a = len * sin(angle * 3.15159 / 180);
        b = len * cos(angle * 3.15159 / 180);
        a = -1 * a;
    }
    else if (t > 3 && t < 6)
    {
        angle = 90 - (t - 3) * 30;
        b = len * sin(angle * 3.15159 / 180);
        a = len * cos(angle * 3.15159 / 180);
    }
    else if (t > 6 && t < 9)
    {
        angle = 90 - (t - 6) * 30;
        a = len * sin(angle * 3.15159 / 180);
        b = len * cos(angle * 3.15159 / 180);
        b = -1 * b;
    }
    else if (t > 9)
    {
        angle = 90 - (t - 9) * 30;
        b = len * sin(angle * 3.15159 / 180);
        a = len * cos(angle * 3.15159 / 180);
        a = -1 * a;
        b = -1 * b;
    }

    fx = x0 + b; // 目标x坐标
    fy = y0 + a; // 目标y坐标
    u8g2.drawLine(x0, y0, fx, fy);
    // display.drawLine(x0,y0,fx,fy,heise);
    // display.nextPage();
    // Serial.print("测试时间 时:  ");Serial.print(t);Serial.print("   x:  ");Serial.print(fx);Serial.print("   y:  ");Serial.println(fy);
}
/**
 * @brief
 * 绘制分针函数
 * @param x0 起始x
 * @param y0 起始y
 * @param len 分针长度
 * @param t 具体时间（分）
 */
void drawMline(int16_t x0, int16_t y0, int16_t len, int16_t t)
{
    float b = 0;   // 三角形的底
    float a = 0;   // 三角形的高
    int16_t angle; // 偏转角度
    int fx = 0, fy = 0;
    if (t == 15)
    {
        b = len;
    }
    else if (t == 0)
    {
        a = -1 * len;
    }
    else if (t == 45)
    {
        b = -1 * len;
    }
    else if (t == 30)
    {
        a = len;
    }
    else if (t < 15)
    {
        angle = 90 - t * 6;
        a = len * sin(angle * 3.15159 / 180);
        b = len * cos(angle * 3.15159 / 180);
        a = -1 * a;
    }
    else if (t > 15 && t < 30)
    {
        angle = 90 - (t - 15) * 6;
        b = len * sin(angle * 3.15159 / 180);
        a = len * cos(angle * 3.15159 / 180);
    }
    else if (t > 30 && t < 45)
    {
        angle = 90 - (t - 30) * 6;
        a = len * sin(angle * 3.15159 / 180);
        b = len * cos(angle * 3.15159 / 180);
        b = -1 * b;
    }
    else if (t > 45)
    {
        angle = 90 - (t - 45) * 6;
        b = len * sin(angle * 3.15159 / 180);
        a = len * cos(angle * 3.15159 / 180);
        a = -1 * a;
        b = -1 * b;
    }

    fx = x0 + b; // 目标x坐标
    fy = y0 + a; // 目标y坐标
    u8g2.drawLine(x0, y0, fx, fy);
    // display.drawLine(x0,y0,fx,fy,heise);
}
/**
 * @brief 横向或纵向移动，只能是一维的
 * @param y 当前y或x坐标或待变化的长度
 * @param fy 目标y或x坐标或目标长度
 * @param step 移动幅度
 * @param slow_cnt 修正幅度
 */
bool ui_siglerun(short *y, short *fy, uint8_t step, uint8_t slow_cnt)
{

    uint8_t temp;
    temp = abs(*fy - *y) > slow_cnt ? step : 1;
    if (*y < *fy)
    {
        *y += temp;
    }
    else if (*y > *fy)
    {
        *y -= temp;
    }
    else
    {
        return true;
    }
    return false;
}
#define DAMPING_FACTOR 0.85    // 阻尼系数，控制回弹效果0.85
#define POSITION_THRESHOLD 2   // 位置阈值
#define VELOCITY_THRESHOLD 0.2 // 速度阈值

/**
 * @brief 带回弹的动画坐标计算函数
 * @param y 要变化的数据
 * @param fy 目标数据
 * @param velocity 速度记录
 * @param step 速度
 * @return
 */
bool ui_bounce(short *y, short *fy, float *velocity, float step)
{
    // if(abs(*y)-abs(*fy)<=POSITION_THRESHOLD){
    //     return false;
    // }
    // 计算加速度
    float acceleration = (*fy - *y) * step;

    // 更新速度
    *velocity = *velocity * DAMPING_FACTOR + acceleration;

    // 更新位置
    *y += (short)*velocity;

    // 检查是否到达目标位置并且速度足够小
    if (fabs(*fy - *y) < POSITION_THRESHOLD && fabs(*velocity) < VELOCITY_THRESHOLD)
    {
        *y = *fy;      // 将位置修正到最终目标位置
        *velocity = 0; // 将速度重置为0
        return false;
    }

    return true;
}
/**
 * @param x 当前x坐标
 * @param fx 目标x坐标
 * @param y 当前y坐标
 * @param fy 目标y坐标
 * @param stepx x移动幅度
 * @param stepy y移动幅度
 * @param slow_cnt 修正幅度
 */
bool ui_run_plane(short *x, short *fx, short *y, short *fy, uint8_t stepx, uint8_t stepy, uint8_t slow_cnt)
{
    uint8_t tempX, tempY;

    // 计算 X 轴的步幅
    tempX = abs(*fx - *x) > slow_cnt ? stepx : 1;
    if (*x < *fx)
    {
        *x += tempX;
    }
    else if (*x > *fx)
    {
        *x -= tempX;
    }

    // 计算 Y 轴的步幅
    tempY = abs(*fy - *y) > slow_cnt ? stepy : 1;
    if (*y < *fy)
    {
        *y += tempY;
    }
    else if (*y > *fy)
    {
        *y -= tempY;
    }

    // 判断是否仍需移动
    if (*x == *fx && *y == *fy)
    {
        return false; // 移动完成
    }
    return true; // 继续移动
}
const unsigned char *nli[10] = {ling, yi, er, san, si, wu, liu, qi, ba, jiu};
short hsc = 0;
short hgc = 0;
short msc = 0;
short mgc = 0;
/**
 * @brief 更新时间4个数位上的数字
 * @param hourx 小时数据
 * @param minitx 分钟数据
 * @param nhs 小时数据的十位
 * @param nhg 小时数据的各位
 * @param nms 分钟数据的十位
 * @param nmg 分钟数据的个位
 */
void prepairClip(short hourx, short minitx, short *nhs, short *nhg, short *nms, short *nmg)
{
    *nhs = hourx / 10;
    *nhg = hourx % 10;
    *nms = minitx / 10;
    *nmg = minitx % 10;
}
/**
 * @brief 翻页时钟消失动画
 */
void drawTimedisapper()
{
    String weekstr = daysOfTheWeek[RXtm.Wday];
    String datestr = String(tmYearToCalendar(RXtm.Year)) + "-" + monthsNames[RXtm.Month] + "-" + String(RXtm.Day);
    u8g2.setDrawColor(1);
    u8g2.setFont(dinkie_9);
    u8g2.setCursor(32 - (weekstr.length() * 4 / 2), 20);
    u8g2.print(weekstr);
    u8g2.setFont(u8g2_font_tinytim_tr);
    u8g2.setCursor(26 - (datestr.length() * 5 / 2), 10);
    u8g2.print(datestr);
    u8g2.drawXBMP(5, 23, 12, 12, wendu);
    u8g2.setCursor(18, 32);
    u8g2.print(temperature);
    u8g2.print("'C");
    u8g2.drawXBMP(5, 36, 12, 12, shidu);
    u8g2.setCursor(18, 46);
    u8g2.print(humidity);
    u8g2.print("%");
    u8g2.drawXBMP(5, 49, 12, 12, kongqi);
    u8g2.setCursor(18, 57);
    u8g2.print("Good");
    u8g2.drawXBMP(62, 3, 38, 54, nli[hsc]);
    u8g2.drawXBMP(98, 3, 38, 54, nli[hgc]);
    u8g2.drawXBMP(158, 3, 38, 54, nli[msc]);
    u8g2.drawXBMP(198, 3, 38, 54, nli[mgc]);
    u8g2.drawRFrame(54, 2, 95, 58, 5);
    u8g2.drawRFrame(151, 2, 95, 58, 5);
    u8g2.setDrawColor(0);
    u8g2.drawBox(0, 0, 255, 2);
    u8g2.drawBox(0, 60, 255, 4);
}
/**
 * @brief 实现滚动动画
 * @param option 当前功能选项
 * @param mode 真：下移 假：上移
 */
void changeMenuPage(short option, bool mode)
{
    // 翻页时钟，天气时钟，闹钟设定，系统设定，固件升级
    const unsigned char *Menuli[5] = {clipClock, weatherClock, alarmClock, sysStting, OTAsetting};
    const unsigned char(*aniArry[5])[512] PROGMEM = {ani_clock, ani_weather, ani_alarm, ani_sys, ani_grade};

    short y, fy, ny, nfy, nop;
    if (mode)
    {
        y = 0;
        fy = 65;
        ny = -65;
        nfy = 0;
        option--;
        if (option < 0)
            option = 4;
        nop = option + 1;
        if (nop >= 5)
            nop = 0;
    }
    else
    {
        y = 0;
        fy = -65;
        ny = 65;
        nfy = 0;
        option++;
        if (option > 4)
            option = 0;
        nop = option - 1;
        if (nop <= -1)
            nop = 4;
    }
    float velocity = 0.0f;
    float step = 0.08f;
    while (ui_bounce(&y, &fy, &velocity, step) && ui_bounce(&ny, &nfy, &velocity, step))
    {
        u8g2.clearBuffer();
        u8g2.setDisplayRotation(U8G2_R0);
        u8g2.drawXBMP(192, y, 64, 64, aniArry[option][0]);
        u8g2.setDisplayRotation(U8G2_R2);
        u8g2.drawXBMP(64, y, 192, 64, Menuli[option]);
        u8g2.setDisplayRotation(U8G2_R0);
        u8g2.drawXBMP(192, ny, 64, 64, aniArry[nop][0]);
        u8g2.setDisplayRotation(U8G2_R2);
        u8g2.drawXBMP(64, ny, 192, 64, Menuli[nop]);
        u8g2.sendBuffer();
    }
}
/**
 * @brief 绘制三级菜单的黑色背景块
 */
void draw_3rd_Black_Box()
{
    short blackbox_x = 268, nblackbox_x = 190;
    u8g2.setDrawColor(0);
    u8g2.drawBox(nblackbox_x, 0, 90, 64);
    u8g2.setDrawColor(1);
    u8g2.drawBox(nblackbox_x - 2, 0, 2, 64);
}
/**
 * @brief 3级菜单回弹动画
 * @param li_order 排好序的存储好图标顺序的数组
 * @param set_str 当前设置的英文名
 * @param des_str 当前设置的描述
 * @param box_len 装饰长条的长度
 */
void draw_3rd_blackbox_back(short li_order[], String set_str, String des_str, short *box_len)
{
    short blackbox_x = 268, nblackbox_x = 190;
    float velocity = 0.0f;
    while (ui_bounce(&nblackbox_x, &blackbox_x, &velocity, 0.06))
    {
        u8g2.clearBuffer();
        menu_bg_static(li_order, set_str, des_str, box_len);
        u8g2.setDrawColor(0);
        u8g2.drawBox(nblackbox_x, 0, 90, 64);
        u8g2.setDrawColor(1);
        u8g2.drawBox(nblackbox_x, 0, 2, 64);
        u8g2.setCursor(nblackbox_x + 5, 20);
        u8g2.sendBuffer();
    }
}
/**
 * @brief 设定亮度功能
 * @param li_order 排好序的存储好图标顺序的数组
 * @param set_str 当前设置的英文名
 * @param des_str 当前设置的描述
 * @param box_len 装饰长条的长度
 */
void draw_Brightness_Setting(short li_order[], String set_str, String des_str, short *box_len)
{
    //  u8g2.setContrast();
    int brightness = EEPROM.read(1);
    u8g2.clearBuffer();
    menu_bg_static(li_order, set_str, des_str, box_len);
    draw_3rd_Black_Box();
    u8g2.drawXBMP(190, 0, 64, 64, liangdu3rd);
    u8g2.setFont(u8g2_font_6x13_tf);
    u8g2.setCursor(215, 50);
    u8g2.print(brightness);
    u8g2.sendBuffer();
    while (1)
    {
        if (digitalRead(key2) == LOW)
        {

            brightness++;
            if (brightness > 255)
                brightness = 255;
            u8g2.setContrast(brightness);
            u8g2.clearBuffer();
            menu_bg_static(li_order, set_str, des_str, box_len);
            draw_3rd_Black_Box();
            u8g2.drawXBMP(190, 0, 64, 64, liangdu3rd);
            u8g2.setFont(u8g2_font_6x13_tf);
            u8g2.setCursor(215, 50);
            u8g2.print(brightness);
            u8g2.sendBuffer();
        }
        if (digitalRead(key1) == LOW)
        {
            brightness--;
            if (brightness < 0)
                brightness = 0;
            u8g2.setContrast(brightness);
            u8g2.clearBuffer();
            menu_bg_static(li_order, set_str, des_str, box_len);
            draw_3rd_Black_Box();
            u8g2.drawXBMP(190, 0, 64, 64, liangdu3rd);
            u8g2.setFont(u8g2_font_6x13_tf);
            u8g2.setCursor(215, 50);
            u8g2.print(brightness);
            u8g2.sendBuffer();
        }
        if (digitalRead(key3) == LOW)
        {
            EEPROM.write(1, brightness);
            EEPROM.commit();
            Serial.printf("存储的亮度：%d\n", brightness);
            return;
        }
        if (digitalRead(key4) == LOW)
        {
            return;
        }
    }
}
/**
 * @brief 绘制时间设定菜单选项
 * @param li_order 排好序的存储好图标顺序的数组
 * @param set_str 当前设置的英文名
 * @param des_str 当前设置的描述
 * @param box_len 装饰长条的长度
 */
void draw_Time_Calibration(short li_order[], String set_str, String des_str, short *box_len)
{
    // 时间校准数组，自动/手动
    const unsigned char *c_type[2] = {zidong, shoudong};
    u8g2.clearBuffer();
    menu_bg_static(li_order, set_str, des_str, box_len);
    draw_3rd_Black_Box();
    u8g2.drawXBMP(190, 0, 64, 64, c_type[0]);
    u8g2.sendBuffer();
    short c = 0, op = 0;
    short ny, ny2, oy, oy2;
    short flag = 0;
    while (1)
    {
        if (digitalRead(key2) == LOW)
        {
            c++;
            if (c > 1)
            {
                c = 0;
                op = 1;
            }
            else
            {
                op = c - 1;
            }
            ny = -64;
            ny2 = 0;
            oy = 0;
            oy2 = 64;
            while (!ui_siglerun(&oy, &oy2, 7, 3) || !ui_siglerun(&ny, &ny2, 7, 3))
            {
                u8g2.clearBuffer();
                menu_bg_static(li_order, set_str, des_str, box_len);
                draw_3rd_Black_Box();
                u8g2.drawXBMP(190, ny, 64, 64, c_type[c]);
                u8g2.drawXBMP(190, oy, 64, 64, c_type[op]);
                u8g2.sendBuffer();
            }
            flag = c;
        }
        if (digitalRead(key1) == LOW)
        {
            c--;
            ny = 0, ny2 = -64, oy = 64, oy2 = 0;
            if (c < 0)
            {
                c = 1;
                op = 0;
            }
            else
            {
                op = c + 1;
            }
            while (!ui_siglerun(&ny, &ny2, 7, 3) || !ui_siglerun(&oy, &oy2, 7, 3))
            {
                u8g2.clearBuffer();
                menu_bg_static(li_order, set_str, des_str, box_len);
                draw_3rd_Black_Box();
                u8g2.drawXBMP(190, ny, 64, 64, c_type[op]);
                u8g2.drawXBMP(190, oy, 64, 64, c_type[c]);
                u8g2.sendBuffer();
            }
            flag = c;
        }
        if (digitalRead(key3) == LOW)
        {
            // Serial.printf("确认操作,op的值是:%d\n", op);
            if (flag == 0)
            {
                if (getNetTime())
                {
                    for (int i = 3; i >= 0; i--)
                    {
                        u8g2.clearBuffer();
                        u8g2.setDrawColor(1);
                        u8g2.drawXBMP(103, 2, 50, 50, gou);
                        u8g2.setCursor(5, 58);
                        u8g2.print("自动校准成功！ ");
                        u8g2.print(i);
                        u8g2.print(" 秒后返回上一步");
                        u8g2.sendBuffer();
                        delay(1000);
                    }
                }
                else
                {
                    for (int i = 3; i >= 0; i--)
                    {
                        u8g2.clearBuffer();
                        u8g2.setDrawColor(1);
                        u8g2.drawXBMP(103, 2, 50, 50, gou);
                        u8g2.setCursor(5, 58);
                        u8g2.print("自动校准失败，请重试！ ");
                        u8g2.print(i);
                        u8g2.print(" 秒后返回上一步");
                        u8g2.sendBuffer();
                        delay(1000);
                    }
                }
            }
            else
            {

                ManualsetRTC();
            }

            return;
        }
        if (digitalRead(key4) == LOW)
        {
            return;
        }
    }
}
void wakeTypeset(short li_order[], String set_str, String des_str, short *box_len)
{
    // 时间校准数组，自动/手动
    const unsigned char *c_type[2] = {zhendong,anniu};
    u8g2.clearBuffer();
    menu_bg_static(li_order, set_str, des_str, box_len);
    draw_3rd_Black_Box();
    if (EEPROM.read(7) != 1)
    {
        u8g2.drawXBMP(190, -1, 64, 64, zhendong);
    }
    else
    {
        u8g2.drawXBMP(190, 0, 64, 64, anniu);
    }
    u8g2.sendBuffer();
    short c = EEPROM.read(7), op = 0;
    short ny, ny2, oy, oy2;
    while (1)
    {
        if (digitalRead(key2) == LOW)
        {
            c++;
            if (c > 1)
            {
                c = 0;
                op = 1;
            }
            else
            {
                op = c - 1;
            }
            ny = -64;
            ny2 = 0;
            oy = 0;
            oy2 = 64;
            while (!ui_siglerun(&oy, &oy2, 7, 3) || !ui_siglerun(&ny, &ny2, 7, 3))
            {
                u8g2.clearBuffer();
                menu_bg_static(li_order, set_str, des_str, box_len);
                draw_3rd_Black_Box();
                u8g2.drawXBMP(190, ny, 64, 64, c_type[c]);
                u8g2.drawXBMP(190, oy, 64, 64, c_type[op]);
                u8g2.sendBuffer();
            }
        }
        if (digitalRead(key1) == LOW)
        {
            c--;
            ny = 0, ny2 = -64, oy = 64, oy2 = 0;
            if (c < 0)
            {
                c = 1;
                op = 0;
            }
            else
            {
                op = c + 1;
            }
            while (!ui_siglerun(&ny, &ny2, 7, 3) || !ui_siglerun(&oy, &oy2, 7, 3))
            {
                u8g2.clearBuffer();
                menu_bg_static(li_order, set_str, des_str, box_len);
                draw_3rd_Black_Box();
                u8g2.drawXBMP(190, ny, 64, 64, c_type[op]);
                u8g2.drawXBMP(190, oy, 64, 64, c_type[c]);
                u8g2.sendBuffer();
            }
        }
        if (digitalRead(key3) == LOW)
        {
            Serial.print("写入7的标记是");Serial.println(c);
            EEPROM.write(7, c);
            if (EEPROM.commit())
            {
                return;
            }
        }
    }
    if (digitalRead(key4) == LOW)
    {
        return;
    }
}
// void offlineWeather_switch(short li_order[], String set_str, String des_str, short *box_len)
// {
//     short blackbox_x = 190;
//     short flag = EEPROM.read(6);

//     u8g2.clearBuffer();
//     menu_bg_static(li_order, set_str, des_str, box_len);
//     draw_3rd_Black_Box();
//     short c = 0, op = 0;

//     if (flag != 1)
//     {
//         u8g2.drawXBMP(blackbox_x + 16, -1, 32, 33, guan);
//     }
//     else
//     {
//         u8g2.drawXBMP(blackbox_x + 16, 0, 32, 33, kai);
//     }
//     u8g2.setFont(dinkie_9);
//     u8g2.setCursor(blackbox_x + 16, 50);
//     if (flag != 1)
//     {
//         u8g2.print("已关闭");
//     }
//     else
//     {
//         u8g2.print("已开启");
//     }
//     u8g2.sendBuffer();
//     short count = 0;
//     if(flag==1)count=1;
//     while (1)
//     {
//         if (digitalRead(key1) == LOW)
//         {
//             delay(200);
//             count--;
//             if (count < 0)
//                 count = 1;
//             u8g2.clearBuffer();
//             menu_bg_static(li_order, set_str, des_str, box_len);
//             draw_3rd_Black_Box();
//             u8g2.setCursor(blackbox_x + 16, 50);
//             count == 1 ? u8g2.drawXBMP(blackbox_x + 16, 0, 32, 33, kai) : u8g2.drawXBMP(blackbox_x + 16, -1, 32, 33, guan);
//             count == 1 ? u8g2.print("已打开") : u8g2.print("已关闭");;
//             u8g2.sendBuffer();
//         }
//         if (digitalRead(key2) == LOW)
//         {
//             delay(200);
//             count++;
//             if (count > 1)
//                 count = 0;
//             u8g2.clearBuffer();
//             menu_bg_static(li_order, set_str, des_str, box_len);
//             draw_3rd_Black_Box();
//             u8g2.setCursor(blackbox_x + 16, 50);
//             count == 1 ? u8g2.drawXBMP(blackbox_x + 16, 0, 32, 33, kai) : u8g2.drawXBMP(blackbox_x + 16, -1, 32, 33, guan);
//             count == 1 ? u8g2.print("已打开") : u8g2.print("已关闭");;
//             u8g2.sendBuffer();
//         }
//         if (digitalRead(key3) == LOW){
//             delay(200);
//             EEPROM.write(6,count);
//             if(EEPROM.commit()){
//                 return;
//             }
//         }
//         if (digitalRead(key4) == LOW)return;
//     }
// }
/**
 * @brief 处理三级菜单事件函数
 * @param option 菜单标记
 * @param li_order 排好序的存储好图标顺序的数组
 * @param set_str 当前设置的英文名
 * @param des_str 当前设置的描述
 * @param box_len 装饰长条的长度
 */
void get_3rd_dec(short option, short li_order[], String set_str, String des_str, short *box_len)
{
    float velocity = 0.0f;
    // 弹出三级菜单
    short blackbox_x = 268, nblackbox_x = 190;
    while (ui_bounce(&blackbox_x, &nblackbox_x, &velocity, 0.06))
    {
        u8g2.clearBuffer();
        menu_bg_static(li_order, set_str, des_str, box_len);
        u8g2.setDrawColor(0);
        u8g2.drawBox(blackbox_x, 0, 90, 64);
        u8g2.setDrawColor(1);
        u8g2.drawBox(blackbox_x - 2, 0, 2, 64);
        u8g2.setCursor(blackbox_x + 5, 20);
        switch (option)
        {
        case 0:
            setsleeptime();
            // u8g2.print("休眠设定");
            // u8g2.setCursor(blackbox_x + 5, 30);
            // u8g2.print("-待完成-");
            break;
        case 1:
            u8g2.drawXBMP(blackbox_x, 0, 64, 64, liangdu3rd);
            // u8g2.print("亮度设置");
            u8g2.setFont(u8g2_font_6x13_tf);
            u8g2.setCursor(blackbox_x + 25, 50);
            u8g2.print(EEPROM.read(1));
            break;
        case 2:
            u8g2.print("离线开关");
            u8g2.setCursor(blackbox_x + 5, 30);
            u8g2.print("-待完成-");
            break;
            // if (EEPROM.read(6) != 1)
            // {
            //     u8g2.drawXBMP(blackbox_x + 16, -1, 32, 33, guan);
            // }
            // else
            // {
            //     u8g2.drawXBMP(blackbox_x + 16, 0, 32, 33, kai);
            // }
            // u8g2.setFont(dinkie_9);
            // u8g2.setCursor(blackbox_x + 16, 50);
            // if (EEPROM.read(6) != 1)
            // {
            //     u8g2.print("已关闭");
            // }
            // else
            // {
            //     u8g2.print("已开启");
            // }
            // break;
        case 3:
            if (EEPROM.read(7) != 1)
            {
                u8g2.drawXBMP(blackbox_x, -1, 64, 64, zhendong);
            }
            else
            {
                u8g2.drawXBMP(blackbox_x, 0, 64, 64, anniu);
            }
            // u8g2.print("唤醒设定");
            // u8g2.setCursor(blackbox_x + 5, 30);
            // u8g2.print("-待完成-");
            break;
        case 4:
            u8g2.drawXBMP(blackbox_x, 0, 64, 64, zidong);
            break;
        default:
            break;
        }
        u8g2.sendBuffer();
    }
    switch (option)
    {
    case 0:
        return;
    case 1:
        draw_Brightness_Setting(li_order, set_str, des_str, box_len);
        draw_3rd_blackbox_back(li_order, set_str, des_str, box_len);
        return;
    // case 2:
    //     offlineWeather_switch(li_order, set_str, des_str, box_len);
    //     draw_3rd_blackbox_back(li_order, set_str, des_str, box_len);
    //     return;
    case 3:
        wakeTypeset(li_order, set_str, des_str, box_len);
        draw_3rd_blackbox_back(li_order, set_str, des_str, box_len);
        return;
    case 4:
        draw_Time_Calibration(li_order, set_str, des_str, box_len);
        draw_3rd_blackbox_back(li_order, set_str, des_str, box_len);
        return;
    default:
        delay(1000);
        draw_3rd_blackbox_back(li_order, set_str, des_str, box_len);
        return;
    }
}
// 校准，休眠，亮度，氛围灯，唤醒
const unsigned char *settingli[5] = {jiaozhun, xiumian, liangdu, lixiantianqi, huanxing};
/**
 * @brief 根据参数绘制设置界面
 * @param *p1~*p6 图标移动动态坐标
 * @param li_order 排好序的存储好图标顺序的数组
 * @param set_str 当前设置的英文名
 * @param des_str 当前设置的描述
 * @param str_len 装饰长条的长度
 */
void menu_bg_animation(short *p1, short *p2, short *p3, short *p4, short *p5, short *p6, short li_order[], String set_str, String des_str, short *box_len)
{
    u8g2.drawXBMP(*p1, 2, 32, 33, settingli[li_order[0]]);
    u8g2.drawXBMP(*p2, 2, 32, 33, settingli[li_order[1]]);
    u8g2.drawXBMP(*p3, 2, 32, 33, settingli[li_order[2]]);
    u8g2.drawXBMP(*p4, 2, 32, 33, settingli[li_order[3]]);
    u8g2.drawXBMP(*p5, 2, 32, 33, settingli[li_order[4]]);
    u8g2.drawXBMP(*p6, 2, 32, 33, settingli[li_order[0]]);
    u8g2.setFont(u8g2_font_6x13_tf);
    u8g2.setCursor(16, 44);
    u8g2.print(set_str);
    u8g2.drawBox(16, 46, *box_len, 3);
    u8g2.setFont(dinkie_9);
    u8g2.setCursor(16, 62);
    u8g2.print(des_str);
}
/**
 * @brief 根据参数绘制设置界面
 * @param li_order 排好序的存储好图标顺序的数组
 * @param set_str 当前设置的英文名
 * @param des_str 当前设置的描述
 * @param box_len 装饰长条的长度
 */
void menu_bg_static(short li_order[], String set_str, String des_str, short *box_len)
{
    // Serial.printf("切换中,当前option：%d",option);
    u8g2.drawXBMP(16, 2, 32, 33, settingli[li_order[1]]);
    u8g2.drawXBMP(64, 2, 32, 33, settingli[li_order[2]]);
    u8g2.drawXBMP(112, 2, 32, 33, settingli[li_order[3]]);
    u8g2.drawXBMP(160, 2, 32, 33, settingli[li_order[4]]);
    u8g2.drawXBMP(208, 2, 32, 33, settingli[li_order[0]]);
    u8g2.setFont(u8g2_font_6x13_tf);
    u8g2.setCursor(16, 44);
    u8g2.print(set_str);
    u8g2.drawBox(16, 46, *box_len, 3);
    u8g2.setFont(dinkie_9);
    u8g2.setCursor(16, 62);
    u8g2.print(des_str);
}
/**
 * @brief 时钟翻页函数
 * @param hourx 实时小时数据
 * @param minitx 实时分钟数据
 */
void clipTime(short hourx, short minitx)
{
    short mfy = 60, my = 3;
    short nmy = -54, umy = 3;
    short a, b, c, d;
    String weekstr = daysOfTheWeek[RXtm.Wday];
    String datestr = String(tmYearToCalendar(RXtm.Year)) + "-" + monthsNames[RXtm.Month] + "-" + String(RXtm.Day);
    prepairClip(hourx, minitx, &a, &b, &c, &d);
    if (d != mgc)
    {
        // Serial.println("切换分钟个位");
        while (!ui_siglerun(&my, &mfy, 3, 2) || !ui_siglerun(&nmy, &umy, 3, 2))
        {
            u8g2.clearBuffer();
            u8g2.setDrawColor(1);
            u8g2.setFont(dinkie_9);
            u8g2.setCursor(32 - (weekstr.length() * 4 / 2), 20);
            u8g2.print(weekstr);
            u8g2.setFont(u8g2_font_tinytim_tr);
            u8g2.setCursor(26 - (datestr.length() * 5 / 2), 10);
            u8g2.print(datestr);
            u8g2.drawXBMP(5, 23, 12, 12, wendu);
            u8g2.setCursor(18, 32);
            u8g2.print(temperature);
            u8g2.print("'C");
            u8g2.drawXBMP(5, 36, 12, 12, shidu);
            u8g2.setCursor(18, 46);
            u8g2.print(humidity);
            u8g2.print("%");
            u8g2.drawXBMP(5, 49, 12, 12, kongqi);
            u8g2.setCursor(18, 57);
            u8g2.print("Good");
            u8g2.drawXBMP(62, 3, 38, 54, nli[hsc]);
            u8g2.drawXBMP(98, 3, 38, 54, nli[hgc]);
            u8g2.drawXBMP(158, 3, 38, 54, nli[msc]);
            u8g2.drawXBMP(198, my, 38, 54, nli[mgc]);
            u8g2.drawXBMP(198, nmy, 38, 54, nli[d]);
            u8g2.drawRFrame(54, 2, 95, 58, 5);
            u8g2.drawRFrame(151, 2, 95, 58, 5);
            u8g2.setDrawColor(0);
            u8g2.drawBox(0, 0, 255, 2);
            u8g2.drawBox(0, 60, 255, 4);
            u8g2.sendBuffer();
        }
        mgc = d;
        mfy = 60;
        my = 3;
        nmy = -54;
        umy = 3;
    }

    if (c != msc)
    {
        // Serial.println("切换分钟十位");
        while (!ui_siglerun(&my, &mfy, 3, 2) || !ui_siglerun(&nmy, &umy, 3, 2))
        {
            u8g2.clearBuffer();
            u8g2.setDrawColor(1);
            u8g2.setCursor(26 - (datestr.length() * 5 / 2), 10);
            u8g2.print(datestr);
            u8g2.setCursor(32 - (weekstr.length() * 4 / 2), 20);
            u8g2.print(weekstr);
            u8g2.drawXBMP(5, 23, 12, 12, wendu);
            u8g2.setCursor(18, 32);
            u8g2.print(temperature);
            u8g2.print("'C");
            u8g2.drawXBMP(5, 36, 12, 12, shidu);
            u8g2.setCursor(18, 46);
            u8g2.print(humidity);
            u8g2.print("%");
            u8g2.drawXBMP(5, 49, 12, 12, kongqi);
            u8g2.setCursor(18, 57);
            u8g2.print("Good");
            u8g2.drawXBMP(62, 3, 38, 54, nli[hsc]);
            u8g2.drawXBMP(98, 3, 38, 54, nli[hgc]);
            u8g2.drawXBMP(158, my, 38, 54, nli[msc]);
            u8g2.drawXBMP(158, nmy, 38, 54, nli[c]);
            u8g2.drawXBMP(198, 3, 38, 54, nli[mgc]);
            u8g2.drawRFrame(54, 2, 95, 58, 5);
            u8g2.drawRFrame(151, 2, 95, 58, 5);
            u8g2.setDrawColor(0);
            u8g2.drawBox(0, 0, 255, 2);
            u8g2.drawBox(0, 60, 255, 4);
            u8g2.sendBuffer();
        }
        msc = c;
        mfy = 60;
        my = 3;
        nmy = -54;
        umy = 3;
    }

    if (b != hgc)
    {
        // Serial.println("切换分钟十位");
        while (!ui_siglerun(&my, &mfy, 3, 2) || !ui_siglerun(&nmy, &umy, 3, 2))
        {
            u8g2.clearBuffer();
            u8g2.setDrawColor(1);
            u8g2.setFont(dinkie_9);
            u8g2.setCursor(32 - (weekstr.length() * 4 / 2), 20);
            u8g2.print(weekstr);
            u8g2.setFont(u8g2_font_tinytim_tr);
            u8g2.setCursor(26 - (datestr.length() * 5 / 2), 10);
            u8g2.print(datestr);
            u8g2.drawXBMP(5, 23, 12, 12, wendu);
            u8g2.setCursor(18, 32);
            u8g2.print(temperature);
            u8g2.print("'C");
            u8g2.drawXBMP(5, 36, 12, 12, shidu);
            u8g2.setCursor(18, 46);
            u8g2.print(humidity);
            u8g2.print("%");
            u8g2.drawXBMP(5, 49, 12, 12, kongqi);
            u8g2.setCursor(18, 57);
            u8g2.print("Good");
            u8g2.drawXBMP(62, 3, 38, 54, nli[hsc]);
            u8g2.drawXBMP(98, my, 38, 54, nli[hgc]);
            u8g2.drawXBMP(98, nmy, 38, 54, nli[b]);
            u8g2.drawXBMP(158, 3, 38, 54, nli[msc]);
            u8g2.drawXBMP(198, 3, 38, 54, nli[mgc]);
            u8g2.drawRFrame(54, 2, 95, 58, 5);
            u8g2.drawRFrame(151, 2, 95, 58, 5);
            u8g2.setDrawColor(0);
            u8g2.drawBox(0, 0, 255, 2);
            u8g2.drawBox(0, 60, 255, 4);
            u8g2.sendBuffer();
        }
        hgc = b;
        mfy = 60;
        my = 3;
        nmy = -54;
        umy = 3;
    }

    if (a != hsc)
    {
        while (!ui_siglerun(&my, &mfy, 3, 2) || !ui_siglerun(&nmy, &umy, 3, 2))
        {
            u8g2.clearBuffer();
            u8g2.setDrawColor(1);
            u8g2.setFont(dinkie_9);
            u8g2.setCursor(32 - (weekstr.length() * 4 / 2), 20);
            u8g2.print(weekstr);
            u8g2.setFont(u8g2_font_tinytim_tr);
            u8g2.setCursor(26 - (datestr.length() * 5 / 2), 10);
            u8g2.print(datestr);
            u8g2.drawXBMP(5, 23, 12, 12, wendu);
            u8g2.setCursor(18, 32);
            u8g2.print(temperature);
            u8g2.print("'C");
            u8g2.drawXBMP(5, 36, 12, 12, shidu);
            u8g2.setCursor(18, 46);
            u8g2.print(humidity);
            u8g2.print("%");
            u8g2.drawXBMP(5, 49, 12, 12, kongqi);
            u8g2.setCursor(18, 57);
            u8g2.print("Good");
            u8g2.drawXBMP(62, my, 38, 54, nli[hsc]);
            u8g2.drawXBMP(62, nmy, 38, 54, nli[a]);
            u8g2.drawXBMP(98, 3, 38, 54, nli[hgc]);
            u8g2.drawXBMP(158, 3, 38, 54, nli[msc]);
            u8g2.drawXBMP(198, 3, 38, 54, nli[mgc]);
            u8g2.drawRFrame(54, 2, 95, 58, 5);
            u8g2.drawRFrame(151, 2, 95, 58, 5);
            u8g2.setDrawColor(0);
            u8g2.drawBox(0, 0, 255, 2);
            u8g2.drawBox(0, 60, 255, 4);
            u8g2.sendBuffer();
        }
        hsc = a;
        mfy = 60;
        my = 3;
        nmy = -54;
        umy = 3;
    }
}

short disapper_temp = 0;
/**
 * @brief UI消失动画
 * @return 消失后返回假，消失中返回真
 */
uint8_t ui_disapper()
{
    // 可优化
    int len = 8 * u8g2.getBufferTileHeight() * u8g2.getBufferTileWidth();
    uint8_t *p = u8g2.getBufferPtr();
    uint8_t return_flag = 0;
    for (int i = 0; i < len; i++)
    {
        p[i] = p[i] & (rand() % 0xff) >> disapper_temp;
    }
    disapper_temp++;
    if (disapper_temp > 8)
    {
        return_flag = 1;
        disapper_temp = 0;
    }
    return return_flag ? 0 : 1;
}
/**
 * @brief 绘制当前时间
 */
void drawTimenow()
{
    // temperature = bmp.readTemperature();
    temperature = aht20.getTemperature();
    humidity = aht20.getHumidity();
    RTC_RX8025T.read(RXtm);
    String weekstr = daysOfTheWeek[RXtm.Wday];
    String datestr = String(tmYearToCalendar(RXtm.Year)) + "-" + monthsNames[RXtm.Month] + "-" + String(RXtm.Day);
    hsc = RXtm.Hour / 10;
    hgc = RXtm.Hour % 10;
    msc = RXtm.Minute / 10;
    mgc = RXtm.Minute % 10;
    u8g2.setDrawColor(1);
    u8g2.setFont(dinkie_9);
    u8g2.setCursor(32 - (weekstr.length() * 4 / 2), 20);
    u8g2.print(weekstr);
    u8g2.setFont(u8g2_font_tinytim_tr);
    u8g2.setCursor(26 - (datestr.length() * 5 / 2), 10);
    u8g2.print(datestr);
    u8g2.drawXBMP(5, 23, 12, 12, wendu);
    u8g2.setCursor(18, 32);
    u8g2.print(temperature);
    u8g2.print("'C");
    u8g2.drawXBMP(5, 36, 12, 12, shidu);
    u8g2.setCursor(18, 46);
    u8g2.print(humidity);
    u8g2.print("%");
    u8g2.drawXBMP(5, 49, 12, 12, kongqi);
    u8g2.setCursor(18, 57);
    u8g2.print("Good");
    u8g2.drawXBMP(62, 3, 38, 54, nli[hsc]);
    u8g2.drawXBMP(98, 3, 38, 54, nli[hgc]);
    u8g2.drawXBMP(158, 3, 38, 54, nli[msc]);
    u8g2.drawXBMP(198, 3, 38, 54, nli[mgc]);
    u8g2.drawRFrame(54, 2, 95, 58, 5);
    u8g2.drawRFrame(151, 2, 95, 58, 5);
    u8g2.setDrawColor(0);
    u8g2.drawBox(0, 0, 255, 2);
    u8g2.drawBox(0, 60, 255, 4);
}

// 雨滴测试
#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 64
#define NUM_RAINDROPS 20
#define MAX_VELOCITY 3
#define MIN_VELOCITY 1
// 雨滴结构体
typedef struct
{
    int16_t x;
    int16_t y;
    float velocity;
} Raindrop;
// 初始化雨滴
void initRaindrops(Raindrop raindrops[], int numRaindrops)
{
    for (int i = 0; i < numRaindrops; i++)
    {
        raindrops[i].x = rand() % SCREEN_WIDTH;
        raindrops[i].y = rand() % SCREEN_HEIGHT;
        raindrops[i].velocity = (rand() % (MAX_VELOCITY - MIN_VELOCITY + 1)) + MIN_VELOCITY;
    }
}

// 更新雨滴位置
void updateRaindrops(Raindrop raindrops[], int numRaindrops)
{
    for (int i = 0; i < numRaindrops; i++)
    {
        raindrops[i].y += raindrops[i].velocity;
        if (raindrops[i].y >= SCREEN_HEIGHT)
        {
            raindrops[i].x = rand() % SCREEN_WIDTH;
            raindrops[i].y = 0;
            raindrops[i].velocity = (rand() % (MAX_VELOCITY - MIN_VELOCITY + 1)) + MIN_VELOCITY;
        }
    }
}

// 绘制雨滴
void drawRaindrops(Raindrop raindrops[], int numRaindrops)
{
    for (int i = 0; i < numRaindrops; i++)
    {
        // 在OLED屏幕上绘制雨滴
        u8g2.drawCircle(raindrops[i].x, raindrops[i].y, random(1, 2));
    }
}
