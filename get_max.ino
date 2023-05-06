// get maximum reading value
void get_max() {
  
  if (i_get_max<100)
  {
    r = analogRead(Vac); 
    if(max_v <= r) max_v = r;
    i_get_max++;
  }else if(i_get_max=100)
  {
    i_get_max=0;
    char buf[10];
    double v = max_v;
    v = v * 1100/4096;
    v /= sqrt(2);
    dtostrf(v, 1, 2, buf);
    //sprintf(buf, "%03u Volts", v);
    client.publish(MQTT_SERIAL_PUBLISH_CH_vac, buf);
    max_v=0;
    r=0;
  }

}
