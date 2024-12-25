
String callHttps(String url);
// void ParseHitokoto(String content, struct Hitokoto* data);
bool ParseActualWeather(String content, struct ActualWeather* data);

bool ParseFutureWeather(String content, struct FutureWeather* data);
// bool ParseActualWeather(String content, struct LifeIndex* data);
//****** HTTP服务器配置 ******
// String language = "en";//"zh-Hans"; // 请求语言
// String url_yiyan = "https://v1.hitokoto.cn/";//一言获取地址
String url_ActualWeather;   //实况天气地址
String url_FutureWeather;   //未来天气地址
//****** 天气数据
//我们要从此网页中提取的数据的类型
struct ActualWeather
{
  char status_code[64];  //错误代码
  char city[16];         //城市名称
  char weather_name[16]; //天气现象名称
  char weather_code[4];  //天气现象代码
  char temp[5];          //温度
  char last_update[25];  //最后更新时间
};
ActualWeather actual;  //创建结构体变量 目前的

struct FutureWeather
{
  char status_code[64];       //错误代码
  char location[14];          //天气属地
  char date0[14];             //今天日期
  char date0_text_day[20];    //白天天气现象名称
  char date0_code_day[4];     //白天天气现象代码
  char date0_text_night[16];  //晚上天气现象名称
  char date0_code_night[4];   //晚上天气现象代码
  char date0_high[5];         //最高温度
  char date0_low[5];          //最低温度
  char date0_humidity[5];     //相对湿度
  char date0_wind_scale[5];   //风力等级

  char date1[14];             //明天日期
  char date1_text_day[20];    //白天天气现象名称
  char date1_code_day[4];     //白天天气现象代码
  char date1_text_night[16];  //晚上天气现象名称
  char date1_code_night[4];   //晚上天气现象代码
  char date1_high[5];         //最高温度
  char date1_low[5];          //最低温度
  //char date1_humidity[5];     //相对湿度

  char date2[14];             //后天日期
  char date2_text_day[20];    //白天天气现象名称
  char date2_code_day[4];     //白天天气现象代码
  char date2_text_night[16];  //晚上天气现象名称
  char date2_code_night[4];   //晚上天气现象代码
  char date2_high[5];         //最高温度
  char date2_low[5];          //最低温度
  //char date2_humidity[5];     //相对湿度
};
FutureWeather future; //创建结构体变量 未来
struct LifeIndex //生活指数
{
  char status_code[64];  //错误代码
  char uvi[10];          //紫外线指数
}; LifeIndex life_index; //创建结构体变量 未来
struct News  //新闻API
{
  char status_code[64]; //错误代码
  char title[11][64];//3个char一个汉字
};
struct Hitokoto  //一言API
{
  char status_code[64]; //错误代码
  char hitokoto[64];
  char author[16];
};
/**
 * @brief Get the Weather Data object
 * 获取实时天气
 */
