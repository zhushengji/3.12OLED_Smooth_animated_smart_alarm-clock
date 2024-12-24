# 3.12OLED_Smooth_animated_smart_alarm-clock
一款基于esp32实现的丝滑动画小闹钟
静石键位及功能说明：
1.顶上四个键是键1，键2，键3，键4，正面是左键，右键；
2.键1键2是选择键，键3是确定，键4是返回；
3.唤醒后长按键4可以进入主菜单；
4.唤醒后左键可进入计时器，再次点击可暂停计时，键4退出；
5.唤醒后右键可进入番茄钟，5秒内左键加1分钟，右键加5分钟，5秒内无操作则进入番茄计时
6.查看实时天气或自动校准时间时需要联网，此时设备会自动连接上次连过的网络，失败后会进入自动配网；
7.自动配网下会自动扫描周围2.4G网络，扫描成功后屏幕会提醒，此时用手机等设备搜索并连接“静石配网wifi”即可进入配网，选择wifi名并填入对应的密码即可完成配网。第三栏的心知天气秘钥是获取天气的，建议自己免费申请一个并填入以获取最优的体验，如果没有可以不填；

项目介绍
本项目是基于ESP32和3.12寸OLED实现的桌面时钟。
此时此刻已实现丝滑翻页、实时天气动画、震动唤醒、温湿度检测、闹钟、OTA、亮度调节、自动校准、手动校准、离线天气预测，番茄钟。后续将持续完善更新。
本作品灵感源于白橙大佬的效果图，但我实现的不好。后续外观多数要升级，目前先发一个给大家玩玩。
本作品名为“静石”。林深藏古寺，老树掩荒碑。能长期与时间作伴的，大概只有静静的顽石吧！希望静石能给各位在忙碌的间隙提供一丝宁静！

项目参数
核心器件：

SSD1362驱动的3.12寸OLED，分辨率256\*64
MPU6050
bmp280
AHT20
RTX8025
顶键\*4
面板按键\*2(尺寸选择6\*6*4.3）
蜂鸣器
注意事项
本作品不含烧录电路，需要烧录器或烧录夹实现烧录，请悉知！
电池尺寸为802095
面板厚度1.0，主板厚度0.8
交流群：855540712 进群请回答问题，群里更新进度最快
![a0bb3fac4f7ee4f2527682af89fc310](https://github.com/user-attachments/assets/77fa6747-1a3b-4ee3-9cd9-8151a1098c77)
![9d6d8b7dc297bdcc11b59f055cea049](https://github.com/user-attachments/assets/48c3e917-b36c-4a9b-b9d1-6b97619550b5)

