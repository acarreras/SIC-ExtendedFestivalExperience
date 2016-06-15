

// Analog pins connected to photocell under each color.
const int colorBlue = A0;  // Blue
const int colorGreen = A4;  // Green


void setup() {
  // initialize serial communications 
  Serial.begin(9600);
}

void loop() {

  
  int sensorValueBlue = analogRead(colorBlue);
  int sensorValueGreen = analogRead(colorGreen);

  
  if (sensorValueBlue<200)
  {
    // Blue selected
    //Serial.println('b');
    Serial.write('b');
   }
   else if (sensorValueGreen<200)
   {
    Serial.write('g');
    }

   else
   {
    Serial.write('0');
    }


    // recognize lift, and send one message
    
  // print the results to the serial monitor:
  //Serial.print("Blue = ");
  //Serial.print(sensorValueBlue);
  //Serial.print("\t Green = ");
  //Serial.println(sensorValueGreen);




  // wait 2 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
  delay(500);
}
