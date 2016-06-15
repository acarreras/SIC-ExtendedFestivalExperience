/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://www.arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
 */
int led = 6;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(led, OUTPUT);
  Serial.begin(9600);
}

// the loop function runs over and over again forever
int t = 0;
bool active = false;
unsigned long activation = 0;
int brightness = 0;
int state = 0;

void loop() {
  if (Serial.available())
  {
    // read the most recent byte (which will be from 0 to 255):
    t = Serial.read();
    t = t - 48;
    Serial.print("turning on for ");
    Serial.print(t);
    Serial.println(" seconds.");
    active = true;
    brightness = 0;
    state = 0;

  }
  else
  {
    //t = 0;
    //active = false;
  }
  delay(10);


  //Serial.println(t);
  if (active)
  {
    switch (state)
    {
      case 0:
        // write brightness
        analogWrite(led, brightness);
        brightness += 5;
        delay(10);
        
        // update brightness

        if (brightness >= 255)
        {
          state = 1;
          activation = millis();
        }
        break;

      case 1: 
        if ( (millis() - activation) > (t*1000) )
        {
          state = 2;
        }
        break;

      case 2:
        brightness -= 5;
        delay(10);
        if (brightness < 0)
        {
          active = false;
          t = 0;
          brightness = 0;
          //activation = millis();
        }
        analogWrite(led, brightness);
        break;

    }

  Serial.println(brightness);
  }


}
