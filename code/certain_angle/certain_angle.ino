// —— 规格参数 ——
const int SERVO_PIN    = A3;
const int PWM_FREQ     = 333;
const int PWM_RES_BITS = 14;

// —— 微调参数 ——
// 25T摆臂半个齿 ≈ 7.2°，对应约78µs
// 摆臂偏右用负值，偏左用正值，根据实际微调
float TRIM_US = 0.0;

// —— 由规格算出的常量 ——
const int   NEUTRAL_US = 1520;
const int   MIN_US     = 500;
const int   MAX_US     = 2500;
const int   SPAN_POS   = MAX_US - NEUTRAL_US;   // 980
const int   SPAN_NEG   = NEUTRAL_US - MIN_US;   // 1020
const long  PERIOD_US  = 1000000L / PWM_FREQ;   // 3003
const uint32_t MAX_DUTY = (1UL << PWM_RES_BITS) - 1; // 16383

// —— 校准后的中位脉宽 ——
const float CALIBRATED_NEUTRAL_US = NEUTRAL_US + TRIM_US;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  analogWriteResolution(PWM_RES_BITS);
  analogWriteFrequency(PWM_FREQ);

  // 归位到 0°
  writeServoCentered(-90);
}

void loop() {
  // 保持不动
}

void writeServoCentered(int angle) {
  // 限幅到 [-90, +90]
  int a = constrain(angle, -90, +90);

  // 计算脉宽（µs）
  float pulse_us;
  if (a >= 0) {
    pulse_us = CALIBRATED_NEUTRAL_US + (SPAN_POS * a) / 90.0;
  } else {
    pulse_us = CALIBRATED_NEUTRAL_US + (SPAN_NEG * a) / 90.0;
  }

  // 安全限幅
  if (pulse_us < MIN_US) pulse_us = MIN_US;
  if (pulse_us > MAX_US) pulse_us = MAX_US;

  Serial.print("Set Angle: ");
  Serial.print(a);
  Serial.print("°, Pulse: ");
  Serial.print(pulse_us, 1);
  Serial.println(" µs");

  // 脉宽 → 占空比
  uint32_t duty = (uint32_t)((pulse_us * (float)MAX_DUTY + PERIOD_US / 2.0) / PERIOD_US);

  // 输出 PWM
  analogWrite(SERVO_PIN, duty);
}