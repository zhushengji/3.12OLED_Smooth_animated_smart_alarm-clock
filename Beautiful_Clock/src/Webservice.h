#include <DNSServer.h>
#include <Preferences.h>
// #include "NTP_RTC.h"
Preferences preferences;
DNSServer dnsServer;
String cnSSID;
String ssid;
String password;
String xz_code;
String ssidList;
bool is_setup_done = false;
bool valid_ssid_received = false;
bool valid_xz_code_received = false;
bool valid_password_received = false;
bool wifi_timeout = false;
boolean settingMode;
String index_html; // index界面
/**
 * @brief
 * 扫描可用wifi并存入ssidlist
 */
void scan_wifi()
{
  Serial.println("扫描wifi列表");
  u8g2.setFont(dinkie_9);
  u8g2.clearBuffer();
  u8g2.drawXBMP(103, 2, 50, 50, saomiao);
  u8g2.setCursor(5, 58);
  u8g2.print("扫描附近可用WiFi");
  u8g2.sendBuffer();
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  int n = WiFi.scanNetworks();
  delay(100);

  for (int i = 0; i < n; ++i)
  {
    ssidList += "<option value=\"";
    ssidList += WiFi.SSID(i);
    ssidList += "\">";
    ssidList += WiFi.SSID(i);
    ssidList += "</option>";
    u8g2.clearBuffer();
    u8g2.drawXBMP(103, 2, 50, 50, saomiao);
    u8g2.setCursor(5, 58);
    u8g2.print("扫描附近可用WiFi");
    for (int j = 0; j <= i; j++)
    {
      u8g2.print(" . ");
    }
    u8g2.sendBuffer();
    delay(300);
  }
  u8g2.clearBuffer();
  u8g2.drawXBMP(103, 2, 50, 50, dengdai);
  u8g2.setCursor(5, 58);
  u8g2.print("扫描结束！等待设备接入中");
  u8g2.sendBuffer();
  /*
  <!DOCTYPE html><html lang="zh-CN"><head><meta charset="UTF-8"><meta name="viewport"content="width=device-width, initial-scale=1.0"><title>静石配网wifi</title><style>body{font-family:Arial,sans-serif;background:#f4f4f4;margin:0;padding:0;display:flex;justify-content:center;align-items:center;height:100vh}.container{background:white;padding:20px;border-radius:8px;box-shadow:0 2px 10px rgba(0,0,0,0.1)}h3{text-align:center;color:#333}form{margin-top:20px}.form-group{margin-bottom:15px}label{display:block;margin-bottom:5px}select,input[type="text"]{width:100%;padding:10px;border:1px solid#ddd;border-radius:4px;box-sizing:border-box}input[type="submit"]{background:#5cb85c;color:white;border:none;padding:10px 20px;border-radius:4px;cursor:pointer;transition:background 0.3s ease}input[type="submit"]:hover{background:#4cae4c}</style></head><body><div class="container"><h3>静石配网wifi</h3><form action="/get"><div class="form-group"><label for="ssid">wifi名称:</label><select name="ssid"id="ssid"><!--这里插入ssid列表--></select></div><div class="form-group"><label for="password">wifi密码:</label><input type="text"name="password"id="password"placeholder="请输入密码"></div><div class="form-group"><label for="xz_code">心知秘钥:</label><input type="text"name="xz_code"id="xz_code"placeholder="请输入秘钥"></div><input type="submit"value="Submit"></form></div></body></html>
  */
  index_html = "<!DOCTYPE html><html lang=\"zh-CN\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\"content=\"width=device-width, initial-scale=1.0\"><title>静石配网wifi</title><style>body{font-family:Arial,sans-serif;background:#f4f4f4;margin:0;padding:0;display:flex;justify-content:center;align-items:center;height:100vh}.container{background:white;padding:20px;border-radius:8px;box-shadow:0 2px 10px rgba(0,0,0,0.1)}h3{text-align:center;color:#333}form{margin-top:20px}.form-group{margin-bottom:15px}label{display:block;margin-bottom:5px}select,input[type=\"text\"]{width:100%;padding:10px;border:1px solid#ddd;border-radius:4px;box-sizing:border-box}input[type=\"submit\"]{background:#5cb85c;color:white;border:none;padding:10px 20px;border-radius:4px;cursor:pointer;transition:background 0.3s ease}input[type=\"submit\"]:hover{background:#4cae4c}</style></head><body><div class=\"container\"><h3>静石配网wifi</h3><form action=\"/get\"><div class=\"form-group\"><label for=\"ssid\">wifi名称:</label><select name=\"ssid\"id=\"ssid\">" + ssidList + "</select></div><div class=\"form-group\"><label for=\"password\">wifi密码:</label><input type=\"text\"name=\"password\"id=\"password\"placeholder=\"请输入密码\"></div><div class=\"form-group\"><label for=\"xz_code\">心知秘钥:</label><input type=\"text\"name=\"xz_code\"id=\"xz_code\"placeholder=\"请输入秘钥\"></div><input type=\"submit\"value=\"Submit\"></form></div></body></html>";
}

