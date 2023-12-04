// SALTSH Innovation Project
#include "LiquidCrystal_I2C.h"
#include "HX711.h"
#include <Servo.h>

#define DEBUG_HX711

// calibration parameter from calibrating code with known values
#define CALIBRATION_FACTOR 249236.0
#define offs_weight 0.00;

// Create the lcd object address 0x3F and 16 columns x 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2);

// data pin and clock pin
byte pinData = 3;
byte pinClk = 2;
// define HX711
HX711 scale;

Servo servo1;
Servo servo2;

#define dcmotor_pin 11

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.print("WEIGHT SCALE");

#ifdef DEBUG_HX711
  // Initialize serial communication
  Serial.begin(9600);
  Serial.println("[HX7] Sensor start HX711");
#endif

  //Initializing sensor
  scale.begin(pinData, pinClk);
  // apply the calibration value
  scale.set_scale(CALIBRATION_FACTOR);
  // Initialize the tare
  //Assuming there is no weight on the scale at start up, reset the scale to 0
  scale.tare();

  //----------------------------------------------------------------------------//
  servo1.attach(9);
  servo2.attach(10);
  pinMode(dcmotor_pin, OUTPUT);

  //---------------------------- init process-----------------------------------//
  delay(1500);  
  servo1.write(180);
  servo2.write(180);
}

float weight;

void loop() {
  weight = scale.get_units();
  displayWeight(weight);

  if (weight > 2.00) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Bucket Full!");
    lcd.setCursor(0, 1);
    lcd.print("Remove salt");
    //stop dcmotor
    dcmotorOff();

    delay(5000);
    //exit(0);
  }

  delay(50);
}

void displayWeight(float weight) {
#ifdef DEBUG_HX711
  Serial.print("[HX7] Reading: ");
  Serial.print(scale.get_units(), 2);
  Serial.print(" Kgs");
  Serial.println();
#endif
  lcd.setCursor(0, 1);
  lcd.print("Reading:");
  lcd.print(getWeight(), 2);
  lcd.print(" Kgs");
}

float getWeight() {
  return scale.get_units() - offs_weight;
}

void dcmotorOn() {
  digitalWrite(dcmotor_pin, HIGH);
}

void dcmotorOff() {
  digitalWrite(dcmotor_pin, LOW);
}
