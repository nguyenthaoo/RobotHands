#include <Arduino.h>
#include <Servo.h>

#define C_MIN_TEMP 20
#define C_MAX_TEMP 50
#define C_MIN_DELAY 10
#define C_MAX_DELAY 500
#define P_TEMPSENSOR_OUT 2
#define P_TOUCHSENSOR_IN 4
#define P_TOUCHENSOR_OUT 5
#define P_SERVO_OUT 7
#define ENABLE_SERVO false
const int tempSensorPin = A0;
Servo servoindex; // Define index servo

// put function declarations here:
int myFunction(int, int);
float convertToC(int);
double convertToF(double);
int getDelayScale(float);
void alltomiddle();
void alltorest();
void alltomax();
void setup()
{
  // put your setup code here, to run once:
  pinMode(P_TEMPSENSOR_OUT, OUTPUT);
  pinMode(P_TOUCHENSOR_OUT, OUTPUT);
  pinMode(P_TOUCHSENSOR_IN, INPUT);
  if (ENABLE_SERVO)
    servoindex.attach(P_SERVO_OUT);
  Serial.begin(9600);
}

void loop()
{
  // put your main code here, to run repeatedly:
  // blink LED
  static bool state = false;
  static int count = 0;
  static int lastTouch = 0;
  static int lastTempMeasurement = 0;
  // temp sensor measurement
  int tempSensorMeasurement = 0;
  float tempC;
  double tempC2;
  char printing_buffer[64] = {0};
  int touchValue = digitalRead(P_TOUCHSENSOR_IN);
  // blink

  // temp sensor read
  tempSensorMeasurement = analogRead(tempSensorPin);
  tempC = convertToC(tempSensorMeasurement);
  tempC2 = (double)tempSensorMeasurement * (5 / 10.24);

  if (lastTempMeasurement != tempSensorMeasurement)
  {
    Serial.print("Temp: ");
    Serial.print(tempC2);
    Serial.print(" C ");
    Serial.print(convertToF(tempC2));
    Serial.println(" F ");
    // Serial.println(tempC);
  }
  lastTempMeasurement = tempSensorMeasurement;
  // touch sensor read
  // check if the sensor is trigger
  if (touchValue == HIGH)
  {
    // Serial.println("Touch detected!");
    if (lastTouch == LOW)
    {
      count += 1;
      snprintf(printing_buffer, sizeof(printing_buffer), "Touch number:%d\n", count);
      Serial.println(printing_buffer);
    }

    // blink the light
    digitalWrite(P_TOUCHENSOR_OUT, state);
    delay(5);
    state = !state;
    digitalWrite(P_TEMPSENSOR_OUT, state);
  }
  else
  {
    digitalWrite(P_TOUCHENSOR_OUT, false);
    digitalWrite(P_TEMPSENSOR_OUT, false);
  }
  lastTouch = touchValue;

  delay(C_MIN_DELAY + getDelayScale(tempC));

  // alltomiddle();        // Example: alltovirtual
  // delay(4000);           // Wait 4000 milliseconds (4 seconds)
  if (ENABLE_SERVO)
  {
    alltorest(); // Uncomment to use this
    delay(4000); // Uncomment to use this
    alltomax();  // Uncomment to use this
    delay(2000); // Uncomment to use this
  }
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
double convertToF(double ctemp)
{
  return (ctemp * 9 / 5) + 32;
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

// Motion to set the servo into "virtual" 0 position: alltovirtual
void alltomiddle()
{
  servoindex.write(90);
}
// Motion to set the servo into "rest" position: alltorest
void alltorest()
{
  servoindex.write(0);
}
// Motion to set the servo into "max" position: alltomax
void alltomax()
{
  servoindex.write(180);
}