class CaptiveRequestHandler : public AsyncWebHandler
{
public:
  CaptiveRequestHandler() {}
  virtual ~CaptiveRequestHandler() {}

  bool canHandle(AsyncWebServerRequest *request)
  {
    // request->addInterestingHeader("ANY");
    return true;
  }
  void handleRequest(AsyncWebServerRequest *request)
  {
    request->send_P(200, "text/html", index_html.c_str());
  }
};
/**
 * @brief
 * web配网（激活）服务
 * @return true
 * @return false
 */
bool setupServer()
{
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    request->send_P(200, "text/html", index_html.c_str());
    
    Serial.println("Client Connected"); });

  server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              vTaskDelay(100);
              // delay(10);
    String inputMessage;
    String inputParam;
    
    if (request->hasParam("ssid")) {
      inputMessage = request->getParam("ssid")->value();
      inputParam = "ssid";
      ssid = inputMessage;
      Serial.print("输入获取的WiFi名：");Serial.println(inputMessage);
      valid_ssid_received = true;
    }
    
    if (request->hasParam("password")) {
      inputMessage = request->getParam("password")->value();
      inputParam = "password";
      password = inputMessage;
      Serial.print("输入获取的WiFi密码：");Serial.println(inputMessage);
      valid_password_received = true;
    }
    if (request->hasParam("xz_code")) {
      inputMessage = request->getParam("xz_code")->value();
      inputParam = "xz_code";
      xz_code = inputMessage;
      Serial.print("输入获取的心知秘钥：");Serial.println(inputMessage);
      valid_xz_code_received = true;
    }
      u8g2.clearBuffer();
      //标记点
  // u8g2.drawXBMP(103, 2, 50, 50, dengdai);
  u8g2.setCursor(5, 38);
  u8g2.print("已获得信息： ");
  u8g2.print(ssid);
  u8g2.print(" ");
  u8g2.print(password);
  u8g2.sendBuffer();
  delay(3000);
    request->send(200, "text/html", "The values entered by you have been successfully sent to the device. It will now attempt WiFi connection");
    return true; });
  return true;
}
/**
 * @brief
 * wifi配网失败后配网模式设置
 */
void WiFiSoftAPSetup()
{
  WiFi.mode(WIFI_AP);
  WiFi.softAP("静石配网WiFi");
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
}
/**
 * @brief
 * 强制门户认证
 */
void StartCaptivePortal()
{
  // Serial.println("ssidList列表：");
  // Serial.println(ssidList);
  scan_wifi();
  Serial.println("开启AP模式");
  WiFiSoftAPSetup();
  Serial.println("设置 Async 服务器");
  if (setupServer())
  {
    Serial.println("开启 DNS 服务");
    dnsServer.start(53, "*", WiFi.softAPIP());
    server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER); // only when requested from AP
    server.begin();
  }
  else
  {
    Serial.println("停止认证！");
  }
}

/**
 * @brief
 * wifi连接操作
 */
