/* 
 * PROJECT: 12kW Aircraft Plating Bath Controller
 * CALIBRATION: PT100 Voltage Divider @ 8MHz
 * OFF TARGET: 80°C (Measured ADC: 442)
 */

const int PIN_TEMP  = A0; // Physical Pin 23
const int PIN_LEVEL = 2;  // Physical Pin 32
const int PIN_SSR   = 11; // Physical Pin 15 (PB3)

// --- FINAL MRO CALIBRATION ---
// Logic: If ADC drops BELOW 445, it is HOT -> Turn OFF
//        If ADC rises ABOVE 470, it is COLD -> Turn ON
const int SETPOINT_ON  = 470; 
const int SETPOINT_OFF = 445; 

void setup() {
  Serial.begin(4800); 
  pinMode(PIN_LEVEL, INPUT);
  pinMode(PIN_SSR, OUTPUT);
  digitalWrite(PIN_SSR, LOW); // Safe initialization
  
  Serial.println(F("================================"));

  Serial.println(F("SYSTEM STATUS: RUNNING"));
  Serial.println(F("================================"));
}

void loop() {
  int adc = analogRead(PIN_TEMP);
  bool waterOk = (digitalRead(PIN_LEVEL) == HIGH);
  static bool heaterState = false;

  // SAFETY & THERMAL INTERLOCK
  if (!waterOk) {
    heaterState = false; // Emergency: Low level
  } else {
    // Temperature Logic (Inverse curve)
    if (adc > SETPOINT_ON)  heaterState = true;  // Tank is COLD
    if (adc < SETPOINT_OFF) heaterState = false; // Tank is HOT
  }

  // DRIVE TRANSISTOR
  digitalWrite(PIN_SSR, heaterState ? HIGH : LOW);

  // TELEMETRY
  Serial.print(F("BATH_ADC: ")); Serial.print(adc);
  Serial.print(F(" | LVL: "));    Serial.print(waterOk ? "SAFE" : "LOW ");
  Serial.print(F(" | HEATER: ")); Serial.println(heaterState ? "ACTIVE" : "IDLE");

  delay(400); 
}