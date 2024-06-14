#define BLUE_WIRE_PIN   2   // Pulses IN

void setup() {
    Serial.begin(9600);
    while (!Serial);

    pinMode(BLUE_WIRE_PIN, INPUT);

    Serial.println ("Running...");
}

void loop() {
  unsigned long time = 0;
  static unsigned long s_prevTime = 0;

  int pinStatus = 0;
  static int s_lastPinStatus = -1;

  pinStatus = digitalRead (BLUE_WIRE_PIN);

  if (pinStatus != s_lastPinStatus)
  {
    // Pin changed. Record the time.
    time = millis();

    if (s_prevTime != 0)
    {
      // Print how much time since last change.
      Serial.println (time - s_prevTime);
    }

    s_prevTime = time;
    s_lastPinStatus = pinStatus;
  }
}
