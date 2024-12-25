/**
 * @brief
 * 番茄钟画饼函数
 * @param x0 圆心x
 * @param y0 圆心y
 * @param len 半径
 * @param t 时间
 */
void drawCake(int16_t x0, int16_t y0, int16_t len, int16_t t)
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
        a = len * sin(angle * 3.14159 / 180);
        b = len * cos(angle * 3.14159 / 180);
        a = -1 * a;
    }
    else if (t > 15 && t < 30)
    {
        angle = 90 - (t - 15) * 6;
        b = len * sin(angle * 3.14159 / 180);
        a = len * cos(angle * 3.14159 / 180);
    }
    else if (t > 30 && t < 45)
    {
        angle = 90 - (t - 30) * 6;
        a = len * sin(angle * 3.14159 / 180);
        b = len * cos(angle * 3.14159 / 180);
        b = -1 * b;
    }
    else if (t > 45)
    {
        angle = 90 - (t - 45) * 6;
        b = len * sin(angle * 3.14159 / 180);
        a = len * cos(angle * 3.14159 / 180);
        a = -1 * a;
        b = -1 * b;
    }

    fx = x0 + b; // 目标x坐标
    fy = y0 + a; // 目标y坐标
    u8g2.drawLine(x0, y0, fx, fy);
}
/**
 * @brief 根据圆弧获取圆心的函数
 * @param startX 圆弧的起始x
 * @param startY 圆弧的起始y
 * @param endX 圆弧的结束x
 * @param endY 圆弧的结束y
 * @param radius 圆弧的半径
 * @param clockwise 方向
 * @param cx 计算出的x坐标
 * @param cy 计算出的y坐标
 *
 */
void drawArc(int startX, int startY, int endX, int endY, int radius, bool clockwise, int *cx, int *cy)
{
    // 起点到终点的向量
    float dx = endX - startX;
    float dy = endY - startY;

    // 起点到终点的距离
    float distance = sqrt(dx * dx + dy * dy);

    // 检查是否可以构成圆
    if (distance > 2 * radius)
    {
        Serial.println("无法绘制圆弧：半径过小！");
        return;
    }

    // 垂直平分线的中点
    float midX = (startX + endX) / 2.0;
    float midY = (startY + endY) / 2.0;

    // 圆心到中点的偏移量
    float centerOffset = sqrt(radius * radius - (distance / 2) * (distance / 2));

    // 垂直平分线的单位向量
    float perpendicularX = -dy / distance;
    float perpendicularY = dx / distance;

    // 计算两个可能的圆心
    float centerX1 = midX + centerOffset * perpendicularX;
    float centerY1 = midY + centerOffset * perpendicularY;

    float centerX2 = midX - centerOffset * perpendicularX;
    float centerY2 = midY - centerOffset * perpendicularY;

    // 选择圆心
    float centerX, centerY;
    if (clockwise)
    {
        centerX = centerX1;
        centerY = centerY1;
    }
    else
    {
        centerX = centerX2;
        centerY = centerY2;
    }
    *cx = (int)centerX;
    *cy = (int)centerY;
    //   // 起点和终点的角度
    //   float startAngle = atan2(startY - centerY, startX - centerX);
    //   float endAngle = atan2(endY - centerY, endX - centerX);

    //   // 顺时针调整角度
    //   if (clockwise && startAngle < endAngle) {
    //     startAngle += 2 * PI;
    //   }
    //   if (!clockwise && startAngle > endAngle) {
    //     endAngle -= 2 * PI;
    //   }

    //   // 打印调试信息
    //   Serial.print("CenterX: ");
    //   Serial.println(centerX);
    //   Serial.print("CenterY: ");
    //   Serial.println(centerY);
    //   Serial.print("StartAngle: ");
    //   Serial.println(startAngle);
    //   Serial.print("EndAngle: ");
    //   Serial.println(endAngle);

    //   // 绘制圆弧
    //   for (float angle = startAngle; clockwise ? (angle >= endAngle) : (angle <= endAngle);
    //        angle += (clockwise ? -0.01 : 0.01)) {
    //     int x = round(centerX + radius * cos(angle));
    //     int y = round(centerY + radius * sin(angle));

    //     // 绘制像素点
    //     u8g2.drawPixel(x, y);
    //   }
}
/**
 * @brief 发出移动的线段
 * @param x0 圆心x
 * @param y0 圆心y
 * @param angle 角度
 * @param maxLen 长度
 */
