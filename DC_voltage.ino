void DC_voltage()
{
  float dc=analogRead(Vdc);
  dc=dc*3.3/4096;
  dc*=4.3;
  //vout=vin*10/43
  //vin=vout*4.3
  char voltage_dc[8];
  dtostrf(dc, 1, 2, voltage_dc);
  client.publish(MQTT_SERIAL_PUBLISH_CH_vdc, voltage_dc);
}
