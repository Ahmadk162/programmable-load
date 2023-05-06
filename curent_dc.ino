void dc_curent() 
{
  // put your main code here, to run repeatedly:
  float current_1=0;
  for(int i=0; i<=49; i++)
  {
    float adc = analogRead(34);
    float voltage = adc*3.3/4096.0;
    float current = (voltage-1.65)/0.185;
    current_1+=current;   
  } 
    current_1/=50;
    char curent_dc[8];
    dtostrf(current_1, 1, 2, curent_dc);
    client.publish(MQTT_SERIAL_PUBLISH_CH_adc, curent_dc);
}
