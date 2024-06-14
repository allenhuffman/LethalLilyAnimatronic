#define BLUE_WIRE_PIN   2   // Pulses IN

void setup() {
    Serial.begin(9600);
    while (!Serial);

    pinMode(BLUE_WIRE_PIN, INPUT);
    
    attachInterrupt(digitalPinToInterrupt(BLUE_WIRE_PIN), pinChangedISR, CHANGE);

    Serial.println ("Running...");
}

void loop() {
}

// This will be called any time the pin changes state.
void pinChangedISR()
{
    unsigned long time = 0;
    static unsigned long s_prevTime = 0;
  
    // Pin changed. Record the time.
    time = millis();

    if (s_prevTime != 0)
    {
      // Print how much time since last change.
      Serial.println (time - s_prevTime);
    }

    s_prevTime = time;
}