void animateLine(int16_t x0, int16_t y0, int16_t angle, int16_t maxLen)
{
    int16_t currentLen = 0;            // 当前长度
    float radian = angle * M_PI / 180; // 角度转换为弧度
    int16_t x1, y1;

    while (currentLen <= maxLen)
    {
        // 计算目标点
        x1 = x0 + currentLen * cos(radian);
        y1 = y0 + currentLen * sin(radian);

        // 清屏（根据图形库，可能是清除或覆盖）
        u8g2.clearBuffer();

        // 绘制线段
        u8g2.drawLine(x0, y0, x1, y1);

        // 刷新屏幕
        u8g2.sendBuffer();

        // 增加长度，控制增长速率
        currentLen += 2; // 调节值控制增长速度
        delay(10);       // 延时控制帧率
    }
}

void ArcTest()
{

    u8g2.clearBuffer();
    u8g2.setDrawColor(1);
    u8g2.drawXBMP(6, 3, 38, 54, nli[0]);
    u8g2.drawXBMP(42, 3, 38, 54, nli[3]);
    u8g2.drawXBMP(173, 3, 38, 54, nli[2]);
    u8g2.drawXBMP(213, 3, 38, 54, nli[8]);
    u8g2.drawRFrame(4, 2, 85, 58, 5);
    u8g2.drawRFrame(166, 2, 85, 58, 5);
    u8g2.setDrawColor(0);
    u8g2.drawBox(0, 0, 255, 2);
    u8g2.drawBox(0, 60, 255, 4);
    u8g2.sendBuffer();
    // 第一阶段：线段从圆心向外发射
    for (int k = 0; k < 360; k += 6) // 每次增加6度，共60次（6度分辨率）
    {
        for (int len = 0; len <= 31; len++) // 动态拉长线段长度，最大为31
        {
            u8g2.setDrawColor(1);
            drawCake(127, 31, len, k / 6); // 逐步绘制当前角度k的线段
            u8g2.sendBuffer();             // 更新屏幕
            // delay(10);                     // 延时控制动画速度
            u8g2.setDrawColor(0);
            u8g2.drawDisc(127, 31, 28); // 用实心圆覆盖多余线段
            // u8g2.sendBuffer();
        }
    }
    u8g2.drawDisc(127, 31, 28); // 用实心圆覆盖多余线段
    u8g2.sendBuffer();

    // 第二阶段：线段从边缘向圆心收缩并消失
    for (int k = 0; k < 360; k += 6) // 遍历每条线段
    {
        for (int len = 31; len >= 0; len--) // 动态缩短线段长度
        {
            // 用黑色清除上一帧的整条线段
            u8g2.setDrawColor(0);
            drawCake(127, 31, len + 1, k / 6); // 多清除一点，确保尾部没有残留

            // 绘制白色线段（线头）
            u8g2.setDrawColor(1);
            drawCake(127, 31, len, k / 6); // 绘制当前长度的线段

            // 更新屏幕
            u8g2.sendBuffer();
            // delay(10); // 控制动画速度
        }
    }

    /*for (int k = 0; k < 360; k += 6) // 每次增加6度，共60次（6度分辨率）
    {
        for (int len = 0; len <= 31; len++) // 动态拉长线段长度，最大为31
        {
            u8g2.setDrawColor(1);
            drawCake(127, 31, len, k / 6); // 逐步绘制当前角度k的线段
            u8g2.sendBuffer();             // 更新屏幕
            // delay(10);                     // 延时控制动画速度
            u8g2.setDrawColor(0);
            u8g2.drawDisc(127, 31, 28); // 用实心圆覆盖多余线段
            // u8g2.sendBuffer();
        }
    }
    u8g2.drawDisc(127, 31, 28); // 用实心圆覆盖多余线段
    u8g2.sendBuffer();

    // 第二阶段：线段飞向圆心并消失
    for (int k = 0; k < 360; k += 6) // 遍历每条线段
    {
        for (int len = 31; len >= 0; len--) // 动态缩短线段
        {
            // 绘制白色线段（线头）
            u8g2.setDrawColor(1);
            drawCake(127, 31, len, k / 6); // 绘制当前长度的线段

            // 绘制黑色尾部覆盖（模拟消失效果）
            u8g2.setDrawColor(0);
            drawCake(127, 31, len + 1, k / 6); // 黑色线段比白色线段稍长

            // 更新屏幕
            u8g2.sendBuffer();
            // delay(10); // 控制动画速度
        }
    }*/

    // for (int k = 25; k >= 0; k--)
    //     for (int i = 0; i < (25 - k) * 2.4; i++)
    //     {
    //         u8g2.setDrawColor(1);
    //         drawCake(127, 31, 31, i);
    //         u8g2.setDrawColor(0);
    //         u8g2.drawDisc(127, 31, 28);
    //         u8g2.setDrawColor(1);
    //         u8g2.drawXBMP(106, 11, 42, 42, shalou);
    //         u8g2.sendBuffer();
    //     }
    //   otafun();
}
short phsc = 0;
short phgc = 0;
short pmsc = 0;
short pmgc = 0;
/**
 * @brief 绘制倒计时的时钟
 * @param min
 * @param second
 */
