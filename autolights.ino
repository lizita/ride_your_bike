// This #include statement was automatically added by the Particle IDE.
#include <TimeLord.h>

// Example by Nick Gammon
// http://forum.arduino.cc/index.php?topic=129249.msg972860#msg972860

// what is our longitude (west values negative) and latitude (south values negative)
#define RELAY  0
float const LONGITUDE = 105.082565;
float const LATITUDE = 40.573372;
unsigned char year =  2019;
byte today[] = {  0, 0, 12, 12, 12, year}; // store today's date (at noon) in an array for TimeLord to use
TimeLord tardis; 
  
void setup()
  {
  Serial.begin (115200);
  Serial.begin (9600);
  
  pinMode(RELAY, OUTPUT); 
  
  tardis.TimeZone(7 * 60); // tell TimeLord what timezone your RTC is synchronized to. You can ignore DST
  // as long as the RTC never changes back and forth between DST and non-DST
  tardis.Position(LATITUDE, LONGITUDE); // tell TimeLord where in the world we are

  Time.zone(-7); // tells particle what time zone we're in
  
  }
void loop() {
    
   digitalWrite(RELAY,1);
   Serial.println("Light ON");

if (tardis.SunRise(today)) // if the sun will rise today (it might not, in the [ant]arctic)
   {
   Serial.print("Sunrise hour: ");
   Serial.println((int) today[tl_hour]);
   Serial.print("Current hour: ");
   Serial.println(Time.hour());
   }
   
  {  if (Time.hour() < 12 && Time.hour() > (today[tl_hour])) 
  {
   digitalWrite(RELAY,0);           // Turns OFF Relay
   Serial.println("Light OFF");
   }
  }
   
   
   if (tardis.SunSet(today)) // if the sun will set today (it might not, in the [ant]arctic)
   {
   Serial.print("Sunset hour: ");
   Serial.println((int) today[tl_hour]);
   Serial.print("Current hour: ");
   Serial.println(Time.hour());
   }
   
  {  if (Time.hour() > 12 && Time.hour() < (today[tl_hour]))
  {
   digitalWrite(RELAY,0);           // Turns OFF Relay
   Serial.println("Light OFF");
   }
  }

      Particle.syncTime();

      delay(5000);
}