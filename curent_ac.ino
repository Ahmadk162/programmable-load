void ac_curent()
{
   if((millis()-start_time) < 1000) //sample for 1 Sec
   {
       readValue = analogRead(sensorIn);
       // see if you have a new maxValue
       if (readValue > maxValue)
       {
           /*record the maximum sensor value*/
           maxValue = readValue;
       }
       if (readValue < minValue)
       {
           /*record the minimum sensor value*/
           minValue = readValue;
       }
   }else
    if((millis()-start_time) > 1000)
    {
           // Subtract min from max
     Voltage = ((maxValue - minValue) * 3.3)/4096.0;
        
     VRMS = (Voltage/2.0) *0.707;   //root 2 is 0.707
     AmpsRMS = (VRMS * 1000)/mVperAmp;
     Serial.print("AC amper= ");
     Serial.println(AmpsRMS);
      maxValue = 0;             // store max value here
      minValue = 1024;          // store min value here
      start_time = millis();
      char curent_ac[8];
      dtostrf(AmpsRMS, 1, 2, curent_ac);
      client.publish(MQTT_SERIAL_PUBLISH_CH_aac, curent_ac);
    }
  

}