void WiFiStationSetup(String rec_ssid, String rec_password, String rec_xzcode /*rec_xzcode*/)
{
  wifi_timeout = false;
  WiFi.mode(WIFI_STA);
  char ssid_arr[20];
  char password_arr[20];
  // char xz_code_arr[20];
  char xz_code_arr[15];
  rec_ssid.toCharArray(ssid_arr, rec_ssid.length() + 1);
  rec_password.toCharArray(password_arr, rec_password.length() + 1);
  rec_xzcode.toCharArray(xz_code_arr, rec_xzcode.length() + 1);
  Serial.print("wifi: ");
  Serial.println(ssid_arr);
  Serial.print("密码: ");
  Serial.println(password_arr);
  Serial.print("心知秘钥: ");
  Serial.println(xz_code);
  if (rec_xzcode.length() > 5)
  {
    XZcode = xz_code;
    preferences.putString("rec_xzcode", rec_xzcode);
  }
  WiFi.begin(ssid_arr, password_arr);
  uint32_t t1 = millis();
  int i = 0, p = 1, k = 0;
  u8g2.setFont(dinkie_9);
  while (WiFi.status() != WL_CONNECTED)
  {
    if (millis() - t1 > 50000) // 50 seconds elapsed connecting to WiFi
    {
      Serial.println();
      Serial.println("连接超时！");
      valid_ssid_received = false;
      valid_password_received = false;
      bool valid_xz_code_received = false;
      is_setup_done = false;
      preferences.putBool("is_setup_done", is_setup_done);
      u8g2.clearBuffer();
      u8g2.drawXBMP(103, 2, 50, 50, cha);
      u8g2.setCursor(5, 58);
      u8g2.print("连接失败！即将启动配网功能");
      u8g2.sendBuffer();
      StartCaptivePortal();
      wifi_timeout = true;

      break;
    }
    u8g2.clearBuffer();
    u8g2.setDrawColor(1);
    u8g2.drawXBMP(0, 0, 256, 64, bgheart);
    u8g2.setDrawColor(0);
    if (p == 1)
    {
      u8g2.drawBox(i, 0, 256 - i, 64);
    }
    else
    {
      u8g2.drawBox(0, 0, 256 - i, 64);
    }
    u8g2.setDrawColor(1);

    u8g2.setCursor(5, 58);
    u8g2.print("正在连接到 ");
    u8g2.print(rec_ssid);
    for (int x = 0; x < k / 50; x++)
    {
      u8g2.print(" . ");
    }
    u8g2.sendBuffer();
    i += p;
    if (i > 255)
      p = -1;
    if (i < 0)
      p = 1;
    k++;
    if (k == 256)
      k = 0;
  }
  if (!wifi_timeout)
  {
    is_setup_done = true;
    Serial.println("");
    Serial.print("WiFi 已连接到: ");
    Serial.println(rec_ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    for (int i = 0; i < 50; i++)
    {
      u8g2.clearBuffer();
      u8g2.setDrawColor(1);
      u8g2.drawXBMP(103, 2, 50, 50, gou);
      u8g2.setDrawColor(0);
      u8g2.drawBox(103 + i, 2, 51 - i, 50);
      u8g2.setCursor(5, 58);
      u8g2.setDrawColor(1);
      u8g2.print("连接成功！已连接到：");
      u8g2.print(rec_ssid);
      u8g2.sendBuffer();
    }

    preferences.putBool("is_setup_done", is_setup_done);
    preferences.putString("rec_ssid", rec_ssid);
    preferences.putString("rec_password", rec_password);
    // preferences.putString("rec_xzcode", rec_xzcode);
  }
}

/**
 * @brief
 * 联网功能函数
 * 默认连接上次连接的网络，
 * 连接不上或首次开机则开启强制门户认证进行配网
 * 联网成功后默认校准时钟和获取天气信息
 */
void wifi_init()
{
  preferences.begin("my-prefi", false);
  is_setup_done = preferences.getBool("is_setup_done", false);
  ssid = preferences.getString("rec_ssid");
  password = preferences.getString("rec_password");

  // 心知天气密钥
  xz_code = preferences.getString("rec_xzcode");
  Serial.println("判断开始");
  if (!is_setup_done)
  {
    Serial.println("准备门户认证");
    u8g2.setFont(dinkie_9);
    u8g2.clearBuffer();
    u8g2.drawXBMP(103, 2, 50, 50, kong);
    u8g2.setCursor(5, 58);
    u8g2.print("未找到联网记录，开启门户配网");
    u8g2.sendBuffer();
    Serial.println("准备就绪，开始认证");
    StartCaptivePortal();
  }
  else
  {
    Serial.println("读取记录联网");
    Serial.println("使用连接记录联网!");
    Serial.print("Saved SSID is ");
    Serial.println(ssid);
    Serial.print("Saved Password is ");
    Serial.println(password);
    // Serial.print("Saved xz_code is "); Serial.println(xz_code);
    WiFiStationSetup(ssid, password, xz_code);
  }
  while (!is_setup_done)
  {
    dnsServer.processNextRequest();
    delay(10);
    if (valid_ssid_received && valid_password_received)
    {
      Serial.println("Attempting WiFi Connection!");
      WiFiStationSetup(ssid, password, xz_code);
      cnSSID = ssid;
    }
  }
  settingMode = false;
  Serial.println("wifi连接成功！");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  // Serial.println("All Done!");
}
/**
 * @brief OTA功能函数
 */
void otafun()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    wifi_init();
  }
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", "Hi! This is a sample response."); });

  AsyncElegantOTA.begin(&server); // Start AsyncElegantOTA
  server.begin();
  Serial.println("HTTP server started");
  AsyncElegantOTA.begin(&server);
  server.begin();
  u8g2.clearBuffer();
  u8g2.drawXBMP(0, 0, 256, 64, OTAwait);
  u8g2.setFont(u8g2_font_6x13_tf);
  u8g2.setCursor(78, 58);
  u8g2.print(WiFi.localIP());
  u8g2.print("/update");
  u8g2.sendBuffer();
}