void clipPomodoro(short min, short second)
{
    short mfy = 60, my = 3;
    short nmy = -54, umy = 3;
    short a, b, c, d;
    // String weekstr = daysOfTheWeek[RXtm.Wday];
    // String datestr = String(tmYearToCalendar(RXtm.Year)) + "-" + monthsNames[RXtm.Month] + "-" + String(RXtm.Day);
    prepairClip(min, second, &a, &b, &c, &d);
    if (d != pmgc)
    {
        // Serial.println("切换秒的个位");
        while (!ui_siglerun(&my, &mfy, 3, 2) || !ui_siglerun(&nmy, &umy, 3, 2))
        {
            u8g2.clearBuffer();
            u8g2.setDrawColor(1);
            u8g2.drawXBMP(2, 7, 50, 50, fanqie);
            u8g2.drawXBMP(60, 3, 38, 54, nli[phsc]);
            u8g2.drawXBMP(96, 3, 38, 54, nli[phgc]);
            u8g2.drawXBMP(156, 3, 38, 54, nli[pmsc]);
            u8g2.drawXBMP(196, my, 38, 54, nli[pmgc]);
            u8g2.drawXBMP(196, nmy, 38, 54, nli[d]);
            u8g2.drawRFrame(54, 2, 85, 58, 5);
            u8g2.drawRFrame(151, 2, 85, 58, 5);
            u8g2.setDrawColor(0);
            u8g2.drawBox(0, 0, 255, 2);
            u8g2.drawBox(0, 60, 255, 4);
            u8g2.sendBuffer();
        }
        pmgc = d;
        mfy = 60;
        my = 3;
        nmy = -54;
        umy = 3;
    }

    if (c != pmsc)
    {
        // Serial.println("切换秒的十位");
        while (!ui_siglerun(&my, &mfy, 3, 2) || !ui_siglerun(&nmy, &umy, 3, 2))
        {
            u8g2.clearBuffer();
            u8g2.setDrawColor(1);
            u8g2.drawXBMP(2, 7, 50, 50, fanqie);
            u8g2.drawXBMP(60, 3, 38, 54, nli[phsc]);
            u8g2.drawXBMP(96, 3, 38, 54, nli[phgc]);
            u8g2.drawXBMP(156, my, 38, 54, nli[pmsc]);
            u8g2.drawXBMP(156, nmy, 38, 54, nli[c]);
            u8g2.drawXBMP(196, 3, 38, 54, nli[pmgc]);
            u8g2.drawRFrame(54, 2, 85, 58, 5);
            u8g2.drawRFrame(151, 2, 85, 58, 5);
            u8g2.setDrawColor(0);
            u8g2.drawBox(0, 0, 255, 2);
            u8g2.drawBox(0, 60, 255, 4);
            u8g2.sendBuffer();
        }
        pmsc = c;
        mfy = 60;
        my = 3;
        nmy = -54;
        umy = 3;
    }

    if (b != phgc)
    {
        // Serial.println("切换分钟十位");
        while (!ui_siglerun(&my, &mfy, 3, 2) || !ui_siglerun(&nmy, &umy, 3, 2))
        {
            u8g2.clearBuffer();
            u8g2.setDrawColor(1);
            u8g2.drawXBMP(2, 7, 50, 50, fanqie);
            u8g2.drawXBMP(60, 3, 38, 54, nli[phsc]);
            u8g2.drawXBMP(96, my, 38, 54, nli[phgc]);
            u8g2.drawXBMP(96, nmy, 38, 54, nli[b]);
            u8g2.drawXBMP(156, 3, 38, 54, nli[pmsc]);
            u8g2.drawXBMP(196, 3, 38, 54, nli[pmgc]);
            u8g2.drawRFrame(54, 2, 85, 58, 5);
            u8g2.drawRFrame(151, 2, 85, 58, 5);
            u8g2.setDrawColor(0);
            u8g2.drawBox(0, 0, 255, 2);
            u8g2.drawBox(0, 60, 255, 4);
            u8g2.sendBuffer();
        }
        phgc = b;
        mfy = 60;
        my = 3;
        nmy = -54;
        umy = 3;
    }

    if (a != phsc)
    {
        // Serial.println("切换分钟个位");
        while (!ui_siglerun(&my, &mfy, 3, 2) || !ui_siglerun(&nmy, &umy, 3, 2))
        {
            u8g2.clearBuffer();
            u8g2.setDrawColor(1);
            u8g2.drawXBMP(2, 7, 50, 50, fanqie);
            u8g2.drawXBMP(60, my, 38, 54, nli[phsc]);
            u8g2.drawXBMP(60, nmy, 38, 54, nli[a]);
            u8g2.drawXBMP(96, 3, 38, 54, nli[phgc]);
            u8g2.drawXBMP(156, 3, 38, 54, nli[pmsc]);
            u8g2.drawXBMP(196, 3, 38, 54, nli[pmgc]);
            u8g2.drawRFrame(54, 2, 85, 58, 5);
            u8g2.drawRFrame(151, 2, 85, 58, 5);
            u8g2.setDrawColor(0);
            u8g2.drawBox(0, 0, 255, 2);
            u8g2.drawBox(0, 60, 255, 4);
            u8g2.sendBuffer();
        }
        phsc = a;
        mfy = 60;
        my = 3;
        nmy = -54;
        umy = 3;
    }
}
/**
 * @brief 番茄钟计时设定
 */
