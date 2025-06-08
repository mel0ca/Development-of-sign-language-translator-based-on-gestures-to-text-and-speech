#include<SoftwareSerial.h>
SoftwareSerial bluetooth(2, 3); // TX, RX

// Flex sensor pins
const int flexPins[5] = {A0, A1, A2, A3, A4};
const int thresholds[5] = {950, 900, 900, 900, 925};  // Adjust as needed

// Gesture messages
String gestureWords[32] = {
  "OK", "Bye", "Who", "What", "How", "Why", "When", "Where",
  "Police", "Fire", "Danger", "Wake up", "Sleep", "Come", "Go", "Stop",
  "Call", "Hospital", "Doctor", "Medicine", "Pain", "Toilet", "Food", "Water",
  "Help", "Please", "Bad", "Good", "Thank You", "No", "Yes", "Hello"
};

String lastSent = "";

// ISD module pins
const int isdPins[5] = {4, 5, 6, 7, 8};  // ISD for Hello, Yes, Good, Thank You, Stop
const int isdGestureIndices[5] = {26, 30, 29, 27, 23};  // Corresponding gestures

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);

  for (int i = 0; i < 5; i++) {
    pinMode(isdPins[i], OUTPUT);
    digitalWrite(isdPins[i], LOW);  // Ensure ISD modules are off initially
  }
}

void loop() {
  int gestureCode = 0;

  for (int i = 0; i < 5; i++) {
    int value = analogRead(flexPins[i]);
    int bent = (value > thresholds[i]) ? 1 : 0;
    gestureCode |= (bent << i);  // Build binary gesture code

    // Debug
    Serial.print("Flex ");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(value);
    Serial.print(" (");
    Serial.print(bent);
    Serial.println(")");
  }

  String message = gestureWords[gestureCode];

  if (message != lastSent) {
    bluetooth.println(message);
    Serial.println("Sent: " + message);
    lastSent = message;

    // Trigger ISD module if applicable
    for (int i = 0; i < 5; i++) {
      if (gestureCode == isdGestureIndices[i]) {
        digitalWrite(isdPins[i], HIGH);
        delay(500);  // Trigger duration
        digitalWrite(isdPins[i], LOW);
        break;
      }
    }
  }

  delay(1500);
}