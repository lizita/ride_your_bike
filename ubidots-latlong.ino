//Code modified from a combination of Ubidots GPS webhooks tutorial and TinyGPS library example

#include "Ubidots.h"

// This #include statement was automatically added by the Particle IDE.
#include <ParticleSoftSerial.h>

// This #include statement was automatically added by the Particle IDE.
#include <TinyGPS.h>


//Define Instances and Constants
const char* WEBHOOK_NAME = "Ubidots";

Ubidots ubidots("webhook", UBI_PARTICLE);

TinyGPS gps;
SoftwareSerial ss(18, 19);

static void smartdelay(unsigned long ms);
static void print_float(float val, float invalid, int len, int prec);
static void print_int(unsigned long val, unsigned long invalid, int len);
static void print_date(TinyGPS &gps);
static void print_str(const char *str, int len);


void setup() {
  Serial.begin(115200);
  ss.begin(9600);
  ubidots.setDebug(true);  // Uncomment this line for printing debug messages
}

void loop() {
  float flat, flon;
  gps.f_get_position(&flat, &flon);
  print_float(flat, TinyGPS::GPS_INVALID_F_ANGLE, 10, 6);
  print_float(flon, TinyGPS::GPS_INVALID_F_ANGLE, 11, 6);
  Serial.print(flat);
  Serial.print(", ");
  Serial.print(flon); 
  Serial.println();

  float value = analogRead(A0);

  /* Harcoded Coordinates */
  float latitude = flat;
  float longitude = flon;

  /* Reserves memory to store context keys values, add as much as needed */
  char* lat = (char *) malloc(sizeof(char) * 10);
  char* lon = (char *) malloc(sizeof(char) * 10);
  
  /* Saves as char the coordinates */
  sprintf(lat, "%f", latitude);
  sprintf(lon, "%f", longitude);

  /* Reserves memory to store context array */
  char* context = (char *) malloc(sizeof(char) * 50);
  
  /* Adds context key-value pairs */
  ubidots.addContext("lat", lat);
  ubidots.addContext("lng", lon);

  /* Builds the context with the coordinates to send to Ubidots */
  ubidots.getContext(context);

  /* Sends the position */
  ubidots.add("position", value, context); // Change for your variable name

  bool bufferSent = false;
  bufferSent = ubidots.send(WEBHOOK_NAME, PUBLIC);  // Will use particle webhooks to send data

  if(bufferSent){
    // Do something if values were sent properly
    Serial.println("Values sent by the device");
  }

  /* frees memory */
  free(lat);
  free(lon);
  free(context);

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