void pomodoroSet()
{
    u8g2.clearBuffer();
    u8g2.setDrawColor(1);
    short mfy = 60, my = 3;
    short nmy = -54, umy = 3;
    u8g2.drawXBMP(2, 7, 50, 50, fanqie);
    u8g2.drawXBMP(60, 3, 38, 54, nli[0]);  // 分钟十位
    u8g2.drawXBMP(96, 3, 38, 54, nli[0]);  // 分钟个位
    u8g2.drawXBMP(156, 3, 38, 54, nli[0]); // 秒十位
    u8g2.drawXBMP(196, 3, 38, 54, nli[0]); // 秒个位
    u8g2.drawRFrame(54, 2, 85, 58, 5);
    u8g2.drawRFrame(151, 2, 85, 58, 5);
    u8g2.setDrawColor(0);
    u8g2.drawBox(0, 0, 255, 2);
    u8g2.drawBox(0, 60, 255, 4);
    u8g2.sendBuffer();
    int pomoCount = 0; // 倒计时分钟数
    int timeCount = 0;

    // 等待用户输入倒计时分钟
    while (timeCount < 50)
    {
        if (digitalRead(keyright) == LOW)
        {
            timeCount = 0;
            delay(200); // 防抖
            pomoCount = min(pomoCount + 5, 60);
        }
        if (digitalRead(keyleft) == LOW)
        {
            timeCount = 0;
            delay(200); // 防抖
            pomoCount = min(pomoCount + 1, 60);
        }
        clipPomodoro(pomoCount, 0); // 动态更新显示
        delay(100);
        timeCount++;
    }
    if (timeCount < 1)
        return;
    // 转换为秒数
    pomoCount *= 60;
    // Serial.printf("总秒数是：%d\n",pomoCount);
    RTC_RX8025T.read(RXtm);
    int ose = RXtm.Second; // 起始秒数
    // Serial.printf("当前os秒数是：%d\n",ose);
    while (pomoCount > 0)
    {
        RTC_RX8025T.read(RXtm);
        int cse = RXtm.Second;
        // Serial.printf("当前cse秒数是：%d\n",cse);
        if (cse != ose) // 每秒检测
        {
            pomoCount -= (cse - ose + 60) % 60;           // 处理秒数循环差值
            clipPomodoro(pomoCount / 60, pomoCount % 60); // 更新动画
            // Serial.printf("处理后的秒数是：%d\n",pomoCount);
            ose = cse;
            if (pomoCount <= 0)
                break; // 提前退出
            // Serial.printf("当前分钟数是：%d   当前秒数是：%d\n",pomoCount / 60, pomoCount % 60);
        }
    }

    while (digitalRead(key1) != LOW && digitalRead(key2) != LOW && digitalRead(key3) != LOW && digitalRead(key4) != LOW && digitalRead(keyleft) != LOW && digitalRead(keyright) != LOW)
    {
        digitalWrite(BUZZ, HIGH);
        delay(1);
        digitalWrite(BUZZ, LOW);
        delay(1);
    }
}
/**
 * @brief 番茄钟启动动画
 */
