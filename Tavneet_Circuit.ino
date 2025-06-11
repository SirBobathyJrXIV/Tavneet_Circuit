#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Create LCD object with I2C address (typically 0x27 or 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin assignments
const int sensorPin = A0;
const int redLED = 2;
const int yellowLED = 3;
const int greenLED = 4;
const int buzzer = 5;

void setup() {
  pinMode(sensorPin, INPUT);
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(buzzer, OUTPUT);

  // Initialize I2C LCD
  Wire.begin();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Soil Moisture:");
  Serial.begin(9600);
}

void loop() {
  int moisture = analogRead(sensorPin);
  int percent = map(moisture, 1023, 270, 0, 100); // Dry = 1023, Wet = 0

  // Update LCD
  lcd.setCursor(0, 1);
  lcd.print("Level: ");
  lcd.print(percent);
  lcd.print("%   ");

  Serial.print("Moisture: ");
  Serial.println(moisture);

  // Manage LEDs based on moisture level
  LEDManager(percent);

  delay(1000);
}

void LEDManager(int moisturePercent) {
  if (moisturePercent < 30) {
    // DRY
    digitalWrite(redLED, HIGH);
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, LOW);
    pulseBuzzer();  // Call new buzzer method
    lcd.setCursor(0, 0);
    lcd.print("Status: DRY     ");
  } else if (moisturePercent < 70) {
    // MOIST
    digitalWrite(redLED, LOW);
    digitalWrite(yellowLED, HIGH);
    digitalWrite(greenLED, LOW);
    noTone(buzzer);
    lcd.setCursor(0, 0);
    lcd.print("Status: MOIST   ");
  } else {
    // WET
    digitalWrite(redLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, HIGH);
    noTone(buzzer);
    lcd.setCursor(0, 0);
    lcd.print("Status: WET     ");
  }
}

void pulseBuzzer() {
  tone(buzzer, 200);    // Turn on buzzer at 200 Hz
  delay(1000);          // Keep on for 1 second
  noTone(buzzer);       // Turn off buzzer
  delay(1000);          // Stay off for 1 second
}