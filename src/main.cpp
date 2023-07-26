#include <Arduino.h>

#define C_MIN_TEMP 20
#define C_MAX_TEMP 50
#define C_MIN_DELAY 10
#define C_MAX_DELAY 500
#define P_TEMPSENSOR_OUT 2
#define P_TOUCHSENSOR_IN 4
#define P_TOUCHENSOR_OUT 5

// put function declarations here:
int myFunction(int, int);
float convertToC(int);
int getDelayScale(float);
void setup()
{
  // put your setup code here, to run once:
  pinMode(P_TEMPSENSOR_OUT, OUTPUT);
  pinMode(P_TOUCHENSOR_OUT, OUTPUT);
  pinMode(P_TOUCHSENSOR_IN, INPUT);
  Serial.begin(9600);
}

const int tempSensorPin = A0;

void loop()
{
  // put your main code here, to run repeatedly:
  // blink LED
  static bool state = false;
  static int count = 0;
  // temp sensor measurement
  int tempSensorMeasurement = 0;
  float tempC;
  char printing_buffer[64] = {0};
  int touchValue = digitalRead(P_TOUCHSENSOR_IN);
  // blink

  // temp sensor read
  tempSensorMeasurement = analogRead(tempSensorPin);
  tempC = convertToC(tempSensorMeasurement);

  Serial.print("Temp:");
  Serial.println(tempC);
  // touch sensor read
  // check if the sensor is trigger
  if (touchValue == HIGH)
  {
    // Serial.println("Touch detected!");
    count += 1;
    snprintf(printing_buffer, sizeof(printing_buffer), "Touch number:%d\n", count);
    Serial.println(printing_buffer);
    // blink the light
    digitalWrite(P_TOUCHENSOR_OUT, state);
    state = !state;
  }
  digitalWrite(P_TEMPSENSOR_OUT, state);
  delay(C_MIN_DELAY + getDelayScale(tempC));
}

// LM 35 datasheet
// output 0 mV + 10.0 mV/°C => 0.01V/°C
// convet to V  value/1024*5   (0-2^10=0v-5v)
// then conver to C
// V*100
float convertToC(int value)
{
  return (value * 1.0f) / 1024 * 500.0;
}
// put function definitions here:
int myFunction(int x, int y)
{
  return x + y;
}

// the higher the tempture , the smaller the delay
// 1 - percent of temp in range min_temp, max_temp

int getDelayScale(float temp)
{
  int delay = (1 - (temp - C_MIN_TEMP) / (C_MAX_TEMP - C_MIN_TEMP)) * C_MAX_DELAY;
  return delay < 1 ? 1 : delay;
}