void pomodoroAnimate()
{
    short y = -60, fy = 6, cx = 102, fx = 2;
    float velocity = 0.0f;
    float step = 0.06f;
    u8g2.setDrawColor(1);
    while (ui_bounce(&y, &fy, &velocity, step))
    {
        u8g2.clearBuffer();
        u8g2.drawXBMP(cx, y, 50, 50, fanqie);
        u8g2.sendBuffer();
    }
    delay(500);
    short fr1sx = 258, fr1fx = 54;
    while (!ui_siglerun(&cx, &fx, 7, 3) || !ui_siglerun(&fr1sx, &fr1fx, 7, 3))
    {
        u8g2.clearBuffer();
        u8g2.setDrawColor(1);
        u8g2.drawXBMP(cx, 7, 50, 50, fanqie);
        u8g2.drawXBMP(fr1sx + 8, 3, 38, 54, nli[0]);
        u8g2.drawXBMP(fr1sx + 44, 3, 38, 54, nli[0]);
        u8g2.drawXBMP(fr1sx + 105, 3, 38, 54, nli[0]);
        u8g2.drawXBMP(fr1sx + 141, 3, 38, 54, nli[0]);
        u8g2.drawRFrame(fr1sx, 2, 85, 58, 5);
        u8g2.drawRFrame(fr1sx + 97, 2, 85, 58, 5);
        u8g2.setDrawColor(0);
        u8g2.drawBox(0, 0, 255, 2);
        u8g2.drawBox(0, 60, 255, 4);
        u8g2.sendBuffer();
    }
}
/**
 * @brief 计时器启动动画
 */
void stopwatchAnimate()
{
    short y = -60, fy = 6, cx = 102, fx = 2;
    float velocity = 0.0f;
    float step = 0.06f;
    u8g2.setDrawColor(1);
    while (ui_bounce(&y, &fy, &velocity, step))
    {
        u8g2.clearBuffer();
        u8g2.drawXBMP(cx, y, 50, 50, daojishi);
        u8g2.sendBuffer();
    }
    delay(500);
    short fr1sx = 258, fr1fx = 54;
    while (!ui_siglerun(&cx, &fx, 7, 3) || !ui_siglerun(&fr1sx, &fr1fx, 7, 3))
    {
        u8g2.clearBuffer();
        u8g2.setDrawColor(1);
        u8g2.drawXBMP(cx, 7, 50, 50, daojishi);
        u8g2.drawXBMP(fr1sx + 8, 3, 38, 54, nli[0]);
        u8g2.drawXBMP(fr1sx + 44, 3, 38, 54, nli[0]);
        u8g2.drawXBMP(fr1sx + 105, 3, 38, 54, nli[0]);
        u8g2.drawXBMP(fr1sx + 141, 3, 38, 54, nli[0]);
        u8g2.drawRFrame(fr1sx, 2, 85, 58, 5);
        u8g2.drawRFrame(fr1sx + 97, 2, 85, 58, 5);
        u8g2.setDrawColor(0);
        u8g2.drawBox(0, 0, 255, 2);
        u8g2.drawBox(0, 60, 255, 4);
        u8g2.sendBuffer();
    }
}
/**
 * @brief 绘制倒计时的时钟
 * @param min
 * @param second
 */
