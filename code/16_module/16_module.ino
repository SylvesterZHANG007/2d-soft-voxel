#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);

#define PWM_FREQ    333
#define SERVO_MIN   682
#define SERVO_MAX   3412
#define NUM_SERVOS  16

int servoAngles[NUM_SERVOS] = {0};

#define CENTER_ANGLE  -45.0
#define AMPLITUDE      45.0
#define FREQUENCY       1.0

const int8_t servoSign[NUM_SERVOS] = {
   1,  1,  -1,  -1,
  1, 1, -1, -1,
   -1,  -1,  1,  1,
  -1, -1, 1, 1
};

bool initialized = false;
unsigned long startTime = 0;

uint16_t angleToPulse(int angle) {
  return map(angle, -90, 90, SERVO_MIN, SERVO_MAX);
}

void setServo(uint8_t channel, int angle) {
  angle = constrain(angle, -90, 90);
  servoAngles[channel] = angle;
  pwm.setPWM(channel, 0, angleToPulse(angle));
}

void setup() {
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(PWM_FREQ);
  delay(10);

  for (uint8_t i = 0; i < NUM_SERVOS; i++) {
    setServo(i, -45);
  }

  delay(2000);  // hold -45° for 2 seconds right here

  startTime = millis();
}

void loop() {
  float t = (millis() - startTime) / 1000.0;
  float wave = sin(2.0 * PI * FREQUENCY * t);

  for (uint8_t i = 0; i < NUM_SERVOS; i++) {
    float angle = CENTER_ANGLE + AMPLITUDE * servoSign[i] * wave;
    setServo(i, (int)angle);
  }

  delay(3);
}