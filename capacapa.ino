#include <LiquidCrystal_I2C.h>
#include <math.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int trigPin = 9;
const int echoPin = 8;

// Tank geometry constants
const float WIDTH_MM = 250.0;        // mm (horizontal distance)
const float TOTAL_HEIGHT_MM = 150.0; // mm (max height)

float duration;
float distance;
float height;
float capacity;

void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600);

  // Display label once
  lcd.setCursor(0, 0);
  lcd.print("Capacity:");
}

void loop() {
  // Trigger ultrasonic pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure echo duration and convert to distance (in mm)
  duration = pulseIn(echoPin, HIGH);
  distance = (0.034 * duration) / 2 * 10;  // mm

  // Compute vertical height using Pythagoras
  float heightCalc = pow(distance, 2) - pow(WIDTH_MM, 2);
  if (heightCalc > 0) {
    height = sqrt(heightCalc);
  } else {
    height = NAN;  // invalid reading (distance < width)
  }

  // Convert height to capacity percentage
  if (!isnan(height)) {
    capacity = (height / TOTAL_HEIGHT_MM) * 100.0;
  } else {
    capacity = 100.0;  // Treat NaN as full capacity
  }

  // Keep within 0–100 range
  if (capacity > 100) capacity = 100;
  if (capacity < 0) capacity = 0;

  // --- LCD Display ---
  lcd.setCursor(0, 1);
  lcd.print("                "); // clear old value
  lcd.setCursor(0, 1);
  lcd.print(capacity, 1); // one decimal place
  lcd.print("%");

  // --- Serial Output ---
  Serial.print("Distance(mm): ");
  Serial.print(distance);
  Serial.print("  Height(mm): ");
  Serial.print(height);
  Serial.print("  Capacity(%): ");
  Serial.println(capacity);

  delay(1000);
}