void clipStopwatch(short min, short second)
{
    short mfy = 60, my = 3;
    short nmy = -54, umy = 3;
    short a, b, c, d;
    // String weekstr = daysOfTheWeek[RXtm.Wday];
    // String datestr = String(tmYearToCalendar(RXtm.Year)) + "-" + monthsNames[RXtm.Month] + "-" + String(RXtm.Day);
    prepairClip(min, second, &a, &b, &c, &d);
    if (d != pmgc)
    {
        // Serial.println("切换秒的个位");
        while (!ui_siglerun(&my, &mfy, 3, 2) || !ui_siglerun(&nmy, &umy, 3, 2))
        {
            u8g2.clearBuffer();
            u8g2.setDrawColor(1);
            u8g2.drawXBMP(2, 7, 50, 50, daojishi);
            u8g2.drawXBMP(60, 3, 38, 54, nli[phsc]);
            u8g2.drawXBMP(96, 3, 38, 54, nli[phgc]);
            u8g2.drawXBMP(156, 3, 38, 54, nli[pmsc]);
            u8g2.drawXBMP(196, my, 38, 54, nli[pmgc]);
            u8g2.drawXBMP(196, nmy, 38, 54, nli[d]);
            u8g2.drawRFrame(54, 2, 85, 58, 5);
            u8g2.drawRFrame(151, 2, 85, 58, 5);
            u8g2.setDrawColor(0);
            u8g2.drawBox(0, 0, 255, 2);
            u8g2.drawBox(0, 60, 255, 4);
            u8g2.sendBuffer();
            if (digitalRead(keyleft) == LOW)
            {
                delay(300);
                while (digitalRead(keyleft) != LOW)
                {
                    delay(100);
                    if (digitalRead(key4) == LOW)
                    {
                        ESP.restart();
                    }
                }
                delay(300);
            }
        }
        pmgc = d;
        mfy = 60;
        my = 3;
        nmy = -54;
        umy = 3;
    }

    if (c != pmsc)
    {
        // Serial.println("切换秒的十位");
        while (!ui_siglerun(&my, &mfy, 3, 2) || !ui_siglerun(&nmy, &umy, 3, 2))
        {
            u8g2.clearBuffer();
            u8g2.setDrawColor(1);
            u8g2.drawXBMP(2, 7, 50, 50, daojishi);
            u8g2.drawXBMP(60, 3, 38, 54, nli[phsc]);
            u8g2.drawXBMP(96, 3, 38, 54, nli[phgc]);
            u8g2.drawXBMP(156, my, 38, 54, nli[pmsc]);
            u8g2.drawXBMP(156, nmy, 38, 54, nli[c]);
            u8g2.drawXBMP(196, 3, 38, 54, nli[pmgc]);
            u8g2.drawRFrame(54, 2, 85, 58, 5);
            u8g2.drawRFrame(151, 2, 85, 58, 5);
            u8g2.setDrawColor(0);
            u8g2.drawBox(0, 0, 255, 2);
            u8g2.drawBox(0, 60, 255, 4);
            u8g2.sendBuffer();
            if (digitalRead(keyleft) == LOW)
            {
                delay(300);
                while (digitalRead(keyleft) != LOW)
                {
                    delay(100);
                    if (digitalRead(key4) == LOW)
                    {
                        ESP.restart();
                    }
                }
                delay(300);
            }
        }
        pmsc = c;
        mfy = 60;
        my = 3;
        nmy = -54;
        umy = 3;
    }

    if (b != phgc)
    {
        // Serial.println("切换分钟十位");
        while (!ui_siglerun(&my, &mfy, 3, 2) || !ui_siglerun(&nmy, &umy, 3, 2))
        {
            u8g2.clearBuffer();
            u8g2.setDrawColor(1);
            u8g2.drawXBMP(2, 7, 50, 50, daojishi);
            u8g2.drawXBMP(60, 3, 38, 54, nli[phsc]);
            u8g2.drawXBMP(96, my, 38, 54, nli[phgc]);
            u8g2.drawXBMP(96, nmy, 38, 54, nli[b]);
            u8g2.drawXBMP(156, 3, 38, 54, nli[pmsc]);
            u8g2.drawXBMP(196, 3, 38, 54, nli[pmgc]);
            u8g2.drawRFrame(54, 2, 85, 58, 5);
            u8g2.drawRFrame(151, 2, 85, 58, 5);
            u8g2.setDrawColor(0);
            u8g2.drawBox(0, 0, 255, 2);
            u8g2.drawBox(0, 60, 255, 4);
            u8g2.sendBuffer();
            if (digitalRead(keyleft) == LOW)
            {
                delay(300);
                while (digitalRead(keyleft) != LOW)
                {
                    delay(100);
                    if (digitalRead(key4) == LOW)
                    {
                        ESP.restart();
                    }
                }
                delay(300);
            }
        }
        phgc = b;
        mfy = 60;
        my = 3;
        nmy = -54;
        umy = 3;
    }

    if (a != phsc)
    {
        // Serial.println("切换分钟个位");
        while (!ui_siglerun(&my, &mfy, 3, 2) || !ui_siglerun(&nmy, &umy, 3, 2))
        {
            u8g2.clearBuffer();
            u8g2.setDrawColor(1);
            u8g2.drawXBMP(2, 7, 50, 50, daojishi);
            u8g2.drawXBMP(60, my, 38, 54, nli[phsc]);
            u8g2.drawXBMP(60, nmy, 38, 54, nli[a]);
            u8g2.drawXBMP(96, 3, 38, 54, nli[phgc]);
            u8g2.drawXBMP(156, 3, 38, 54, nli[pmsc]);
            u8g2.drawXBMP(196, 3, 38, 54, nli[pmgc]);
            u8g2.drawRFrame(54, 2, 85, 58, 5);
            u8g2.drawRFrame(151, 2, 85, 58, 5);
            u8g2.setDrawColor(0);
            u8g2.drawBox(0, 0, 255, 2);
            u8g2.drawBox(0, 60, 255, 4);
            u8g2.sendBuffer();
            if (digitalRead(keyleft) == LOW)
            {
                delay(300);
                while (digitalRead(keyleft) != LOW)
                {
                    delay(100);
                    if (digitalRead(key4) == LOW)
                    {
                        ESP.restart();
                    }
                }
                delay(300);
            }
        }
        phsc = a;
        mfy = 60;
        my = 3;
        nmy = -54;
        umy = 3;
    }
}
/**
 * @brief 计时器开始
 */
void stopwatchStart()
{
    RTC_RX8025T.read(RXtm);
    u8g2.clearBuffer();
    int pomoCount = 0;
    RTC_RX8025T.read(RXtm);
    int ose = RXtm.Second; // 起始秒数
    int cse;
    int lastUpdateTime = ose; // 记录上次更新时间

    while (true)
    {
        if (digitalRead(keyleft) == LOW)
        {
            delay(300);
            while (digitalRead(keyleft) != LOW)
            {
                delay(100);
                if (digitalRead(key4) == LOW)
                {
                    ESP.restart();
                }
            }
            delay(300);
        }
        if (digitalRead(key4) == LOW)
        {
            break;
        }

        RTC_RX8025T.read(RXtm);
        cse = RXtm.Second;

        if (cse != lastUpdateTime) // 如果秒数变化了
        {
            pomoCount++; // 增加秒数
            lastUpdateTime = cse; // 更新最后更新时间
            // 更新动画，避免每次增加秒数时都更新界面
            short min = pomoCount / 60;
            short second = pomoCount % 60;
            clipStopwatch(min, second); // 更新动画
        }
    }
    ESP.restart();
}