RTC_DATA_ATTR uint16_t arrindex = 0;
/**
 * @brief Get the Variance object
 * 获取气压变化的强度
 * @param val 气压值
 * @return float 
 */
float getVariance(float val){
    float r=0;
    arrindex++;
    arr[arrindex-1]=val;
    Serial.print("arrindex数值：");Serial.println(arrindex);
    if(arrindex>=16)arrindex=0;
    float s=0;
    for(int i=0;i<arrindex;i++){
        Serial.print("气压数据记录：");Serial.println(arr[i]);
        s+=arr[i];
    }
    float avg=s*1.0/arrindex;
    for(int i=0;i<arrindex;i++){
        r=r+pow((arr[i]-avg),2);
    }
    Serial.print("实时气压方差：");Serial.println(r);
    return r;
}