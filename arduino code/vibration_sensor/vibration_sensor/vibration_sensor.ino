//defines the pin connections
int GroundPin= 0;
int sensePin= 1;
int LEDPin= 13;

//defines normal and threshold voltage levels
int threshold= 500;

//sets GroundPin and LEDPin as output pins, with GroundPin being set to LOW
void setup()
{
Serial.begin(9600);
pinMode(GroundPin, OUTPUT);
digitalWrite(GroundPin, LOW);
pinMode(LEDPin, OUTPUT);
}

//if the reading is higher than the threshold value, then the LED is turned on
void loop()
{
int reading= analogRead(sensePin);
Serial.println(reading);
if (reading > threshold)
{
digitalWrite(LEDPin, HIGH);
Serial.println("on");
delay(1000);
digitalWrite(LEDPin, LOW);
}
}
