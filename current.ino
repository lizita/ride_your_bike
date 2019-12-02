{
  // read the analog in value:
  for (int i = 0; i < avgSamples; i++)
  {
    sensorValue += analogRead(analogInPin);

    // wait 2 milliseconds before the next loop
    // for the analog-to-digital converter to settle
    // after the last reading:
    delay(2);

  }

  sensorValue = sensorValue / avgSamples;

  // The on-board ADC is 10-bits -> 2^10 = 1024 -> 5V / 1024 ~= 4.88mV  3.3V / 1024 = ~= 3.22mV
  // The voltage is in millivolts
  float voltage = 3.22 * sensorValue;

  // This will calculate the actual current (in mA)
  // Using the Vref and sensitivity settings you configure
  float current = (voltage - Vref) * sensitivity;

  // This is the raw sensor value, not very useful without some calculations
  //Serial.print(sensorValue);

  /*************************************************************************************
   * Step 1.)
   * Uncomment and run the following code to set up the baseline voltage 
   * (the voltage with 0 current flowing through the device).
   * Make sure no current is flowing through the IP+ and IP- terminals during this part!
   * 
   * The output units are in millivolts. Use the Arduino IDE's Tools->Serial Plotter
   * To see a plot of the output. Adjust the Vref potentiometer to set the reference
   * voltage. This allows the sensor to output positive and negative currents!
   *************************************************************************************/

  //Serial.print(voltage);
  //Serial.print("mV");

  /*************************************************************************************
   * Step 2.)
   * Keep running the same code as above to set up the sensitivity
   * (how many millivolts are output per Amp of current.
   * 
   * This time, use a known load current (measure this with a multimeter)
   * to give a constant output voltage. Adjust the sensitivity by turning the
   * gain potentiometer.
   * 
   * The sensitivity will be (Vreading - Vref)/(known current).
   *************************************************************************************/

    /*************************************************************************************
   * Step 3.)
   * Comment out the code used for the last two parts and uncomment the following code.
   * When you have performed the calibration steps above, make sure to change the 
   * global variables "sensitivity" and "Vref" to what you have set up.
   * 
   * This next line of code will print out the calculated current from these parameters.
   * The output is in mA
   *************************************************************************************/

  Serial.print(current);
  Serial.print("mA");


  // -- DO NOT UNCOMMENT BELOW THIS LINE --
  Serial.print("\n");

  // Reset the sensor value for the next reading
  sensorValue = 0;
}