void GetWeatherData() {
  //拼装实况天气API地址
  url_ActualWeather = "https://api.seniverse.com/v3/weather/now.json";
  url_ActualWeather += "?key=" + XZcode;
  url_ActualWeather += "&location=ip" ;
  url_ActualWeather += "&language=en";
  url_ActualWeather += "&unit=c";
  //拼装生活指数
  //  String url_LifeIndex = "https://api.seniverse.com/v3/life/suggestion.json";
  //  url_LifeIndex += "?key=" + XZcode;
  //  url_LifeIndex += "&location=ip";
  ParseActualWeather(callHttps(url_ActualWeather), &actual);
  //  ParseActualWeather(callHttps(url_LifeIndex), &life_index); //获取生活指数
}
void Getfutrueweatherdata()
{
  //拼装实况未来API地址
  url_FutureWeather = "https://api.seniverse.com/v3/weather/daily.json";
  url_FutureWeather += "?key=" + XZcode;
  url_FutureWeather += "&location=ip";
  url_FutureWeather += "&language=en" ;
  url_FutureWeather += "&unit=c";
  url_FutureWeather += "&start=0";
  url_FutureWeather += "&days=3";
  //请求数据并Json处理
  //  display_partialLine(7, "获取生活指数");
  /*两个半小时更新一次天气，节省访问次数且不影响使用*/
  ParseFutureWeather(callHttps(url_FutureWeather), &future);
}
//使用Json解析天气数据，今天和未来2天
bool ParseFutureWeather(String content, struct FutureWeather* data)
{
  DynamicJsonDocument json(2560); //分配内存,动态
  DeserializationError error = deserializeJson(json, content); //解析json
  // serializeJson(json, Serial);//构造序列化json,将内容从串口输出
  if (error)
  {
    Serial.print("未来天气json配置失败:");
    Serial.println(error.c_str());
    Serial.println(" ");
    String z = "未来天气加载json配置失败:" + String(error.c_str()) + " " + content;
    //display_partialLine(7, z);
    //    esp_sleep(0);
    return false;
  }

  //检查API是否有返回错误信息，有返回则进入休眠
  if (json["status_code"].isNull() == 0) //检查到不为空
  {
    strcpy(data->status_code, json["status_code"]);
    String z;
    if (String(future.status_code) == "AP010001") z = "API 请求参数错误" ;
    else if (String(future.status_code) == "AP010002") z = "没有权限访问这个 API 接口" ;
    else if (String(future.status_code) == "AP010003") z = "API 密钥 key 错误" ;
    else if (String(future.status_code) == "AP010004") z = "签名错误" ;
    else if (String(future.status_code) == "AP010005") z = "你请求的 API 不存在" ;
    else if (String(future.status_code) == "AP010006") z = "没有权限访问这个地点: ";
    else if (String(future.status_code) == "AP010007") z = "JSONP 请求需要使用签名验证方式" ;
    else if (String(future.status_code) == "AP010008") z = "没有绑定域名" ;
    else if (String(future.status_code) == "AP010009") z = "API 请求的 user-agent 与你设置的不一致" ;
    else if (String(future.status_code) == "AP010010") z = "没有这个地点" ;
    else if (String(future.status_code) == "AP010011") z = "无法查找到指定 IP 地址对应的城市" ;
    else if (String(future.status_code) == "AP010012") z = "你的服务已经过期" ;
    else if (String(future.status_code) == "AP010013") z = "访问量余额不足" ;
    else if (String(future.status_code) == "AP010014") z = "访问频率超过限制" ;
    else if (String(future.status_code) == "AP010015") z = "暂不支持该城市的车辆限行信息" ;
    else if (String(future.status_code) == "AP010016") z = "暂不支持该城市的潮汐数据" ;
    else if (String(future.status_code) == "AP010017") z = "请求的坐标超出支持的范围" ;
    else if (String(future.status_code) == "AP100001") z = "心知系统内部错误：数据缺失" ;
    else if (String(future.status_code) == "AP100002") z = "心知系统内部错误：数据错误" ;
    else if (String(future.status_code) == "AP100003") z = "心知系统内部错误：服务内部错误" ;
    else if (String(future.status_code) == "AP100004") z = "心知系统内部错误：网关错误" ;
    else z = "未来天气异常:" + String(future.status_code);
    //display_partialLine(7, z);
    Serial.print("未来天气异常:"); Serial.println(future.status_code);
    //esp_sleep(60);
  }
  Serial.println("网络连接正常，开始配置结构体！");
  // Serial.println(json["results"][0]["location"][1]["name"]);
  // 复制我们感兴趣的字符串，先检查是否为空，空会复制失败导致系统无限重启
  if (json["results"][0]["location"]["name"].isNull() == 0)        //属地
    strcpy(data->location, json["results"][0]["location"]["name"]);
  if (json["results"][0]["daily"][0]["date"].isNull() == 0)        //日期
    strcpy(data->date0, json["results"][0]["daily"][0]["date"]);
  if (json["results"][0]["daily"][1]["date"].isNull() == 0)
    strcpy(data->date1, json["results"][0]["daily"][1]["date"]);
  if (json["results"][0]["daily"][2]["date"].isNull() == 0)
    strcpy(data->date2, json["results"][0]["daily"][2]["date"]);

  if (json["results"][0]["daily"][0]["text_day"].isNull() == 0)    //白天天气现象
    strcpy(data->date0_text_day, json["results"][0]["daily"][0]["text_day"]);
  if (json["results"][0]["daily"][1]["text_day"].isNull() == 0)
    strcpy(data->date1_text_day, json["results"][0]["daily"][1]["text_day"]);
  if (json["results"][0]["daily"][2]["text_day"].isNull() == 0)
    strcpy(data->date2_text_day, json["results"][0]["daily"][2]["text_day"]);

  if (json["results"][0]["daily"][0]["text_night"].isNull() == 0)    //晚间天气现象
    strcpy(data->date0_text_night, json["results"][0]["daily"][0]["text_night"]);
  if (json["results"][0]["daily"][1]["text_night"].isNull() == 0)
    strcpy(data->date1_text_night, json["results"][0]["daily"][1]["text_night"]);
  if (json["results"][0]["daily"][2]["text_night"].isNull() == 0)
    strcpy(data->date2_text_night, json["results"][0]["daily"][2]["text_night"]);

  if (json["results"][0]["daily"][0]["high"].isNull() == 0)
    strcpy(data->date0_high, json["results"][0]["daily"][0]["high"]);  //最高温度
  if (json["results"][0]["daily"][1]["high"].isNull() == 0)
    strcpy(data->date1_high, json["results"][0]["daily"][1]["high"]);
  if (json["results"][0]["daily"][2]["high"].isNull() == 0)
    strcpy(data->date2_high, json["results"][0]["daily"][2]["high"]);

  if (json["results"][0]["daily"][0]["low"].isNull() == 0)             //最低温度
    strcpy(data->date0_low, json["results"][0]["daily"][0]["low"]);
  if (json["results"][0]["daily"][1]["low"].isNull() == 0)
    strcpy(data->date1_low, json["results"][0]["daily"][1]["low"]);
  if (json["results"][0]["daily"][2]["low"].isNull() == 0)
    strcpy(data->date2_low, json["results"][0]["daily"][2]["low"]);

  if (json["results"][0]["daily"][0]["humidity"].isNull() == 0)                //湿度
    strcpy(data->date0_humidity, json["results"][0]["daily"][0]["humidity"]);

  if (json["results"][0]["daily"][0]["wind_scale"].isNull() == 0)        //风力等级
    strcpy(data->date0_wind_scale, json["results"][0]["daily"][0]["wind_scale"]);
  // 这不是强制复制，你可以使用指针，因为他们是指向"内容"缓冲区内，所以你需要确保
  // 当你读取字符串时它仍在内存中
  return true;
}
/**
 * 解析当前天气
*/
bool ParseActualWeather(String content, struct ActualWeather* data)
{
  DynamicJsonDocument json(1536); //分配内存,动态
  DeserializationError error = deserializeJson(json, content); //解析json
  //serializeJson(json, Serial);//构造序列化json,将内容从串口输出
  if (error)
  {
    Serial.print("实况天气加载json配置失败:");
    Serial.println(error.c_str());
    Serial.println(" ");
    String z = "实况天气json配置失败:" + String(error.c_str()) + " " + content;
    //display_partialLine(7, z);
    //    esp_sleep(0);
    return false;
  }

  //检查API是否有返回错误信息，有返回则进入休眠
  if (json["status_code"].isNull() == 0) //检查到不为空
  {
    strcpy(data->status_code, json["status_code"]);
    String z;
    if (String(actual.status_code) == "AP010001") z = "API 请求参数错误" ;
    else if (String(actual.status_code) == "AP010002") z = "没有权限访问这个 API 接口" ;
    else if (String(actual.status_code) == "AP010003") z = "API 密钥 key 错误" ;
    else if (String(actual.status_code) == "AP010004") z = "签名错误" ;
    else if (String(actual.status_code) == "AP010005") z = "你请求的 API 不存在" ;
    else if (String(actual.status_code) == "AP010006") z = "没有权限访问这个地点" ;
    else if (String(actual.status_code) == "AP010007") z = "JSONP 请求需要使用签名验证方式" ;
    else if (String(actual.status_code) == "AP010008") z = "没有绑定域名" ;
    else if (String(actual.status_code) == "AP010009") z = "API 请求的 user-agent 与你设置的不一致" ;
    else if (String(actual.status_code) == "AP010010") z = "没有这个地点";
    else if (String(actual.status_code) == "AP010011") z = "无法查找到指定 IP 地址对应的城市" ;
    else if (String(actual.status_code) == "AP010012") z = "你的服务已经过期" ;
    else if (String(actual.status_code) == "AP010013") z = "访问量余额不足" ;
    else if (String(actual.status_code) == "AP010014") z = "访问频率超过限制" ;
    else if (String(actual.status_code) == "AP010015") z = "暂不支持该城市的车辆限行信息" ;
    else if (String(actual.status_code) == "AP010016") z = "暂不支持该城市的潮汐数据" ;
    else if (String(actual.status_code) == "AP010017") z = "请求的坐标超出支持的范围" ;
    else if (String(actual.status_code) == "AP100001") z = "心知系统内部错误：数据缺失" ;
    else if (String(actual.status_code) == "AP100002") z = "心知系统内部错误：数据错误" ;
    else if (String(actual.status_code) == "AP100003") z = "心知系统内部错误：服务内部错误" ;
    else if (String(actual.status_code) == "AP100004") z = "心知系统内部错误：网关错误" ;
    else z = "实况天气异常:" + String(actual.status_code);
    //display_partialLine(7, z);
    Serial.print("实况天气异常:"); Serial.println(actual.status_code);
    //esp_sleep(60);
  }

  // 复制我们感兴趣的字符串 ,先检查是否为空，空会导致系统无限重启
  // isNull()检查是否为空 空返回1 非空0
  if (json["results"][0]["location"]["name"].isNull() == 0)
    strcpy(data->city, json["results"][0]["location"]["name"]);
  if (json["results"][0]["now"]["text"].isNull() == 0)
    strcpy(data->weather_name, json["results"][0]["now"]["text"]);
  if (json["results"][0]["now"]["code"].isNull() == 0)
    strcpy(data->weather_code, json["results"][0]["now"]["code"]);
  if (json["results"][0]["now"]["temperature"].isNull() == 0)
    strcpy(data->temp, json["results"][0]["now"]["temperature"]);
  if (json["results"][0]["last_update"].isNull() == 0)
    strcpy(data->last_update, json["results"][0]["last_update"]);
  // 这不是强制复制，你可以使用指针，因为他们是指向"内容"缓冲区内，所以你需要确保
  // 当你读取字符串时它仍在内存中

  return true;
}
String callHttps(String url)
{
  String payload;
  Serial.print("requesting URL: ");
  Serial.println(url);
  HTTPClient https;
  if (https.begin(url))
  {
    int httpsCode = https.GET();
    if (httpsCode > 0)  //判断有无返回值
    {
      /*payload = https.getString();
        Serial.println(payload);
        return payload;*/
      if (httpsCode == 200 || httpsCode == 304 || httpsCode == 403 || httpsCode == 404 || httpsCode ==   500) //判断请求是正确
      {
        payload = https.getString();
        //        Serial.println(payload);
        Serial.println(" ");
        return payload;
      }
      else
      {
        Serial.print("请求错误："); Serial.println(httpsCode); Serial.println(" ");
        char* httpsCode_c = new char[8];
        itoa(httpsCode, httpsCode_c, 10); //int转char*
        payload = "{\"status_code\":\"" + String("请求错误:") + String(httpsCode_c) + "\"}";
        return payload;
      }
    }
    else
    {
      Serial.println(" "); Serial.print("GET请求错误："); Serial.println(httpsCode);
      Serial.printf("[HTTPS] GET... 失败, 错误: %s\n", https.errorToString(httpsCode).c_str());
      payload = "{\"status_code\":\"" + String(https.errorToString(httpsCode).c_str()) + "\"}";
      //Serial.println(payload);
      return payload;
    }
  }
  else
  {
    Serial.printf("[HTTPS] 无法连接服务器\n");
    payload = "{\"status_code\":\"" + String("无法连接服务器") + "\"}";
    return payload;
  }
  https.end();
}