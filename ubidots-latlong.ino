// This #include statement was automatically added by the Particle IDE.
#include <Ubidots.h>

// This example sends data to a variable along with hardcoded 
// GPS coordinates to Ubidots using Particle Webhooks.

/****************************************
 * Include Libraries
 ****************************************/

#include "Ubidots.h"

/****************************************
 * Define Instances and Constants
 ****************************************/

const char* WEBHOOK_NAME = "Ubidots";

Ubidots ubidots("webhook", UBI_PARTICLE);


/****************************************
 * Auxiliar Functions
 ****************************************/

//Put here your auxiliar functions


/****************************************
 * Main Functions
 ****************************************/

void setup() {
  Serial.begin(115200);
  ubidots.setDebug(true);  // Uncomment this line for printing debug messages
}

void loop() {
  float value = analogRead(A0);

  /* Harcoded Coordinates */
  float latitude = 37.773;
  float longitude = -6.2345;

  /* Reserves memory to store context keys values, add as much as needed */
  char* str_lat = (char *) malloc(sizeof(char) * 10);
  char* str_lng = (char *) malloc(sizeof(char) * 10);

  /* Saves as char the coordinates */
  sprintf(str_lat, "%f", latitude);
  sprintf(str_lng, "%f", longitude);

  /* Reserves memory to store context array */
  char* context = (char *) malloc(sizeof(char) * 50);

  /* Adds context key-value pairs */
  ubidots.addContext("lat", str_lat);
  ubidots.addContext("lng", str_lng);

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
  free(str_lat);
  free(str_lng);
  free(context);

  delay(5000);
}