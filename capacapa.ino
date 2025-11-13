#include <LiquidCrystal_I2C.h>
#include <math.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int trigPin = 9;
const int echoPin = 8;

// Tank geometry constants
const float WIDTH_MM = 250.0;        // mm (horizontal distance)
const float TOTAL_HEIGHT_MM = 160.0; // mm (max height)

float duration;
float distance;
float height;
float capacity;
float heightCalc;

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

  // Compute  height 
  float heightCalc = distance * sin(RADIANS);
  float height = TOTAL_HEIGHT_MM - heightCalc;
  float capacity = (height/TOTAL_HEIGHT_MM)*100;

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
