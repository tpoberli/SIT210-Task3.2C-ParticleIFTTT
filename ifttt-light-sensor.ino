// This #include statement was automatically added by the Particle IDE.
#include <JsonParserGeneratorRK.h>
// This #include statement was automatically added by the Particle IDE.
#include <BH1750.h>

bool already_on = false;
int count = 0;
BH1750 sensor(0x23, Wire);
void setup()
{
  sensor.begin();

  sensor.set_sensor_mode(BH1750::forced_mode_high_res2);

  Serial.begin();
  
}

void loop()
{
  sensor.make_forced_measurement();

  Serial.println(String::format("%f", sensor.get_light_level())); // for debugging purposes

  if(sensor.get_light_level() > 3000 && already_on == false)
  {
      already_on = true;
      createEventPayload(sensor.get_light_level(), count);
  }
  if(sensor.get_light_level() < 3000 && already_on == true)
  {
      already_on = false;
      createEventPayload(sensor.get_light_level(), count);
  }
  if(sensor.get_light_level() > 3000) //keep track of time in the sun
  {
      count++;
  }
  if(count == 5)//change to 120 (for 2 hours) to get an email showing 2 hours in sunlight has elapsed 
  {
      createEventPayload(sensor.get_light_level(), count);
  }
  delay(10000);//change to 60,000 to check sunlight every minute, otherwise will lead to excessive emails
}
//json loader
void createEventPayload(int light, int count){
    JsonWriterStatic<256> jw;
    {
        JsonWriterAutoObject obj(&jw);
        jw.insertKeyValue("light value: ", light);
        jw.insertKeyValue("Count value: ", count);
    }
    // Publish data to the Particle cloud. 
    Particle.publish("light", jw.getBuffer(), PRIVATE);
}
