// Code combines from various tutorials and examples for SOCR final project - ride_your_bike

/*  Adapted from SparkFun ACS712 and ACS723 Demo by George Beckstein for SparkFun */             //start of gps code
// This #include statement was automatically added by the Particle IDE.
#include <ParticleSoftSerial.h>

// This #include statement was automatically added by the Particle IDE.
#include <TinyGPS++.h>

// This #include statement was automatically added by the Particle IDE.
#include <TinyGPS.h>


/* This sample code demonstrates the normal use of a TinyGPS object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 18(rx) and 19(tx).
*/

TinyGPS gps;
SoftwareSerial ss(18, 19);  //gps pins

static void smartdelay(unsigned long ms);
static void print_float(float val, float invalid, int len, int prec);
static void print_int(unsigned long val, unsigned long invalid, int len);
static void print_date(TinyGPS &gps);
static void print_str(const char *str, int len);                                                    //end of gps code


#define LEDPIN D3         //LED brightness (PWM) writing                                            //start of light sensor code
#define LIGHTSENSORPIN A3 //Ambient light sensor reading pin
#define publish_delay 30000                                                                         //end of light sensor code


const int analogInPin = A0;  //current sensor data in pin                                           //start of current sensor code

const int avgSamples = 15;  // Number of samples to average the reading over

int sensorValue = 0;
unsigned int lastPublish = 0;

float sensitivity = 30.0 / 35.0; //30mA per 50mV = 0.6
float Vref = 6600; // Output voltage with no current: ~ 2500mV or 2.5V                              //end of current sensor code

void setup() {
  // initialize serial communications at 9600 bps:                                                  //applies to all
  Serial.begin(9600);
 
 
  pinMode(LIGHTSENSORPIN, INPUT);                                                                   //light sensor
  pinMode(LEDPIN, OUTPUT); 
  
  Serial.begin(115200);                                                                             //start gps code

  Serial.println("Sats HDOP Latitude  Longitude  Fix  Date       Time     Date Alt    Course Speed Card  Distance Course Card  Chars Sentences Checksum");
  Serial.println("          (deg)     (deg)      Age                      Age  (m)    --- from GPS ----  ---- to London  ----  RX    RX        Fail");
  ss.begin(9600);
}                                                                                                   // end of setup loop

void loop() {                                                                                       //start void loop
  // read the analog in value:                                                                      //start current sensor code
for (int i = 0; i < avgSamples; i++)
{
    sensorValue += analogRead(analogInPin);

    // wait 2 milliseconds before the next loop
    // for the analog-to-digital converter to settle
    // after the last reading:
    delay(100);

  sensorValue = sensorValue / avgSamples;

  // The on-board ADC is 10-bits -> 2^10 = 1024 -> 5V / 1024 ~= 4.88mV  3.3V / 1024 = ~= 3.22mV
  // The voltage is in millivolts
  float voltage = 3.22 * sensorValue;

  // This will calculate the actual current (in mA)
  // Using the Vref and sensitivity settings you configure
  float current = (voltage - Vref) * sensitivity;

  Serial.print(current);
  Serial.print("mA, ");                                                                             //end current sensor code
  }


  {
  float reading = analogRead(LIGHTSENSORPIN); //Read light level                                    //start light sensor code
  float square_ratio = reading / 1023.0;      //Get percent of maximum value (1023)
  //square_ratio = pow(square_ratio, 2.0);      //Square to make response more obvious

  analogWrite(LEDPIN, 255.0 * square_ratio);  //Adjust LED brightness relatively
  Serial.println(reading);                    //Display reading in serial monitor                   //end light sensor code
  }
  
   unsigned long now = millis();                                                                   //start webhook code
    if ((now - lastPublish) < publish_delay) {
        return;
    }

    int value = analogRead(A0);                                                                     
    Particle.publish("thingSpeakWrite_A0", "{ \"1\": \"" + String(value) + "\", \"k\": \"********************\" }", 60, PRIVATE);
    lastPublish = now;
    

    
  // -- DO NOT UNCOMMENT BELOW THIS LINE --
  Serial.print("\n");

  // Reset the sensor value for the next reading
  sensorValue = 0;


                                                                                                   //start gps code

  float flat, flon;
  unsigned long age, date, time, chars = 0;
  unsigned short sentences = 0, failed = 0;
  static const double LONDON_LAT = 51.508131, LONDON_LON = -0.128002;
  
  print_int(gps.satellites(), TinyGPS::GPS_INVALID_SATELLITES, 5);
  print_int(gps.hdop(), TinyGPS::GPS_INVALID_HDOP, 5);
  gps.f_get_position(&flat, &flon, &age);
  print_float(flat, TinyGPS::GPS_INVALID_F_ANGLE, 10, 6);
  print_float(flon, TinyGPS::GPS_INVALID_F_ANGLE, 11, 6);
  print_int(age, TinyGPS::GPS_INVALID_AGE, 5);
  print_date(gps);
  print_float(gps.f_altitude(), TinyGPS::GPS_INVALID_F_ALTITUDE, 7, 2);
  print_float(gps.f_course(), TinyGPS::GPS_INVALID_F_ANGLE, 7, 2);
  print_float(gps.f_speed_kmph(), TinyGPS::GPS_INVALID_F_SPEED, 6, 2);
  print_str(gps.f_course() == TinyGPS::GPS_INVALID_F_ANGLE ? "*** " : TinyGPS::cardinal(gps.f_course()), 6);
  print_int(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0xFFFFFFFF : (unsigned long)TinyGPS::distance_between(flat, flon, LONDON_LAT, LONDON_LON) / 1000, 0xFFFFFFFF, 9);
  print_float(flat == TinyGPS::GPS_INVALID_F_ANGLE ? TinyGPS::GPS_INVALID_F_ANGLE : TinyGPS::course_to(flat, flon, LONDON_LAT, LONDON_LON), TinyGPS::GPS_INVALID_F_ANGLE, 7, 2);
  print_str(flat == TinyGPS::GPS_INVALID_F_ANGLE ? "*** " : TinyGPS::cardinal(TinyGPS::course_to(flat, flon, LONDON_LAT, LONDON_LON)), 6);

  gps.stats(&chars, &sentences, &failed);
  print_int(chars, 0xFFFFFFFF, 6);
  print_int(sentences, 0xFFFFFFFF, 10);
  print_int(failed, 0xFFFFFFFF, 9);
  Serial.println();
  
  smartdelay(1000);
}

static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

static void print_float(float val, float invalid, int len, int prec)
{
  if (val == invalid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(' ');
  }
  smartdelay(0);
}

static void print_int(unsigned long val, unsigned long invalid, int len)
{
  char sz[32];
  if (val == invalid)
    strcpy(sz, "*******");
  else
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i=strlen(sz); i<len; ++i)
    sz[i] = ' ';
  if (len > 0) 
    sz[len-1] = ' ';
  Serial.print(sz);
  smartdelay(0);
}

static void print_date(TinyGPS &gps)
{
  int year;
  byte month, day, hour, minute, second, hundredths;
  unsigned long age;
  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
  if (age == TinyGPS::GPS_INVALID_AGE)
    Serial.print("********** ******** ");
  else
  {
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d %02d:%02d:%02d ",
        month, day, year, hour, minute, second);
    Serial.print(sz);
  }
  print_int(age, TinyGPS::GPS_INVALID_AGE, 5);
  smartdelay(0);
}

static void print_str(const char *str, int len)
{
  int slen = strlen(str);
  for (int i=0; i<len; ++i)
    Serial.print(i<slen ? str[i] : ' ');
  smartdelay(0);
}                                                                                                   //end gps code

  
