#define RELAY  0                        
void setup()

{    


Serial.begin(9600);
  pinMode(RELAY, OUTPUT);       

}

  void loop()

{

   digitalWrite(RELAY,0);           // Turns ON Relays 1
   Serial.println("Light ON");
   delay(2000);                                      // Wait 2 seconds

   digitalWrite(RELAY,1);          // Turns Relay Off
   Serial.println("Light OFF");
   delay(2000);
   
}