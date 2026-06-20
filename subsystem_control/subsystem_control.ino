/**
 * SUBSYSTEM CONTROLS - STARTER SKETCH
 * Target Board: Raspberry Pi Pico 2 W (or Pico 2 / Pico W)
 * IDE Compatibility: Arduino IDE (with Arduino Mbed OS RP2040/RP2350 Boards core or Earle Philhower's Pico core)
 * 
 * Core Feature:
 * - Continuously monitors a safety toggle switch on GP14 (Active-Low).
 * - When Armed (switch closed / GP14 connected to GND): Blinks the built-in status LED at 500ms intervals.
 * - When Disarmed (switch open / GP14 pulled HIGH internally): Turns off the built-in status LED.
 */

// Pin Definitions
const int SAFETY_SWITCH_PIN = 14; // GPIO Pin for the safety toggle switch
// Note: LED_BUILTIN is pre-defined by the Arduino Pico core.
// For the Pico W / Pico 2 W, this typically routes through the wireless module's GPIO.

// Timing & Blinking Configuration
const unsigned long BLINK_INTERVAL = 500; // Blink interval in milliseconds (500ms ON / 500ms OFF)
unsigned long lastBlinkTime = 0;          // Stores the last time the LED was toggled
bool ledState = LOW;                      // Current state of the status LED

void setup() {
  // Initialize Serial communication for debug logging
  Serial.begin(115200);
  
  // Initialize the built-in LED as an output
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  // Initialize the safety toggle switch with the internal pull-up resistor
  // Switch closed (Armed) = LOW (GND)
  // Switch open (Disarmed) = HIGH (Internal Pull-Up)
  pinMode(SAFETY_SWITCH_PIN, INPUT_PULLUP);
  
  Serial.println("Subsystem Control Brain Initialized.");
}

void loop() {
  // Read the state of the safety switch (LOW = closed/Armed, HIGH = open/Disarmed)
  bool isArmed = (digitalRead(SAFETY_SWITCH_PIN) == LOW);
  
  if (isArmed) {
    // System is ARMED: Blink status LED using non-blocking timing
    unsigned long currentMillis = millis();
    
    if (currentMillis - lastBlinkTime >= BLINK_INTERVAL) {
      lastBlinkTime = currentMillis;
      ledState = !ledState; // Toggle LED state
      digitalWrite(LED_BUILTIN, ledState);
      
      // Print debug telemetry
      Serial.print("System State: ARMED | LED: ");
      Serial.println(ledState ? "ON" : "OFF");
    }
  } else {
    // System is DISARMED: Ensure LED is OFF
    if (ledState != LOW) {
      digitalWrite(LED_BUILTIN, LOW);
      ledState = LOW;
      Serial.println("System State: DISARMED | LED: OFF");
    }
  }
  
  // Minor delay to prevent loop from running too fast (limits power consumption / polling overhead)
  delay(10);
}
